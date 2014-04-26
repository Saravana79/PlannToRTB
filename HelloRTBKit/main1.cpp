#include <vector>
#include <iostream>
#include "/hiredis-master/hiredis.h"
#include "DataSorter.h"

int main()
{
	vector<CAdDataSorter> *vDS = new vector<CAdDataSorter>();
	CAdDataSorter ds;
	ds.id = 1;
	ds.val = .1;

	for (int i = 0; i < 5; i++)
	{
		CAdDataSorter ds;
		ds.id = rand();
		ds.val = .00001 * rand();
		vDS->push_back(ds);
	}

	cout << vDS->at(0).name << endl;

	// applying the simple bubble sort on the objects
	CAdDataSorter temp;
	for (int i = 1; i <= vDS->size(); i++)
	{
		for (int j = 0; j < vDS->size() - 1; j++)
		{
			if (vDS->at(j).val > vDS->at(j+1).val)
			{
				temp = vDS->at(j);
				vDS->at(j) = vDS->at(j+1);
				vDS->at(j+1) = temp;
			}
		}
	}

	cout << vDS->at(0).name << endl;
}

