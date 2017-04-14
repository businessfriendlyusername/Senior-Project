#include "Chromosome.h"
#include "Population.h"
#include <iostream>
#include "Data.h"
#include "Network.h"
#include "belvis.h"
#include "Gene_tree.h"

using namespace std;
int main()
{
	Chromosome c;
	c.set_struct(Chromosome::rand_struct(3.0, 2.0, 10, 2));
	set<char> alpha;
	alpha.insert('a');
	alpha.insert('b');
	alpha.insert('c');
	alpha.insert('d');
	
	c.set_genes(Chromosome::rand_genes(10, 3.0, 2.0, 5, 2, 40, 10, alpha));
	c.gen_codes();
	//c.printGenes();
	//c.printadj();
	//c.gen_codes();
	//c.printCodes();
	c.validate_genes();

	//for (int i = 0; i < c.genes.size(); i++)
	//	cout << c.genes[i].gene_code << endl;
	//int var;
	//for (int i = 0; i < c.genes.size(); i++)
	//{
	//	for (int j = 0; j < c.genes[i].gene_nodes.size(); j++)
	//	{
	//		cout << c.genes[i].gene_nodes[j] << endl;
	//		var = g.decode(c.genes[i].gene_nodes[j]);
	//		cout << var;
	//		system("pause");
	//	}
	//}
}
