#pragma once
#include <vector>
#include <set>
#include <unordered_map>
#include "Gene.h"
//TODO add support for genes with same map code being chosen randomly
using namespace std;
class Gene_tree
{
public:
	int gene_index = -2;
	unordered_map<char, Gene_tree*> children;
	Gene_tree(vector<Gene>);
	Gene_tree();
	int decode(string);
};


