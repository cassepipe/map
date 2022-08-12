#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <ostream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <utility>

//#include "simple_aa_tree.hpp"
#include "aa_tree.hpp"

using std::vector;
using std::ostream;
using std::map;
using std::cout;
using std::string;

	template< typename T >
std::ostream& operator<<(ostream& os, std::vector<T> vec)
{
	for (typename vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		os << *it;
	}
	return os;
}

struct NotDefaultConstructiblePerson
{
	int age;
	string name;

	/*Constructor*/ NotDefaultConstructiblePerson(int age, string name) :
		age(age),
		name(name) {}
};

 
template<typename It>
void printInsertionStatus(It it, bool success)
{
    std::cout << "Insertion of " << it->first << (success ? " succeeded\n" : " failed\n");
}

using ft::AA_tree;
int main()
{
	if (1)
	{
		using ft::AA_tree;

		AA_tree<int, string> tree;

		//tree.insert(1, string("one"));
		//tree.insert(2, string("two"));
		//tree.insert(30, string("thirty"));
		tree.insert(3, string("three"));
		tree.insert(4, string("four"));
		tree.insert(0, string("zero"));
		tree.insert(7, string("seven"));
		tree.insert(5, "five");
		//tree.insert(16, string("sixteen"));
		tree.insert(8, string("eight"));
		tree.insert(6, string("six"));

		//tree.print_dot();
		//
		//tree.remove(4);
		//tree.remove(3);
		//tree.remove(0);
		//tree.remove(2);

		//tree.insert(20, string("twenty"));

		//tree.remove(0);
		//tree.remove(0);

		//tree.remove(4);

		//tree.insert(0, "zero");
		//tree.insert(4, "four");
		
		tree.print_dot();
		
	}
	if (0)
	{
		using ft::AA_tree;

		map<int, NotDefaultConstructiblePerson> my_map;
		my_map.insert(std::make_pair(1, NotDefaultConstructiblePerson(42, "Jean")));

		AA_tree<int, NotDefaultConstructiblePerson> tree;

		tree.insert(1, NotDefaultConstructiblePerson(42, "Jean"));
		tree.insert(2, NotDefaultConstructiblePerson(54, "Gérard"));
		tree.insert(3, NotDefaultConstructiblePerson(21, "Thomas"));

		tree.print_dot();
	}
	if (0)
	{
		map<int, int> my_map;

		my_map.insert(std::make_pair(1, 1));
		my_map.insert(std::make_pair(2, 2));
		my_map.insert(std::make_pair(3, 3));

		map<int, int >::const_iterator it = my_map.begin();
		--it;

		map<int, int >::reverse_iterator rit = my_map.rbegin(); 
		for(; rit != my_map.rend(); ++rit);
	}
	if (0)
	{
		map<int, int> my_map;

		my_map.insert(std::make_pair(1, 1));
		my_map.insert(std::make_pair(2, 2));
		my_map.insert(std::make_pair(3, 3));

		for(map<int, int >::const_iterator it = my_map.begin(); it != my_map.end(); ++it)
		{
			std::cout << it->first << " " << it->second <<  "\n";
		}

		for(map<int, int >::reverse_iterator it = my_map.rbegin(); it != my_map.rend(); ++it)
		{
			std::cout << it->first << " " << it->second <<  "\n";
		}

	}
	if (0)
	{
		using ft::AA_tree;

		AA_tree<int, NotDefaultConstructiblePerson> tree;

		tree.insert(1, NotDefaultConstructiblePerson(42, "Jean"));
		tree.insert(1, NotDefaultConstructiblePerson(23, "Thomas"));
		tree.print_dot();
	}

	return 0;
}
