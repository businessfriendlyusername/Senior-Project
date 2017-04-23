#pragma once
#include <string>
#include "belvis.h"
#include <random>
#include <vector>
using namespace std;

class Gene
{
public:
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Member Variables~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	string gene_code;//code indicating the compatability of this gene with another gene
	double dominance;//gene dominance
	vector<int> inputs;//data about which nodes are input nodes and which edges feed into which nodes(see below)
	//the index n of the vector corrosponds to the nth incoming edge in the parent matrix, the value it stores
	//corrosponds to which node that edge feeds into, this scheme prevents restrictions with the "seperate genes" scheme

	vector<int> outputs;//data about which nodes are output nodes and where the output nodes are directed in the parent network
	//the index in the the vector corrosponds to the nth outgoing edge in the parent matrix and the value corrosponds to the
	//index of the node in gene_nodes

	vector<vector<string>> gene_weights;//genetic code stored as an nxn adjacency matrix
	vector<int> gene_indicies;//vector of indicies (in parent vector) which codes correspond to
	vector<string> gene_nodes;//vector of codes corresponding to genes in the genes vector
	float structure_mutation_rate;//mutation rate of the gene structure
	vector<float> nodes_code_mutation_rate;//mutation rate of the gene_nodes strings
	float mutation_rates_mutation_rate;//the mutation rate of the mutation rates






	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Member Functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Gene();
	string randWeight(double mean, double standard_dev, bool threshold = false);//only assigns constant weight if threshold == true
	void randFF(double mean, double standard_dev);//generate random edges and thresholds for feed forward network
	void randFB(double mean, double standard_dev);//generate random feedback network
	void print_weights();//prints adjacency matrix of weight values
	void print_info();
};