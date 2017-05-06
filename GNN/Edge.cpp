#include "Edge.h"
Edge::Edge() {}

Edge::Edge(float w, Neuron* n)
{
	weight = w;
	address = n;
}

void Edge::fire(float sum)
{
	address->receive(sum * weight);
}