#pragma once

#include "Neuron.h"
#include "Chromosome.h"

using namespace std;
class Network
{
private:
	vector<Neuron> neurons;
	bool buy = false;
	bool sell = false;
	bool none = false;
public:
	
	//vector<Input> input;//the input symbols feed into neurons with the same index in the neurons vector
	char step(float, float, float, float);//OHLC
	Network(vector<vector<string>>);
};