#include "belvis.h"
using namespace std;

string belvis::trim_string(string s, int len)
{
	if (s.size() <= len)
		return s;
	s.erase(s.begin() + len, s.end());
	return s;
}


double belvis::rand_double(double max)
{
	random_device rd;
	double random = rd() / rd.max();
	random *= max;
	return random;
}


vector<string> belvis::chr_split(string str, char delim)
{
	list<int> indices;//the index of each delimeter
	int chunks = 1;//the number of chunks to split the string into
	vector<string> substrings;
	std::size_t substring_start = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == delim)
		{
			chunks++;
			string substr = str.substr(substring_start, i - substring_start);
			if (substr != "")
				substrings.push_back(substr);
			substring_start = i + 1;
		}
	}
	string substr = str.substr(substring_start, string::npos);
	if (substr != "")
		substrings.push_back(substr);
	return substrings;
}



string belvis::chr_remove(string str, char remove)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == remove)
		{
			str = str.substr(0, i) + str.substr(i + 1, string::npos);//comebine the half of the string before the delimeter with the half after the delimeter
			i -= 1;//account for char being removed
		}
	}
	return str;
}


string belvis::chr_replace(string str, char remove, char replace)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == remove)
			str[i] = replace;
	}
	return str;
}


bool belvis::file_is_empty(ifstream& file)
{
	return file.peek() == ifstream::traits_type::eof();
}

long double* belvis::shabba(long double rangz)
{
	long double shit[4];
	shit[0] = rangz;
	shit[1] = log2(rangz);//# of gold chainz
	shit[2] = log2(shit[1]);//# of bad bitches
	shit[3] = log2(shit[2]);//# of gold toofs
	return shit;
}

int belvis::reverse_permutation(int permutations, int objects)
{
	int product = 1;
	for (int i = 2; i <= objects; i++)
	{
		product *= i;
	}
	if (product >= permutations)
		return objects;
	int i = 1;
	while (product < permutations)
	{
		product *= objects + i;
		product /= i;
		i++;
	}
	return objects + i - 1;
}

char belvis::randomChar(set<char> alphabet)
{
	if (alphabet.size() == 0)
		return '0';
	static default_random_engine gen;
	static bool first = true;
	if (first)
	{
		gen.seed(time(NULL));
		first = false;
	}
	uniform_int_distribution<unsigned int> distribution(0, alphabet.size() - 1);
	set<char>::iterator it = alphabet.begin();
	advance(it, distribution(gen));
	return *it;
}

string belvis::randomString(int len, set<char> alphabet)
{
	string s;
	for (int i = 0; i < len; i++)
	{
		s += randomChar(alphabet);
	}
	return s;
}

int belvis::round_dtoi(double d)
{
	int integer = (int)d;
	double decimal = d - (double)integer;
	if (decimal >= 0.5)
		return integer + 1;
	else
		return integer;
}