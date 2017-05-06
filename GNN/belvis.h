/*
~~~~~~~~~~~~DISCLAIMER~~~~~~~~~~~~~~~~
the randomness of these random functions is quite bad
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
some useful, but mostly un-related c++ functions
Author: Stephen Blevins
*/
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <fstream>
#include <random>
#include <set>
#include <time.h>
using namespace std;
class belvis
{
public:
	static string trim_string(string, int);//removes characters off the end of a string so that it is the length of int


	static double rand_double(double max);//generates a random double between 0 and max


	static vector<string> chr_split(string, char);//splits a string into n pieces along a single char delimeter


	static string chr_remove(string, char);//removes all instances of char from a string

	static string chr_replace(string, char remove, char replace);//replaces all instances of remove with replace in string

	//you still need to seed the random number generator using srand() before using this function
	//static bool prob(double);//randomly returns true or false based on the probability parameter between 0 and 1


	static bool file_is_empty(ifstream& file);//returns true if a file is empty

	static long double* shabba(long double rangs);//returns number of chainz, bad bitches, and gold teeth based on number of rangz

	static int reverse_permutation(int p, int a);//returns the minimum required length of a string with an alphabet of size a so that it has at least p permutations

	static char randomChar(set<char> alphabet);//returns a random char in range alphabet which counts down from 122 'z' on the ascii table

	static string randomString(int len, set<char> alphabet);//returns a random string of leng len composed of the set alphabet

	static int round_dtoi(double d);//rounds a double to int appropriately
};