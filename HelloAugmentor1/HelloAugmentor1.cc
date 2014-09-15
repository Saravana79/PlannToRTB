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
   		//std::string tagid = val["imp"][0]["tagid"].asString();
   		if(urlName !="" || urlName.find("anonymous") == std::string::npos)
		{
			gotIDs = helper.processBidURL(urlName, item_ids, advertisementid,eCPM, click_url,val,request.bidRequest->toJsonStr());
		}
		

		if (gotIDs)
		{

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
					if(urlName.find("youtube.com") != std::string::npos)
					{
						//pass ssl data

						if(advertisementid == "10")
						{
							advertisementid = 1;
						}

						if(advertisementid == "11")
						{
							advertisementid = 5;
						}

						if(advertisementid == "12")
						{
							advertisementid = 3;
						}
						result[account].tags.insert("Advertisment-" + advertisementid + "-S");
					}
					else
					{
						result[account].tags.insert("Advertisment-" + advertisementid);
					}
					recordHit("accounts." + account[0] + ".UrlMatch");
					Json::Value metadata;
					metadata["adsid"] = advertisementid;
					metadata["item_ids"] = item_ids;
					metadata["eCPM"] = eCPM;
					metadata["click_url"] = click_url;
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


