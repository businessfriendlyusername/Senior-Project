#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;
class Neuron
{
private:
	float sum;
	float tempsum;
	vector<Neuron*> addresses;
	vector<float> weights;
public:
	Neuron(vector<string>, vector<Neuron*>);
	Neuron();
	bool fire();
	void receive(int);
	void flush();
	void full_flush();
};