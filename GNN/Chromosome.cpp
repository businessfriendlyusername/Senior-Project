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
	xmat(string ine, int inp, int gin)
	{
		in_edge = ine;
		index = 0;
		input = inp;
		gindex = gin;
	};
	int input;//the number of edges in counted so far
	int gindex;//index of gene being expanded in genes member variable
	string in_edge;//the value of the incoming edge if there is one
	int index;//the index of the current iteration
};

class ymat
{
public:
	ymat(int gin, vector<string> pw, int b, int supb)
	{
		index = 0;
		super_begin = supb;
		gindex = gin;
		parent_weights = pw;
		begin = b;
	};
	int gindex;//the index of this gene in the genes member variable
	vector<string> parent_weights;//the output weight vector of the parent node
	int index;//current iteration of the node
	int begin;//the starting index of the matrix relative to the compiled matrix
	int super_begin;//the starting index of the super-matrix relative to the compiled matrix
};

vector<vector<string>> Chromosome::build_matrix()
{
	vector<vector<string>> matrix;
	list<ymat> ystack;
	vector<int> t;//temp
	for (int y = 0; y < gene_indicies.size(); y++)
	{
		if (gene_indicies[y] == -1)//y node is not a submatrix
		{
			vector<string> row;
			list<xmat> xstack;
			for (int x = 0; x < gene_indicies.size(); x++)
			{
				if (gene_indicies[x] == -1)//x node is not a submatrix
				{
					row.push_back(gene_weights[y][x]);
				}
				else//x node is a submatrix, expand
				{
					int incount = 0;
					for (int i = 0; i <= y; i++)//count number of edges that have fed into the node
					{
						if (gene_weights[i][x] != "0")
							incount++;
					}
					xstack.push_back(xmat(gene_weights[y][x], incount, gene_indicies[x]));
					while (!xstack.empty())//fully expand node and continue
					{

					}
				}
			}
		}
		else//node is a submatrix
		{
			ystack.push_back(ymat(gene_indicies[y], gene_weights[y], y, 0));
			while (!ystack.empty())//full expand node and continue
			{
				ymat ytop = ystack.front();
				Gene tgene = genes[ystack.front().gindex];
				while (ytop.index < tgene.gene_indicies.size())//iterate through node on top of the stack
				{
					if (tgene.gene_indicies[ytop.index] == -1)//y node is not a submatrix, populate row
					{
						vector<string> row;
						list<xmat> xstack;
						int outcount = 0;
						for (int i = 0; i < gene_indicies.size(); i++)
						{
							if (gene_indicies[i] == -1)//x node is not a submatrix, insert
							{
								if (row.size() < ytop.super_begin)//the row has not reached the ynode's parent expansion in the compiled matrix, it can have no connections
								{
									row.push_back("0");
								}
								else if (row.size() < ytop.begin)//the row has reached it's parent's expansion, but it has not reached its own expansion in the x direction yet, check for outputs
								{
									if (ytop.parent_weights[row.size() - ytop.super_begin] != "0")//outgoing edge detected
									{
										if (tgene.outputs[outcount] == ytop.index)//parent's outgoing edge comes from this subnode
										{
											row.push_back(ytop.parent_weights[row.size() - ytop.begin]);
										}
										else//outgoing edge comes from another subnode
											row.push_back("0");
										outcount++;
									}
								}
								else if (row.size() - ytop.begin < tgene.gene_indicies.size())//the row has reached its own expansion, interconnect
								{
									row.push_back(tgene.gene_weights[]);
								}
							}
							else//x node is a submatrix, expand
							{
								int incount = 0;
								if(gene_weights[y])
								for (int z = 0; z < matrix.size(); z++)
								{
									if (matrix[z][row.size()] != "0")//count how many edges have fed into the node being expanded
										incount++;
								}
								
							}


						}
					}
					else//ynode is a submatrix, expand
					{

					}
				}
			}
		}
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
					ystack.front().weights,//the structure and value of this node's output, so that it can coordinate with the output data to connect the sub-nodes to the super-node
					yindex,//the starting index of the expansion of this gene in its expanded parent matrix, so it knows where to interconnect itself
					ystack.front().master_y));//the location of this gene in the master matrix, to start populating its output row
			}
			else//node is not a submatrix, populate row in compiled matrix
			{
				list<xmat> xstack;
				vector<string> row;
				int incount = 0;
				for (int i = 0; i < gene_indicies.size(); i++)
				{
					if (gene_indicies[i] == -1)//node is not a submatrix, populate index[row][i]
					{
						if (gene_weights[ystack.front][i] != "0")//check if this y subnode corrosponds to this x output
						{
							if (ystack.front().outputs[incount] == ystack.front().index)//node corrosponds to output
							{
								row.push_back(gene_weights[ystack.front().master_y][i]);
							}
							else//incorrect output, push 0
							{
								row.push_back("0");
							}
						}
						else
							row.push_back("0");
					}
					else//node is a submatrix, expand
					{
						if (gene_weights[ystack.front().master_y][i] == "0")//no edge feeds in from master matrix, the expansion of this gene will be all 0's for this row
						{
							xstack.push_back(xmat("0", 0, genes[gene_indicies[i]].gene_indicies, gene_indicies[i]));
							while (!xstack.empty())
							{
								while (xstack.front().index < xstack.front().gene_indicies.size())
								{
									if (xstack.front().gene_indicies[xstack.front().index] != -1)
									{
										int index = xstack.front().index;
										xstack.front().index++;
										xstack.push_front(xmat("0", 0, genes[xstack.front().gene_indicies[index]].gene_indicies, gene_indicies[i]));
									}
									else
									{
										row.push_back("0");
										xstack.front().index++;
									}
								}
								xstack.pop_front();
							}
						}
						else//edge feeds in from master matrix, count how many edges have fed into compiled matrix, and remember the value of the edge in
						{
							int xindex = row.size();
							int c = 1;
							for (int z = 0; z < matrix.size(); z++)
							{
								if (matrix[z][xindex] != "0")
									c++;
							}
							xstack.push_front(xmat(gene_weights[ystack.front().master_y][i], c, genes[gene_indicies[i]].gene_indicies, gene_indicies[i]));
							while (!xstack.empty())
							{
								while (xstack.front().index < xstack.front().gene_indicies.size())
								{
									if (xstack.front().gene_indicies[xstack.front().index] != -1)//node is a submatrix, expand
									{
										int index = xstack.front().index;
										xstack.front().index++;
										int in_index = genes[xstack.front().gindex].inputs[xstack.front().input];
										
										if (in_index == index)//input from parent matrix feeds into submatrix
										{
											int xindex = row.size();
											int c = 1;
											for (int z = 0; z < matrix.size(); z++)
												if (matrix[z][xindex] != "0")
													c++;
											xstack.push_front(xmat(xstack.front().in_edge, c, genes[xstack.front().gene_indicies[index]].gene_indicies, xstack.front().gene_indicies[xstack.front().index]));
										}
										else
										{
											xstack.push_front(xmat("0", 0, genes[xstack.front().gene_indicies[index]].gene_indicies, xstack.front().gene_indicies[xstack.front().index]));
										}
									}
									else//node is not a submatrix, populate index
									{
										
									}
								}
							}
						}
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
