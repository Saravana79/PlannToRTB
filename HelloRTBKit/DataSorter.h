#include <string>

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

