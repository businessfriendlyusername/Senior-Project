#pragma once
#include <list>
#include <unordered_map>
#include <iostream>
#include <string>
#include <fstream>
#include "belvis.h"
#include <unordered_set>
using namespace std;
class Data
{
private:
	vector<vector<short>> data_matrix;//data matrix, supervector is ordered by date, subvector is ordered by data symbol
	unordered_map<string, int> data_index;//index of data in data_matrix subvector
public:
	Data(vector<vector<short>>, unordered_map<string, int>);
	vector<vector<short>> getMatrix();
	Data getSubmatrix(vector<string>);
};