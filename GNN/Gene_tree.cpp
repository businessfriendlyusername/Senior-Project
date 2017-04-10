#include "Gene_tree.h"
Gene_tree::Gene_tree()
{

}

Gene_tree::Gene_tree(vector<Gene> genes)
{
	gene_index = -1;
	for (int i = 0; i < genes.size(); i++)
	{
		Gene_tree* cur = this;
		for (int j = 0; j < genes[i].gene_code.length(); j++)
		{
			if (cur->children.count(genes[i].gene_code[j]) == 0)
			{
				Gene_tree* temp = new Gene_tree;
				cur->children.insert(make_pair(genes[i].gene_code[j], temp));
				cur = temp;
			}
			else
				cur = cur->children.at(genes[i].gene_code[j]);
		}
		if(cur != this && cur->gene_index == -2)
			cur->gene_index = i;
	}
}


int Gene_tree::decode(string code)
{
	Gene_tree* cur = this;
	int index = cur->gene_index;
	for (int i = 0; i < code.length(); i++)
	{
		if (cur->children.count(code[i]) != 0)
		{
			cur = cur->children.at(code[i]);
			if (cur->gene_index != -2)
				index = cur->gene_index;
		}
		else
			return index;
	}
	return index;
}