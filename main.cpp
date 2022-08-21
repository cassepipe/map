#define NAMESPACE ft

#if NAMESPACE==ft
# include "pair.hpp"
# include "vector.hpp"
# include "map.hpp"
#endif

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

using NAMESPACE::vector;
using NAMESPACE::map;
using NAMESPACE::pair;
using NAMESPACE::make_pair;

using std::ostream;
using std::cout;
using std::string;

// Print a vector
	template< typename T >
std::ostream& operator<<(ostream& os, std::vector<T> vec)
{
	for (typename vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		os << *it;
	}
	return os;
}

// Not Default Constructible
struct NDCPerson
{
	int age;
	string name;

	/*Constructor*/ NDCPerson(int age, string name) :
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
	using ft::vector;
	using ft::map;
	using ft::pair;
	using ft::make_pair;
	{

		map<int, string> tree;

		tree.insert( ft::make_pair( 1, string("one")) );
		tree.insert( ft::make_pair( 2, string("two")));
		tree.insert( ft::make_pair( 30, string("thirty")));
		tree.insert( ft::make_pair( 3, string("three")));
		tree.insert( ft::make_pair( 4, string("four")));
		tree.insert( ft::make_pair( 0, string("zero")));
		tree.insert( ft::make_pair( 7, string("seven")));
		tree.insert( ft::make_pair( 5, string("five")));
		tree.insert( ft::make_pair( 16, string("sixteen")));
		tree.insert( ft::make_pair( 8, string("eight")));
		tree.insert( ft::make_pair( 6, string("six")));
		tree.insert( ft::make_pair( 6, string("six")));

		tree.print_dot(1);
	}

	{

		map<int, NDCPerson> my_map;
		my_map.insert( make_pair(1, NDCPerson(42, "Jean") ));

		map<int, NDCPerson> tree;

		tree.insert( make_pair( 1, NDCPerson(42, "Jean") ));
		tree.insert( make_pair( 2, NDCPerson(54, "Gérard") ));
		tree.insert( make_pair( 3, NDCPerson(21, "Thomas") ));

		tree.print_dot(2);
	}
	{

		map<int, int> tree;

		tree.insert( make_pair(1, 1) );
		tree.insert( make_pair(2, 2) );
		tree.erase(0);
		tree.insert( make_pair(3, 3) );
		tree.insert( make_pair(4, 4) );
		tree.insert( make_pair(0, 0) );
		tree.insert( make_pair(7, 7) );
		tree.erase(4);
		tree.insert( make_pair(5, 5) );
		tree.erase(3);
		tree.insert( make_pair(8, 8) );
		tree.erase(0);
		tree.insert( make_pair(6, 6) );
		tree.erase(7);
		tree.insert( make_pair(9, 9) );
		tree.erase(7);

		tree.print_dot(3);

		for(map<int, int >::iterator it = tree.begin(); it != tree.end(); ++it)
		{
			std::cout << it->first << it->second << "\n";
		}
		std::cout <<  "\n";

		for(map<int, int>::const_iterator it = tree.begin(); it != tree.end(); ++it)
		{
			std::cout << it->first << it->second << "\n";
		}
		std::cout <<  "\n";

		for(map<int, int>::reverse_iterator it = tree.rbegin(); it != tree.rend(); ++it)
		{
			std::cout << it->first << it->second << "\n";
		}
		std::cout <<  "\n";
	}
	{
		// Checking value replacement
		using ft::map;

		map<int, NDCPerson> tree;

		tree.insert( make_pair( 1, NDCPerson( 42, "Jean" )) );
		tree.insert( make_pair( 1, NDCPerson(23, "Thomas")) );

		tree.insert( make_pair( -1, NDCPerson( 30, "Eli" )) );
		tree.insert( make_pair( 5, NDCPerson(29, "Jean")) );

		tree.print_dot(4);
	}
	return 0;
}
