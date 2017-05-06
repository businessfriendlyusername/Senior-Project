#pragma once

#include "Neuron.h"
#include "Chromosome.h"

using namespace std;
class Network
{
private:
	vector<Neuron> neurons;
public:
	char position = 'n';//none
	float price = 0.0f;
	double fitness = 0;
	//vector<Input> input;//the input symbols feed into neurons with the same index in the neurons vector
	char step(float, float, float, float);//OHLC
	Network(vector<vector<string>>);
	bool fire();
};