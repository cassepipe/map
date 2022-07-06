/* ################################# VECTOR ################################# */

namespace ft
{
template < typename T, typename Alloc = std::allocator<T> > // Space is actually required I think
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

	typedef vector_iterator<value_type>                                iterator;
	// What if value_type is already const ? Well the compiler is smart enough to remove the extra const
	// Note that vector<const T> still won't compile because of that leads std::allocator to having
	// two functions with the same type signature, unless we write our own const-friendly allocator  
	typedef vector_iterator<const value_type>                    const_iterator;
	typedef vector_reverse_iterator<value_type>                reverse_iterator;
	typedef vector_reverse_iterator<const value_type>    const_reverse_iterator; //Why do I need ft:: ?

	typedef std::ptrdiff_t difference_type;
	typedef std::size_t size_type;

  protected:

	/** DATA **/
	// We need to store an allocator inside our instance in order to call its methods
	allocator_type allocator_;
	// The underlying data we are operating on
	pointer data_;
	// The number of stored elements
	size_type size_;
	// Remaining allocated memory
	size_type capacity_;

	/** HELPER FUNCTIONS **/
	
	size_type range_check_(size_type n)
	{
		if (n >= size_)
		{
			std::stringstream stream;
			stream << "ft::vector::range_check_\nTried to access element at index ";
			stream << n;
			stream << " while maximum addressable index is ";
			if (size_ == 0)
				stream << "none\n";
			else 
				stream << size_ - 1 << '\n';
			throw std::out_of_range(stream.str());
		}
		return n;
	}

  public:
	/** INTERFACE **/

	// Default constructor. Why is explicit needed ? (Prototype comes for cplusplus.com
	explicit vector(const allocator_type& alloc = allocator_type()); // Calls Alloc<T> default ctor by default

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
	vector(const vector& other)
	{
		operator=(other);
	}

	// Destructor. Calls allocator's destroy() and deallocate() methods to release heap memory
	~vector()
	{
		clear();
		allocator_.deallocate(data_, capacity_);
	}

	vector& operator=(const vector& rhs) // Performs deep copy
	{
		if (this != &rhs)
		{
			clear(); //Does not change capacity. Call destructors for every object in the vector

			if (rhs.size_ > capacity_) // If we don't have enough room, let's make some
			{
				allocator_.deallocate(data_, capacity_);
				capacity_ = rhs.size_;
				data_ = allocator_.allocate(capacity_);
			}
			size_ = rhs.size_;
			for (size_type i = 0; i < size_; ++i)
			{
				allocator_.construct(&data_[i], rhs.data_[i]);
			}
		}
		return *this;
	}

	/* Iterators */

	//The reason why we have const versions is because we can't return a iterator from a const vector !
	iterator begin() 
	{
		return iterator(data_); //Calls our iterator's constructor !
	}

	const_iterator begin() const
	{
		return const_iterator(data_); 
	}

	iterator end()
	{
		return iterator(data_ + size_); //Points to one past our storage
	}

	const_iterator end() const
	{
		return const_iterator(data_ + size_); //Points to one past our storage
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(data_); //Calls our iterator's constructor !
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(data_); //Calls our iterator's constructor !
	}

	reverse_iterator rend()
	{
		return reverse_iterator(data_ + size_); //Calls our iterator's constructor !
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(data_ + size_); //Calls our iterator's constructor !
	}

	/* Capacity */

	size_type size() const
	{
		return size_;
	}

	size_type max_size() const 
	{
		return allocator_.max_size();
	}

	void resize(size_type n, value_type val = value_type()) // No deallocation here. This is not shrink_to_fit()
	{
		if (capacity_ < n)
		{
			pointer tmp = allocator_.allocate(n);
			for (size_type i = 0; i < size_ ; ++i)
			{
				allocator_.construct(&tmp[i], data_[i]);
				allocator_.destroy(&data_[i]); //Is this faster than calling destroy in another loop ? I guess so
			}
			allocator_.deallocate(data_);
			data_ = tmp;
			capacity_ = n;
		}
		if (n < size_)
		{
			for (--size_ ; size_ >= n ; --size_)
				allocator_.destroy(&data_[size_]);
			++size_;
		}
		else
		{
			for (; size_ <= n ; ++size_)
				allocator_.construct(&data_[size_], value_type()); //Yes you can pass a constructor as a const ref to a value
		}
	}

	size_type capacity() const
	{
		return capacity_;
	}

	bool empty() const
	{
		if (size_ ==  0)
			return true;
		else
			return false;
	}

	void reserve(size_type n)
	{
		if (n > allocator_.max_size())
			throw std::length_error("vector::reserve");
		else if (n > capacity_)
		{
			pointer tmp = allocator_.allocate(n);
			for (size_type i = 0; i < size_ ; ++i)
			{
				allocator_.construct(&tmp[i], data_[i]);
				allocator_.destroy(&data_[i]);
			}
			allocator_.deallocate(data_);
			data_ = tmp;
			capacity_ = n;
		}
	}

	/* Element access */

	// The reason we need const_reference versions is to be able to access 
	// elements in case the object is declared as const.
	// Reminder : In the case where an instance is declared const, having a func
	// that returns a simple reference version is not an issue as long we are not
	// using that function.
	reference operator[](size_type n)
	{
		return data_[n];
	}

	const_reference operator[](size_type n) const 
	{
		return data_[n];
	}

	reference at(size_type n)
	{
		return data_[range_check_(n)];
	}

	const_reference at(size_type n) const 
	{
		return data_[range_check_(n)];
	}

	reference front() // I don't care if vector is empty because then it is UB
	{
		return data_[0];
	}

	const_reference front() const 
	{
		return data_[0];
	}

	reference back() // I don't care if vector is empty because then it is UB
	{
		return data_[size_ - 1];
	}

	const_reference back() const 
	{
		return data_[size_ - 1];
	}

	allocator_type get_allocator() const
	{
		return allocator_;
	}

	/* Modifiers */

	template <class InputIterator>
		void assign(InputIterator first, InputIterator last);

	void assign(size_type n, const value_type& val);

	void push_back(const value_type& val);

	void pop_back()
	{
		--size_;
		allocator_.destroy(&data_[size_]);
	}

	iterator insert(iterator position, const value_type& val);

	void insert(iterator position, size_type n, const value_type& val);

	template <class InputIterator> void insert(iterator position, InputIterator first, InputIterator last);
	iterator erase(iterator position);

	iterator erase(iterator first, iterator last);

	void swap(vector& x);

	void clear()
	{
		for (--size_; size_ >= 0; --size_)
		{
			allocator_.destroy(&data_[size_]);
		}
		++size_;
	}
};

template <class T, class Alloc>
  bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);


template <class T, class Alloc>
  bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);


template <class T, class Alloc>
  bool operator<  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);


template <class T, class Alloc>
  bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);


template <class T, class Alloc>
  bool operator>  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);


template <class T, class Alloc>
  bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

} // namespace ft
