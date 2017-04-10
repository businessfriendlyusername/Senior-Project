#pragma once
#include "Chromosome.h"
using namespace std;
class Population
{
private:
public:
	Population(vector<string>, int inSize, int outSize, int popSize);//~~~TODO needs revision~~~~
	vector<Chromosome> pool;
};