#pragma once
#include "Chromosome.h"
#include "Network.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
using namespace std;
class Population
{
private:
	vector<Chromosome> cpool;
	vector<Network> npool;
	vector<vector<float>> data;
public:
	Population(int popsize, int num_genes, double weights_mean, double weights_dev, int nodes_mean, int nodes_dev, float mutation_mean, float mutation_dev, set<char> alpha);
	void getData(string);
	void fitness_test(vector<float>);
	vector<Chromosome> simulate();
	vector<int> cull(vector<pair<int, float>>);//returns indicies of culled items
	void reproduce(vector<int>, int);
	Chromosome top();//returns fittest chromosome
};