#include "Neuron.h"
Neuron::Neuron(){}

Neuron::Neuron(vector<string> out, vector<Neuron*> addresses)
{
	for (int i = 0; i < out.size(); i++)
	{
		if (out[i] != "0")
		{
			if (out[i][0] == 's')
				out[i] = out[i].substr(std::size_t(0), INT64_MAX);
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
	if (sum > 0)
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