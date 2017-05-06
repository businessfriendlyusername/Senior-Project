#include "Chromosome.h"
#include "Population.h"
#include <iostream>
#include "Data.h"
#include "Network.h"
#include "belvis.h"
#include "Gene_tree.h"
#define FILENAME "data.csv"
#define SAVE "savedState2.txt"

using namespace std;
int main()
{
	set<char> alpha;
	alpha.insert('a');
	alpha.insert('b');
	alpha.insert('c');
	alpha.insert('d');
	Population pop(50, 5, 3.0, 2.0, 5, 2, 40, 10, alpha);
	pop.getData(FILENAME);
	for (int i = 0; i < 10; i++)
	{
		pop.simulate();
	}
	Chromosome c = pop.top();
	c.save();
	system("pause");
}
