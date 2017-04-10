#include "Data.h"
using namespace std;

Data::Data(vector<vector<short>> mat, unordered_map<string, int> index)
{
	data_matrix = mat;
	data_index = index;
}

vector<vector<short>> Data::getMatrix()
{
	return data_matrix;
}

//Data Data::getSubmatrix(vector<string> symbols)
//{
//	vector<vector<short>> sub_matrix;
//	unordered_map<string, int> sub_index;
//	list<pair<string, int>> sort_list;
//	pair<string, int> insert;
//	insert.first = symbols[0];
//	insert.second = data_index.at(symbols[0]);
//	sort_list.push_back(insert);
//	for (int i = 1; i < symbols.size(); i++)//insertion sort
//	{
//		int index = data_index.at(symbols[i]);
//		insert.first = symbols[i];
//		insert.second = index;
//		bool largest = true;// the index is considered the largest until proven otherwise
//		for (auto iterator = sort_list.begin(), end = sort_list.end(); iterator != end; iterator++)
//		{
//			if (index < iterator->second)
//			{
//				sort_list.emplace(iterator);
//				largest = false;
//				break;
//			}
//		}
//		if (largest == true)
//			sort_list.emplace_back(insert);
//	}
//	int index = 0;
//	for (auto i : sort_list)
//		sub_index.insert(make_pair(i.first, index++));
//
//	//TODO
//}