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

using NAMESPACE::pair;
using NAMESPACE::make_pair;
using NAMESPACE::vector;
//using NAMESPACE::stack;
using NAMESPACE::map;

using std::ostream;
using std::cout;
using std::string;

#include "test_vector.hpp"
#include "test_stack.hpp"
#include "test_map.hpp"

int main()
{
	//test_vector();
	//test_stack();
	test_map();
}


