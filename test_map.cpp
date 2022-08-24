#include <exception>
#include <iostream>

#include "test.h"
#include "test_map.hpp"

int test_map()
{
	test_map_begin();
	/*test( test_map_begin() )*/
	/*test( test_map_clear() )*/
	/*test( test_map_constructor() )*/
	/*test( test_map_count() )*/
	/*test( test_map_empty() )*/
	/*test( test_map_end() )*/
	/*test( test_map_equal_range() )*/
	/*test( test_map_erase() )*/
	/*test( test_map_find() )*/
	/*test( test_map_get_allocator() )*/
	/*test( test_map_insert() )*/
	/*test( test_map_key_comp() )*/
	/*test( test_map_lower_bound() )*/
	/*test( test_map_operator_bracket() )*/
	/*test( test_map_operator_equal() )*/
	/*test( test_map_rbegin() )*/
	/*test( test_map_relational_operators() )*/
	/*test( test_map_rend() )*/
	/*test( test_map_size() )*/
	/*test( test_map_swap() )*/
	/*test( test_map_swap_overload() )*/
	/*test( test_map_tags() )*/
	/*test( test_map_upper_bound() )*/
	/*test( test_map_value_comp() )*/
	return 0;
}

int	test_map_begin()
{
	map<char, int> tree;

	tree['b'] = 100;
	tree['a'] = 200;
	tree['c'] = 300;

	tree.print_dot();

	for ( NAMESPACE::map<char, int>::iterator it = tree.begin(); it != tree.end(); ++it)
		std::cout << it->first << "=>" << it->second << std::endl;

	return 0;
}

int	test_map_clear()
{

}

int	test_map_constructor()
{

}

int	test_map_count()
{

}

int	test_map_empty()
{

}

int	test_map_end()
{

}

int	test_map_equal_range()
{

}

int	test_map_erase()
{

}

int	test_map_find()
{

}

int	test_map_get_allocator()
{

}

int	test_map_insert()
{

}

int	test_map_key_comp()
{

}

int	test_map_lower_bound()
{

}

int	test_map_operator_bracket()
{

}

int	test_map_operator_equal()
{

}

int	test_map_rbegin()
{

}

int	test_map_relational_operators()
{

}

int	test_map_rend()
{

}

int	test_map_size()
{

}

int	test_map_swap()
{

}

int	test_map_swap_overload()
{

}

int	test_map_tags()
{

}

int	test_map_upper_bound()
{

}

int	test_map_value_comp()
{

}
