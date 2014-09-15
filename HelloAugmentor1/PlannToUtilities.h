#include <string>
#include <vector>
#include <iostream>
#include <exception>
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




const char HEX2DEC[256] = 
{
    /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
    /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,
    
    /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    
    /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    
    /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
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
	std::string excluded_item_ids;
	std::string excluded_sites;
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


std::string getCurrentTime()
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );

    string currentTime = to_string(now->tm_year + 1900) + '-' + to_string((now->tm_mon + 1)) + '-' + to_string(now->tm_mday);
    return currentTime;
}			


std::string UriDecode(const std::string & sSrc)
	{
	   // Note from RFC1630: "Sequences which start with a percent
	   // sign but are not followed by two hexadecimal characters
	   // (0-9, A-F) are reserved for future extension"
	 
	   const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
	   const int SRC_LEN = sSrc.length();
	   const unsigned char * const SRC_END = pSrc + SRC_LEN;
	   // last decodable '%' 
	   const unsigned char * const SRC_LAST_DEC = SRC_END - 2;
	 
	   char * const pStart = new char[SRC_LEN];
	   char * pEnd = pStart;
	 
	   while (pSrc < SRC_LAST_DEC)
	   {
	      if (*pSrc == '%')
	      {
	         char dec1, dec2;
	         if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
	            && -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
	         {
	            *pEnd++ = (dec1 << 4) + dec2;
	            pSrc += 3;
	            continue;
	         }
	      }
	 
	      *pEnd++ = *pSrc++;
	   }
	 
	   // the last 2- chars
	   while (pSrc < SRC_END)
	      *pEnd++ = *pSrc++;
	 
	   std::string sResult(pStart, pEnd);
	   delete [] pStart;
	   return sResult;
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
  
  std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
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
  
   bool processBidURL(std::string url, std::string & item_ids, std::string & advertisementid, std::string & eCPM,std::string & click_url, Json::Value bidrequestjson,std::string jsonstring)
				{
					redisContext* m_redisContext;
					//const char *hostname = "54.83.203.184";
					const char *hostname = "127.0.0.1";
					int port = 6379;
					
					try
        			{	
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
						url = UriDecode(url);
						urlKey = prfx + url;
			        	std::string cmd = "";
		        		redisReply *reply = 0;
		        		std::string userid;
		        		std::string tagid;
		        		userid = bidrequestjson["user"]["id"].asString();
		        		tagid = bidrequestjson["imp"][0]["tagid"].asString();
		        		long reservePrice =0;
		        		if(!bidrequestjson["imp"][0]["reservePrice"].isNull())
		        		{
		        		
		        			reservePrice  = bidrequestjson["imp"][0]["reservePrice"][0].asInt();
		        	
		        		}
		        		
		        		double eCPMController = 1.0; 
		        		
		        		//reply= (redisReply*)redisCommand(m_redisContext, "AUTH %s", "pLanTto@Rtb");
						//if (reply->type == REDIS_REPLY_ERROR) {
						//    cout <<  "authentication failed - redis" << endl ;
						//}
						//freeReplyObject(reply);



				        	cmd = "HMGET " + urlKey + " item_ids article_type";
				        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
				        	if (m_redisContext->err == 0  && reply->element[0]->type != REDIS_REPLY_NIL)
			        		{
					            item_ids = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
					        	std::string article_type = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";

					        // domain specific tagging needs to be capture here.. for now its commented as its not required.	
					        //	if(url.find("gadgets.ndtv.com") != std::string::npos)
					        //	{
					        //		cmd = "HSET spottags-special  siddetails \"" + ReplaceAll(ReplaceAll(jsonstring,"\"","\\\"")," ","") + "\"";	
			        		//		reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			        			
					        //	}

				        		if (article_type != "HowTo/Guide" && article_type != "Apps")
				        		{	
						        	cmd = "HMGET spottags:" + tagid + " sid_details ectr";	
			        				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			        			
			        				if(m_redisContext->err == 0  && reply->element[0]->type == REDIS_REPLY_NIL)
			        				{
			        				//	cmd = "HSET spottags:" + tagid + " sid_details \"" + ReplaceAll(ReplaceAll(jsonstring,"\"","\\\"")," ","") + "\"";	
									//	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			        				}
			        			
			        				if(m_redisContext->err == 0  && reply->type == REDIS_REPLY_ARRAY && reply->element[1]->type != REDIS_REPLY_NIL)
			        				{

			        					eCPMController = atof(reply->element[1]->str) ;
			        						
											if(eCPMController == 0)
						   					{
						   						// cout << "Low Probability" << " - url - " <<  url << endl;
						   						redisFree(m_redisContext);
						   						return false;
						   					}
						   			
			        				}
			        				std::vector <std::string> vItemIDs;
							    	vItemIDs = parseString(item_ids);
						   	    	std::string itemtype;
							 
					    			for (unsigned int i = 0; i < vItemIDs.size(); i++)
						   		    {

							   		    	cmd = "HMGET items:" + vItemIDs[i] + " price type vendor_id advertisement_id pc_vendor_id";
								        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

								        
								        	if (m_redisContext->err == 0  && reply->type == REDIS_REPLY_ARRAY)
							        		{
							        			Item item;
							        			item.id = atoi(vItemIDs[i].c_str());
							        			item.price = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
							        			item.type = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";
							        			item.vendor_id = reply->element[2]->type != REDIS_REPLY_NIL ? reply->element[2]->str : "";
							        			item.advertisement_id = reply->element[3]->type != REDIS_REPLY_NIL ? reply->element[3]->str : "";
							        			item.pc_vendor_id = reply->element[4]->type != REDIS_REPLY_NIL ? reply->element[4]->str : "";
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
										//	for (unsigned int i = 0; i < vAdIDs.size(); i++)
											for (unsigned int i = 0; i < 1; i++)
											{
												if(ads.find(atoi(vAdIDs[i].c_str())) == ads.end())
												{
													cmd = "HMGET advertisments:" + vAdIDs[i] + " type vendor_id dailybudget ecpm click_url enabled exclusive_item_ids excluded_sites";
													reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
													if (m_redisContext->err == 0  && reply->type == REDIS_REPLY_ARRAY)
									        		{
									        			Advertisement ad;
									        			ad.id = atoi(vAdIDs[i].c_str());
									        			ad.type = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
									        			ad.vendor_id = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";
									        			ad.dailybudget = reply->element[2]->type != REDIS_REPLY_NIL ? reply->element[2]->str : "";
									        			ad.eCPM = reply->element[3]->type != REDIS_REPLY_NIL ? reply->element[3]->str : "0";
									        			ad.click_url = reply->element[4]->type != REDIS_REPLY_NIL ? reply->element[4]->str : "";
									        			ad.enabled = reply->element[5]->type != REDIS_REPLY_NIL ? reply->element[5]->str : "";
									        			ad.excluded_item_ids = reply->element[6]->type != REDIS_REPLY_NIL ? reply->element[6]->str : "";
									        			ad.excluded_sites = reply->element[7]->type != REDIS_REPLY_NIL ? reply->element[7]->str : "";
								    	    			if(ad.enabled == "true")
									        			{
									        				string domainstr ="";
									        				if(ad.excluded_sites !="")
									        				{
									        					char domain[250];
																sscanf(url.c_str(), "http://%[^/]", domain);
																string domainstr = string(domain);
																if(domainstr.find("www.") != std::string::npos)
																{
																	domainstr = domainstr.substr(4);
																}
															}
								        					if (domainstr =="" || ad.excluded_sites.find(domainstr) == std::string::npos) 
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
										bool admissingduetouser = false; 
								
										if(vDS->size() > 0)
										{         	
											sort(vDS->begin(),vDS->end(),greater_than_key());
											vendorIDString = vendorIDString + ",";
											std::vector<CAdDataSorter>::iterator It;
											for (It = vDS->begin(); It != vDS->end(); ++It)
											{
												
												Advertisement ad = ads[It->id];

												int count = 0 ;
												if(ad.excluded_item_ids != "")
												{
													std::vector <std::string> eItemIDs;
											    	eItemIDs = parseString(ad.excluded_item_ids);
										    		
											    	for (unsigned int i = 0; i < eItemIDs.size(); i++)
							   		    			{	
							   		    				if(item_ids.find(eItemIDs[i]) != std::string::npos)
							   		    				{
							   		    					count = count +1;
							   		    				}

							   		    			}

							   		    				
												
										    	}
										    	if(count != vItemIDs.size())
										    	{
													if(ad.type == "dynamic")
													{	
														if (vendorIDString.find(ad.vendor_id) != std::string::npos) 
															{

																admatch = true;
																
															}		
													}

													if(ad.type == "dynamicpc")
													{
															if (pcvendorIDString.find(ad.vendor_id) != std::string::npos) 
															{
																		admatch = true;
														   	   
															}
													}

													if(ad.type == "static")
													{
															admatch = true;
													}
												 }
												if(admatch)
												{
													cmd = "EXISTS " + userid + "-Advertisement-" + std::to_string(It->id);
										        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
										        	if (m_redisContext->err == 0  && reply->type != REDIS_REPLY_NIL  && reply->integer == 0)
			        				        		{
									        			advertisementid = std::to_string(It->id);
														eCPM = std::to_string(It->val * eCPMController);
														click_url = ad.click_url;
														
														float bpfloat = It->val*eCPMController;
														float rpfloat = reservePrice/(1000000.0);
														if((reservePrice > 0) && ((bpfloat) < rpfloat))
														{
															// if(eCPMController > .9 && rpfloat < 150)
															// {
															// 	eCPM = 	std::to_string(ceil(rpfloat));
															// }
															// else
															// {
																admatch = false;
																admissingduetouser = true;
																cout << "Missing due to high minimum cpm - " << rpfloat <<" - " << bpfloat << " - url - " << url << endl;
																break;
															// }
														}
													 cout << "Bid Price" << bpfloat << " Reserver Price " << rpfloat << " Final eCPM" << eCPM << " - url - " << url << endl;
													cmd = "SETEX " + userid + "-Advertisement-" + std::to_string(It->id) + " 5 Yes";
									        	    reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());		
													break;

									        		}
									        		else
									        		{
									        			admatch = false;
									        			admissingduetouser = true;
									        		//	 cout << "Missing ad due to user match" << endl;
									        		}
												}
											
											}
											
										}
									
										if(admatch)
										{
											//	cmd = "HINCRBY " + urlKey + " count 1";
									        //	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
									        	// cout << " AdMatch Id " << advertisementid << endl;;
										}	
										else
										{
											if(admissingduetouser)
											{

											}
											else
											{		
											 //	cmd = "HINCRBY missingad:" + url  + " count 1";	
									         //	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
									        //	 cout << "missing ad" << endl;
									        }
										}	
										
									
										redisFree(m_redisContext);
										return admatch;
								}
							else
							{
								 //cout << "Skip this tag or article type" << endl;
								 redisFree(m_redisContext);
								return false;
							}		
			   				
			   				
		         		}
		         		else
		         		{

		         				if(url !="" && url.find("anonymous") == std::string::npos)
		         				{	
									cmd = "HINCRBY missingurl:" + url  + " count 1";	
				        			reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
				        			//cout << "No Match" << endl;

			        				if (m_redisContext->err == 0  && reply->type != REDIS_REPLY_NIL  && reply->integer > 150)
					        		{
					        			/*string curTime = getCurrentTime();
					        			cmd = "SADD missingurl-" + curTime  + " " + url;	
				        				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

				        				cmd = "EXPIRE missingurl-" + curTime  + " " + "86400";	
				        				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());*/

				        				cmd = "SADD missingurl-toplist "  + url;	
				        				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

					        		}
					        		else	
					        		{

									 if(m_redisContext->err == 0  && reply->type != REDIS_REPLY_NIL  && reply->integer > 30 && url.find("youtube.com") != std::string::npos )						        		
				        				{
			        					cmd = "SADD missingurl-toplist "  + url;	
				        				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
				        				}
					        		}
					        	}
					        	else
					        	{
					        	//	cout << "Anonymous or empty url" << endl;
					        	}	

			        			

					        	/*	

									//storing the site related information.
			        				std::string site;
									site = to_string(bidrequestjson["site"]["id"].asInt());
			        					if (tagid == "")
					        			{
					        				tagid = "NoTag";
					        			}
	
					        		cmd = "SADD site:" + site  + " " + tagid;	
					        			reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

				        			cmd = "SETNX spottags:" + tagid + " \"" + ReplaceAll(ReplaceAll(jsonstring,"\"","\\\"")," ","") + "\"";	
				        			reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());*/


			        			redisFree(m_redisContext);
			        			return false;

		         		}	

		         	}
		         	catch(std::exception& e)
		         	{
		         		cout << "*****Exception******" << e.what() << endl;
		         		cout << "Exception url : - " << url << endl;
		         		return false;
		         	}



		}

};

