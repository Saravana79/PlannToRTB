#include "rtbkit/common/bids.h"
#include "rtbkit/core/banker/slave_banker.h"
#include "rtbkit/core/agent_configuration/agent_config.h"
#include "rtbkit/plugins/bidding_agent/bidding_agent.h"
#include "soa/service/service_utils.h"

#include "../HelloAugmentor1/hiredis-master/hiredis.h"

#include "DataSorter.h"

#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace ML;

namespace RTBKIT {

	struct FixedPriceBiddingAgent :
		public BiddingAgent
	{
		FixedPriceBiddingAgent(
			std::shared_ptr<Datacratic::ServiceProxies> services,
			const string& serviceName) :
		BiddingAgent(services, serviceName),
			accountSetup(false)
		{}

		redisContext* m_redisContext;
		redisContext* connectRedisSvr()
		{
			redisContext *c;
			const char *hostname = "127.0.0.1";
			int port = 6379;

			struct timeval timeout = { 1, 500000 }; // 1.5 seconds
			c = redisConnectWithTimeout(hostname, port, timeout);
			if (c == NULL || c->err) {
				if (c) {
					printf("Connection error: %s\n", c->errstr);
					redisFree(c);
				} else {
					printf("Connection error: can't allocate redis context\n");
				}
			}

			return c;
		}

		std::vector<std::string> parseString(std::string str)
		{
		std::vector <std::string> vec;
		std::string tmp;
		while( (str.find(",", 0)) != string::npos )
		{
			size_t pos = str.find(",", 0);
			tmp = str.substr(0, pos);
			str.erase(0, pos + 1);
			vec.push_back(tmp);
		}
		vec.push_back(str);
		return vec;
		}

		void init()
		{

			m_redisContext = connectRedisSvr();

			// We only want to specify a subset of the callbacks so turn the
			// annoying safety belt off.
			strictMode(false);

			onBidRequest = bind(
				&FixedPriceBiddingAgent::bid, this, _1, _2, _3, _4, _5, _6);

			// This component is used to speak with the master banker and pace the
			// rate at which we spend our budget.
			budgetController.init(getServices()->config);
			budgetController.start();

			// Update our pacer every 10 seconds. Note that since this interacts
			// with the budgetController which is only synced up with the router
			// every few seconds, the wait period shouldn't be set too low.
			addPeriodic("FixedPriceBiddingAgent::pace", 10.0,
				[&] (uint64_t) { this->pace(); });

			BiddingAgent::init();
		}

		void start()
		{
			BiddingAgent::start();

			// Build our configuration and tell the world about it.
			setConfig();
		}

		void shutdown()
		{
			BiddingAgent::shutdown();

			budgetController.shutdown();
		}


		// Sets up an agent configuration
		void setConfig()
		{
			config = AgentConfig();

			config.creatives.push_back(Creative::sampleBB);
			config.creatives.push_back(Creative::sampleWS);
			config.creatives.push_back(Creative::sampleLB);
			config.exchangeFilter.include.push_back("adx");
			for (auto & c: config.creatives) {
				c.exchangeFilter.include.push_back("adx");
				c.providerConfig["adx"]["externalId"] = "1234";
				c.providerConfig["adx"]["htmlTemplate"] = 
					"<html><body><iframe src=\"http://www.plannto.com/show_ads?size=%{creative.width}x%{creative.height}&click_url=%%CLICK_URL_UNESC%%%%WINNING_PRICE%%\"/></body></html>";
				c.providerConfig["adx"]["clickThroughUrl"] = "http://click.usmc.com";
				c.providerConfig["adx"]["agencyId"] = 59;
				c.providerConfig["adx"]["vendorType"] = "113";
				c.providerConfig["adx"]["attribute"]  = "";
				c.providerConfig["adx"]["restrictedCategories"]  = "0";
				c.providerConfig["adx"]["sensitiveCategory"]  = "0";
				c.providerConfig["adx"]["adGroupId"]  = 1643;
				c.providerConfig["adx"]["sensitiveCategory"]  = "0";
			}



			// Accounts are used to control the allocation of spending budgets for
			// an agent. The whole mechanism is fully generic and can be setup in
			// whatever you feel it bests suits you.
			config.account = {"hello", "world"};

			// Indicate to the router that we want our bid requests to be augmented
			// with our frequency cap augmentor example.
			{
				AugmentationConfig augConfig;

				// Name of the requested augmentor.
				augConfig.name = "urlMatcher";

				// If the augmentor was unable to augment our bid request then it
				// should be filtered before it makes it to our agent.
				augConfig.required = true;

				// Config parameter sent used by the augmentor to determine which
				// tag to set.
				augConfig.config = Json::Value(42);

				// Instruct to router to filter out all bid requests who have not
				// been tagged by our frequency cap augmentor.
				//augConfig.filters.include.push_back("pass-frequency-cap-ex");
				// "urlMatcher"
				augConfig.filters.include.push_back("UrlMatch");

				config.addAugmentation(augConfig);
			}

			// Tell the world about our config. We can change the configuration of
			// an agent at any time by calling this function.
			doConfig(config);
		}

		void bid(
			double timestamp,
			const Id & id,
			std::shared_ptr<RTBKIT::BidRequest> br,
			Bids bids,
			double timeLeftMs,
			const Json::Value & augmentations)
		{
			vector<CAdDataSorter> *vDS = new vector<CAdDataSorter>();

			for (Bid& bid : bids) {

				// In our example, all our creatives are of the different sizes so
				// there should only ever be one biddable creative. Note that that
				// the router won't ask for bids on imp that don't have any
				// biddable creatives.
				ExcAssertEqual(bid.availableCreatives.size(), 1);
				int availableCreative = bid.availableCreatives.front();

				// We don't really need it here but this is how you can get the
				// AdSpot and Creative object from the indexes.
				(void) br->imp[bid.spotIndex];
				(void) config.creatives[availableCreative];

				// Create a 0.0001$ CPM bid with our available creative.
				// Note that by default, the bid price is set to 0 which indicates
				// that we don't wish to bid on the given spot.
				bid.bid(availableCreative, MicroUSD(100));
			}

			// fetch the details about the requested URL from the redis db
        		std::string urlName(br->url.c_str());
			std::string prfx = "url:";
			urlName = prfx + urlName;
	        	cout << urlName << endl;
	        	cout << "\t\t URL From the Requester --> " << urlName << endl;

	        	std::string cmd = "";
        		redisReply *reply = 0;

	        	cmd = "HGET " + urlName + " item_ids";
	        	cout << "\t command: " << cmd << endl;
	        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
	        	if (reply->type == REDIS_REPLY_NIL)
        		{
	        	} else {
		            std::string str = reply->str;
			    std::vector <std::string> vItemIDs;
			    vItemIDs = parseString(str);
		            //  HGET item:123 avertisement_id

    			    for (unsigned int i = 0; i < vItemIDs.size(); i++)
	   		    {
				cmd = "HGET item:" + vItemIDs[i] + " avertisement_id";
				cout << "\t\t Next command: " << cmd << endl;
				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
				if (reply->type == REDIS_REPLY_NIL)
				{
				} else {
					std::string str = reply->str;
					cout << "\t\t Ad IDs " << str.c_str() << endl;
					std::vector <std::string> vAdIDs;
					vAdIDs = parseString(str);

					// HGET advertisments:1 eCPM
					for (unsigned int i = 0; i < vAdIDs.size(); i++)
					{
						cmd = "HGET advertisments:" + vAdIDs[i] + " eCPM";
						cout << "\t\t\t Next command: " << cmd << endl;
						reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
						if (reply->type == REDIS_REPLY_NIL)
						{
						} else {
							std::string str = reply->str;
							cout << "\t\t\t eCPMs " << str.c_str() << endl;
							std::vector <std::string> vECpms;
							vECpms = parseString(str);

							// sorting the ads to get the min ecpm
							CAdDataSorter ds;
							ds.id = atoi(vAdIDs[i].c_str());
							ds.val = atof(vECpms[0].c_str());


						}
					}
 
				}

			    }

	         	}
        		freeReplyObject(reply);

			// applying the simple bubble sort on the objects
			CAdDataSorter temp;
			for (int i = 1; i <= vDS->size(); i++)
			{
				for (int j = 0; j < vDS->size() - 1; j++)
				{
					if (vDS->at(j).val > vDS->at(j+1).val)
					{
						temp = vDS->at(j);
						vDS->at(j) = vDS->at(j+1);
						vDS->at(j+1) = temp;
					}
				}
			}

			// A value that will be passed back to us when we receive the result of
			// our bid.
			//Json::Value metadata = 42;
			Json::Value metadata;
			metadata["advertisementids"] = vDS->at(0).id;
			metadata["eCPM"] = vDS->at(0).val;
			

			// Send our bid back to the agent.
			doBid(id, bids, metadata);
		}


		// Simple pacing scheme which allocates 1$ to spend every period.
		void pace()
		{
			// We need to register our account once with the banker service.
			if (!accountSetup) {
				accountSetup = true;
				budgetController.addAccountSync(config.account);
			}

			// Make sure we have 1$ to spend for the next period.
			budgetController.topupTransferSync(config.account, USD(1));
		}


		AgentConfig config;

		bool accountSetup;
		SlaveBudgetController budgetController;
	};

} // namepsace RTBKIT



// main()
int main(int argc, char** argv)
{
	using namespace boost::program_options;

	Datacratic::ServiceProxyArguments args;

	options_description options = args.makeProgramOptions();
	options.add_options() ("help,h", "Print this message");

	variables_map vm;
	store(command_line_parser(argc, argv).options(options).run(), vm);
	notify(vm);

	if (vm.count("help")) {
		cerr << options << endl;
		return 1;
	}

	auto serviceProxies = args.makeServiceProxies();
	RTBKIT::FixedPriceBiddingAgent agent(serviceProxies, "HelloRTBKit-agent");
	agent.init();
	agent.start();

	while (true) this_thread::sleep_for(chrono::seconds(10));

	// Won't ever reach this point but this is how you shutdown an agent.
	agent.shutdown();

	return 0;
}

