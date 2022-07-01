#include <cstddef>
#include <iterator>
#include <memory>

//// ITERATOR TRAITS

namespace ft
{

///  Marking input iterators.
struct input_iterator_tag
{
};

///  Marking output iterators.
struct output_iterator_tag
{
};

/// Forward iterators support a superset of input iterator operations.
struct forward_iterator_tag : public input_iterator_tag
{
};

/// Bidirectional iterators support a superset of forward iterator
/// operations.
struct bidirectional_iterator_tag : public forward_iterator_tag
{
};

/// Random-access iterators support a superset of bidirectional
/// iterator operations.
struct random_access_iterator_tag : public bidirectional_iterator_tag
{
};

/**
 *  @brief  Common %iterator class.
 *
 *  This class does nothing but define nested typedefs.  %Iterator classes
 *  can inherit from this class to save some work.  The typedefs are then
 *  used in specializations and overloading.
 *
 *  In particular, there are no default implementations of requirements
 *  such as @c operator++ and the like.  (How could there be?)
 */
template <typename Category, typename T, typename _Distance = ptrdiff_t, typename Pointer = T*, typename Reference = T&>
struct iterator
{
	/// One of the @link iterator_tags tag types@endlink.
	typedef Category iterator_category;
	/// The type "pointed to" by the iterator.
	typedef T value_type;
	/// Distance between iterators is represented as this type.
	typedef _Distance difference_type;
	/// This type represents a pointer-to-value_type.
	typedef Pointer pointer;
	/// This type represents a reference-to-value_type.
	typedef Reference reference;
};

/**
 *  @brief  Traits class for iterators.
 *
 *  This class does nothing but define nested typedefs.  The general
 *  version simply @a forwards the nested typedefs from the Iterator
 *  argument.  Specialized versions for pointers and pointers-to-const
 *  provide tighter, more correct semantics.
 */
template <typename _Iterator> struct iterator_traits
{
	typedef typename _Iterator::iterator_category iterator_category;
	typedef typename _Iterator::value_type value_type;
	typedef typename _Iterator::difference_type difference_type;
	typedef typename _Iterator::pointer pointer;
	typedef typename _Iterator::reference reference;
};

/// Partial specialization for pointer types.
template <typename T> struct iterator_traits<T*>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;
};

/// Partial specialization for const pointer types.
template <typename T> struct iterator_traits<const T*>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef const T* pointer;
	typedef const T& reference;
};

// If _Iterator has a base returns it otherwise _Iterator is returned
// untouched
template <typename _Iterator, bool _HasBase> struct _Iter_base
{
	typedef _Iterator iterator_type;
	static iterator_type _S_base(_Iterator __it)
	{
		return __it;
	}
};

template <typename _Iterator> struct _Iter_base<_Iterator, true>
{
	typedef typename _Iterator::iterator_type iterator_type;
	static iterator_type _S_base(_Iterator __it)
	{
		return __it.base();
	}
};

} // namespace ft

//// VECTOR ITERATOR

namespace ft
{
template <typename T> class vector_iterator
{
  protected:
	T* current_;

  public:
	/// EXPOSED TYPES
	//
	// It is expected that iterators expose some traits for the purpose of using alogrithms that operates on them
	// They are pointers with metadata in a sort
	// We are not using iterator_traits here because T is supposed to be a pointer
	// and not an iterator, no need for forwarding anything
	typedef ft::random_access_iterator_tag iterator_category;
	typedef std::ptrdiff_t difference_type;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;

	/// CONSTRUCTORS

	// We must write a default ctor because we have written a value constructor

	/*Defautl Ctor*/ vector_iterator() : current_(NULL)
	{
	}
	/*Value Ctor*/ vector_iterator(T* ptr) : current_(ptr)
	{
	}

	// Compiler will generate copy ctor and operator=()

	/// ARITHMETIC OPERATORS

	vector_iterator& operator++()
	{
		++(current_);
	}

	vector_iterator& operator--()
	{
		--(current_);
	}

	vector_iterator& operator++(int)
	{
		vector_iterator tmp = *this;

		++(current_);
		return tmp;
	}

	vector_iterator& operator--(int)
	{
		vector_iterator tmp = *this;

		--(current_);
		return tmp;
	}

	vector_iterator operator+(const vector_iterator& rhs) const
	{
		return this->vector_iterator(current_ + rhs.current_);
	}

	vector_iterator operator-(const vector_iterator& rhs) const
	{
		return this->vector_iterator(current_ - rhs.current_);
	}

	vector_iterator operator+(difference_type i) const
	{
		return this->vector_iterator(current_ + i);
	}

	vector_iterator operator-(difference_type i) const
	{
		return this->vector_iterator(current_ - i);
	}

	vector_iterator operator+=(const vector_iterator& rhs)
	{
		current_ += rhs.current_;

		return this->vector_iterator(current_);
	}

	vector_iterator operator-=(const vector_iterator& rhs)
	{
		current_ -= rhs.current_;

		return this->vector_iterator(current_);
	}

	vector_iterator operator+=(difference_type i)
	{
		current_ += i;

		return this->vector_iterator(current_);
	}

	vector_iterator operator-=(difference_type i)
	{
		current_ -= i;

		return this->vector_iterator(current_);
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
		return (current_ + i);
	}

	/// LOGICAL OPERATORS

	bool operator!() const
	{
		return !current_;
	}

	/// RELATIONAL OPERATORS

	bool operator==(const vector_iterator& rhs) const
	{
		return (current_ == rhs.current_);
	}

	bool operator<(const vector_iterator& rhs) const
	{
		return (current_ < rhs.current_);
	}

	bool operator>(const vector_iterator& rhs) const
	{
		return (current_ > rhs.current_);
	}

	bool operator>=(const vector_iterator& rhs) const
	{
		return (current_ >= rhs.current_);
	}

	bool operator<=(const vector_iterator& rhs) const
	{
		return (current_ >= rhs.current_);
	}
};
} //namespace ft

// VECTOR_CONST_ITERATOR
namespace ft
{
	template <typename T> class vector_const_iterator
	{
		public:
		//EXPOSED TYPES
		typedef	random_access_iterator_tag	iterator_category;
		typedef	std::ptrdiff_t				difference_type;
		typedef	const T						value_type;
		typedef	const T& 					reference;
		typedef	const T*					pointer; 

		protected:
		
		pointer								current_;

		public:

		/*Default ctor*/ vector_const_iterator() : current_(NULL)
	   {}

		// Is this what actually allows the compiler to determine T ?
		/*Value ctor*/ vector_const_iterator(pointer ptr) : current_(ptr) 
		{}

		// Copy and assiggment operator= generated by the compiler

	};
} // namespace ft

// REVERSE_ITERATOR

namespace ft
{
template <typename Iterator> class reverse_iterator
{
  protected:
	Iterator current;

  public:
	typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type value_type;
	typedef typename iterator_traits<Iterator>::difference_type difference_type;
	typedef typename iterator_traits<Iterator>::pointer pointer;
	typedef typename iterator_traits<Iterator>::reference reference;
};
} // namespace ft

namespace ft
{
template <typename T, typename Alloc = std::allocator<T>> // Space is actually required I think
class vector
{
  public:
	// types
	typedef T											value_type;
	typedef Alloc										allocator_type;
	typedef typename allocator_type::reference			reference;
	typedef typename allocator_type::const_reference	const_reference;
	typedef typename allocator_type::pointer			pointer;
	typedef typename allocator_type::const_pointer		const_pointer;

	typedef vector_iterator<value_type>					iterator;
	typedef vector_iterator<const value_type>			const_iterator;
	typedef reverse_iterator<iterator>					reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator; //Why do I need ft:: ?

	typedef std::ptrdiff_t difference_type;
	typedef std::size_t size_type;

  protected:
	// We need to store an allocator inside our instance in order to call its methods
	allocator_type allocator_;
	// The underlying data we are operating on
	T* data_;
	// The number of stored elements
	size_type size_;
	// Remaining allocated memory
	size_type capacity_;

  public:
	/// INTERFACE

	// Default constructor. Why is explicit needed ? (Prototype comes for cplusplus.com
	explicit vector(const allocator_type& alloc = allocator_type()); // Calls A<T> default ctor by default

	// Fill constructor
	explicit vector(size_type n,
	                const value_type& val =
	                    value_type(), // Call be like "vector<Obj>(5));" and passes rvalue of T() ctor by default
	                const allocator_type& alloc = allocator_type());

	// Range constructor
	template <class InputIterator>
	vector(InputIterator first, InputIterator last,
	       const allocator_type& alloc =
	           allocator_type()); // Do some enable_if<> = 0 wizardry to ensure InputIterator is not anything

	// Copy constructor. Shall perform deep copy using operator=
	vector(const vector& x);

	// Destructor. Calls allocator's destroy() and deallocate() methods to release heap memory
	~vector();

	vector& operator=(const vector& x); // Perform deep copy

	// Iterators methods
	iterator begin();
	const_iterator begin() const; // One marked const and not the other because polymorphism can't work only eith return types
	iterator end();
	const_iterator end() const;
	reverse_iterator rbegin();
	const_reverse_iterator rbegin() const;
	reverse_iterator rend();
	const_reverse_iterator rend() const;

	// Capacity
	size_type size() const
	{
		return size_;
	}

	size_type max_size() const 
	{
		return allocator_.max_size();
	}

	void resize(size_type n, value_type val = value_type());
	size_type capacity() const;
	bool empty() const;
	void reserve(size_type n);

	// Element access
	reference operator[](size_type n);
	const_reference operator[](size_type n) const;
	reference at(size_type n);
	const_reference at(size_type n) const;
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;

	// Modifiers
	template <class InputIterator> void assign(InputIterator first, InputIterator last);
	void assign(size_type n, const value_type& val);
	void push_back(const value_type& val);
	void pop_back();
	iterator insert(iterator position, const value_type& val);
	void insert(iterator position, size_type n, const value_type& val);
	template <class InputIterator> void insert(iterator position, InputIterator first, InputIterator last);
	iterator erase(iterator position);
	iterator erase(iterator first, iterator last);
	void swap(vector& x);
	void clear();
};
} // namespace ft
