#include "enable_if.hpp"
#include <iterator>

template< typename T>
class some_bidirectional_iterator 
{
	public:
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef std::ptrdiff_t difference_type;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;

	protected:
	pointer current_;

	public:

	reference operator+(difference_type n)
	{
		return some_bidirectional_iterator<T>(current_ + n);
	}
};

int main()
{
	char *msg = "Hello world";

	std::reverse_iterator(msg);
}
