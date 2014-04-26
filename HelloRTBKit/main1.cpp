#include <vector>
#include <iostream>
#include "hiredis/hiredis.h"
#include "DataSorter.h"


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

bool compare(const CAdDataSorter x, const CAdDataSorter y)
	{
    return x.val > y.val;
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
				urlKey = prfx + url;
	        		cout << urlKey << endl;
	        		cout << "\t\t URL From the Requester --> " << url << endl;

	        	std::string cmd = "";
        		redisReply *reply = 0;
				

	        	cmd = "HGET " + urlKey + " item_ids";
	        	cout << "\t command: " << cmd << endl;
	        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
	        	if (reply->type == REDIS_REPLY_NIL)
        		{
	        	} 
	        	else 
        		{
		            item_ids = reply->str;
		            cout << "\t\t item_ids From the Requester --> " << item_ids << endl;
			    	std::vector <std::string> vItemIDs;
			    	vItemIDs = parseString(item_ids);
		            //  HGET item:123 avertisement_id

	    			for (unsigned int i = 0; i < vItemIDs.size(); i++)
		   		    {
						cmd = "HGET items:" + vItemIDs[i] + " advertisement_id";
						cout << "\t\t Next command: " << cmd << endl;
						reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
						if (reply->type == REDIS_REPLY_NIL)
						{
						} 
						else 
						{
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
									vDS->push_back(ds);

								}
							}
	 
						}

				    }

	         	}
	         	
		redisFree(m_redisContext);
		if(vDS->size() > 0)
		{         	
			sort(vDS->begin(),vDS->end(),compare);
			advertisementid = std::to_string((vDS->at(0).id));
			eCPM = std::to_string(vDS->at(0).val);
			return true;
		}
		else
		{

			return false;
		}

	return false;
}

int main(int argc, char *argv[])
{


   std::string url = argv[1];
   std::string item_ids="";
   std::string advertisementid="";
   std::string eCPM="";
	
	// applying the simple bubble sort on the objects
	if(processBidURL(url, item_ids, advertisementid,eCPM ))
	{         	
		cout << "advertismentid " << advertisementid  << endl;
		cout << "itemids  " << item_ids  << endl;
		cout << "eCPM  " << eCPM  << endl;
		//pass the meta data here.
		//do the bidding here.
	}
	else
	{
		cout << "no match" << endl;
	}


}


