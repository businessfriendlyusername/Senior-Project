#include "Gene.h"
using namespace std;

Gene::Gene() {}

string Gene::randWeight(double mean, double standard_dev, bool thresh)
{
	string weight;
	random_device rd;
	default_random_engine generator;
	//~~~~~~TODO this distribution is not very modular and needs tweaking for final version~~~~~~~~~~~~~
	//~~~~~~TODO change seed to StochasticLib1 random number generator~~~~~~~~~~~~~~~~
	normal_distribution<double> distribution(mean, standard_dev);//random number generated along normal distribution standard deviation = 1, mean = 0
	if (rd() % 2 == 0)//no edge
		weight = "0";
	else if (rd() % 2 == 0 || !thresh)//output sum of all inputs multiplied by some weight
		weight = "s" + to_string(distribution(generator));
	else//output is a constant value
		weight = to_string(distribution(generator));
	return weight;
}


void Gene::randFF(double mean, double standard_dev)
{
	random_device rd;
	default_random_engine gen;
	normal_distribution<double> n_dist(mean, standard_dev);

	for (int i = 0; i < inputs.size() + 1; i++)//for each input node generate random edges to the rest of the graph ( +1 for the bias input)
	{
		vector<string> tempvec;//vector that stores the output edges of a single neuron
							   //the if else block correspond to whether or not a threshold is set
		for (int j = 0; j < inputs.size(); j++)//push zeros into the input nodes' edge vectors so they dont input into other input nodes
			tempvec.push_back("0");
		for (int j = inputs.size(); j < gene_nodes.size(); j++)//generate random edges for the rest of the graph
			tempvec.push_back(belvis::trim_string(randWeight(mean, standard_dev, true), 10));
		gene_weights.push_back(tempvec);
	}
	for (int x = inputs.size() + 1; x < gene_nodes.size() + 1; x++)//generate the rest of the matrix without cycles
	{
		vector<string> tempvec; //vector that stores the output edges of a single neuron
		for (int j = 0; j < x + 1; j++)//in order for the matrix to represent a DAG there must be all zeroes on and below its scalar
			tempvec.push_back("0");//assign all zeros below the scalar
		for (int j = x + 1; j < gene_nodes.size(); j++)
			tempvec.push_back(belvis::trim_string(randWeight(mean, standard_dev, true), 10));//assign random weights above the scalar
		gene_weights.push_back(tempvec);
	}
}


void Gene::randFB(double mean, double standard_dev)
{
	random_device rd;
	default_random_engine gen;
	normal_distribution<double> n_dist(mean, standard_dev);

	for (int i = 0; i < inputs.size() + 1; i++)//for each input node generate random edges to the rest of the graph ( +1 for the bias input)
	{
		vector<string> tempvec;//vector that stores the output edges of a single neuron
							   //the if else block correspond to whether or not a threshold is set
		for (int j = 0; j < inputs.size(); j++)//push zeros into the input nodes' edge vectors so they dont input into other input nodes
			tempvec.push_back("0");
		for (int j = inputs.size(); j < gene_nodes.size(); j++)//generate random edges for the rest of the graph
			tempvec.push_back(belvis::trim_string(randWeight(mean, standard_dev, true), 10));
		gene_weights.push_back(tempvec);
	}
	for (int x = inputs.size() + 1; x < gene_nodes.size(); x++)//generate the rest of the matrix without cycles
	{
		vector<string> tempvec; //vector that stores the output edges of a single neuron
		for (int j = 0; j < gene_nodes.size(); j++)//in order for the matrix to represent a DAG there must be all zeroes on and below its scalar
			tempvec.push_back(belvis::trim_string(randWeight(mean, standard_dev, true), 10));
		gene_weights.push_back(tempvec);
	}
}

void Gene::print_weights()
{
	for (int i = 0; i < gene_weights.size(); i++)
	{
		for (int j = 0; j < gene_weights[i].size(); j++)
		{
			if (atof(gene_weights[i][j].c_str()) > 0.0)
				cout << '+' << ' ';
			else if (atof(gene_weights[i][j].c_str()) < 0.0)
				cout << '-' << ' ';
			else
				cout << "  ";
		}
		cout << endl;
	}
}

void Gene::print_info()
{
	cout << "identity: " << gene_code << endl;
	cout << "dominance: " << dominance << endl;
	cout << "struct mrate: " << structure_mutation_rate << endl;
	for(int i = 0; i < gene_nodes.size(); i++)
	{
		cout << gene_nodes[i] << " mrate: " << nodes_code_mutation_rate[i]<< endl;
	}
	for (int i = 0; i < inputs.size(); i++)
	{
		cout << i << " -> " << inputs[i] << endl;
	}
	cout << endl;
	for (int i = 0; i < outputs.size(); i++)
	{
		cout << outputs[i] << " -> " << i << endl;
	}
	print_weights();
}