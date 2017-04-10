#include "Population.h"

Population::Population(vector<string> symbols, int inSize, int outSize, int popSize)
{
	int network_size = (int)ceil((float)(inSize + outSize) * 1.5f);
	for (int i = 0; i < popSize; i++)
	{
		for (int i = 0; i < network_size; i++)
		{   
			Chromosome gene;
			pool.push_back(gene);
		}
	}
}