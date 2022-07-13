/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_traits.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassepipe <norminet@42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 15:41:04 by cassepipe         #+#    #+#             */
/*   Updated: 2022/07/07 15:41:04 by cassepipe        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

#include <cstdlib>

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
 *  Traits class for iterators.
 *
 *  This class does nothing but define nested typedefs.  The general
 *  version simply @a forwards the nested typedefs from the Iterator
 *  argument.  Specialized versions for pointers and pointers-to-const
 *  provide tighter, more correct semantics.
 */
template <typename Iterator> struct iterator_traits
{
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
};

/// Partial specialization for pointer types.
template <typename T> struct iterator_traits<T*>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef std::ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;
};

/// Partial specialization for const pointer types.
template <typename T> struct iterator_traits<const T*>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef std::ptrdiff_t difference_type;
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

#endif /* ITERATOR_TRAITS_HPP */
