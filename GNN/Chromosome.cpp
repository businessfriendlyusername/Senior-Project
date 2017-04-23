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

class xmat
{
public:
	xmat(vector<string> g, string ine, int c, vector<int> inp, vector<int> indi)
	{
		gene_weights = g;
		in_edge = ine;
		count = c;
		index = 0;
		inputs = inp;
		gene_indicies = indi;
	};
	vector<string> gene_weights;
	vector<int> inputs;
	vector<int> gene_indicies;
	string in_edge;
	int count;
	int index;
};

class ymat
{
public:
	ymat(vector<int> outp, vector<int> indi, vector<string> w)
	{
		index = 0;
		outputs = outp;
		gene_indicies = indi;
	};
	vector<int> outputs;//indicies of this matrix's output nodes
	vector<int> gene_indicies;//the class and order of nodes in the matrix
	vector<string> weights;//the output weight vector of the parent matrix
	int index;//current iteration of the node
};

vector<vector<string>> Chromosome::build_matrix()
{
	vector<vector<string>> matrix;
	list<mat> stack;
	for (int y = 0; y < gene_codes.size(); y++)
	{
		for (int x = 0; x < gene_codes.size(); x++)
		{
			if (gene_indicies[x] != -1)//node is a submatrix
			{
				if (gene_weights[y][x] != "0")//edge leads into submatrix
				{
					int count = 0;
					for (int i = 0; i < y; i++)//count how many edges have fed into the submatrix so far
					{
						if (gene_weights[i][x] != "0")
							count++;
					}
					stack.push_front(mat(genes[gene_indicies[x]], gene_weights[y][x], genes[gene_indicies[x]].inputs[count]));
					while (!stack.empty())
					{
						int end = stack.front().gene.gene_indicies.size();
						while(stack.front().index < end)
						{
							if (stack.front().gene.gene_indicies[stack.front().index] != -1)//(sub)node is a (sub)submatrix  :P
							{
								if (stack.front().count == stack.front().index)//edge in from parent matrix
								{
									int count = 0;
									for (int i = 0; i < stack.front().index; i++)//count how many edges have fed into the submatrix so far
									{
										if (stack.front().gene.gene_weights[i][stack.front().index] != "0")
											count++;
									}
									stack.front().index++;
									stack.push_front(mat(genes[stack.front().gene.gene_indicies[stack.front().index]], stack.front().in_edge, genes[stack.front().gene.gene_indicies[stack.front().index]].inputs[count]));//absolutely disgusting
									break;
								}
								else// no edge leads into the (sub)submatrix from this (sub)node, expand but do not feed in
								{
									stack.front().index++;
									stack.push_front(mat(genes[stack.front().gene.gene_indicies[stack.front().index]], "0", 0));
									break;
								}
							}
							else//(sub)node is not a (sub)submatrix
							{
								if (stack.front().count == stack.front().index)//edge in from parent matrix
									row.push_back(stack.front().in_edge);
								else
									row.push_back("0");

								row.push_back(stack.front().gene.gene_weights[y][stack.front().index]);

								stack.front().index++;
							}
							
						}
					}
				}
				else//no edge leads into the submatrix from this node
				{
					stack.push_front(mat(genes[gene_indicies[x]], "0", 0));
					while (!stack.empty())
					{

					}
				}
			}
			else
			{
				row.push_back(gene_weights[y][x]);
			}
		}
	}
}

vector<vector<string>> Chromosome::build_matrix()
{
	vector<vector<string>> matrix;
	list<ymat> ystack;
	vector<int> t;//temp
	for (int i = 0; i < gene_indicies.size(); i++)
	{
		if (gene_indicies[i] == -1)//node is not a submatrix
			ystack.push_back(ymat(t, t, gene_weights[i]));
		else
			ystack.push_back(ymat(genes[gene_indicies[i]].outputs, genes[gene_indicies[i]].gene_indicies, gene_weights[i]));
	}
	int yindex = 0;//y index of the node at the top of the stack in the compiled matrix
	while (!ystack.empty())
	{
		while (ystack.front().index < ystack.front().gene_indicies.size())//iterate through each index on top of the stack
		{
			if (ystack.front().gene_indicies[ystack.front().index] != -1)//node is a submatrix, expand
			{
				int index = ystack.front().index;
				ystack.front().index++;
				//push the following data onto the top of the stack:
				ystack.push_front(ymat(genes[ystack.front().gene_indicies[index]].outputs,//the node's output data, to connect the sub-nodes to the super-node
					genes[ystack.front().gene_indicies[index]].gene_indicies,//the components of the node, to iterate through
					ystack.front().weights));//the structure and value of this node's output, so that it can coordinate with the output data to connect the sub-nodes to the super-node
			}
			else//node is not a submatrix, populate row in compiled matrix
			{
				list<xmat> xstack;
				for (int i = 0; i < gene_indicies.size(); i++)
				{
					if (gene_indicies[i] != -1)//node is a submatrix, expand
					{
						
					}
					else//node is not a submatrix
					{

					}
				}
				yindex++;
			}
		}
		ystack.pop_front();//done iterating through element, pop it
	}
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
		for (auto node : genes[i].gene_indicies)
		{
			if(node != -1)
				s.insert(node);
		}
		
		contains.push_back(s);
	}
	contains = make_dag(contains);//prune edges to make graph a DAG
	for (int i = 0; i < genes.size(); i++)
	{
		for (int j = 0; j < genes[i].gene_indicies.size(); j++)
		{
			if(genes[i].gene_indicies[j] != -1)//optimization
				if (contains[i].count(genes[i].gene_indicies[j]) == 0)//include is not allowed, prune
				{
					genes[i].gene_indicies[j] = -1;
					genes[i].gene_nodes[j] = "0";
				}
		}
	}
}


class compare//used in make_dag() list for sorting
{
public:
	bool operator() (pair<int, set<int>>& left, pair<int, set<int>>& right)
	{
		return (left.second.size() < right.second.size());
	}
};


vector<set<int>> Chromosome::make_dag(vector<set<int>> graph)
{
	list<pair<int, set<int>>> l;
	int i = 0;
	for (auto x : graph)
		l.push_front(make_pair(i++, x));
	l.sort(compare());
	set<int> acyclic;//nodes that can be legally contained by other nodes
	pair<int, set<int>> cur = l.front();
	while (cur.second.size() == 0)//add all genes which include no other genes to the acyclic set
	{
		acyclic.insert(cur.first);
		l.pop_front();

		cur = l.front();
	}
	while (l.size() != 0)
	{
		int smallest_diff = INT_MAX;
		vector<int> small;
		list<pair<int, set<int>>>::iterator small_it;
		int c = 0;
		int m = l.size();
		auto i = l.begin();
		while (c < m)//iterate through list. A lot of dp optimization is possible here but I am lazy and n should not be that big
		{
			vector<int> diff;

			set_difference(i->second.begin(), i->second.end(), acyclic.begin(), acyclic.end(), inserter(diff, diff.begin()));
			if (diff.size() == 0)//legal node found
			{
				acyclic.insert(i->first);
				auto temp = i;
				++temp;
				l.erase(i);
				i = temp;
				smallest_diff = -1;
				m--;
			}
			else if (smallest_diff > diff.size())//new smallest size found
			{
				smallest_diff = diff.size();
				small = diff;
				small_it = i;
			}
			c++;
			if(i != l.end())
				i++;
		}
		if (smallest_diff != -1)//cycle detected
		{
			for (auto s : small)
			{
				graph[small_it->first].erase(s);
			}
			acyclic.insert(small_it->first);
			l.erase(small_it);
		}
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
	for (int i = 0; i < genes.size(); i++)
	{
		for (int j = 0; j < genes[i].gene_nodes.size(); j++)
		{
			genes[i].gene_indicies.push_back(resolver.decode(genes[i].gene_nodes[j]));
		}
	}
}
