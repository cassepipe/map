#include "test_map.hpp"

void test_map()
{
	test_begin();
	test_clear();
	test_constructor();
	test_count();
	test_empty();
	test_end();
	test_equal_range();
	test_erase();
	test_find();
	test_get_allocator();
	test_insert();
	test_key_comp();
	test_lower_bound();
	test_operator_bracket();
	test_operator_equal();
	test_rbegin();
	test_relational_operators();
	test_rend();
	test_size();
	test_swap();
	test_swap_overload();
	test_tags();
	test_upper_bound();
	test_value_comp();
}
