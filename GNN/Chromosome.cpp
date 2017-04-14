#include "Chromosome.h"
using namespace std;

Chromosome::Chromosome(){}

const string Chromosome::randWeight(double mean, double standard_dev, bool thresh)
{
	string weight;
	random_device rd;
	default_random_engine generator;
	normal_distribution<double> distribution(mean, standard_dev);//random number generated along normal distribution standard deviation = 1, mean = 0
	if (rd() % 2 == 0)//no edge
		weight = "0";
	else if (rd() % 2 == 0 || !thresh)//output sum of all inputs multiplied by some weight
		weight = "s" + to_string(distribution(generator));
	else//output is a constant value
		weight = to_string(distribution(generator));
	return weight;
}

void Chromosome::printweights()
{
	for (int i = 0; i < gene_weights.size(); i++)
	{
		for (int j = 0; j < gene_weights[i].size(); j++)
		{
			cout << gene_weights[i][j] << " ";
		}
		cout << endl;
	}
}

void Chromosome::set_struct(vector<vector<string>> s)
{
	gene_weights = s;
}

void Chromosome::printadj()
{
	for (int i = 0; i < gene_weights.size(); i++)
	{
		for (int j = 0; j < gene_weights[i].size(); j++)
		{


			if (gene_weights[i][j] == "0")
				cout << "0 ";
			else
				cout << "1 ";
		}
		cout << endl;
	}
}

//void Chromosome::rand_struct_FF(double mean, double standard_dev)
//{
//	random_device rd;
//	default_random_engine gen;
//	normal_distribution<double> n_dist(mean, standard_dev);
//
//	for (int i = 0; i < inputs.size() + 1; i++)//for each input node generate random edges to the rest of the graph ( +1 for the bias input)
//	{
//		vector<string> tempvec;//vector that stores the output edges of a single neuron
//		//the if else block correspond to whether or not a threshold is set
//		for (int j = 0; j < inputs.size(); j++)//push zeros into the input nodes' edge vectors so they dont input into other input nodes
//			tempvec.push_back("0");
//		for (int j = inputs.size(); j < gene_codes.size(); j++)//generate random edges for the rest of the graph
//			tempvec.push_back(belvis::trim_string(randWeight(mean, standard_dev, true), 10));
//		gene_weights.push_back(tempvec);
//	}
//	for (int x = inputs.size() + 1; x < gene_codes.size() + 1; x++)//generate the rest of the matrix without cycles
//	{
//		vector<string> tempvec; //vector that stores the output edges of a single neuron
//		for (int j = 0; j < x + 1; j++)//in order for the matrix to represent a DAG there must be all zeroes on and below its scalar
//			tempvec.push_back("0");//assign all zeros below the scalar
//		for (int j = x + 1; j < gene_codes.size(); j++)
//			tempvec.push_back(belvis::trim_string(randWeight(mean, standard_dev, true), 10));//assign random weights above the scalar
//		gene_weights.push_back(tempvec);
//	}
//}


vector<vector<string>> Chromosome::rand_struct(double mean, double standard_dev, int nodes_mean, int nodes_dev, int inputs)
{
	static default_random_engine gen;
	//gen.seed(time(NULL));
	normal_distribution<double> distribution(nodes_mean, nodes_dev);
	int nodes = belvis::round_dtoi(distribution(gen));
	vector<vector<string>> gene_weights;
	for (int i = 0; i < nodes + 1; i++)//+1 for bias input
	{
		vector<string> tempvec; //vector that stores the output edges of a single neuron
		for (int j = 0; j < nodes + 1; j++)
			tempvec.push_back(belvis::trim_string(randWeight(mean, standard_dev, true), 10));
		gene_weights.push_back(tempvec);
	}
	return gene_weights;
}



void Chromosome::set_inputs(vector<pair<string, int>> in) 
{
	inputs = in;
}

void Chromosome::set_outputs(vector<pair<string, int>> out)
{
	outputs = out;
}

void Chromosome::set_genes(vector<Gene> g)
{
	genes = g;
}

void Chromosome::set_codes(vector<string> s)
{
	gene_codes = s;
}

void Chromosome::gen_codes()
{
	static default_random_engine gen;
	//gen.seed(time(NULL));
	uniform_int_distribution<int> dist(0, genes.size() - 1);
	for (int i = 0; i < gene_weights.size(); i++)
	{
		gene_codes.push_back(genes[dist(gen)].gene_code);
	}
	resolve_genes();
}

const vector<Gene> Chromosome::rand_genes(int num_genes, double weights_mean, double weights_dev, int nodes_mean, int nodes_dev, float mutation_mean, float mutation_dev, set<char> alpha)
{
	static default_random_engine gen;
	gen.seed(time(NULL));
	static normal_distribution<double> dominance_distribution(.5, .1);
	normal_distribution<float> mutation_distribution(mutation_mean, mutation_dev);
	static discrete_distribution<int> rand{ 1, 1 };
	vector<Gene> genes;
	int code_len = belvis::reverse_permutation(num_genes, alpha.size());
	uniform_int_distribution<int> code_len_dist(1, code_len);



	for (int i = 0; i < num_genes; i++)
	{
		Gene g;
		g.gene_weights = rand_struct(weights_mean, weights_dev, nodes_mean, nodes_dev);
		int gene_size = g.gene_weights.size();
		int one_third = gene_size - (gene_size % 3) / 3;//   1/3 of the number of nodes in the gene
		int one_sixth = gene_size - (gene_size % 6) / 3;
		uniform_int_distribution<int> udistribution(0, gene_size - 1);
		
		normal_distribution<double> IO_distribution(one_third, one_sixth);
		g.gene_code = belvis::randomString(code_len, alpha);
		g.dominance = dominance_distribution(gen);
		int num_IO = belvis::round_dtoi(IO_distribution(gen));
		for (int j = 0; j < num_IO; j++)
			g.inputs.push_back(udistribution(gen));
		num_IO = belvis::round_dtoi(IO_distribution(gen));
		for (int j = 0; j < num_IO; j++)
			g.outputs.push_back(udistribution(gen));
		
		vector<string> gene_codes;
		for (int j = 0; j < gene_size; j++)
		{
			if (rand(gen) == 0)
			{
				code_len = code_len_dist(gen);
				gene_codes.push_back(belvis::randomString(code_len, alpha));
			}
			else
			{
				gene_codes.push_back("0");
			}
		}

		g.gene_nodes = gene_codes;
		g.structure_mutation_rate = mutation_distribution(gen);
		g.mutation_rates_mutation_rate = mutation_distribution(gen);
		if (g.structure_mutation_rate > 1.0f)
		g.structure_mutation_rate = 1.0f;
		if (g.mutation_rates_mutation_rate > 1.0f)
		g.mutation_rates_mutation_rate = 1.0f;
		vector<float> nodes_code_rates;
		for (int j = 0; j < gene_size; j++)
		{
			nodes_code_rates.push_back(mutation_distribution(gen));
		}
		g.nodes_code_mutation_rate = nodes_code_rates;
		genes.push_back(g);
			}
			return genes;
}



void Chromosome::printGenes()
{
	for (auto gene : genes)
	{
		gene.print_info();
		cout << endl;
	}
}

vector<vector<string>> Chromosome::build_matrix()
{
	vector<vector<string>> matrix;
	return matrix;
}

void Chromosome::printCodes()
{
	for (auto code : gene_codes)
	{
		cout << code << endl;
	}
}

void Chromosome::validate_genes()
{
	vector<set<int>> contains;
	for (int i = 0; i < genes.size(); i++)
	{
		set<int> s;
		for (auto node : genes[i].gene_nodes)
		{
			int x = resolver.decode(node);
			if(x != -1)
				s.insert(x);
		}
		
		contains.push_back(s);
	}
	contains = make_dag(contains);//prune edges to make graph a DAG
}


class compare//used in make_dag() priority queue
{
public:
	bool operator() (pair<int, set<int>>& left, pair<int, set<int>>& right)
	{
		return (left.second.size() < right.second.size());
	}
};


vector<set<int>> Chromosome::make_dag(vector<set<int>> graph)
{
	gene_includes.clear();
	vector<bool> complete;//indicies submatricies have been completely resolved if bool == true
	for (int i = 0; i < graph.size(); i++)
	{
		vector<int> temp;
		temp.reserve(graph.size());
		gene_includes.push_back(temp);
		complete.push_back(false);
	}
	priority_queue<pair<int, set<int>>, vector<pair<int, set<int>>>, compare> pq;


	for (int i = 0; i < graph.size(); i++)
	{
		pq.push(make_pair(i, graph[i]));
		vector<int> m;
		for (auto source : graph[i])
		{
			m.push_back(0);
		}
		gene_includes[i] = m;
	}
	vector<bool> included;
	for (int i = 0; i < graph.size(); i++)
		included.push_back(false);
	pair<int, set<int>> vertex = pq.top();
	pq.pop();
	vertex.second.clear();//at least one vertex must be empty so we use the vertex with the fewest edges
	pq.push(vertex);
	while (!pq.empty())
	{
		for (int i = 0; i < graph.size(); i++)
			included[i] = false;
		vertex = pq.top();
		pq.pop();
		included[vertex.first] = true;
		if (vertex.second.size() == 0)
		{
			complete[vertex.first] = true;
			continue;
		}

		bool comp = true;
		
		for (set<int>::iterator i = vertex.second.begin(); i != vertex.second.end(); i++)
		{
			int edge = *i;
			if (complete[edge])//the outgoing edge has already been completely expanded and is free of cycles
			{
				for (auto it : graph[edge])
				{
					included[it] = true;
					gene_includes[vertex.first][it]++;
				}
			}
			else if (included[edge])//cycle detected, matrix includes itself
			{
				comp = false;
				vertex.second.erase(edge);
			}
			else//no cycle detected and the outgoing edge has not been expanded yet
			{
				comp = false;
				vertex.second.insert(edge);
			}
			included[edge] = true;
		}
		if (comp)//if comp is true then all outgoing edges from the vertex are complete, therefore this vertex is complete
		{
			complete[vertex.first] = true;
			graph[vertex.first].clear();
			for (int e = 0; e < gene_includes[vertex.first].size(); e++)
			{
				if (gene_includes[vertex.first][e] > 0)
					graph[vertex.first].insert(e);
			}
		}
		else
			pq.push(vertex);
	}
	return graph;
}

void Chromosome::resolve_genes()
{
	resolver.build(genes);
	for (int i = 0; i < gene_codes.size(); i++)
	{
		int x = resolver.decode(gene_codes[i]);
		gene_indicies.push_back(x);
	}
}