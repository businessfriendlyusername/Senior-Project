#include "Network.h"

using namespace std;

Network::Network(vector<vector<string>> mat)
{
	vector<Neuron*> addresses;
	for (int i = 0; i < mat.size(); i++)
	{
		Neuron n;
		neurons.push_back(n);
		addresses.push_back(&neurons[i]);
	}
	for (int i = 0; i < mat.size(); i++)
	{
		Neuron n(mat[i], addresses);
		neurons[i] = n;
		addresses[i] = &neurons[i];
	}
}

char Network::step(float o, float h, float l, float c)
{

	neurons[0].receive(o);
	neurons[1].receive(h);
	neurons[2].receive(l);
	neurons[3].receive(c);

	for (int i = 0; i < neurons.size() - 3; i++)
	{
		neurons[i].fire();
	}
	int count = neurons.size() - 2;
	buy = neurons[count++].fire();
	sell = neurons[count++].fire();
	none = neurons[count].fire();

	for (int i = 0; i < neurons.size(); i++)
	{
		neurons[i].flush();
	}
	if ((!buy && !sell) || (buy && sell) || none)//revision needed
		return 'n';
	else if (buy)
		return 'b';
	else
		return 's';
}