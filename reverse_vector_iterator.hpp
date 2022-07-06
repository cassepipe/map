/* ######################## REVERSE VECTOR ITERATOR ######################### */

namespace ft
{

template <typename T> class vector_reverse_iterator
{
  protected:
	T* current_;

  public:
	/// EXPOSED TYPES
	
	// It is expected that iterators expose some traits for the purpose of using alogrithms that operates on them
	// They are pointers with metadata in a sort
	// We are not using iterator_traits here because T is supposed to be a pointer
	// and not an iterator, no need for forwarding anything
	typedef ft::random_access_iterator_tag                    iterator_category;
	typedef std::ptrdiff_t                                      difference_type;
	typedef T                                                        value_type;
	typedef T*                                                          pointer;
	typedef T&                                                        reference;

	/// CONSTRUCTORS

	// We must write a default ctor because we have written a value constructor

	/*Defautl Ctor*/ vector_reverse_iterator() : current_(NULL)
	{
	}
	/*Value Ctor*/ vector_reverse_iterator(T* ptr) : current_(ptr)
	{
	}

	// Compiler will generate copy ctor and operator=()

	/// ARITHMETIC OPERATORS

	vector_reverse_iterator& operator++()
	{
		--(current_);
	}

	vector_reverse_iterator& operator--()
	{
		++(current_);
	}

	vector_reverse_iterator& operator++(int)
	{
		vector_reverse_iterator tmp = *this;

		--(current_);
		return tmp;
	}

	vector_reverse_iterator& operator--(int)
	{
		vector_reverse_iterator tmp = *this;

		++(current_);
		return tmp;
	}

	vector_reverse_iterator operator+(const vector_reverse_iterator& rhs) const
	{
		return vector_reverse_iterator(current_ - rhs.current_);
	}

	vector_reverse_iterator operator-(const vector_reverse_iterator& rhs) const
	{
		return vector_reverse_iterator(current_ + rhs.current_);
	}

	vector_reverse_iterator operator+(difference_type i) const
	{
		return vector_reverse_iterator(current_ - i);
	}

	vector_reverse_iterator operator-(difference_type i) const
	{
		return vector_reverse_iterator(current_ + i);
	}

	vector_reverse_iterator operator+=(const vector_reverse_iterator& rhs)
	{
		current_ -= rhs.current_;

		return vector_reverse_iterator(current_);
	}

	vector_reverse_iterator operator-=(const vector_reverse_iterator& rhs)
	{
		current_ += rhs.current_;

		return vector_reverse_iterator(current_);
	}

	vector_reverse_iterator operator+=(difference_type i)
	{
		current_ -= i;

		return vector_reverse_iterator(current_);
	}

	vector_reverse_iterator operator-=(difference_type i)
	{
		current_ += i;

		return vector_reverse_iterator(current_);
	}

	/// DEREFERENCE OPERATORS

	T& operator*() const
	{
		return *current_;
	}

	T* operator->() const // This one I don't understand
	{
		return current_;
	}

	T& operator[](int i) const
	{
		return (current_ - i);
	}

	/// LOGICAL OPERATORS

	bool operator!() const
	{
		return !current_;
	}

	/// RELATIONAL OPERATORS Are they reversed too ?

	bool operator==(const vector_reverse_iterator& rhs) const
	{
		return (current_ == rhs.current_);
	}

	bool operator<(const vector_reverse_iterator& rhs) const
	{
		return (current_ < rhs.current_);
	}

	bool operator>(const vector_reverse_iterator& rhs) const
	{
		return (current_ > rhs.current_);
	}

	bool operator>=(const vector_reverse_iterator& rhs) const
	{
		return (current_ >= rhs.current_);
	}

	bool operator<=(const vector_reverse_iterator& rhs) const
	{
		return (current_ >= rhs.current_);
	}
};
} //namespace ft

