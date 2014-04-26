#include <iostream>
#include <vector>

#include "hiredis-master/hiredis.h"

using namespace std;


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


int main(int argc, char** argv)
{
	redisContext* m_redisContext = connectRedisSvr();
	redisReply *reply = 0;

	// testing
	string urlName = "url:http://www.nytimes.com/pages/technology/index.html";

	std::string cmd = "";
	cmd = "HGET " + urlName + " item_ids";
	cout << "\t command: " << cmd << endl;
	reply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
    if (reply->type == REDIS_REPLY_NIL)
    {
    } else {
            std::string str = reply->str;
	    std::vector <std::string> vItemIDs;
	    vItemIDs = parseString(str);
	    cout << vItemIDs[0] << endl;
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
					cout << vAdIDs[0] << endl;

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
							cout << vECpms[0] << endl;
						}
					}
 
				}

		}


    }


	return 0;
}
