#include "Network.h"

using namespace std;

//Network::Network(Chromosome c)
//{
//	for (int i = 0; i < c.numNeurons; i++)//declare vector of empty neurons
//	{
//		Neuron n;
//		neurons.push_back(n);
//	}
//	for (int i = c.numNeurons - 1; i >= 0; i--)
//	{
//		vector<pair<float, Neuron*>> v;
//		for (int j = 0; j < c.numNeurons; j++)
//		{
//			if (c.gene_weights[i][j] != "0")
//			{
//				pair<float, Neuron*> p;
//				p.first = c.gene_weights[i][j];
//				p.second = &neurons[j];
//				v.push_back(p);
//			}
//		}
//		Neuron n(c.threshold[i], v);
//		neurons[i] = n;
//	}
//}

