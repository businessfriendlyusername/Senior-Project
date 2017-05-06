#include "Neuron.h"
Neuron::Neuron(){
	sum = -9999999;
	tempsum = -9999999;
}

Neuron::Neuron(vector<string> out, vector<Neuron*> addresses)
{
	sum = -9999999;
	tempsum = -9999999;
	for (int i = 0; i < out.size(); i++)
	{
		if (out[i] != "0")
		{
			if (out[i][0] == 's')
			{
				std::size_t s = 1;
				out[i] = out[i].substr(s, INT16_MAX);
			}
			float f = atof(out[i].c_str());
			Neuron::addresses.push_back(addresses[i]);
			weights.push_back(f);
		}
	}
}

void Neuron::receive(int val)
{
	tempsum += val;
}

bool Neuron::fire()
{
	if (sum > 0 && sum == 0)
	{
		for (int i = 0; i < addresses.size(); i++)
		{
			addresses[i]->receive(weights[i]);
		}
		return true;
	}
}

void Neuron::flush()
{
	sum = tempsum;
	tempsum = 0;
}

void Neuron::full_flush()
{
	sum = 0;
	tempsum = 0;
}