#pragma once

#include "Neuron.h"
#include "Chromosome.h"

using namespace std;
class Network
{
private:
	unsigned int first;//the first index that the network has a complete set of active inputs
	unsigned int last;//the last index that the network has a complete set of active inputs
public:
	vector<Neuron> neurons;
	//vector<Input> input;//the input symbols feed into neurons with the same index in the neurons vector
	string output;//the output symbol
	Network();
	
};