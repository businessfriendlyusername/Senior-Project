#pragma once
#include <vector>
#include "Data.h"
#include "belvis.h"
#include <string>
#include <random>
#include <iostream>
#include "Gene.h"
#include <set>
#include "Network.h"
#include "Gene_tree.h"
#include <algorithm>
#include <iterator>
using namespace std;

class Chromosome
{
private:
	set<char> alphabet;
	vector<pair<string, int>> inputs;//indicies of the input nodes
	vector<pair<string, int>> outputs;//indicies of the output nodes
	vector<vector<string>> gene_weights;//genetic code stored as an nxn adjacency matrix
	vector<string> gene_codes;//vector of codes corresponding to genes in the genes vector
	vector<int> gene_indicies;//vector of indicies which codes correspond to
	void resolve_genes();//resolves genes to gene_indicies
	Gene_tree resolver;//resolves gene codes to their indicies in the gene vector
	vector<set<int>> make_dag(vector<set<int>>);//used by validate_genes


public:
	void validate_genes();//validates genes to make sure they do not have include cycles
	vector<vector<string>> build_matrix();//compiles the genes and codes into a single matrix
	double fitness = 0;
	float genes_mutation_rate;//mutation rate of the gene pool
	float structure_mutation_rate;//mutation rate of the gene structure
	vector<float> codes_code_mutation_rate;//mutation rate of the gene_codes strings
	float mutation_rates_mutation_rate;//the mutation rate of the mutation rates
	vector<Gene> genes;//list of genes in the chromosome
	Chromosome();
	void set_genes(vector<Gene>);
	static const vector<Gene> rand_genes(int num_genes, double weights_mean, double weights_dev, int nodes_mean, int nodes_dev, float mutation_mean, float mutation_dev, set<char> alpha);
	//populates gene vector with random genes

	void set_inputs(vector<pair<string, int>> in);
	void set_outputs(vector<pair<string, int>> out);
	static vector<vector<string>> rand_struct(double weights_mean, double weights_dev, int nodes_mean, int nodes_dev, int inputs = 0);//generate random feedback network
	void set_struct(vector<vector<string>>);
	static const string randWeight(double mean, double standard_dev, bool threshold = false);//only assigns constant weight if threshold == true
	void set_codes(vector<string>);
	void gen_codes();
	void printweights();//prints adjacency matrix of weight values
	void printadj();//prints binary adjacency matrix 1=edge 0=no edge
	void printGenes();//prints genes in gene vector
	void printCodes();//prints gene codes
	//void printthreshold();//prints the thresholds of the neurons
	//void print_inputs();//prints info about all inputs
};

