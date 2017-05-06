#pragma once
#include "Neuron.h"

using namespace std;
struct Edge {
private:
	float weight;
	Neuron* address;
public:
	Edge();
	Edge(float, Neuron*);
	void fire(float sum=1);
};