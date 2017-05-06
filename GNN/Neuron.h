#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;
class Neuron
{
private:
	float sum = 0;
	float tempsum = 0;
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