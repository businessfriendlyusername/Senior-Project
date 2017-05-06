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
	
	c.set_genes(Chromosome::rand_genes(5, 3.0, 2.0, 5, 2, 40, 10, alpha));
	c.gen_codes();
	c.printGenes();
	c.validate_genes();
	c.printGenes();
	system("pause");
}
