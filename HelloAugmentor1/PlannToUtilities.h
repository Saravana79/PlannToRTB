#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <map>
#include <math.h>
#include "hiredis-master/hiredis.h"
//#include "jsoncpp-src-0.6.0-rc2/include/json/json.h"

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

class Vertical
{
	public:
		Vertical(void){};
		~Vertical(void){};

		int id;
		float weight;
};

struct greater_than_key_vertical
{
    inline bool operator() (const Vertical& struct1, const Vertical& struct2)
    {
        return (struct1.weight > struct2.weight);
    }
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
	std::string impressions;
	std::string ectr;
	std::string orders;
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
	std::string device;
	std::string miscellanous;
	std::string supported_sizes;
	int total_time;
	std::string skip;
	bool remarketing;
	std::string remarketing_item_ids;
	float val;

	
};

class PlannToUser
{
public:
	PlannToUser(void){};
	~PlannToUser(void){};

	std::string id;
	std::string top_item_ids;
	std::string all_item_ids;
	std::string last_appearance;
	std::string first_appearance;
	std::string appearance_count;
	std::string buying_cycle;
	std::string buying_cycle_date;
	std::string housing_lad;
	std::string source;
	
};


class GoogleUser
{
public:
	GoogleUser(void){};
	~GoogleUser(void){};

	std::string id;
	std::string top_item_ids;
	std::string all_item_ids;
	std::string items_count;
	std::string appearance_count;
	std::string last_appearance;
	std::string first_appearance;
	std::string buying_cycle;
	std::string buying_cycle_date;
};

class videodetails
{
	public:
	videodetails(void){};
	~videodetails(void){};

	bool isVideo;
	int v_duration;
	int v_skipable_duration;
	std::string inventory_type;
	std::string skippable ;
	std::string format;
};

struct Helper
{
 
  Helper(void){};
  ~Helper(void){};

  std::vector<std::string> parseString(std::string str,std::string delimiter)
	{
			std::vector <std::string> vec;
			std::string tmp;
			while( (str.find(delimiter, 0)) != string::npos )
			{
				size_t pos = str.find(delimiter, 0);
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

std::string getCurrentDay()
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    return to_string(now->tm_mday);
}	

std::tm make_tm(int year, int month, int day)
{
    std::tm tm = {0};
    tm.tm_year = year - 1900; // years count from 1900
    tm.tm_mon = month - 1;    // months count from January=0
    tm.tm_mday = day;         // days count from 1
    return tm;
}


int getDiffInDays(string fromdate)
{
	vector<string> datev = parseString(fromdate,"-");
	std::tm tm =  make_tm(atoi(datev[0].c_str()),atoi(datev[1].c_str()),atoi(datev[2].c_str()));
	time_t f = mktime(&tm);
	time_t t = time(0); 
	if ( f != (std::time_t)(-1) && t != (std::time_t)(-1) )
	    {
	        int difference = floor(std::difftime(t, f) / (60 * 60 * 24));
	        return difference;
	    }
    return 10000;
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


 void updateLocationItemsids(std::string & item_ids, int geo_id)
  			{
			//Advertisment which as associated direclty to that type is taken based on item's type.
						    std::string root_item_id;
						    if (geo_id == 1007809)
						    {
						    	//chennai
						    	if(item_ids.find("29673") == std::string::npos)
						    	{
						    		item_ids = item_ids + ",29673";
						    	}
						    }
				    	 	if (geo_id == 1007768)
				    	 	{
				    	 		////bangalore
					    		if(item_ids.find("29574") == std::string::npos)
						    	{
						    		item_ids = item_ids + ",29574";
						    	}
							}
						 	if (geo_id == 1007740)
				    	 	{
				    	 		//hyd
							    if(item_ids.find("29461") == std::string::npos)
						    	{
						    		item_ids = item_ids + ",29461";
						    	}
							}
						 	if (geo_id == 1007788)
				    	 	{
				    	 		//pune
					    		if(item_ids.find("29729") == std::string::npos)
						    	{
						    		item_ids = item_ids + ",29729";
						    	}
							}
						 	if (geo_id == 1007785)
				    	 	{
				    	 		//mumbai
							    if(item_ids.find("29858") == std::string::npos)
						    	{
						    		item_ids = item_ids + ",29858";
						    	}
							}
							if (geo_id == 1007751)
				    	 	{
				    	 		//delhi
				    	 		if(item_ids.find("29758") == std::string::npos)
						    	{
						    		item_ids = item_ids + ",29758";
						    	}

				    	 	}

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
							if (itemtype == "WearableGadget")
				    	 	{
							    	root_item_id = "15432";
							}
							if (itemtype == "Television")
				    	 	{
							    	root_item_id = "22649";
							}
							if (itemtype == "Lens")
				    	 	{
							    	root_item_id = "4126";
							}
							if (itemtype == "Console")
				    	 	{
							    	root_item_id = "15431";
							}
							if (itemtype == "Car")
				    	 	{
							    	root_item_id = "3214";
							}
								if (itemtype == "Bike")
				    	 	{
							    	root_item_id = "3218";
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


void getRequestAndUserDetails(redisContext* m_redisContext, PlannToUser & puser, GoogleUser & guser,string url, string & item_ids, string & article_type)
{
	string prfx = "url:";
	string cmd ="";
	string urlKey = prfx + url;
	redisReply *reply = 0;

	cmd = "HMGET " + urlKey + " item_ids article_type";
	redisAppendCommand(m_redisContext,cmd.c_str());

	 cmd = "GET cm:" + guser.id  ;
	 redisAppendCommand(m_redisContext,cmd.c_str());

	

	 cmd = "HMGET users:buyinglist:" + guser.id  + " item_ids count all_item_ids lad fad ap_c bs bsd";
	 redisAppendCommand(m_redisContext,cmd.c_str());

	//getting item_ids for url 
	redisGetReply(m_redisContext,(void**)&reply);
	//reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

	if ((m_redisContext != NULL && m_redisContext->err == 0) && reply->element[0]->type != REDIS_REPLY_NIL)
	{
	    item_ids = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
		article_type = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";
	}
	else
	{
		item_ids = "";
		article_type ="";

		if(m_redisContext->err != 0)
		{
			cout << "wrong url that is failing -" << url << endl;
			return;
		}
	}

	
	freeReplyObject(reply);

	//getting cookie mapping.

	redisGetReply(m_redisContext,(void**)&reply);

	//cout << guser.id << endl;

	if ((m_redisContext != NULL && m_redisContext->err == 0)  && (reply->type != REDIS_REPLY_NIL))
	{
			puser.id  = reply->type != REDIS_REPLY_NIL ? reply->str : "";
			
	}

	//cout << puser.id << endl;
	freeReplyObject(reply);

	//getting google user details.
	redisGetReply(m_redisContext,(void**)&reply);
	if ((m_redisContext != NULL && m_redisContext->err == 0)  && (reply->type == REDIS_REPLY_ARRAY && reply->element[0]->type != REDIS_REPLY_NIL))
	{
			guser.top_item_ids  = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
			guser.items_count = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";
			guser.all_item_ids = reply->element[2]->type != REDIS_REPLY_NIL ? reply->element[2]->str : "";
			guser.last_appearance  = reply->element[3]->type != REDIS_REPLY_NIL ? reply->element[3]->str : "";
			guser.first_appearance  = reply->element[4]->type != REDIS_REPLY_NIL ? reply->element[4]->str : "";
			guser.appearance_count  = reply->element[5]->type != REDIS_REPLY_NIL ? reply->element[5]->str : "";
		//	cout << "Buying List for this user - " + guser.id + " - item ids " + guser.top_item_ids + " - " + url << endl;; 
	}
	freeReplyObject(reply);

	//getting plannto user details.
	if(puser.id != "")
	{ 
		 cmd = "HMGET users:buyinglist:plannto:" + puser.id  + " item_ids all_item_ids fad lad ap_c bs bsd source housinglad";
		reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
		
		if ((m_redisContext != NULL && m_redisContext->err == 0)  && (reply->type == REDIS_REPLY_ARRAY))
		{
	     	puser.top_item_ids  = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
	 		puser.all_item_ids = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";
	 		puser.first_appearance  = reply->element[2]->type != REDIS_REPLY_NIL ? reply->element[2]->str : "";
	 		puser.last_appearance  = reply->element[3]->type != REDIS_REPLY_NIL ? reply->element[3]->str : "";
	 		puser.appearance_count = reply->element[4]->type != REDIS_REPLY_NIL ? reply->element[4]->str : "";
	 		puser.buying_cycle = reply->element[5]->type != REDIS_REPLY_NIL ? reply->element[5]->str : "";
			puser.buying_cycle_date = reply->element[6]->type != REDIS_REPLY_NIL ? reply->element[6]->str : "";
			puser.source = reply->element[7]->type != REDIS_REPLY_NIL ? reply->element[7]->str : "";
	 		puser.housing_lad = reply->element[8]->type != REDIS_REPLY_NIL ? reply->element[8]->str : "";

	 		
	 	
		}

		if(guser.all_item_ids != "" && (atoi(guser.items_count.c_str()) < 20))
		{
		//	cout << "**********+++++++++++ - cookie matching " << puser.id << " - " <<  puser.all_item_ids << " - " << puser.top_item_ids << " - " << url <<"**********+++++++++++"<< endl;	
	 	//	cout << "**********+++++++++++ - cookie matching " << guser.id << " - "<<  guser.all_item_ids << " - " << guser.top_item_ids  << " - "<< url <<"**********+++++++++++"<< endl;	
		}
		freeReplyObject(reply);
	}
	
}

string getRemarketingItemids(PlannToUser & puser, GoogleUser & guser)
{

	string puseritemids = puser.all_item_ids;
	string guseritemids = guser.all_item_ids;
	std::vector <std::string> pItemIDs = parseString(puser.all_item_ids,",");
	std::vector <std::string> gItemIDs = parseString(guser.all_item_ids,",");

	size_t gsize = gItemIDs.size();
	size_t psize = pItemIDs.size();
	size_t size = 0;
	string r_item_ids ="";
	if(gsize > psize)
	{
		size = gsize;
	}
	else
	{
		size = psize;
	}


	if(size > 20)
	{
		size = 20;
	}

	for (unsigned int i = 0; i < size; i++)
    {
    	if((gsize > i) && (r_item_ids.find(gItemIDs[i]) == std::string::npos))
    	{
    		r_item_ids = r_item_ids + gItemIDs[i] + ",";
    	}

		if((psize > i) && (r_item_ids.find(pItemIDs[i]) == std::string::npos))
    	{
    		r_item_ids = r_item_ids + pItemIDs[i] + ",";
    	}

    }
    return r_item_ids;
}

string getItemsAndDetails(redisContext* m_redisContext,std::vector <std::string> vItemIDs,bool & hasorders, bool & highectr,string & itemtype,std::string & vendorIDString, std::string & pcvendorIDString, GoogleUser guser, bool & userexistswithitemid)
{
	std::map<int,Item> items;
	redisReply *reply = 0;
	string cmd ="";
	string adIdsStr;
	size_t size = vItemIDs.size();
	if(size < 30)
    	{
    	    
    	    if(size > 5)
    	    {
    	    	size = 5;
    	    }

	    	for (unsigned int i = 0; i < size; i++)
			    {

	   		    	cmd = "HMGET items:" + vItemIDs[i] + " price type vendor_id advertisement_id pc_vendor_id impressions ectr orders";
		        	redisAppendCommand(m_redisContext,cmd.c_str());
		        
		     	} 

			for (unsigned int i = 0; i < size; i++)
			    {
	   		    	redisGetReply(m_redisContext,(void**)&reply);
		        
		        	if ((m_redisContext != NULL && m_redisContext->err == 0) && reply->type == REDIS_REPLY_ARRAY)
	        		{
	        			Item item;
	        			item.id = atoi(vItemIDs[i].c_str());
	        			item.price = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
	        			item.type = reply->element[1]->type != REDIS_REPLY_NIL ? reply->element[1]->str : "";
	        			item.vendor_id = reply->element[2]->type != REDIS_REPLY_NIL ? reply->element[2]->str : "";
	        			item.advertisement_id = reply->element[3]->type != REDIS_REPLY_NIL ? reply->element[3]->str : "";
	        			item.pc_vendor_id = reply->element[4]->type != REDIS_REPLY_NIL ? reply->element[4]->str : "";
	        			item.impressions = reply->element[5]->type != REDIS_REPLY_NIL ? reply->element[5]->str : "";
	        			item.ectr = reply->element[6]->type != REDIS_REPLY_NIL ? reply->element[6]->str : "";
	        			item.orders = reply->element[7]->type != REDIS_REPLY_NIL ? reply->element[7]->str : "";

	        			//temporary, dont like to bid for items greater than 300000
	        			if((atof(item.price.c_str()) < 60000.00) || (item.type != "Mobile"))
	        			{
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

		        			if(atoi(item.impressions.c_str()) > 500)
		        			{
		        				if(highectr == false && atof(item.ectr.c_str()) > 0.01)
		        				{
		        					highectr = true;
		        				}
		        				if(hasorders == false && atoi(item.orders.c_str()) > 0)
		        				{
		        					hasorders = true;
		        				}
		        			}
		        			else
		        			{
		        				highectr = true	;
		        				hasorders = true;
		        			}
		        			//whether we have bid for more.
		        			if(guser.top_item_ids.find(vItemIDs[i]) != std::string::npos)
		        			{
		        				userexistswithitemid = true;
		        			}	
		        			
		        			items[item.id] = item;
		        			if(item.type != "CarGroup" && item.type != "CarGroup")
		        			{
		        				itemtype = item.type;
		        			}
		        		}	
		        	
	        			
		        	} 
		        	freeReplyObject(reply);
	        

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
	    		freeReplyObject(reply);

		    }

		  

	  
		}    
	return adIdsStr;
}
  
bool checkSpotTagsPerformance(redisContext* m_redisContext,string & tagid,double & eCPMController,GoogleUser guser,int viewability,string url)
{
	redisReply *reply = 0;
	string cmd ="";

    cmd = "HMGET spottags:" + tagid + " sid_details ectr";	
	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

	
	if((m_redisContext != NULL && m_redisContext->err == 0)  && (reply->type == REDIS_REPLY_ARRAY && reply->element[1]->type != REDIS_REPLY_NIL))
	{
		//eCPMController = atof(reply->element[1]->str) ;
		 	if(eCPMController == 0 || (atoi(guser.items_count.c_str())>30))
				{
					// cout << "Low Probability" << " - url - " <<  url << endl;
					freeReplyObject(reply);
					return false;
				}
		
	}

	freeReplyObject(reply);
	
	if(viewability < 20)
	{
		//cout << " bad viewability - " << viewability << " - " << url << endl;
		return false;
	}
	else
	{
		if(viewability >= 90 && viewability < 101)
		{
			eCPMController = eCPMController + 0.25;
		}

		if(viewability >= 70 && viewability < 90)
		{
			eCPMController = eCPMController + 0.2;
		}

		if(viewability >= 50 && viewability < 70)
		{
			eCPMController = eCPMController + 0.1;
		}


		if((viewability >= 30 && viewability < 50) && url.find("fonearena.com") != std::string::npos && url.find("gsmarena.com") != std::string::npos )
		{
			eCPMController = eCPMController -  0.15;
		}

		if((viewability >= 20 && viewability < 30) && url.find("fonearena.com") != std::string::npos && url.find("gsmarena.com") != std::string::npos )
		{
			eCPMController = eCPMController -  0.25;
		}

	}

	if(url.find("fonearena.com") != std::string::npos || url.find("youtube.com") != std::string::npos || url.find("mysmartprice.com") != std::string::npos || url.find("smartprix.com") != std::string::npos || url.find("pricebaba.com") != std::string::npos || url.find("snapsort.com") != std::string::npos)
	{
		eCPMController = eCPMController + 0.15;
	}

	if(url.find("gsmarena.com") != std::string::npos || url.find("m.gsmarena.com") != std::string::npos )
	{
		eCPMController = eCPMController + 0.25;
	}

	return true;
}


void getAdvertisements(redisContext* m_redisContext,std::map<int,Advertisement> & ads, string adIdsStr, vector<CAdDataSorter> *vDS,string pcvendorIDString,string url,bool isRemarketing, bool isMobile,videodetails vdetails,string format)
{
	std::vector <std::string> vAdIDs;
	redisReply *reply = 0;
	string cmd ="";

	vAdIDs = parseString(adIdsStr,",");
	
	//remove the duplicates
	sort( vAdIDs.begin(), vAdIDs.end() );
	vAdIDs.erase( unique( vAdIDs.begin(), vAdIDs.end() ), vAdIDs.end() );
	
		// HGET advertisments:1 eCPM

	for (unsigned int i = 0; i < vAdIDs.size(); i++)
	{
		   
			cmd = "HMGET advertisments:" + vAdIDs[i] + " type vendor_id dailybudget ecpm click_url status exclusive_item_ids excluded_sites miscellanous remarketing device supported_sizes total_time skip remarketing_item_ids";
			redisAppendCommand(m_redisContext,cmd.c_str());	
	}

	for (unsigned int i = 0; i < vAdIDs.size(); i++)
	{
	    	redisGetReply(m_redisContext,(void**)&reply);
			if ((m_redisContext != NULL && m_redisContext->err == 0)  && reply->type == REDIS_REPLY_ARRAY)
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
				ad.miscellanous = reply->element[8]->type != REDIS_REPLY_NIL ? reply->element[8]->str : "";

				if(reply->element[9]->type != REDIS_REPLY_NIL)
				{
					string temp = reply->element[9]->str;
					if(temp == "true")
					{
						ad.remarketing = true;
					}
					else
					{
						ad.remarketing = false;
					}

				}
				else
				{
					ad.remarketing = false;
				}
				ad.device = reply->element[10]->type != REDIS_REPLY_NIL ? reply->element[10]->str : "";
				ad.supported_sizes = reply->element[11]->type != REDIS_REPLY_NIL ? reply->element[11]->str : "";
				ad.total_time = reply->element[12]->type != REDIS_REPLY_NIL ? atoi(reply->element[12]->str) : 0;
				ad.skip = reply->element[13]->type != REDIS_REPLY_NIL ? reply->element[13]->str : "";
				ad.remarketing_item_ids = reply->element[14]->type != REDIS_REPLY_NIL ? reply->element[14]->str : "";

				if(ad.eCPM == "")
				{
					ad.eCPM = "0";
				}

				// valid ad and supported size check.
				if(ad.enabled == "enabled" && (ad.supported_sizes == "" || ad.supported_sizes.find(format) != std::string::npos))
				{
					//whether its video ad
					if((ad.type =="video" && vdetails.isVideo) || (ad.type != "video" && !vdetails.isVideo))
					{	
						if(!vdetails.isVideo || ((vdetails.v_skipable_duration >= ad.total_time || vdetails.v_duration >= ad.total_time) && vdetails.inventory_type == "0" && vdetails.format.find("300x60") != std::string::npos && vdetails.skippable != "BLOCK_SKIPPABLE"))
						{
							//device support check
							if((ad.device.find("mobile") != std::string::npos && isMobile) || (ad.device.find("pc") != std::string::npos && !isMobile))
							{
								//remareting support.
								if((ad.remarketing && isRemarketing) || (!isRemarketing))
								{
									string domainstr ="";

									//exlcuded site check
									if(ad.excluded_sites !="")
									{
										char domain[250];
										sscanf(url.c_str(), "http://%[^/]", domain);
										sscanf(url.c_str(), "https://%[^/]", domain);
									    domainstr = string(domain);
							
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
										if(pcvendorIDString.find(ad.vendor_id) == 0 && ad.type == "dynamicpc")
										{
											//Adding extra amount (20%) if the this vendor is selling less price.
											ds.val = stof(ad.eCPM)* 1.2;	
											//eCPMController = eCPMController + 0.2;																
										}
										else
										{
											ds.val = stof(ad.eCPM);
										}
										vDS->push_back(ds);


									}
								}

							}
							else
							{
								//cout << "Not supported device type" << isMobile << endl;
							}	
						}
					}	
				}
	    	} 
	    	freeReplyObject(reply);
	   } 	
}													

bool processAdvertisement(redisContext* m_redisContext,std::map<int,Advertisement> ads, vector<CAdDataSorter> *vDS,string vendorIDString,string pcvendorIDString, bool highectr, bool hasorders,double eCPMController, double reservePrice, string & item_ids, std::vector <std::string> vItemIDs,string & eCPM, string & advertisementid, string & click_url,string url, GoogleUser guser,PlannToUser puser,std::string & returnValue, bool isRemarketing, string add_details, string & article_type)
{
	redisReply *reply = 0;
	string cmd ="";
	bool admatch = false;
	sort(vDS->begin(),vDS->end(),greater_than_key());
	vendorIDString = vendorIDString + ",";
	std::vector<CAdDataSorter>::iterator It;

	
	for (It = vDS->begin(); It != vDS->end(); ++It)
	{
		Advertisement ad = ads[It->id];
		std::size_t pcFound=1;
		int count = 0 ;
		if(ad.excluded_item_ids != "")
		{
			std::vector <std::string> eItemIDs;
	    	eItemIDs = parseString(ad.excluded_item_ids,",");
    		
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

				if(isRemarketing && admatch)
				{
				//	cout << "remark item id actual " << ad.remarketing_item_ids << endl;
					if(ad.remarketing_item_ids != "")
						{
							if(ad.remarketing_item_ids.find("3215") == std::string::npos && ad.remarketing_item_ids.find("3216") == std::string::npos && ad.remarketing_item_ids.find("3217") == std::string::npos && ad.remarketing_item_ids.find("15412") == std::string::npos && ad.remarketing_item_ids.find("22649") == std::string::npos && ad.remarketing_item_ids.find("15431") == std::string::npos && ad.remarketing_item_ids.find("4126") == std::string::npos)
							{
								bool remarketingmatch = false;
						    	std::string r_new_item_ids ="";
					    		
						    	for (unsigned int i = 0; i < vItemIDs.size(); i++)
					    			{	
					    				if(ad.remarketing_item_ids.find(vItemIDs[i]) != std::string::npos)
					    				{
				    						if(vItemIDs[i] != "")
					    					{
					    						remarketingmatch = true;
					    						if(r_new_item_ids.find(vItemIDs[i]) == std::string::npos)
					    						{
					    							r_new_item_ids = r_new_item_ids + vItemIDs[i] + ",";
					    						}
					    					}
					    				}

					    			}
									admatch = remarketingmatch;

									//cout <<  " ad match for remarketing " << admatch << r_new_item_ids << endl;

								if(admatch == true)
								{
									item_ids = r_new_item_ids;
								}
							}
							else
							{
								admatch = true;
							}
			    	}


			       std::vector <std::string> vReItemIDs;
          		   vReItemIDs = parseString(item_ids,",");

          		  size_t size = vReItemIDs.size();
				  string f_new_item_ids = "";
				  if(size > 6)
					{

						for (unsigned int i = 0; i < 6; i++)
	    					{
	    						f_new_item_ids = f_new_item_ids + vReItemIDs[i] + ",";
	    					}

	    				item_ids =f_new_item_ids;	
	    			}

				}	
			}

			if(ad.type == "dynamicpc")
			{
					 pcFound = pcvendorIDString.find(ad.vendor_id);
					if (pcFound != std::string::npos) 
					{
							admatch = true;														   	   
					}

					if( (admatch) && ad.miscellanous.find("highectr") != std::string::npos)
					{
						admatch = highectr;
					}
					if((admatch) && ad.miscellanous.find("hasorders") != std::string::npos)
					{
						admatch = hasorders;
					}
					
					if((admatch) && ad.miscellanous.find("lowestprice") != std::string::npos)
					{
						if(pcFound != 0)
						{
							admatch = false;
						}
					}

					if((admatch) && isRemarketing)
					{
						if(pcFound != 0)
						{
							//admatch = false;
						}
					}


			}

			if(ad.type == "video")
			{
					// if (pcvendorIDString.find(ad.vendor_id) != std::string::npos) 
					// {
					// 		admatch = true;														   	   
					// }
					admatch = true;
			}

			if(ad.type == "static" )
			{
				admatch = true;
			}


			if(ad.type == "fashion" )
			{
				admatch = true;
			}


			if(ad.type == "resale" )
			{
				if(article_type == "Resale")
				{
					admatch = true;
				}
			}

		}
		
		if(admatch)
		{
				// if(pcFound == 0)
				// {
				// 	eCPMController = eCPMController + 0.25;
				// 	admatch = true;
				// }	

        		float bpfloat = It->val*eCPMController;
        		float rpfloat = reservePrice/(1000000.0);

				if((reservePrice > 0) && ((bpfloat) < rpfloat))
				{
					if (bpfloat * 1.15 > rpfloat)
					{
						bpfloat = bpfloat * 1.15; 
						eCPMController = eCPMController + .15;
					}
				  	else
					{
							admatch = false;
					//		admissingduetouser = true;
							if(It->id < 40)
							{
								cout << "Missing due to high minimum cpm - " << rpfloat <<" - " << bpfloat << " - url - " << url << " - id - " << std::to_string(It->id) << endl;
							}
							returnValue  = "MissingDueHighCPM";
							break;
					}
				}

				cmd = "HMGET " + guser.id + "-Advertisement-" + std::to_string(It->id) + " url count";

	        	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
	        	
	        	string tempurl = url;
	        	if(url =="")
	        	{
	        		tempurl = "emptyurl";
	        	}

	        	if(isRemarketing)
	        	{
	        		tempurl = "remarketingurl";
	        	}
	        	
	        	if(advertisementid == "34")
	        	{
	        			char domain[250];
						sscanf(url.c_str(), "http://%[^/]", domain);
						sscanf(url.c_str(), "https://%[^/]", domain);
					    tempurl = string(domain);
				}
				if ((m_redisContext != NULL && m_redisContext->err == 0)  && ((reply->type == REDIS_REPLY_ARRAY) || (reply->type == REDIS_REPLY_NIL)) )
				{
						string prevurl  = reply->element[0]->type != REDIS_REPLY_NIL ? reply->element[0]->str : "";
						int count  = reply->element[1]->type != REDIS_REPLY_NIL ? atoi(reply->element[1]->str) : 0;
						int maxcount = 1;

						if(It->id == 21)
						{
						//	maxcount = 2;
						}

					 if(prevurl != tempurl || count < maxcount)
					 {	
		        		 freeReplyObject(reply);

		    			 cmd = "HMSET " + guser.id + "-Advertisement-" + std::to_string(It->id) + " url " + tempurl ;
						 redisAppendCommand(m_redisContext,cmd.c_str());

						 cmd = "HINCRBY "+ guser.id + "-Advertisement-" + std::to_string(It->id) + " count 1";	
						 redisAppendCommand(m_redisContext,cmd.c_str());
					
						 cmd = "EXPIRE " + guser.id + "-Advertisement-" + std::to_string(It->id)  + " " + "200";	
						 redisAppendCommand(m_redisContext,cmd.c_str());
						 
						 if(puser.id != "")
						 {
							cmd = "GET pu:" + puser.id  + ":"+ std::to_string(It->id) + ":count";
						 	redisAppendCommand(m_redisContext,cmd.c_str());

						 	cmd = "GET pu:" + puser.id  + ":"+ std::to_string(It->id) + ":" + getCurrentDay();
						 	redisAppendCommand(m_redisContext,cmd.c_str());

						 	cmd = "GET pu:" + puser.id  + ":"+ std::to_string(It->id) + ":clicks:count";
						 	redisAppendCommand(m_redisContext,cmd.c_str());

						 	cmd = "GET pu:" + puser.id  + ":"+ std::to_string(It->id) + ":clicks:" + getCurrentDay();
						 	
						 	redisAppendCommand(m_redisContext,cmd.c_str());
						 }

						redisGetReply(m_redisContext,(void**)&reply);
						freeReplyObject(reply);	
						redisGetReply(m_redisContext,(void**)&reply);
						freeReplyObject(reply);
						redisGetReply(m_redisContext,(void**)&reply);
						freeReplyObject(reply);

						int addailyCount =0;
						int adtotalCount =0;
						int adClickCount =0;
						int adClicktotalCount =0;

						if(puser.id != "")
						{

							redisGetReply(m_redisContext,(void**)&reply);
							if ((m_redisContext != NULL && m_redisContext->err == 0)  && (reply->type != REDIS_REPLY_NIL))
							{
									adtotalCount = reply->type != REDIS_REPLY_NIL ? atoi(reply->str) : 0;
									
							}
							freeReplyObject(reply);

							redisGetReply(m_redisContext,(void**)&reply);

							if ((m_redisContext != NULL && m_redisContext->err == 0)  && (reply->type != REDIS_REPLY_NIL))
							{
									addailyCount  = reply->type != REDIS_REPLY_NIL ?  atoi(reply->str) : 0;
							}
							freeReplyObject(reply);

							redisGetReply(m_redisContext,(void**)&reply);

							if ((m_redisContext != NULL && m_redisContext->err == 0)  && (reply->type != REDIS_REPLY_NIL))
							{
									adClicktotalCount  = reply->type != REDIS_REPLY_NIL ?  atoi(reply->str) : 0;
							}
							freeReplyObject(reply);

							redisGetReply(m_redisContext,(void**)&reply);

							if ((m_redisContext != NULL && m_redisContext->err == 0)  && (reply->type != REDIS_REPLY_NIL))
							{
									adClickCount  = reply->type != REDIS_REPLY_NIL ?  atoi(reply->str) : 0;
							}
							freeReplyObject(reply);	        	

	    				}	
	    			 	//cout << addailyCount << adtotalCount << adClickCount << endl;
	    			  if(addailyCount < 15 && adtotalCount < 25 && adClickCount < 1)
					    {
					    		returnValue = "BidForUserImpressionCount" + to_string(addailyCount);
					    		advertisementid = std::to_string(It->id);
					    		if((advertisementid == "32" || advertisementid == "33") && (add_details != "BangGeneric" && add_details != "TooGeneric"))
					    		{
					    			if(addailyCount == 0)
					    			{
					    				eCPM = std::to_string(bpfloat * 3);
					    			}
					    			else
					    			{
						    			if(addailyCount < 4)
						    			{
						    				eCPM = std::to_string(bpfloat * 2);
						    			}
						    			else
						    			{
						    				if(addailyCount <  10)
						    				{
						    					eCPM = std::to_string(bpfloat * 1.25);
						    				}
						    			}
					    			}
					    		}
					    		else
					    		{
									eCPM = std::to_string(bpfloat);
								}
								click_url = ad.click_url;
							    //cout << "Bid Price" << bpfloat << " Reserver Price " << rpfloat << " Final eCPM" << eCPM << " - url - " << url << endl;
								break;
						}
						else
						{
							returnValue = "MissingDueToFrequencyCap";
							admatch = false;
						}
					}
					else
					{
						returnValue = "MissingBidsDueToUser";
        				admatch = false;
					}
				
        		}
        		else
        		{
        			returnValue = "MissingBidsDueToUser";
        			admatch = false;
        		//	admissingduetouser = true;
        		//	 cout << "Missing ad due to user match" << endl;
        		}
		}
		
	}

	return admatch;						
}


string getItemIDForFashion(string url)
{
	string temp_item_ids = "";
	if(url.find("saree") != std::string::npos || url.find("blouse") != std::string::npos)
	{
		temp_item_ids = "72274";
	}
	else
	{
		if(url.find("salwar") != std::string::npos || url.find("anarkali") != std::string::npos || url.find("patiala") != std::string::npos)
		{
			temp_item_ids = "72275" ;
		}
		else
		{
			if(url.find("sneaker") != std::string::npos || url.find("foot-wear") != std::string::npos)
			{
				temp_item_ids = "72353";
			}
		}
	}

	return temp_item_ids;
}


bool canRemarketingPossible(GoogleUser guser,PlannToUser puser,bool & isRemarketingBlocked)
{
	if(!isRemarketingBlocked)
	{
          if(puser.id != "" && guser.id != "")
          {
          				std::vector <std::string> vReItemIDs;
          				//cout << puser.all_item_ids << endl;
				    	vReItemIDs = parseString(puser.all_item_ids,",");

				    	if((guser.last_appearance != "" && getDiffInDays(guser.last_appearance) <= 3) || (puser.last_appearance != "" && getDiffInDays(puser.last_appearance) <= 3) )
				    	{	
				    		if((puser.top_item_ids !="" || guser.top_item_ids !=""))
							{
								return true;
							}
					    	for (unsigned int i = 0; i < vReItemIDs.size(); i++)
							{
								if((guser.top_item_ids !="") && (guser.last_appearance != "" && getDiffInDays(guser.last_appearance) <= 2) &&  (puser.all_item_ids !="") && (guser.top_item_ids.find(vReItemIDs[i]) != std::string::npos))
								{
									return true;
								}
							
							}

						 if(((puser.buying_cycle == "true" && getDiffInDays(puser.buying_cycle_date) <= 1) || (guser.buying_cycle == "true" && getDiffInDays(guser.buying_cycle_date) <= 1)) && puser.all_item_ids != "" )
						 {
						 	return true;
						 }

						 if(guser.all_item_ids.find("5727") != std::string::npos || guser.all_item_ids.find("18093") != std::string::npos || guser.all_item_ids.find("28712") != std::string::npos || puser.all_item_ids.find("5727") != std::string::npos || puser.all_item_ids.find("18093") != std::string::npos || puser.all_item_ids.find("28712") != std::string::npos)
						 {
						 	//cout << " user for remarketing " << guser.id <<  " " << guser.all_item_ids << "   " << puser.all_item_ids << endl;
						 	return true;
						 }
						}
          }
      
	}
		return false;
}

bool canHousingRemarketing(GoogleUser guser,PlannToUser puser,bool & isRemarketingBlocked)
{
	if(!isRemarketingBlocked)
	{
          if(puser.id != "" && puser.source.find("housing") != std::string::npos)
          {
          		    	if((puser.housing_lad == "" || getDiffInDays(puser.housing_lad) <= 20) )
				    	{	
				    			//user is from housing.com
				    		//	cout << "user is from housing - " <<  guser.id << endl;
				    			return true;
						}
	
          }
      
	}
	return false;
	
}

void processMissingURL(redisContext* m_redisContext,string url,string & returnValue, string groupid,string strverticals)
{

	if((url.find("gsmarena.com") == std::string::npos) && (groupid =="16025452000"))
	{
	//	cout << "Inside missing url - " << url << endl;
	}

	redisReply *reply = 0;
	string cmd;
	bool processmissing = false;
	if(m_redisContext->err == 0)
		{
			if(url !="" && url.find("anonymous") == std::string::npos)
			{	
				cmd = "HINCRBY missingurl:" + url  + " count 1";	
			reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			//cout << "Missing url - " << url << endl;

			if (m_redisContext->err == 0  && reply->type == REDIS_REPLY_INTEGER  && reply->integer > 30)
    		{
    			/*string curTime = getCurrentTime();
    			cmd = "SADD missingurl-" + curTime  + " " + url;	
				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

				cmd = "EXPIRE missingurl-" + curTime  + " " + "86400";	
				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());*/

				processmissing = true;
				
    		}
    		else	
    		{

			 if(m_redisContext->err == 0  && reply->type == REDIS_REPLY_INTEGER  && reply->integer > 15 && (url.find("youtube.com") != std::string::npos || url.find("review") != std::string::npos || url.find("vs") != std::string::npos ))						        		
				{
					processmissing = true;
				
				}

			 else
			 {
			 	if(m_redisContext->err == 0  && reply->type == REDIS_REPLY_INTEGER  && reply->integer > 10 && groupid == "20616639040"  && (url.find("used") != std::string::npos || url.find("second-hand") != std::string::npos || url.find("classified") != std::string::npos ))						        		
				{
				
					processmissing = true;
				}
			 }
    		}

    		if(processmissing)
    		{
				cmd = "HGET missingurl:" + url  + " feed_url_id";	
				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

				if(m_redisContext->err == 0  && reply->type == REDIS_REPLY_NIL )						        		
				{	
	    			cmd = "SADD missingurl-toplist "  + url;	
					redisAppendCommand(m_redisContext,cmd.c_str());

					cmd = "HSET missingurl:" + url + " verticals " + strverticals;
					redisAppendCommand(m_redisContext,cmd.c_str());

					redisGetReply(m_redisContext,(void**)&reply);
					redisGetReply(m_redisContext,(void**)&reply);
				}
    		}




    		freeReplyObject(reply);
    		returnValue = "MissingUrlImpressions";
    	}
    	else
    	{
    		returnValue = "AnonymousImpressions";
    	//	cout << "Anonymous or empty url" << endl;
    	}		
	}	
}
bool processBidURLWrapper(std::string url, std::string & item_ids, std::string & advertisementid, std::string & eCPM,std::string & click_url, Json::Value bidrequestjson,std::string jsonstring,bool & isRemarketingBlocked, bool & bidthroughRemarketing,bool & hastaggingrestricted, string & add_details)
{
	
	//const char *hostname = "rtbtarget.plannto.com";
	const char *path = "/tmp/redis.sock";
	
	// const char *hostname = "127.0.0.1";
	// int port = 6379;
	 redisContext* m_redisContext;
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	//m_redisContext = redisConnectWithTimeout(hostname, port, timeout);
	m_redisContext = redisConnectUnixWithTimeout(path, timeout);
	
	if (m_redisContext == NULL || m_redisContext->err) 
		{
			if (m_redisContext) {
				printf("Connection error: %s\n", m_redisContext->errstr);
				redisFree(m_redisContext);
			} else {
				printf("Connection error: can't allocate redis context\n");
			}
			return false;
		}

			
	std::string returnValue = "";
	bool isRemarketingPossible = false;
	//	cout << "URL - start " <<  url << endl;
	bool admatch = processBidURL(m_redisContext,url, item_ids, advertisementid,eCPM, click_url,bidrequestjson,jsonstring,returnValue,isRemarketingBlocked,isRemarketingPossible, bidthroughRemarketing,hastaggingrestricted,add_details);

	//cout << "URL - end" << url << endl;
	// //Update statistics
	// int counter = 1;
	 redisReply *reply = 0;

	 string cmd="";
	// if (m_redisContext != NULL && m_redisContext->err == 0) 
	// {
	// 	cmd = "INCR adengine:impressions:" + getCurrentDay();
	// 	redisAppendCommand(m_redisContext,cmd.c_str());
		
	// 	if(returnValue == "BadSpot")
	// 	{
	// 		cmd = "INCR adengine:impressions:badspot:" + getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}
	
	// 	if(returnValue == "BiddableBuyerImpression" || returnValue.find("BidForUserImpressionCount") != std::string::npos)
	// 	{
	// 		cmd = "INCR adengine:impressions:biddablebuyerimpression:" + getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}

	// 	if(returnValue.find("BidForUserImpressionCount") != std::string::npos)
	// 	{
	// 		cmd = "INCR adengine:impressions:" + returnValue + ":" + getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}

	// 	if(returnValue.find("MissingAdImpression") != std::string::npos)
	// 	{
	// 		cmd = "INCR adengine:impressions:missingad:" + getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}


	// 	if(returnValue.find("BiddableOwnerImpression") != std::string::npos)
	// 	{
	// 		cmd = "INCR adengine:impressions:ownerimpression:" +  getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}

		

	// 	if(returnValue.find("MissingUrlImpressions") != std::string::npos)
	// 	{
	// 		cmd = "INCR adengine:impressions:missingurl:" +  getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}

		

	// 	if(returnValue.find("AnonymousImpressions") != std::string::npos)
	// 	{
	// 		cmd = "INCR adengine:impressions:anonymous:" +  getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}

		

	// 	if(returnValue.find("MissingBidsDueToUser") != std::string::npos)
	// 	{
	// 		cmd = "INCR adengine:impressions:missingbidsduetouser:"  + getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}

	// 	if(isRemarketingPossible)
	// 	{
	// 		cmd = "INCR adengine:impressions:possibleremarketing:"  + getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}

	// 	if(returnValue.find("MissingDueHighCPM") != std::string::npos)
	// 	{
	// 		cmd = "INCR adengine:impressions:missingduetohighcpm:"  + getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}

	// 	if(returnValue.find("MissingURLRemarketing") != std::string::npos)
	// 	{
	// 		cmd = "INCR adengine:impressions:missingurlremarketing:"  + getCurrentDay();
	// 		redisAppendCommand(m_redisContext,cmd.c_str());
	// 		counter = counter +1;
	// 	}
		
	// 	for( int a = 0; a < counter; a = a + 1 )
	// 	   {
	// 	   	redisGetReply(m_redisContext,(void**)&reply);
	// 		freeReplyObject(reply);	
	//        }
	// }

	//if(returnValue != "BadSpot" && returnValue != "MissingURLRemarketing")
	if(bidrequestjson["imp"][0]["pmp"]["ext"]["adgroup_id"].asString() == "16951559440" && returnValue != "AnonymousImpressions" )
	{

	//	cout << url << " - " << bidrequestjson["user"]["id"].asString() << " - " << returnValue << " -"  << bidrequestjson["imp"][0]["tagid"].asString() << add_details << bidrequestjson["device"]["ext"]["geo_criteria_id"] << endl;
	}



	  if (m_redisContext != NULL && m_redisContext->err == 0 ) 
	  {
	 	  if(admatch)
	 	  {
	 		cmd = "SET request:id:" + bidrequestjson["id"].asString() + " " + url + "<<" + advertisementid +  "<<" + eCPM  ;
			redisAppendCommand(m_redisContext,cmd.c_str());
			//cout << cmd << endl;
			//cout << "bid request id" << bidrequestjson["id"].asString() << endl;
			cmd = "EXPIRE request:id:" + bidrequestjson["id"].asString()  + " " + "300";	
			redisAppendCommand(m_redisContext,cmd.c_str());
		  }

		 Json::Value pmpNode = bidrequestjson["imp"][0]["pmp"]["ext"]["feedback"];
		 string requestid = "";
		 int status_code=0;
		 //long cpm_micros=0;

		 if(!pmpNode.isNull())
		 {
		 	requestid = pmpNode[0]["request_id"].asString();
		 	status_code = pmpNode[0]["status_code"].asInt();
		 	//cpm_micros = pmpNode[0]["cpm_micros"].asInt();
		 	//cout << "feedback request id" << requestid << endl;
	 		cmd = "GET request:id:" + requestid   ;
			redisAppendCommand(m_redisContext,cmd.c_str());

		  }



		if(admatch)
		{
			redisGetReply(m_redisContext,(void**)&reply);
		     if (m_redisContext != NULL && m_redisContext->err == 0)
			{
				freeReplyObject(reply);	
		    	redisGetReply(m_redisContext,(void**)&reply);
	    		freeReplyObject(reply);	
			}
		}

		 if(!pmpNode.isNull())
		 {
		 	   redisGetReply(m_redisContext,(void**)&reply);

			    
		 	 if (m_redisContext != NULL && m_redisContext->err == 0 && reply->type != REDIS_REPLY_NIL  && status_code != 1 )
				{
					   string tempstr = reply->str;

					   //if(tempstr.find("<<43<<") != std::string::npos || tempstr.find("<<44<<") != std::string::npos)
					   {
					   	
					  		cout  << "Feedback  - "  << tempstr<< " - status code - "  << status_code  << " - wp - " << pmpNode[0]["cpm_micros"] << endl;
					  	}
				}

				freeReplyObject(reply);	
		 }
			
	
	 }
	redisFree(m_redisContext);			
	return admatch;
}

bool bidforrealestate(string & item_ids, string groupid, int geo_id, string url,string & add_details, double & eCPMController)
{
	return false;
	if(groupid == "18102001720")
	{
        
	    
		if(geo_id == 1007768 && (url.find("bus") == std::string::npos  && url.find("job") == std::string::npos && url.find("movie") == std::string::npos && url.find("dictionary") == std::string::npos 
				&& url.find("helpdesk") == std::string::npos && url.find("icafemanager") == std::string::npos && url.find("car") == std::string::npos && url.find("bike") == std::string::npos && url.find("school") == std::string::npos && url.find("pincode") == std::string::npos 
				&& url.find("weather") == std::string::npos && url.find("recruitment") == std::string::npos && url.find("cookcounty") ==  std::string::npos && url.find(".au") ==  std::string::npos && url.find("mlsli") ==  std::string::npos  && url.find("guest") ==  std::string::npos
				&& url.find("gossip") ==  std::string::npos && url.find("trips") ==  std::string::npos && url.find("celebrit") ==  std::string::npos && url.find("pharma") ==  std::string::npos && url.find("Cinema") ==  std::string::npos))
		{



			// cout << "real estate" <<  url << geo_id << endl;

		    if(url.find("commonfloor") != std::string::npos  || url.find("rentals.sulekha.com") != std::string::npos || url.find("olx.in") != std::string::npos )
		    {
		    	item_ids = "29574";
		    	cout << "bidding for real estate " << url << " - "  << geo_id << endl;
		    }

			// if(item_ids.find("29574") == std::string::npos && item_ids.find("29575") == std::string::npos)
			// {
			// 	add_details = "BangGeneric";
			// 	if(url.find("apartment") == std::string::npos)
			// 	{
			// 		eCPMController = eCPMController - 0.3;
			// 	}

			// 	if(item_ids == "")
			// 	{
			// 		item_ids = "29574";

			// 	}
			// 	else
			// 	{

			// 		item_ids = item_ids + ",29574" ;
			// 	}
			// }
			// else
			// {
			// 	eCPMController = eCPMController + 0.3;
			// 	add_details = "BangSpecific";
			// }

			// if(item_ids.find("29979") != std::string::npos || item_ids.find("60109") != std::string::npos || item_ids.find("67548") != std::string::npos || item_ids.find("60021") != std::string::npos)
			// {
			// 	eCPMController = eCPMController + 0.3;
			// 	add_details = "BangVerySpecific";
			// }	
			// return true;
		}	

		// if(item_ids.find("35284") != std::string::npos || item_ids.find("35236") != std::string::npos || item_ids.find("35318") != std::string::npos || item_ids.find("71602") != std::string::npos)
		// {
		// 	add_details = "VerySpecific";
		// 	return true;
		// }
	// }
	// else
	// {
	// 	if(geo_id == 1007768 && (url.find("bang") != std::string::npos || url.find("bangalore") != std::string::npos || url.find("house") != std::string::npos || url.find("land") != std::string::npos || url.find("apartment") != std::string::npos))
	// 	{
	// 		add_details = "TooGeneric";

	// 			if(item_ids == "")
	// 			{
	// 				item_ids = "29574";

	// 			}
	// 			else
	// 			{

	// 				item_ids = item_ids + ",29574" ;
	// 			}
	// 		return true;
	// 	}
	}
	
	return false;
}

bool processBidURL(redisContext* m_redisContext,std::string url, std::string & item_ids, std::string & advertisementid, std::string & eCPM,std::string & click_url, Json::Value bidrequestjson,std::string jsonstring,std::string & returnValue,bool & isRemarketingBlocked,bool & isRemarketingPossible, bool & bidthroughRemarketing,bool & hastaggingrestricted,string & add_details)
{		
		try
		{	
			
			std::map<int,Advertisement> ads;
			vector<CAdDataSorter> *vDS = new vector<CAdDataSorter>();
			vector<Vertical> *verticals = new std::vector<Vertical>();
			PlannToUser puser ;
			GoogleUser guser;
			std::string adIdsStr ;
			std::string vendorIDString ="";
			std::string pcvendorIDString ="";
			std::string itemtype ="";
			url = UriDecode(url);
			
        	std::string cmd = "";
       		redisReply *reply = 0;
    		std::string tagid;
    		std::string aduserstr;
    		bool userexistswithitemid = false;
    		bool highectr = false;
    		bool hasorders = false;
    		std::string article_type;
    		double eCPMController = 1.0; 
    		long reservePrice =0;
    		bool isMobile = false;
    		bool isFashion = false;
    		videodetails  vdetails;
    		vdetails.isVideo = false;
    		string format = "1x1";
    		string strverticals  ="";
    		string org_item_ids ="";

    		guser.id  = bidrequestjson["user"]["id"].asString();
    		tagid = bidrequestjson["imp"][0]["tagid"].asString();

    		if(!bidrequestjson["imp"][0]["reservePrice"].isNull())
    		{
    			reservePrice  = bidrequestjson["imp"][0]["reservePrice"][0].asInt();
    		}

    		if(!bidrequestjson["segments"]["AdxDetectedVerticals"].isNull())
    		{
    			if(bidrequestjson["segments"]["AdxDetectedVerticals"].size() > 0)
    			{
    						Vertical ver;


    					for (unsigned int i = 0; i < bidrequestjson["segments"]["AdxDetectedVerticals"].size(); i++)
						    {
							if(bidrequestjson["segments"]["AdxDetectedVerticals"][i].isString())
						    	{
							    	std::string str = bidrequestjson["segments"]["AdxDetectedVerticals"][i].asString();
							    	size_t pos = str.find(":", 0);
									std::string tmp = str.substr(0, pos);
									str.erase(0, pos + 1);
									ver.id = stoi(tmp);
	    							ver.weight = stof(str);
	    							verticals->push_back(ver);
    							}
    							else
    							{
    								if(bidrequestjson["segments"]["AdxDetectedVerticals"][i].isInt())
						    		{
						    			ver.id = bidrequestjson["segments"]["AdxDetectedVerticals"][i].asInt();
	    								ver.weight = 1.0;
	    								verticals->push_back(ver);

						    		}
						    		else
						    		{
						    			cout << "Invalid verticals" << url << " - " <<  bidrequestjson["segments"]["AdxDetectedVerticals"][i] << endl;
    								}
    							}
    						}	
    						sort(verticals->begin(),verticals->end(),greater_than_key_vertical());
							for (std::vector<Vertical>::iterator It = verticals->begin();  It != verticals->end(); ++It)
								{
									strverticals = strverticals + to_string(It->id) + ",";
    							}
    			}

    		}
    	

    		string groupid="";
    		if(!bidrequestjson["imp"][0]["pmp"]["ext"]["adgroup_id"].isNull())
    		{
    			groupid = bidrequestjson["imp"][0]["pmp"]["ext"]["adgroup_id"].asString();
    		}

    			if((url.find("used") != std::string::npos || url.find("second-hand") != std::string::npos || url.find("classified") != std::string::npos) && groupid == "20616639040")  
    		{
    			//cout << "used cars  -  " << url << " - " << strverticals << " - " << groupid << " - " << guser.id << endl;; 
    		}


    		//	cout << "All items  -  " << url << " - " << strverticals << " - " << groupid << endl;
    		
    		
    		
    		if(!bidrequestjson["device"]["devicetype"].isNull() && bidrequestjson["device"]["devicetype"] == 1)
    		{
    			 isMobile = true;
    		}

    		if(!bidrequestjson["imp"][0]["video"].isNull())
    		{
    			vdetails.isVideo = true;
    		//	cout << " video -  "<< url << " - " << endl ;
    		}
    		if(groupid =="21902983000")
    		{
    			if((!bidrequestjson["user"]["ext"]["gender"].isNull()) && (bidrequestjson["user"]["ext"]["gender"] != "UNKNOWN"))
    			{
						cout << " Fashion User details - " << url << " - " << bidrequestjson["user"]["ext"]["gender"].asString() << " - " << bidrequestjson["user"]["ext"]["age_low"].asString() << " - " << bidrequestjson["user"]["ext"]["age_high"].asString() << endl;
				}	
				//cout << " Fashion " << url << endl;
				isFashion = true;

			}



			if((!bidrequestjson["user"]["ext"]["gender"].isNull()) && (bidrequestjson["user"]["ext"]["gender"] != "UNKNOWN"))
			{
				//	cout << " ***********************" << endl;
				//	cout << " Gender available - " << url << " - " << bidrequestjson["user"]["ext"]["gender"].asString() << " - " << bidrequestjson["user"]["ext"]["age_low"].asString() << " - " << bidrequestjson["user"]["ext"]["age_high"].asString() << endl;
				//	cout << " ***********************" << endl;
			}

    
    		if(!bidrequestjson["imp"][0]["video"]["ext"]["skippable_max_ad_duration"].isNull())
    		{
    			vdetails.v_skipable_duration = atoi(bidrequestjson["imp"][0]["video"]["ext"]["skippable_max_ad_duration"].asString().c_str());
    		}

			if(!bidrequestjson["imp"][0]["video"]["max_ad_duration"].isNull())
    		{
    			vdetails.v_duration = bidrequestjson["imp"][0]["video"]["max_ad_duration"].asInt();
    		}

			if(!bidrequestjson["imp"][0]["video"]["ext"]["video_ad_skippable"].isNull())
    		{
    			vdetails.skippable = bidrequestjson["imp"][0]["video"]["ext"]["video_ad_skippable"].asString();
    		}

    		if(!bidrequestjson["imp"][0]["video"]["companionad"][0]["w"].isNull())
    		{
    			if(bidrequestjson["imp"][0]["video"]["companionad"][0]["w"].size() > 1)
    			{
    			vdetails.format = std::to_string(bidrequestjson["imp"][0]["video"]["companionad"][0]["w"][0].asInt()) + "x" +std::to_string(bidrequestjson["imp"][0]["video"]["companionad"][0]["h"][0].asInt());
	    		vdetails.format = vdetails.format  + "," + std::to_string(bidrequestjson["imp"][0]["video"]["companionad"][0]["w"][1].asInt()) + "x" +std::to_string(bidrequestjson["imp"][0]["video"]["companionad"][0]["h"][1].asInt());
	    		}
	    		else
    			{
				vdetails.format = vdetails.format  + "," + std::to_string(bidrequestjson["imp"][0]["video"]["companionad"][0]["w"].asInt()) + "x" +std::to_string(bidrequestjson["imp"][0]["video"]["companionad"][0]["h"].asInt());
    			}

	    		if(bidrequestjson["imp"][0]["video"]["companionad"].size() > 1)
	    			{
	    				vdetails.format = vdetails.format  + "," + std::to_string(bidrequestjson["imp"][0]["video"]["companionad"][1]["w"].asInt()) + "x" +std::to_string(bidrequestjson["imp"][0]["video"]["companionad"][1]["h"].asInt());
	    			}
    		}




			if(!bidrequestjson["imp"][0]["video"]["ext"]["inventory_type"].isNull())
    		{
    			vdetails.inventory_type = bidrequestjson["imp"][0]["video"]["ext"]["inventory_type"].asString();
    		}
    		if(vdetails.isVideo)
    		{
    		//	cout << "video details" << vdetails.isVideo << " - " << vdetails.skippable << " - " << vdetails.v_duration << " - "  << vdetails.v_skipable_duration << " - " << vdetails.inventory_type<< " - " << vdetails.format  << endl;
    		}
    		if(!bidrequestjson["imp"][0]["formats"].isNull() && !bidrequestjson["imp"][0]["formats"][0].isNull())
    		{
    			format = bidrequestjson["imp"][0]["formats"][0].asString();
    			//cout << url << " - " << format ;
    		}

    		if(!bidrequestjson["imp"][0]["pmp"]["ext"]["is_predicted_to_be_ignored"].isNull())
    		{
    			//	cout << url <<  bidrequestjson["imp"][0]["pmp"]["ext"]["is_predicted_to_be_ignored"] << isVideo << endl;
    		}

    		getRequestAndUserDetails(m_redisContext,puser,guser,url,item_ids,article_type);

    		org_item_ids = item_ids;

    		isRemarketingPossible = canRemarketingPossible(guser,puser,isRemarketingBlocked);

    		//isRemarketingPossible = false;
    		//cout << isRemarketingPossible << endl;


    		int geo_id = 0;

			if(!bidrequestjson["device"]["ext"]["geo_criteria_id"].isNull())
    		{
    			geo_id = bidrequestjson["device"]["ext"]["geo_criteria_id"].asInt();
    		}

			if(groupid =="22314286720" && url != "")
			{
				//cout << " Insurance Details - " << url << " - geo - " <<  geo_id << " - format - " <<  format << endl;
			}

			if(groupid == "18102001720" &&  url != "" && url.find("anonymous") == std::string::npos && url.find("commonfloor") == std::string::npos && url.find("sulekha.com") == std::string::npos && (url.find("propertywala") != std::string::npos || url.find("gharabari.com") != std::string::npos || url.find("harshasagar.com") != std::string::npos || url.find("realestatehungama.com") != std::string::npos || url.find("bangalorenest.com") != std::string::npos || url.find("indianrealestateboard.com") != std::string::npos || url.find("indiarealestateinfo.com") != std::string::npos || url.find("anandproperties.com") != std::string::npos || url.find("property.mitula.in") != std::string::npos || url.find("property.trovit.co.in") != std::string::npos || url.find("bangalore.locanto.in") != std::string::npos || url.find("bangalore.click.in") != std::string::npos ))
				//&& url.find("commonfloor") == std::string::npos && url.find("sulekha.com") == std::string::npos && (url.find("propertywala") != std::string::npos || url.find("gharabari.com") != std::string::npos || url.find("harshasagar.com") != std::string::npos))
			{	 			
				//	cout << " Real estate url - " << url <<  " - geo - " << geo_id << endl;
			}
				

			if(groupid == "18102001720" &&  geo_id == 1007768)
			{
				//cout << " Bangalore Real estate url - " << url <<  " - geo - "  << geo_id << endl;
			}

			if(groupid == "20616639040")
				//&& url.find("commonfloor") == std::string::npos && url.find("sulekha.com") == std::string::npos && (url.find("propertywala") != std::string::npos || url.find("gharabari.com") != std::string::npos || url.find("harshasagar.com") != std::string::npos))
			{	 			
				//	cout << " Car  - " << url << endl;
			}

			if(isRemarketingPossible)
    		{


			//	cout << "**********+++++++++++ - cookie matching " << puser.id << " - " <<  puser.all_item_ids << " - " << puser.top_item_ids << " - " << url <<"**********+++++++++++"<< endl;	
	 		//	cout << "**********+++++++++++ - cookie matching " << guser.id << " - "<<  guser.all_item_ids << " - " << guser.top_item_ids  << " - "<< url <<"**********+++++++++++"<< endl;	
	
    		}

			bool housing =  canHousingRemarketing(guser,puser,isRemarketingBlocked);
			if(housing)
			{
				//cmd = "SADD housing-list-" + getCurrentTime() + " "  + guser.id;	
				
				cmd = "SADD housing-list-2014-12-22 "  + guser.id;	
				if (m_redisContext != NULL && m_redisContext->err == 0 ) 
	  			{	
					reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
				}
			}

    		bool admatch = false; 
    		bool canbidforrealesate = bidforrealestate(item_ids, groupid, geo_id,url,add_details,eCPMController);

    		//this is added for car testing
    		//if(groupid == "20616639040" && url != "" && url.find("anonymous") == std::string::npos)
    		//{
    		//	item_ids = item_ids + ",3214";
    		//}

    		//for now blocking mysmartprice 250x250 ads.
    		if(url.find("mysmartprice.com") && format == "250x250")
    		{
    			return false;
    		}
    		
	        if(item_ids != "" || isRemarketingPossible || canbidforrealesate  || isFashion)
	        {	
        		if ((article_type != "HowTo/Guide" && article_type != "Apps" && article_type != "Accessories") || isRemarketingPossible ||  canbidforrealesate || isFashion)
        		{	

       //  				if(hastaggingrestricted && guser.all_item_ids == "" && puser.all_item_ids == "" && !isRemarketingPossible)
       //  				{

    			// 			if(guser.id != "" && (itemtype == "Mobile" || itemtype == "Tablet" || itemtype == "Car" || itemtype == "Bike" || itemtype == "Tablet" || itemtype == "Laptop"))
							// {
							// 	cmd = "RPUSH users:visits "  + guser.id + "<<" + url + "<<" + article_type + "<<" + item_ids + "<<" + "";
			    //  				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());		
			    //  				freeReplyObject(reply);
			    //     	    }
							// returnValue = "NonConvertingUser";
			   	//     		return false;
       //  				}
        				int viewability =  0;
        				string strview = bidrequestjson["imp"][0]["pmp"]["ext"]["viewability"].asString();
        				if(strview != "")
        				{
        					viewability = atoi(strview.c_str());
        				}
        				else
        				{
        					viewability = 100;
        				}

        				bool goodSpotTag = checkSpotTagsPerformance(m_redisContext,tagid,eCPMController,guser,viewability,url);

        				if(!goodSpotTag)
			   	    	{	
			   	    		if(guser.id != "" && ((itemtype == "Mobile" || itemtype == "Tablet" || itemtype == "Car" || itemtype == "Bike" || itemtype == "Tablet" || itemtype == "Laptop" || itemtype == "Television" || itemtype == "Lens" || itemtype == "WearableGadget" || itemtype == "Console") || (url.find("makeupandbeauty.com") != std::string::npos || url.find("stylecraze.com") != std::string::npos || url.find("southindiafashion.com") != std::string::npos || url.find("wiseshe.com") != std::string::npos)))
							{
								cmd = "RPUSH users:visits "  + guser.id + "<<" + url + "<<" + article_type + "<<" + item_ids + "<<" + "" + "<<" + itemtype + "<<" + puser.id + "<<" + to_string(geo_id);
			     				reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());		
			     				freeReplyObject(reply);
			        	    }
							returnValue = "BadSpot";
			   	    		return false;
			   	    	}
						

			   	    
        				if(((item_ids != "") && (article_type != "HowTo/Guide" && article_type != "Apps" && article_type != "Accessories")) || 
        					canbidforrealesate || isFashion)
        				{
        					std::vector <std::string> vItemIDs;
        					if(!isFashion)
        					{

        					if(article_type == "Resale")	
        					{
        						updateLocationItemsids(item_ids,geo_id);
        					}
				    		vItemIDs = parseString(item_ids,",");
				   			adIdsStr = getItemsAndDetails(m_redisContext,vItemIDs,hasorders,highectr,itemtype,vendorIDString, pcvendorIDString,guser, userexistswithitemid);

				   			  if( /*vdetails.isVideo && */adIdsStr.find("35") != std::string::npos)
		    					{					
		    						adIdsStr = adIdsStr + ",25";
		    					}	

				   			}
				   			else
				   			{
				   				
				   				item_ids = getItemIDForFashion(url);
				   				vItemIDs = parseString(item_ids,",");
				   				adIdsStr = "40,47";
				   			}	

				   			if(!adIdsStr.empty())
							{
								getAdvertisements(m_redisContext,ads, adIdsStr, vDS,pcvendorIDString,url,false,isMobile,vdetails,format);
								
							}
							
								if(groupid == "20616639040")
									//&& url.find("commonfloor") == std::string::npos && url.find("sulekha.com") == std::string::npos && (url.find("propertywala") != std::string::npos || url.find("gharabari.com") != std::string::npos || url.find("harshasagar.com") != std::string::npos))
								{	 			
									//	cout << " Car  - " << url << " - " << item_ids  << " - " << article_type << " - "  << adIdsStr <<  " - "  << format << endl;
								}
						 	
							//bool admissingduetouser = false; 
							if(vDS->size() > 0)
							{    
								//adjusting eCPM with buyer list.
								if(guser.top_item_ids != "")
		        				{
		        					eCPMController = eCPMController + 0.2;
		        				}
					   			
		   					   	if(userexistswithitemid)
								{
									eCPMController = eCPMController + 0.2;
									//cout << "price increase for users" <<  endl;
								}


								if(item_ids.find("28712") != std::string::npos || item_ids.find("18093") != std::string::npos || item_ids.find("73319") != std::string::npos || item_ids.find("28947") != std::string::npos || item_ids.find("73841") != std::string::npos || item_ids.find("74790") != std::string::npos || item_ids.find("73841") != std::string::npos)
			    				{
				    					eCPMController = eCPMController + 0.2;
			    				}

								if(!hastaggingrestricted)
								{
									eCPMController = eCPMController + 0.2;
								}     	

								admatch =  processAdvertisement(m_redisContext,ads,vDS,vendorIDString,pcvendorIDString,highectr,hasorders,eCPMController, reservePrice, item_ids,vItemIDs,eCPM,advertisementid, click_url, url, guser,puser,returnValue,false,add_details,article_type);

								//cout << "advertisementid " << advertisementid << endl;

								if(admatch && (advertisementid == "43" || advertisementid == "44" ))
								{
								//	cout << "admatch " << admatch << endl;
								//	cout << "geo " << geo_id << endl;
								//	cout << " item id" << item_ids << endl;
								//			cout << " article type" << article_type << endl;
									if(//(geo_id != 1007768 && geo_id != 1007809 && geo_id != 1007740 && geo_id != 1007785 && geo_id != 1007788 && geo_id != 1007751 && geo_id != 1007765 && geo_id != 1007826 && geo_id != 1007753 && geo_id != 1007828 && geo_id != 1007760 && geo_id != 1007801)
										//|| 
										((item_ids.find("72892") == std::string::npos  && item_ids.find("72893") == std::string::npos) && (article_type != "Reviews"  && article_type != "Comparisons" && article_type != "Spec" && article_type != "Photo" /*&& article_type != "News" && article_type != "Others"*/)))
									{
										cout << "inside fail case " <<  url << " - " << geo_id << " - " << article_type <<  " - " << bidrequestjson["imp"][0]["pmp"]["ext"]["is_predicted_to_be_ignored"] << endl;
												admatch	 = false;
												item_ids = "";
												advertisementid = "";
												eCPM = 0.0;
									}
									else

									{
										//cout << "inside pass case " <<  url << " - " << geo_id << " - " << article_type <<  " - " << bidrequestjson["imp"][0]["pmp"]["ext"]["is_predicted_to_be_ignored"] << endl;
									}

								}

								//for realestate

								// if((advertisementid == "32") && (item_ids.find("29979") != std::string::npos || item_ids.find("60109") != std::string::npos || item_ids.find("67548") != std::string::npos))
								// {
								// 	eCPM = std::to_string(stof(eCPM) * 1.3);
									
								// }

								// if((advertisementid == "33") && (item_ids.find("60021") != std::string::npos))
								// {
								// 	eCPM = std::to_string(stof(eCPM) * 1.3);
								// }

								// if((advertisementid == "33" || advertisementid == "32") && (url.find("propertywala") != std::string::npos || url.find("gharabari.com") != std::string::npos || url.find("harshasagar.com") != std::string::npos || url.find("realestatehungama.com") != std::string::npos || url.find("bangalorenest.com") != std::string::npos || url.find("indianrealestateboard.com") != std::string::npos || url.find("indiarealestateinfo.com") != std::string::npos || url.find("anandproperties.com") != std::string::npos || url.find("property.mitula.in") != std::string::npos || url.find("property.trovit.co.in") != std::string::npos || url.find("bangalore.locanto.in") != std::string::npos || url.find("bangalore.click.in") != std::string::npos))
								// {
								// 	eCPM = std::to_string(stof(eCPM) * 1.5);

								// 	if(url.find("bangalorenest.com") != std::string::npos )
								// 	{
								// 		eCPM = std::to_string(stof(eCPM) * 1.2);
								// 	}
								// }

								 if(advertisementid == "35")
								 {
								 //	cout << " bidding - " << " - "<< advertisementid << " - "<<  url << " - " << eCPM << endl;
								 }

					
							}

						
			   	    	}

			   	    	if((guser.id != "") && ((itemtype == "Mobile" || itemtype == "Tablet" || itemtype == "Car" || itemtype == "Bike" || itemtype == "Tablet" || itemtype == "Laptop" || itemtype == "Television" || itemtype == "Lens" || itemtype == "WearableGadget" || itemtype == "Console") || (url.find("makeupandbeauty.com") != std::string::npos || url.find("stylecraze.com") != std::string::npos || url.find("southindiafashion.com") != std::string::npos || url.find("wiseshe.com") != std::string::npos)))
						{
							cmd = "RPUSH users:visits "  + guser.id + "<<" + url + "<<" + article_type + "<<" + org_item_ids + "<<" + advertisementid + "<<" + itemtype + "<<" + puser.id + "<<" + to_string(geo_id);
							redisAppendCommand(m_redisContext,cmd.c_str());
			        		redisGetReply(m_redisContext,(void**)&reply);	
		        	    	freeReplyObject(reply);
		        		}

						if((!admatch) && (isRemarketingPossible))
						{
							//cout << "Remareting happening " << url << endl;
							if(item_ids=="" && ((groupid == "16951559440" || groupid =="20841722560" || groupid == "20616639040" || groupid == "16025452000") || ((groupid == "18102001720") && (url.find("propertywala") != std::string::npos || url.find("gharabari.com") != std::string::npos || url.find("harshasagar.com") != std::string::npos || url.find("realestatehungama.com") != std::string::npos || url.find("bangalorenest.com") != std::string::npos || url.find("indianrealestateboard.com") != std::string::npos || url.find("indiarealestateinfo.com") != std::string::npos || url.find("anandproperties.com") != std::string::npos || url.find("property.mitula.in") != std::string::npos || url.find("property.trovit.co.in") != std::string::npos || url.find("bangalore.locanto.in") != std::string::npos || url.find("bangalore.click.in") != std::string::npos))))
							{
								processMissingURL(m_redisContext,url,returnValue,groupid,strverticals);
							}
							std::vector <std::string> vItemIDs;
							string tempIDs ="";
							 // if(guser.all_item_ids.find("5727") != std::string::npos || puser.all_item_ids.find("5727") != std::string::npos)
							 // {
							 // 	tempIDs = "5727,";
							 // }

							 //  if(guser.all_item_ids.find("18093") != std::string::npos || puser.all_item_ids.find("18093") != std::string::npos)
							 // {
							 // 	tempIDs = tempIDs + "18093,";
							 // }

							 //  if(guser.all_item_ids.find("28712") != std::string::npos || puser.all_item_ids.find("28712") != std::string::npos)
							 // {
							 // 	tempIDs = tempIDs + "28712,";
							 // }
						
							
							
			    			item_ids = getRemarketingItemids(puser,guser);
							vItemIDs = parseString(item_ids,",");

							//cout << "remark item ids " << item_ids  << endl;
				    	
				    		vendorIDString = "";
				    		pcvendorIDString = "";
				    		ads.clear();
				    		eCPMController = 1;
				   			adIdsStr = getItemsAndDetails(m_redisContext,vItemIDs,hasorders,highectr,itemtype,vendorIDString, pcvendorIDString,guser, userexistswithitemid);
				   			//adjusting eCPM with buyer list.
						
	   					   //	eCPMController = eCPMController + 0.25;
	
				   			if(!adIdsStr.empty())
							{
								getAdvertisements(m_redisContext,ads, adIdsStr, vDS,pcvendorIDString,url,true,isMobile,vdetails,format);
								
							}
						
							
							if(vDS->size() > 0)
							{         	
								admatch =  processAdvertisement(m_redisContext,ads,vDS,vendorIDString,pcvendorIDString,highectr,hasorders,eCPMController, reservePrice, item_ids,vItemIDs,eCPM,advertisementid, click_url, url, guser,puser,returnValue,true,add_details,article_type);

							}

							//cout << "Remareting happening " << url <<  " - " << bidthroughRemarketing <<endl;

							if(admatch)
							{
								bidthroughRemarketing = true;

							//	cout << "remarketing bidding - " << guser.id <<url << endl;
								returnValue = "BiddingThroughRemarketing";
							}
							else
							{
								if(returnValue == "")
								{
								 returnValue ="MissingAdImpression";
								}
							}
						
						}
						
						
					
						if(returnValue == "")
						{	
							returnValue = "BiddableBuyerImpression";
						}
						return admatch;
				}
				else
				{
					 //cout << "Skip this tag or article type" << endl;
				 	returnValue = "BiddableOwnerImpression";
					return false;
				}		
   				
   				
	 		}
	 		else
	 		{

				

					if((groupid == "16951559440" || groupid == "20841722560" || groupid == "20616639040" || groupid =="16025452000") || ((groupid == "18102001720") && (url.find("propertywala") != std::string::npos || url.find("gharabari.com") != std::string::npos || url.find("harshasagar.com") != std::string::npos || url.find("realestatehungama.com") != std::string::npos || url.find("bangalorenest.com") != std::string::npos || url.find("indianrealestateboard.com") != std::string::npos || url.find("indiarealestateinfo.com") != std::string::npos || url.find("anandproperties.com") != std::string::npos || url.find("property.mitula.in") != std::string::npos || url.find("property.trovit.co.in") != std::string::npos || url.find("bangalore.locanto.in") != std::string::npos || url.find("bangalore.click.in") != std::string::npos )))
					{
						processMissingURL(m_redisContext,url,returnValue,groupid,strverticals);

					}
					else
					{
						returnValue = "MissingURLRemarketing";
					}

	    			return false;

	 		}	

     	}
     	catch(std::exception& e)
     	{
     		cout << "*****Exception******" << e.what() << endl;
     		cout << "Exception url : - " << url <<  jsonstring << endl;
     		return false;
     	}



	}

};

