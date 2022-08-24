#ifndef TEST_H
#define TEST_H

#if NAMESPACE==ft
# include "vector.hpp"
# include "map.hpp"
#endif

#include <vector>
#include <map>
#include <stack>

using NAMESPACE::pair;
using NAMESPACE::make_pair;
using NAMESPACE::vector;
//using NAMESPACE::stack;
using NAMESPACE::map;

using std::cout;
using std::string;

typedef int (*func_ptr_t)();

int test(func_ptr_t);

#endif /* TEST_H */
