#include "Population.h"

Population::Population(int popsize, int num_genes, double weights_mean, double weights_dev, int nodes_mean, int nodes_dev, float mutation_mean, float mutation_dev, set<char> alpha)
{
	for (int i = 0; i < popsize; i++)
	{
		Chromosome c;
		c.set_struct(Chromosome::rand_struct(weights_mean, weights_dev, nodes_mean, nodes_dev));
		c.set_genes(Chromosome::rand_genes(num_genes, weights_mean, weights_dev, nodes_mean, nodes_dev, mutation_mean, mutation_dev, alpha));
		cpool.push_back(c);
		Network n(c.build_matrix());
		npool.push_back(n);
	}
}

void Population::getData(string filename) {
	vector<vector<float>> receivedData;
	vector<float> temp;
	string value;
	ifstream file;
	file.open(filename);

	int counter = 0;
	if (!file.is_open())
	{
		cout << "FAILED TO OPEN FILE\n";
	}
	getline(file, value);
	while (getline(file, value))
	{
		istringstream line(value);
		string token;
		while (getline(line, token, ','))
		{
			counter++;
			if (counter > 6)
			{
				counter = 1;
				receivedData.push_back(temp);
				temp.clear();
			}

			if (counter != 1 && counter != 6)
			{
				if (token == "")
				{
					break;
				}
				float price = stof(token) / 100;
				temp.push_back(price);	
			}
		}
	}
	file.close();
	data = receivedData;
}

void Population::fitness_test(vector<float> day)
{
	for (int i = 0; i < npool.size(); i++)
	{
		char output = npool[i].step(day[0], day[1], day[2], day[3]);
		if (npool[i].position != output)//position changed
		{
			if (npool[i].position == 'l')//switch from long to short
			{
				npool[i].fitness += ((day[0] + day[3]) / 2) - npool[i].price;
				npool[i].price = ((day[0] + day[3]) / 2);
			}
			else if (npool[i].position == 's')//switch from short to long
			{
				npool[i].fitness += npool[i].price - ((day[0] + day[3]) / 2);
				npool[i].price = ((day[0] + day[3]) / 2);
			}
		}
	}
}

class compare
{
public:
	bool operator() (pair<Network, int>& left, pair<Network, int>& right)
	{
		return left.first.fitness > right.first.fitness;
	}
};
vector<Chromosome> Population::simulate()
{
	for(int i = 0; i < 4; i++)
		cout << data[0][i];
	
	for (int d = 0; d < 20; d++)
	{
		for (int i = 0; i < npool.size(); i++)
		{
			npool[i].step(data[d][0], data[d][1], data[d][2], data[d][3]);
		}
	}
	for (int i = 20; i < data.size(); i++)
	{
		fitness_test(data[i]);
		if (i % 19 == 0)
		{
			list<pair<Network, int>> sorted;
			vector<pair<int, float>> survival;
			for (int n = 0; n < npool.size(); n++)
				sorted.push_back(make_pair(npool[n], n));
			sorted.sort(compare());
			float sep = 100.0f / (float)sorted.size();
			int count = 0;
			for (auto x : sorted)
			{
				survival.push_back(make_pair(x.second, count * sep));
				count++;
			}
			vector<int> culled = cull(survival);
			reproduce(culled, i);
		}
	}
	return cpool;
}

bool Network::fire()
{
	static default_random_engine gen;
	uniform_real_distribution<> rand(0.0, 1.0);
	if (rand(gen) > .5)
		return true;
	else
		return false;
}

vector<int> Population::cull(vector<pair<int, float>> survival)
{
	static default_random_engine gen;
	uniform_real_distribution<> rand(0.0, 100.0);
	uniform_real_distribution<> offset(-3.0, 5.0);
	vector<int> culled;
	for (auto s : survival)
	{
		if (s.second + offset(gen) >= rand(gen))
		{
			culled.push_back(s.first);
		}
	}
	return culled;
}

void Population::reproduce(vector<int> culled, int data_index)
{
	static default_random_engine gen;
	uniform_int_distribution<> rand(0, cpool.size() - 1);
	list<int> sorted;
	for (auto c : culled)
		sorted.push_back(c);
	sorted.sort();
	vector<pair<int, Chromosome>> children;
	return;
	for (int i = 0; i < cpool.size(); i++)
	{
		if (i == sorted.front())//replace this 1
		{
			int r1 = rand(gen);
			bool dead = true;
			while (dead)
			{
				dead = false;
				r1 = rand(gen);
				for (auto s : sorted)
				{
					if (r1 == s)//random gene is dead
						break;
				}
			}
			dead = true;
			int r2 = rand(gen);
			while (dead)
			{
				dead = false;
				r2 = rand(gen);
				for (auto s : sorted)
				{
					if (r2 == s || r2 == r1)//random gene is dead
						break;
				}
			}
			sorted.pop_front();
			Chromosome child = cpool[r1].crossover(cpool[r2]);
			children.push_back(make_pair(i, child));
		}
	}
	for (auto child : children)
	{
		Network n(child.second.build_matrix());
		for (int i = data_index - 15; i <= data_index; i++)
			n.step(data[i][0], data[i][1], data[i][2], data[i][3]);
		cpool[child.first] = child.second;
		npool[child.first] = n;
	}
}

Chromosome Population::top()
{
	list<pair<Network, int>> sorted;
	for (int i = 0; i < npool.size(); i++)
		sorted.push_back(make_pair(npool[i], i));
	sorted.sort(compare());
	int fittest = sorted.back().second;
	return cpool[fittest];
}