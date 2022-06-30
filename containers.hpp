#include <cstddef>
#include <iterator>
#include <memory>


namespace ft {

    ///  Marking input iterators.
    struct input_iterator_tag { };

  ///  Marking output iterators.
    struct output_iterator_tag { };

  /// Forward iterators support a superset of input iterator operations.
    struct forward_iterator_tag : public input_iterator_tag { };

  /// Bidirectional iterators support a superset of forward iterator
    /// operations.
    struct bidirectional_iterator_tag : public forward_iterator_tag { };

  /// Random-access iterators support a superset of bidirectional
    /// iterator operations.
    struct random_access_iterator_tag : public bidirectional_iterator_tag { };
    
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
    template<typename Category, typename T, typename _Distance = ptrdiff_t,
             typename Pointer = T*, typename Reference = T&>
      struct iterator
      {
          /// One of the @link iterator_tags tag types@endlink.
          typedef Category  iterator_category;
          /// The type "pointed to" by the iterator.
          typedef T        value_type;
          /// Distance between iterators is represented as this type.
          typedef _Distance  difference_type;
          /// This type represents a pointer-to-value_type.
          typedef Pointer   pointer;
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
    template<typename _Iterator>
      struct iterator_traits
      {
          typedef typename _Iterator::iterator_category iterator_category;
          typedef typename _Iterator::value_type        value_type;
          typedef typename _Iterator::difference_type   difference_type;
          typedef typename _Iterator::pointer           pointer;
          typedef typename _Iterator::reference         reference;
        };
  
    /// Partial specialization for pointer types.
    template<typename T>
      struct iterator_traits<T*>
      {
          typedef random_access_iterator_tag iterator_category;
          typedef T                         value_type;
          typedef ptrdiff_t                   difference_type;
          typedef T*                        pointer;
          typedef T&                        reference;
        };
  
    /// Partial specialization for const pointer types.
    template<typename T>
      struct iterator_traits<const T*>
      {
          typedef random_access_iterator_tag iterator_category;
          typedef T                         value_type;
          typedef ptrdiff_t                   difference_type;
          typedef const T*                  pointer;
          typedef const T&                  reference;
        };
  
    /**
     *  This function is not a part of the C++ standard but is syntactic
     *  sugar for internal library use only.
  */
    template<typename _Iter>
      inline  
      typename iterator_traits<_Iter>::iterator_category
      __iterator_category(const _Iter&)
      { return typename iterator_traits<_Iter>::iterator_category(); }
  
    //@}
  
    // If _Iterator has a base returns it otherwise _Iterator is returned
    // untouched
    template<typename _Iterator, bool _HasBase>
      struct _Iter_base
      {
          typedef _Iterator iterator_type;
          static iterator_type _S_base(_Iterator __it)
      { return __it; }
        };
  
    template<typename _Iterator>
      struct _Iter_base<_Iterator, true>
      {
          typedef typename _Iterator::iterator_type iterator_type;
          static iterator_type _S_base(_Iterator __it)
      { return __it.base(); }
        };
  
  } // namespace
}

namespace ft
{
	template< typename T, typename A = std::allocator<T> > //Space is actually required I think
		class vector
		{
			public:
				//types
				typedef	T															value_type;
				typedef	Alloc														allocator_type;
				typedef typename allocator_type::reference							reference;
				typedef typename allocator_type::const_reference					const_reference;
				typedef typename allocator_type::pointer							pointer;
				typedef typename allocator_type::const_pointer						const_pointer;

				typedef typename std::iterator_traits<pointer>						iterator;

				typedef std::ptrdiff_t												difference_type;
				typedef std::size_t													size_type;




			//INTERFACE
			
			//Default constructor
			explicit vector (const allocator_type& alloc = allocator_type()); //Calls A<T> default ctor by default

			//Fill constructor
			explicit vector (size_type n, const value_type& val = value_type(), //Call be like "vector<Obj>(5));" and passes rvalue of T() ctor by default
					const allocator_type& alloc = allocator_type());

			//Range constructor
			template <class InputIterator>
				vector (InputIterator first, InputIterator last,
						const allocator_type& alloc = allocator_type()); //Do some enable_if<> = 0 wizardry to ensure InputIterator is not anything

			//Copy constructor
			vector (const vector& x);

			//Destructor
			~vector();

			vector& operator= (const vector& x);

			//Iterators
			iterator begin();
			const_iterator begin() const;


		};
} //namespace ft
