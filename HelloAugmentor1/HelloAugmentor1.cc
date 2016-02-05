/*
CKY: Feb-05-2015

*/

#include "augmentor_ex.h"
// Now inluded in augmentor_ex.h header
//#include "hiredis-master/hiredis.h"

//#include "redispp.h"

#include "rtbkit/core/agent_configuration/agent_configuration_listener.h"
#include "rtbkit/core/agent_configuration/agent_config.h"
#include "rtbkit/plugins/augmentor/augmentor_base.h"
#include "rtbkit/common/bid_request.h"
#include "soa/service/zmq_named_pub_sub.h"
#include "jml/utils/exc_assert.h"
#include "PlannToUtilities.h"
#include <unordered_map>
#include <mutex>
#include <vector>

using namespace std;

namespace RTBKIT {


	/******************************************************************************/
	/* FREQUENCY CAP STORAGE                                                      */
	/******************************************************************************/

	/** A very primitive storage for frequency cap events.

	Ideally this should be replaced by some kind of low latency persistent
	storage (eg. Redis).

	Note that the locking scheme for this class is unlikely to scale well. A
	better scheme is left as an exercise to the reader.
	*/
	struct FrequencyCapStorage
	{
		/** Returns the number of times an ad for the given account has been shown
		to the given user.
		*/
		size_t get(const RTBKIT::AccountKey& account, const RTBKIT::UserIds& uids)
		{
			lock_guard<mutex> guard(lock);
			return counters[uids.exchangeId][account[0]];
		}

		/** Increments the number of times an ad for the given account has been
		shown to the given user.
		*/
		void inc(const RTBKIT::AccountKey& account, const RTBKIT::UserIds& uids)
		{
			lock_guard<mutex> guard(lock);
			counters[uids.exchangeId][account[0]]++;
		}

	private:

		mutex lock;
		unordered_map<Datacratic::Id, unordered_map<string, size_t> > counters;

	};

	/******************************************************************************/
	/* FREQUENCY CAP AUGMENTOR                                                    */
	/******************************************************************************/

	/** Note that the serviceName and augmentorName are distinct because you may
	have multiple instances of the service that provide the same
	augmentation.
	*/
	FrequencyCapAugmentor::
		FrequencyCapAugmentor(
		std::shared_ptr<Datacratic::ServiceProxies> services,
		const string& serviceName,
		const string& augmentorName) :
	SyncAugmentor(augmentorName, serviceName, services),
		storage(new FrequencyCapStorage()),
		agentConfig(getZmqContext()),
		palEvents(getZmqContext())
	{
		recordHit("up");
	}

redisContext* FrequencyCapAugmentor::connectRedisSvr()
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

bool getProbability(double check)
{
	double val = (double)rand() / RAND_MAX;

	//int random;

	if(val < check)
		{ return true;}
	else
		{return false;}

	// if (val < 0.05)       //  5%
	//     random = 0;
	// else if (val < 0.25)  //  5% + 20%
	//     random = 1;
	// else if (val < 0.55)  //  5% + 20% + 30%
	//     random = 2;
	// else
	//     random = 3;
}

	/** Sets up the internal components of the augmentor.

	Note that SyncAugmentorBase is a MessageLoop so we can attach all our
	other service providers to our message loop to cut down on the number of
	polling threads which in turns reduces the number of context switches.
	*/
	void
		FrequencyCapAugmentor::
		init()
	{
		SyncAugmentor::init(2 /* numThreads */);

		/* Manages all the communications with the AgentConfigurationService. */
		agentConfig.init(getServices()->config);
		addSource("FrequencyCapAugmentor::agentConfig", agentConfig);

		cout << "Augment Init" << endl;

		palEvents.init(getServices()->config);

		/* This lambda will get called when the post auction loop receives a win
		on an auction.
		*/
		palEvents.messageHandler = [&] (const vector<zmq::message_t>& msg)
		{
			RTBKIT::AccountKey account(msg[19].toString());
			RTBKIT::UserIds uids =
				RTBKIT::UserIds::createFromString(msg[15].toString());

			storage->inc(account, uids);
			recordHit("wins");
		};

		palEvents.connectAllServiceProviders(
			"rtbPostAuctionService", "logger", {"MATCHEDWIN"});
		addSource("FrequencyCapAugmentor::palEvents", palEvents);

		// Connecting the redis once and saving its context
		

	}


	/** Augments the bid request with our frequency cap information.

	This function has a 5ms window to respond (including network latency).
	Note that the augmentation is in charge of ensuring that the time
	constraints are respected and any late responses will be ignored.
	*/
	RTBKIT::AugmentationList
		FrequencyCapAugmentor::
		onRequest(const RTBKIT::AugmentationRequest& request)
	{
		recordHit("requests");

		RTBKIT::AugmentationList result;

		bool gotIDs = false;
		std::string urlName(request.bidRequest->url.c_str());
		Json::Value val = request.bidRequest->toJson();

		//std::string userid = val["user"]["id"].asString();
		//std::string prfx = "url:";
		Helper helper;
		//urlName = prfx + urlName;
		//cout << "\t\t URL  --> " << urlName << endl;

		std::string item_ids="";
   		std::string advertisementid="";
   		std::string eCPM="";
   		std::string click_url = "";
   		bool hastaggingrestricted = false;
		bool isSSLRequired = false;

   		bool isRemarketingBlocked = false;
   		//std::string tagid = val["imp"][0]["tagid"].asString();


		auto& pmpNode = request.bidRequest->imp[0].pmp;

		string groupid="";
		bool addressRemarketing = true;

		if(request.bidRequest->isTest)
		{
			cout << urlName << request.bidRequest->isTest << endl;
		}
		// if(!val["imp"][0]["pmp"]["ext"]["adgroup_id"].isNull())
		// {
			groupid = val["imp"][0]["pmp"]["ext"]["adgroup_id"].asString();

			if((groupid == "24913477600") )
		
			{
				cout << "ukraine " << urlName << endl;	
			}	

		// }
		
		
		for (auto i = 0; pmpNode->ext["excluded_attribute"].size() > i; i += 1) 
		{
        	if (pmpNode->ext["excluded_attribute"][i] == "7") {
             hastaggingrestricted =  true;
        	}

        	if (pmpNode->ext["excluded_attribute"][i] == "48") {
        	//	cout << "****** URL  that requires SSS ******--> " << urlName << endl; 
             isSSLRequired =  true;
        	}

    		//if (pmpNode->ext["excluded_attribute"][i] == "34") {
        	//	cout << "****** URL  that disables flash ******--> " << urlName << endl; 
             
        	//}

    		if (pmpNode->ext["excluded_attribute"][i] == "8") {
        	//	cout << "****** URL  that requires SSS ******--> " << urlName << endl; 
             isRemarketingBlocked =  true;
        	}


    	}
    	bool bidthroughRemarketing = false;
    	string add_details="";
   		if(urlName !="" && urlName.find("anonymous") == std::string::npos)
		{
			//cout << "\t\t URL  --> " << urlName << endl;
			
		}

		if(addressRemarketing && urlName.find("%") == std::string::npos && groupid != "24913477600")
		{
			gotIDs = helper.processBidURLWrapper(urlName, item_ids, advertisementid,eCPM, click_url,val,request.bidRequest->toJsonStr(),isRemarketingBlocked,bidthroughRemarketing,hastaggingrestricted,add_details);
		}
		
		
     


		if (gotIDs)
		{
			//cout << "\t\t URL  --> " << urlName << "publiser settings id "<< pmpNode->ext["publisher_settings_list_id"] << endl;
			
			for (const string& agent : request.agents) 
			{

				RTBKIT::AgentConfigEntry config = agentConfig.getAgentEntry(agent);

				/* When a new agent comes online there's a race condition where the
				router may send us a bid request for that agent before we receive
				its configuration. This check keeps us safe in that scenario.
				*/
				if (!config.valid()) {
					recordHit("unknownConfig");
					continue;
				}

				const RTBKIT::AccountKey& account = config.config->account;
		
				
				// VICKY: Feb-05-2014: START

				if (gotIDs)
				{

				    if(request.bidRequest->imp[0].video == NULL)
				    {
						if(isSSLRequired)
						{
							if(bidthroughRemarketing)
							{
								result[account].tags.insert("Advertisment-1-S-R");
							}
							else
							{
								if(hastaggingrestricted)
								{
									result[account].tags.insert("Advertisment-1-S");
								}
								else
								{
									result[account].tags.insert("Advertisment-1-S-B");
								}

							}
						
						}
						else
						{
							if(advertisementid != "74")
							{
								if(bidthroughRemarketing)
								{
									result[account].tags.insert("Advertisment-1-R");
								}
								else
								{
									if(hastaggingrestricted)
									{
										result[account].tags.insert("Advertisment-1");
									}
									else
									{
										result[account].tags.insert("Advertisment-1-B");
									}
								}
							}	
							
						}
					}	
					else
					{
						cout << "video bidding" << urlName << endl;
						result[account].tags.insert("Advertisment-1-V");	
					}

					recordHit("accounts." + account[0] + ".UrlMatch");
					Json::Value metadata;
					if((bidthroughRemarketing) && (advertisementid == "21"))
					{
						metadata["adsid"] = "23";
					}
					else
					{
						metadata["adsid"] = advertisementid;
					}	
					metadata["item_ids"] = item_ids;
					metadata["eCPM"] = eCPM;
					metadata["click_url"] = click_url;
					metadata["add_details"] = add_details;
					//metadata["tagid"] = tagid;
					result[account].data = metadata;

				}
			//	else
			//	{
					//cout << "else condition " << endl;
			//	}


			}
		}
		return result;
	}


	/** Returns the frequency cap configured by the agent.

	This function is a bit brittle because it makes no attempt to validate
	the configuration.
	*/
	size_t
		FrequencyCapAugmentor::
		getCap( const string& augmentor,
		const string& agent,
		const RTBKIT::AgentConfigEntry& config) const
	{
		for (const auto& augConfig : config.config->augmentations) {
			if (augConfig.name != augmentor) continue;
			return augConfig.config.asInt();
		}

		/* There's a race condition here where if an agent removes our augmentor
		from its configuration while there are bid requests being augmented
		for that agent then we may not find its config. A sane default is
		good to have in this scenario.
		*/

		return 0;
	}

} // namespace RTBKIT


