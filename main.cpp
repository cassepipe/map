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

int main()
{
		using ft::AA_tree;
#ifdef ONE
	{

		//AA_tree<int, string> tree;

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
		//tree.erase(4);
		//tree.erase(3);
		//tree.erase(0);
		//tree.erase(2);

		//tree.insert(20, string("twenty"));

		//tree.erase(0);
		//tree.erase(0);

		//tree.erase(4);

		//tree.insert(0, "zero");
		//tree.insert(4, "four");
		
		tree.print_dot();
	}
#endif
#ifdef B
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
#endif
#define C
#ifdef C
	{

		using ft::AA_tree;

		AA_tree<int, int> tree;

		tree.insert(std::make_pair(1, 1));
		tree.insert(std::make_pair(2, 2));

		tree.erase(0);

		tree.insert(std::make_pair(3, 3));
		tree.insert(std::make_pair(4, 4));
		tree.insert(std::make_pair(0, 0));
		tree.insert(std::make_pair(7, 7));

		tree.erase(4);

		tree.insert(std::make_pair(5, 5));

		tree.erase(3);

		tree.insert(std::make_pair(8, 8));

		tree.erase(0);

		tree.insert(std::make_pair(6, 6));

		tree.erase(7);

		tree.insert(std::make_pair(9, 9));

		tree.erase(7);


		tree.print_dot();

		for(AA_tree<int, int >::iterator it = tree.begin(); it != tree.end(); ++it)
		{
			std::cout << it->first << it->second << "\n";
		}
		std::cout <<  "\n";

		for(AA_tree<int, int>::const_iterator it = tree.begin(); it != tree.end(); ++it)
		{
			std::cout << it->first << it->second << "\n";
		}
		std::cout <<  "\n";

		for(AA_tree<int, int>::reverse_iterator it = tree.rbegin(); it != tree.rend(); ++it)
		{
			std::cout << it->first << it->second << "\n";
		}
		std::cout <<  "\n";
	}
#endif
#ifdef D
	{
		// Checking value replacement
		using ft::AA_tree;

		AA_tree<int, NotDefaultConstructiblePerson> tree;

		tree.insert(1, NotDefaultConstructiblePerson(42, "Jean"));
		tree.insert(1, NotDefaultConstructiblePerson(23, "Thomas"));
		tree.print_dot();
	}
#endif

	return 0;
}
