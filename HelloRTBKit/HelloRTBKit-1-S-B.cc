#include "rtbkit/common/bids.h"
#include "rtbkit/core/banker/slave_banker.h"
#include "rtbkit/core/agent_configuration/agent_config.h"
#include "rtbkit/plugins/bidding_agent/bidding_agent.h"
#include "soa/service/service_utils.h"


#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;
using namespace ML;

namespace RTBKIT {

	class PlannToAdSpot
	{
	public:	
		PlannToAdSpot(void){};
		~PlannToAdSpot(void){};

		std::string id;
		std::string group_id;
	};

	static std::string agent_ad_id = "1";

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

	struct FixedPriceBiddingAgent :
		public BiddingAgent
	{
		FixedPriceBiddingAgent(
			std::shared_ptr<Datacratic::ServiceProxies> services,
			const string& serviceName) :
		BiddingAgent(services, serviceName),
			accountSetup(false)
		{}

		/*redisContext* m_redisContext;
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
		*/
		/* Start redis finding code */
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

			//m_redisContext = connectRedisSvr();

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
			config.creatives.push_back(Creative::sampleBBB);
			config.creatives.push_back(Creative::sampleLBS);

			config.creatives.push_back(Creative::sample336);
			config.creatives.push_back(Creative::sample200);
			config.creatives.push_back(Creative::sample250);
			config.creatives.push_back(Creative::sample160);
			config.creatives.push_back(Creative::sample320);
			config.exchangeFilter.include.push_back("adx");
			for (auto & c: config.creatives) {
				c.exchangeFilter.include.push_back("adx");
				c.providerConfig["adx"]["externalId"] = "PlannTo-Creative-%{meta.advertisementids}-" + RTBKIT::agent_ad_id;
				c.providerConfig["adx"]["htmlTemplate"] = 
					"<html><body><iframe src=\"https://www.plannto.com/advertisments/show_ads?item_id=%{meta.item_ids}&ads_id=%{meta.advertisementids}&size=%{creative.width}x%{creative.height}&click_url=%%CLICK_URL_ESC%%&wp=%%WINNING_PRICE%%&sid=%{meta.tagid}&ref_url=%{bidrequest.url}&device=%{meta.device}&v=%{meta.viewability}&cb=%%CACHEBUSTER%%&t=1&a=%{meta.add_details}&gid=%{meta.user_id}&l=%{meta.geo_id}\" width=\"%{creative.width}\" height=\"%{creative.height}\" style=\"border:0px;\"/></body></html>";
				c.providerConfig["adx"]["videoUrl"] = 
				"<html><body><iframe src=\"http://www.plannto.com/advertisments/video_ads?item_id=%{meta.item_ids}&ads_id=%{meta.advertisementids}&size=%{creative.width}x%{creative.height}&click_url=%%CLICK_URL_ESC%%&wp=%%WINNING_PRICE%%&sid=%{meta.tagid}&ref_url=%{bidrequest.url}&cb=%%CACHEBUSTER%%\" width=\"%{creative.width}\" height=\"%{creative.height}\" style=\"border:0px;\"/></body></html>";	
	
				c.providerConfig["adx"]["clickThroughUrl"] = "%{meta.click_url}";
				c.providerConfig["adx"]["agencyId"] = 59;
				c.providerConfig["adx"]["vendorType"] = "113";
				c.providerConfig["adx"]["attribute"]  = "47 7 50";
				c.providerConfig["adx"]["restrictedCategories"]  = "0";
				c.providerConfig["adx"]["sensitiveCategory"]  = "0";
				//c.providerConfig["adx"]["adGroupId"]  = "%{meta.group_id}";
				
			}



			// Accounts are used to control the allocation of spending budgets for
			// an agent. The whole mechanism is fully generic and can be setup in
			// whatever you feel it bests suits you.
				config.account = {"PlannToAccount_3" , "PC"};
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
				augConfig.filters.include.push_back("Advertisment-" + agent_ad_id + "-S-B");

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


			
			std::string urltemp = br->url.toString();
			Json::Value val = br->toJson();
   			std::string item_ids="";
			std::string advertisementid="";
   			std::string eCPM="";
   			std::string click_url="";
   			std::string priority = "";
   			std::string tagid="";
   			std::vector<string> vAdIDs;
   			//int position=0;
   			int pri =1;
   			bool match= true;
   			
   			advertisementid = augmentations["urlMatcher"]["data"]["adsid"].asString();


				// vAdIDs = parseString(advertisementid) ;
			
	   // 			for (unsigned int i = 0; i < vAdIDs.size(); i++)
				// {
				// 	if(vAdIDs[i] == agent_ad_id)
				// 	{
				// 		position = i;
				// 		match = true;
				// 		pri = vAdIDs.size() - position;
				// 		break;
				// 	}
				// }


			if(match)
			{
				std::vector<string> veCPMs;
				std::vector<string> vclick_urls;

	   			eCPM = augmentations["urlMatcher"]["data"]["eCPM"].asString();
	   			priority = augmentations["urlMatcher"]["data"]["priority"].asString();
	   			item_ids = augmentations["urlMatcher"]["data"]["item_ids"].asString();
	   			click_url = augmentations["urlMatcher"]["data"]["click_url"].asString();
	   			tagid = val["imp"][0]["tagid"].asString();
	   			
				Json::Value metadata;
				string add_details = "";
				add_details = augmentations["urlMatcher"]["data"]["add_details"].asString();
				metadata["add_details"] = add_details;
	    		metadata["advertisementids"] = advertisementid;
	    		metadata["click_url"] = click_url;
				metadata["item_ids"] = item_ids;
				metadata["tagid"] = tagid;
				metadata["user_id"] = val["user"]["id"].asString();
				
				if(!br->toJson()["device"]["devicetype"].isNull() && br->toJson()["device"]["devicetype"] == 1)
	    		{
	    			 metadata["device"] = "mobile";
	    		}
	    		else
	    		{
	    			metadata["device"] = "pc";	
	    		}

	    		if(!br->toJson()["device"]["ext"]["geo_criteria_id"].isNull())
	    		{
	    			metadata["geo_id"] = br->toJson()["device"]["ext"]["geo_criteria_id"].asInt();
	    		}
	    		
				metadata["viewability"] = br->toJson()["imp"][0]["pmp"]["ext"]["viewability"].asString();

				//metadata["attribute"] = "1";
				//metadata["group_id"] = 16025452000;
				float bidValueforsingleImpression = stof(eCPM)/1000 ;

				std::string group_id = br->toJson()["imp"][0]["pmp"]["ext"]["adgroup_id"].asString();
				//metadata["group_id"] = std::stoi(group_id);
				
				cout << "url:" <<  urltemp << "-->" << advertisementid << endl;

				for (Bid& bid : bids) 
				{

					// In our example, all our creatives are of the different sizes so
					// there should only ever be one biddable creative. Note that that
					// the router won't ask for bids on imp that don't have any
					// biddable creatives.
					//ExcAssertEqual(bid.availableCreatives.size(), 1);
					int availableCreative = bid.availableCreatives.front();

					// We don't really need it here but this is how you can get the
					// AdSpot and Creative object from the indexes.
					(void) br->imp[bid.spotIndex];
					(void) config.creatives[availableCreative];

					/*cout << "auction id - " << id << endl;
					cout << "adspot id - " << bid.spotIndex << endl;
					*/
					// Create a 0.0001$ CPM bid with our available creative.
					// Note that by default, the bid price is set to 0 which indicates
					// that we don't wish to bid on the given spot.
					//if(bid.spotIndex <= position)
					//{
						bid.bid(availableCreative, USD(bidValueforsingleImpression),pri);
						
					//}
				}

				// Send our bid back to the agent.
				doBid(id, bids, metadata);


			}
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
			budgetController.topupTransferSync(config.account, USD(4));
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
	RTBKIT::FixedPriceBiddingAgent agent(serviceProxies, "HelloRTBKit-agent-" + RTBKIT::agent_ad_id + "-S");
	agent.init();
	agent.start();

	while (true) this_thread::sleep_for(chrono::seconds(10));

	// Won't ever reach this point but this is how you shutdown an agent.
	agent.shutdown();

	return 0;
}

