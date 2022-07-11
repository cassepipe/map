/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassepipe <norminet@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 15:39:18 by cassepipe         #+#    #+#             */
/*   Updated: 2022/07/07 15:39:18 by cassepipe        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory>
#include <sstream>
#include <algorithm>
#include <tuple>
#include "vector_iterator.hpp"
#include "reverse_iterator.hpp"

namespace ft
{
template < typename T, typename Alloc = std::allocator<T> > // Space is actually required I think
class vector
{
  public:
	/// EXPOSED TYPES
	typedef T                                                        value_type;
	typedef Alloc									             allocator_type;
	typedef typename allocator_type::reference                        reference;
	typedef typename allocator_type::const_reference            const_reference;
	typedef typename allocator_type::pointer                            pointer;
	typedef typename allocator_type::const_pointer                const_pointer;

	// What if value_type is already const ? Well the compiler is smart enough 
	// to remove the extra const. Note that vector<const T> still won't compile
	// because of that leads std::allocator to having two functions with the 
	// same type signature. Writing our own const-friendly allocator would work
	typedef vector_iterator<value_type>                                iterator;
	typedef vector_iterator<const value_type>                    const_iterator;
	typedef	reverse_iterator<iterator>                         reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>       const_reverse_iterator; //Why do I need ft:: ?

	// Why ptrdiff_t ? Because it is the signed equivalent of size_t.
	// Good for pointer arithmetics
	typedef std::ptrdiff_t                                      difference_type;
	typedef std::size_t                                               size_type;

  protected:

	/** DATA **/
	// We need to store an allocator inside our instance in order to call its 
	// methods to, you know, allocate.
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

	// Default constructor. 
	// It is explicit because we won't allow anything to be converted implicity
	// to an allocator.
	explicit vector(const allocator_type& alloc = allocator_type()); // Calls Alloc<T> default ctor by default

	// Fill constructor
	// If a call is like "vector<Obj>(5));" and passes then the value of Obj() is passed by default
	explicit vector(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type());

	// Range constructor
	template <class InputIterator>
	vector(InputIterator first, InputIterator last,
	       const allocator_type& alloc =
	           allocator_type()); // Do some enable_if<> = 0 wizardry to ensure InputIterator is not anything

	// Copy constructor. Shall perform deep copy using operator=
	vector(const vector& other)
	{
		this->operator=(other);
	}

	// Destructor. Calls allocator's destroy() and deallocate() methods to release heap memory
	~vector()
	{
		this->clear();
		allocator_.deallocate(data_, capacity_);
	}

	/* CONVERSION OPERATOR */

	vector& operator=(const vector& rhs) // Performs deep copy
	{
		if (this != &rhs)
		{
			this->clear(); //Does not change capacity. Call destructors for every object in the vector

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
		return const_iterator(data_ + size_); 
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(data_);
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(data_); 
	}

	reverse_iterator rend()
	{
		return reverse_iterator(data_ + size_); 
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(data_ + size_); 
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
	// elements in case the vector itself is declared as const.
	// Reminder : In the case where an instance is declared const, having a func
	// that returns a simple reference version is not an issue as long we are not
	// using that function which polypmorphism prevents here.
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

	iterator erase(iterator position)
	{
		return erase(position, position + 1);
	}

	// No need for reallocation
	// The behaviour for last < first is not specified, making my own
	iterator erase(iterator first, iterator last)
	{
		iterator end = this->end();
		if (first > last)
			std::swap(first, last);
		if (first >= end)
			return end;
		if (first == last)
			return last;
		size_type distance = last - first;
		size_ -= distance;
		for (; last < end ; --distance, ++first, ++last)
		{
			allocator_.destroy(&(*first)); 
			allocator_.construct(&(*first), *last);
		}
		for (; distance > 0 ; --distance, ++first)
		{
			allocator_.destroy(&(*first)); 
		}
		return (++first);
	}

	void swap(vector& x)
	{
		vector<T> tmp = *this;
		*this = x;
		x = tmp;
	}

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
  bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;
	typename vector<T, Alloc>::iterator lit = lhs.begin();
	typename vector<T, Alloc>::iterator lend = lhs.end();
	typename vector<T, Alloc>::iterator rit = rhs.begin();
	typename vector<T, Alloc>::iterator rend = rhs.end();
	while (lit != lend)
	{
		if (*lit != *rit)
			return false;
		++rit;
		++lit;
	}
	return true;
}


template <class T, class Alloc>
  bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
{
	return !(lhs == rhs);
}


template <class T, class Alloc>
  bool operator<  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);


template <class T, class Alloc>
  bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);


template <class T, class Alloc>
  bool operator>  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);


template <class T, class Alloc>
  bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);

} // namespace ft

#endif /* VECTOR_HPP */
