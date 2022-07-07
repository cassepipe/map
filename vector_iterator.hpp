/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_iterator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassepipe <norminet@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 16:10:39 by cassepipe         #+#    #+#             */
/*   Updated: 2022/07/06 16:10:39 by cassepipe        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_ITERATOR_HPP
#define VECTOR_ITERATOR_HPP

#include <cstdlib>
#include "iterator_traits.hpp"

namespace ft
{
template <typename T> class vector_iterator
{
  protected:
	T*    current_;

  public:
	/// EXPOSED TYPES
	
	// It is expected that iterators expose some traits for the purpose of using alogrithms that operates on them
	// They are pointers with metadata in a sort
	typedef ft::random_access_iterator_tag                    iterator_category;
	//Why ptrdiff_t ? Because it is the signed equivalent of size_t. No, ssize_t is not.
	typedef std::ptrdiff_t                                      difference_type;
	typedef T                                                        value_type;
	typedef T*                                                          pointer;
	typedef T&                                                        reference;

	/// CONSTRUCTORS

	// We must write a default ctor because we have written a value constructor

	/*Defautl Ctor*/ vector_iterator() : current_(NULL)
	{
	}
	
	/*Value Ctor*/ vector_iterator(pointer ptr) : current_(ptr)
	{
	}

	// Compiler will generate trivial copy ctor and operator=()
	
	// CONVERSION OPERATOR
	// Allow non-const to const implicit conversion !!!
	operator vector_iterator<const T>() const
	{
		return (vector_iterator<const T>(current_));
	}
	
	/// DEREFERENCE OPERATORS 
	//	Basic requirements
	
	reference operator*() const
	{
		return *current_;
	}

	pointer operator->() const // This one I don't understand
	{
		return current_;
	}

	// Random access requirement
	reference operator[](int i) const
	{
		return (current_ + i);
	}
	
	/// INCREMENT OPERATORS
	// Forward Iterator requirements

	vector_iterator& operator++()
	{
		++current_;
		return *this;
	}

	vector_iterator& operator++(int)
	{
		vector_iterator tmp = *this;
		++current_;
		return tmp;
	}

	// DECREMENT OPERATORS
	// Bidrectional iterator requirements

	vector_iterator& operator--()
	{
		--current_;
		return *this;
	}

	vector_iterator& operator--(int)
	{
		vector_iterator tmp = *this;
		--current_;
		return tmp;
	}

	/// ARITHMETIC OPERATORS
	// Random access iterator requirements

	vector_iterator operator+(difference_type i) const
	{
		return vector_iterator(current_ + i);
	}

	vector_iterator operator-(difference_type i) const
	{
		return vector_iterator(current_ - i);
	}

	//Pas le choix si tu veux faire marcher des expressions telles que (-3 -it)
	friend vector_iterator operator+(difference_type i, const vector_iterator& it)
	{
		return vector_iterator(it.current__ + i);
	}

	friend vector_iterator operator-(difference_type i, const vector_iterator& it)
	{
		return vector_iterator(it.current__ - i);
	}
 
	vector_iterator operator+=(difference_type i)
	{
		current_ += i;

		return vector_iterator(current_);
	}

	vector_iterator operator-=(difference_type i)
	{
		current_ -= i;

		return vector_iterator(current_);
	}

	//Allow for const to non const comparisons
	template<typename RightIterator, typename LeftIterator>
	friend bool operator==(const vector_iterator<RightIterator>& lhs, const vector_iterator<LeftIterator>& rhs)
	{
		return lhs.current_ == rhs.current_;
	}

	template<typename RightIterator, typename LeftIterator>
	friend bool operator<(const vector_iterator<RightIterator>& lhs, const vector_iterator<LeftIterator>& rhs)
	{
		return lhs.current_ < rhs.current_;
	}

	template<typename RightIterator, typename LeftIterator>
	friend bool operator>(const vector_iterator<RightIterator>& lhs, const vector_iterator<LeftIterator>& rhs)
	{
		return lhs.current_ > rhs.current_;
	}
};

template<typename RightIterator, typename LeftIterator>
bool operator!=(const vector_iterator<RightIterator>& lhs, const vector_iterator<LeftIterator>& rhs)
{
	return !(lhs == rhs);
}

template<typename RightIterator, typename LeftIterator>
bool operator>=(const vector_iterator<RightIterator>& lhs, const vector_iterator<LeftIterator>& rhs)
{
	return !(lhs < rhs);
}

template<typename RightIterator, typename LeftIterator>
bool operator<=(const vector_iterator<RightIterator>& lhs, const vector_iterator<LeftIterator>& rhs)
{
	return !(lhs > rhs);
}

} //namespace ft

#endif /* VECTOR_ITERATOR_HPP */
