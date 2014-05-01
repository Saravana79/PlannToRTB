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
#include <vector>
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


		std::string appendStringswithComma(const std::string mainstr, const std::string newstr)
			{
				std::string finalstr;
			 	if(mainstr.empty())
					{
						finalstr =  newstr;
					}
					else
					{
						finalstr = mainstr + "," + newstr;
					}
					return finalstr;
			}

		std::string getRootItemID(const std::string itemtype)
			{
			//Advertisment which as associated direclty to that type is taken based on item's type.
						    std::string root_item_id;
						    if (itemtype == "Mobile")
						    {
						    		root_item_id = "3215";
						    }
				    	 	if (itemtype == "Tablet")
				    	 	{
							    	root_item_id = "3217";
							}
						 	if (itemtype == "Camera")
				    	 	{
							    	root_item_id = "3216";
							}
						 	if (itemtype == "Game")
				    	 	{
							    	root_item_id = "15411";
							}
						 	if (itemtype == "Laptop")
				    	 	{
							    	root_item_id = "15412";
							}
						 	if (itemtype == "Car")
				    	 	{
							    	root_item_id = "3214";
							}
						 	if (itemtype == "Bike")
				    	 	{
							    	root_item_id = "3218";
							}
							if (itemtype == "Cycle")
				    	 	{
							    	root_item_id = "3210";
							}
							return root_item_id;

			}				

		bool processBidURL(const std::string url, std::string &item_ids, std::string &advertisementid, std::string &eCPM )
			{
					redisContext* m_redisContext;
					const char *hostname = "54.83.203.184";
					//const char *hostname = "127.0.0.1";
					int port = 6379;
					
					vector<CAdDataSorter> *vDS = new vector<CAdDataSorter>();
					struct timeval timeout = { 1, 500000 }; // 1.5 seconds
					m_redisContext = redisConnectWithTimeout(hostname, port, timeout);
					if (m_redisContext == NULL || m_redisContext->err) {
						if (m_redisContext) {
							printf("Connection error: %s\n", m_redisContext->errstr);
							redisFree(m_redisContext);
						} else {
							printf("Connection error: can't allocate redis context\n");
						}
					}
						std::string prfx = "url:";
						std::string urlKey = "";
						std::map<int,Item> items;
						std::map<int,Advertisement> ads;
						std::string adIdsStr ;
						std::string vendorIDString =",";
						urlKey = prfx + url;
			        	std::string cmd = "";
		        		redisReply *reply = 0;
					
			        	cmd = "HGET " + urlKey + " item_ids";
			        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			        	if (reply->type != REDIS_REPLY_NIL)
		        		{
				            item_ids = reply->str;
				        	std::vector <std::string> vItemIDs;
					    	vItemIDs = parseString(item_ids);
				            //  HGET item:123 avertisement_id
					    	std::string itemtype;
			    			for (unsigned int i = 0; i < vItemIDs.size(); i++)
				   		    {

					   		    	cmd = "HMGET items:" + vItemIDs[i] + " price type vendor_id advertisement_id";
						        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

						        
						        	if (reply->type == REDIS_REPLY_ARRAY)
					        		{
					        			Item item;
					        			item.id = atoi(vItemIDs[i].c_str());
					        			item.price = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
					        			item.type = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";
					        			item.vendor_id = reply->element[2]->type != REDIS_REPLY_NIL ? reply->element[2]->str : "";
					        			item.advertisement_id = reply->element[3]->type != REDIS_REPLY_NIL ? reply->element[3]->str : "";

					        			if(!item.advertisement_id.empty())
					        				{
					        					adIdsStr = appendStringswithComma(adIdsStr,item.advertisement_id);
											
				        					}
					        			if(!item.vendor_id.empty())
					        			{
					        				vendorIDString = appendStringswithComma(vendorIDString,item.vendor_id);
					        			}
					        			items[item.id] = item;
					        			itemtype = item.type;
					        			
						        	} 
						    }
						    std::string root_item_id = getRootItemID(itemtype);
						    if(!root_item_id.empty())
						    {

						    	cmd = "HGET items:" + root_item_id + " advertisement_id";

						    
						    	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
				        		if (reply->type != REDIS_REPLY_NIL)
				        		{
				        			adIdsStr = appendStringswithComma(adIdsStr,reply->str);
				        		}

						    }

					        if(!adIdsStr.empty())
								{
									std::vector <std::string> vAdIDs;
									vAdIDs = parseString(adIdsStr);
										// HGET advertisments:1 eCPM
									for (unsigned int i = 0; i < vAdIDs.size(); i++)
									{
										if(ads.find(atoi(vAdIDs[i].c_str())) == ads.end())
										{
											cmd = "HMGET advertisments:" + vAdIDs[i] + " type vendor_id dailybudget ecpm";
											reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
											if (reply->type == REDIS_REPLY_ARRAY)
							        		{
							        			Advertisement ad;
							        			ad.id = atoi(vAdIDs[i].c_str());
							        			ad.type = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
							        			ad.vendor_id = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";
							        			ad.dailybudget = reply->element[2]->type != REDIS_REPLY_NIL ? reply->element[2]->str : "";
							        			ad.eCPM = reply->element[3]->type != REDIS_REPLY_NIL ? reply->element[3]->str : "0";
							        			ads[ad.id] = ad;

					        					CAdDataSorter ds;
												ds.id = ad.id;
												ds.val = stof(ad.eCPM);
										
												vDS->push_back(ds);
							
								        	} 

											
										}	
									}
			 
								}
		   
		         		}
			         	
						    
						redisFree(m_redisContext);
						if(vDS->size() > 0)
						{         	
							sort(vDS->begin(),vDS->end(),greater_than_key());
							vendorIDString = vendorIDString + ",";
							std::vector<CAdDataSorter>::iterator It;
							for (It = vDS->begin(); It != vDS->end(); ++It)
							{
								cout << "Ad id - " << It->id << endl;
								Advertisement ad = ads[It->id];
								if(ad.type == "dynamic")
								{	
									if (vendorIDString.find(ad.vendor_id) != std::string::npos) 
										{
										    advertisementid = std::to_string(It->id);
											eCPM = std::to_string(It->val);
											return true;
										}		
								}
								else
								{
								  advertisementid = std::to_string(It->id);
								  eCPM = std::to_string(It->val);
								  return true;
								}
							
							}
							
						}
						else
						{

							return false;
						}

				return false;
		}

		/* end redis access code */



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
			config.exchangeFilter.include.push_back("adx");
			for (auto & c: config.creatives) {
				c.exchangeFilter.include.push_back("adx");
				c.providerConfig["adx"]["externalId"] = "1234";
				c.providerConfig["adx"]["htmlTemplate"] = 
					"<html><body><iframe src=\"http://www.plannto.com/advertisments/show_ads?item_id=%{meta.item_ids}&ads_id=%{meta.advertisementids}&size=%{creative.width}x%{creative.height}&click_url=%%CLICK_URL_UNESC%%&wp=%%WINNING_PRICE%%&ref_url=%{bidrequest.url}\" width=\"%{creative.width}\" height=\"%{creative.height}\"/></body></html>";
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


			
			std::string urltemp = br->url.toString();
   			std::string item_ids="";
			std::string advertisementid="";
   			std::string eCPM="";

		

			if(processBidURL(urltemp, item_ids, advertisementid,eCPM ))
			{
				Json::Value metadata;
	    		metadata["advertisementids"] = advertisementid;
				metadata["eCPM"] = eCPM;
				metadata["item_ids"] = item_ids;


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

