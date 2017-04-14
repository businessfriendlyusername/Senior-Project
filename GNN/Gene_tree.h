#pragma once
#include <vector>
#include <set>
#include <map>
#include "Gene.h"

//TODO add support for genes with same map code being chosen randomly
using namespace std;
class Gene_tree
{
public:
	Gene_tree();
	void build(vector<Gene>);
	~Gene_tree();//Destructor must be a function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int gene_index = -2;
	map<char, Gene_tree*> children;
	
	
	
	int decode(string);
};


