#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "hiredis-master/hiredis.h"


using namespace std;

#pragma once
class CAdDataSorter
{
public:
	CAdDataSorter(void){};
	~CAdDataSorter(void){};

	int id;
	std::string name;
	float val;

	// index
	int idx;

	
};



struct greater_than_key
{
    inline bool operator() (const CAdDataSorter& struct1, const CAdDataSorter& struct2)
    {
        return (struct1.val > struct2.val);
    }
};



class Item
{
public:
	Item(void){};
	~Item(void){};

	int id;
	std::string name;
	std::string advertisement_id;
	std::string vendor_id;
	std::string pc_vendor_id;
	std::string price;
	std::string type;	
	// index
	int idx;

	
};


class Advertisement
{
public:
	Advertisement(void){};
	~Advertisement(void){};

	int id;
	std::string type;
	std::string vendor_id;
	std::string eCPM;
	std::string dailybudget;
	std::string click_url;
	std::string enabled;
	float val;

	// index
	int idx;

	
};

struct Helper
{
 
  Helper(void){};
  ~Helper(void){};

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
  
  
   bool processBidURL(const std::string url, std::string & item_ids, std::string & advertisementid, std::string & eCPM,std::string & click_url)
				{
					redisContext* m_redisContext;
					//const char *hostname = "54.83.203.184";
					const char *hostname = "127.0.0.1";
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
						std::string pcvendorIDString =",";
						urlKey = prfx + url;
			        	std::string cmd = "";
		        		redisReply *reply = 0;
		        		//reply= (redisReply*)redisCommand(m_redisContext, "AUTH %s", "pLanTto@Rtb");
						//if (reply->type == REDIS_REPLY_ERROR) {
						//    cout <<  "authentication failed - redis" << endl ;
						//}
						//freeReplyObject(reply);
			        	cmd = "HGET " + urlKey + " item_ids";
			        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			        	if (reply->type != REDIS_REPLY_NIL)
		        		{
				            item_ids = reply->str;
				        	std::vector <std::string> vItemIDs;
					    	vItemIDs = parseString(item_ids);
				            //  HGET item:123 avertisement_id
					    	std::string itemtype;
					    	 cout << "url " <<  vItemIDs<< endl;
			    			for (unsigned int i = 0; i < vItemIDs.size(); i++)
				   		    {

					   		    	cmd = "HMGET items:" + vItemIDs[i] + " price type vendor_id advertisement_id pc_vendor_id";
						        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

						        
						        	if (reply->type == REDIS_REPLY_ARRAY)
					        		{
					        			Item item;
					        			item.id = atoi(vItemIDs[i].c_str());
					        			item.price = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
					        			item.type = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";
					        			item.vendor_id = reply->element[2]->type != REDIS_REPLY_NIL ? reply->element[2]->str : "";
					        			item.advertisement_id = reply->element[3]->type != REDIS_REPLY_NIL ? reply->element[3]->str : "";
					        			item.pc_vendor_id = reply->element[4]->type != REDIS_REPLY_NIL ? reply->element[4]->str : "";
					        			cout << "pc vendor id" << item.pc_vendor_id << endl;
					        			if(!item.advertisement_id.empty())
					        				{
					        					adIdsStr = appendStringswithComma(adIdsStr,item.advertisement_id);
											
				        					}
					        			if(!item.vendor_id.empty())
					        			{
					        				vendorIDString = appendStringswithComma(vendorIDString,item.vendor_id);
					        			}

					        			if(!item.pc_vendor_id.empty())
					        			{
					        				pcvendorIDString = appendStringswithComma(pcvendorIDString,item.pc_vendor_id);

					        			}
					        			items[item.id] = item;
					        			itemtype = item.type;

					        			 cout << "adid " << adIdsStr << endl;
					        			 cout << "vendorid " << vendorIDString << endl;
					        			 cout << "pcvendorid " << pcvendorIDString << endl;
					        			
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

						    cout << "root item id " << root_item_id << endl;
						    if(!adIdsStr.empty())
								{
									std::vector <std::string> vAdIDs;
									vAdIDs = parseString(adIdsStr);
										// HGET advertisments:1 eCPM
									for (unsigned int i = 0; i < vAdIDs.size(); i++)
									{
										if(ads.find(atoi(vAdIDs[i].c_str())) == ads.end())
										{
											cmd = "HMGET advertisments:" + vAdIDs[i] + " type vendor_id dailybudget ecpm click_url enabled";
											reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
											if (reply->type == REDIS_REPLY_ARRAY)
							        		{
							        			Advertisement ad;
							        			ad.id = atoi(vAdIDs[i].c_str());
							        			ad.type = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
							        			ad.vendor_id = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";
							        			ad.dailybudget = reply->element[2]->type != REDIS_REPLY_NIL ? reply->element[2]->str : "";
							        			ad.eCPM = reply->element[3]->type != REDIS_REPLY_NIL ? reply->element[3]->str : "0";
							        			ad.click_url = reply->element[4]->type != REDIS_REPLY_NIL ? reply->element[4]->str : "";
							        			ad.enabled = reply->element[5]->type != REDIS_REPLY_NIL ? reply->element[5]->str : "";

						    	    			if(ad.enabled == "true")
							        			{

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
		   
		         		}
			         	
						 
						bool admatch = false;    
						
						if(vDS->size() > 0)
						{         	
							sort(vDS->begin(),vDS->end(),greater_than_key());
							vendorIDString = vendorIDString + ",";
							std::vector<CAdDataSorter>::iterator It;
							for (It = vDS->begin(); It != vDS->end(); ++It)
							{
								
								Advertisement ad = ads[It->id];
								if(ad.type == "dynamic")
								{	
									if (vendorIDString.find(ad.vendor_id) != std::string::npos) 
										{

										    advertisementid = appendStringswithComma(advertisementid,std::to_string(It->id));
											eCPM = appendStringswithComma(eCPM,std::to_string(It->val));
											click_url = appendStringswithComma(click_url,ad.click_url);
											admatch = true;
											
										}		
								}

								if(ad.type == "dynamicpc")
								{
										if (pcvendorIDString.find(ad.vendor_id) != std::string::npos) 
										{
										   advertisementid = appendStringswithComma(advertisementid,std::to_string(It->id));
										   eCPM = appendStringswithComma(eCPM,std::to_string(It->val));
										   click_url = appendStringswithComma(click_url,ad.click_url);
										   
										   admatch = true;
										}
								}

								if(ad.type == "static")
								{
								       advertisementid = appendStringswithComma(advertisementid,std::to_string(It->id));
										eCPM = appendStringswithComma(eCPM,std::to_string(It->val));
										click_url = appendStringswithComma(click_url,ad.click_url);
										admatch = true;
								}
							
							}
							
						}
				
				if(admatch)
				{
						cmd = "HINCRBY " + urlKey + " count 1";
			        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			        
				}	
				else
				{
						cmd = "HINCRBY missingad:" + url  + " count 1";	
			        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
				}	
				cout << "ad id " <<  advertisementid << endl;
				redisFree(m_redisContext);
				return admatch;
		}

};

