#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

namespace ft
{

template<typename InputIt1, typename InputIt2>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
			InputIt2 first2, InputIt2 last2)
	{
		for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
			if (*first1 < *first2) return true;
			if (*first2 < *first1) return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

template<typename InputIt1, typename InputIt2, typename Compare>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
			InputIt2 first2, InputIt2 last2,
			Compare comp)
	{
		for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
			if (comp(*first1, *first2)) return true;
			if (comp(*first2, *first1)) return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

template< typename InputIterator1, typename InputIterator2 >
bool	equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2 ) {

	while ( first1 != last1 )
	{
		if ( *first1 != *first2 )
			return false;
		++first1;
		++first2;
	}
	return true;
}

template< typename InputIterator1, typename InputIterator2, typename BinaryPredicate >
bool	equal( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred ) {

	while ( first1 != last1 )
	{
		if ( !pred( *first1, *first2 ) )
			return false;
		++first1;
		++first2;
	}
	return true;
}

} // namespace ft

#endif /* ALGORITHMS_HPP */
