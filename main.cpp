#include "algorithms.hpp"
#include "enable_if.hpp"
#include "is_integral.hpp"
#include "iterator_traits.hpp"
#include "remove_cv.hpp"
#include "reverse_iterator.hpp"
#include "reverse_vector_iterator.hpp"
#include "vector.hpp"
#include "vector_iterator.hpp"

#include <iostream>

int main()
{
	ft::vector<int> vec;

	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);

	vec.pop_back();
	vec.pop_back();
	vec.pop_back();

	std::cout << vec.capacity() << std::endl;
	std::cout << vec.size() << std::endl;
}
