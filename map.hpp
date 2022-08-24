#ifndef MAP_HPP
#define MAP_HPP

#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <utility>

#include "iterator_traits.hpp"
#include "reverse_iterator.hpp"
#include "pair.hpp"
#include "algorithm.hpp"

namespace ft
{

template <typename Key, typename Value, typename KeyCmpFn = std::less<Key>,
          typename Alloc = std::allocator<std::pair<const Key, Value> > >
class map
{
  protected:
	struct AA_base_node;
	struct AA_node;

	template<typename MaybeConstValue>
	class aat_iterator;

  public:
	typedef Key                                                        key_type;
	typedef Value                                                   mapped_type;
	typedef ft::pair<const Key, Value>                               value_type;
	typedef std::size_t                                               size_type;
	typedef std::ptrdiff_t                                      difference_type;
	typedef Alloc                                                allocator_type;
	typedef KeyCmpFn                                                key_compare;
	typedef value_type&                                               reference;
	typedef value_type const&                                   const_reference;
	typedef typename Alloc::pointer                                     pointer;
	typedef typename Alloc::const_pointer                         const_pointer;
	typedef aat_iterator<Value>   			                           iterator;
	typedef aat_iterator<const Value>                            const_iterator;
	typedef ft::reverse_iterator<iterator>                     reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>         const_reverse_iterator;

  protected:
	// the template keyword is only here so that the < can be correctly parsed
	typedef ft::pair<const Key, Value>                              pair_type_t;
	typedef AA_node                                                      node_t;
	typedef AA_node *                                                node_ptr_t;
	typedef typename
	Alloc::template rebind<node_t>::other                          node_alloc_t;

	// Were we able to use c++11, we would have used std::allocator_traits like this
	// typedef typename
	// std::allocator_traits<Alloc>::template rebind_alloc<node_t>          node_allocator_type;
	

	/* STATE */
	node_ptr_t          root_;
	size_type           size_;
	node_alloc_t  node_alloc_;
	key_compare         compare_func_;

	// Singleton for the NIL node 
#define NIL get_nil_()
	static AA_node *get_nil_()
	{
		static AA_base_node nil; 
		return static_cast<AA_node *>(&nil);
	}

	/* NESTED NODE CLASSES */

	// This allows us to use our tree with non default-constructible Key and Value
	// because using an having a Key and Value as members would force the call
	// of Key() and Value() at the time of creation of the NIL node
	struct AA_base_node
	{
		AA_node *left;
		AA_node *right;
		int      level;

		// To make the the nil node in get_nil()
		// NIL wants to pretend it is just like any other node
		/*Default Constructor*/ AA_base_node() :
			left(static_cast<AA_node*>(this)),
			right(static_cast<AA_node*>(this)),
			level(0)
		{ }

	  protected:

		/*Constructor*/ AA_base_node(AA_node *left, AA_node *right, int lvl) :
			left(left),
			right(right),
			level(lvl)
		{ }
	};

	struct AA_node : public AA_base_node
	{
		std::pair<Key, Value> pair;
		AA_node *parent;

		/*Constructor*/ AA_node(Key k, Value v, AA_node *p) :
			AA_base_node(NIL, NIL, 1),
			pair(k, v),
			parent(p)
		{ }

		typename std::pair<Key, Value>::first_type & key() { return pair.first; }
		typename std::pair<Key, Value>::second_type & value() { return pair.second;}
	};

	/* HELPERS */

	static node_ptr_t update_root(node_ptr_t root)
	{
		// If the the parent of the root is NIL
		// while (root != NIL)
		
		// If the root is its own parent use instead :
		while (root != root->parent)
		    root = root->parent;
		return root;
	}

	static node_ptr_t leftmost_(node_ptr_t node)
	{
		while (node->left != NIL)
			node = node->left;
		return node;
	}

	static node_ptr_t rightmost_(node_ptr_t node)
	{
		while (node->right != NIL)
			node = node->right;
		return node;
	}

	static node_ptr_t in_order_successor_(node_ptr_t node)
	{
		if (node->right == NIL)
			return node;
		else
			node = node->right;
		while (node->left != NIL)
			node = node->left;
		return node;
	}

	static node_ptr_t in_order_predecessor_(node_ptr_t node)
	{
		if (node->left == NIL)
			return node;
		else
			node = node->left;
		while (node->right != NIL)
			node = node->right;
		return node;
	}

	/*TREE BALANCING*/

	static node_ptr_t skew_(node_ptr_t root)
	{
		if (root->level && root->left->level == root->level) // red node to our left?
			return rotate_right_(root);
		else
			return root; // else no change neeeded
	}

	static node_ptr_t rotate_right_(node_ptr_t oldroot)
	{
		// Right rotation
		node_ptr_t newroot = oldroot->left;

		newroot->parent = oldroot->parent; // Parenting
		oldroot->parent = newroot;
		if (newroot->right != NIL)
			newroot->right->parent = oldroot;

		oldroot->left        = newroot->right;
		newroot->right       = oldroot;

		// return pointer of the node that came out on top
		return newroot;
	}

	static node_ptr_t split_(node_ptr_t root)
	{
		if (root->level && root->right->right->level == root->level) // 2 red nodes on our right ?
			return rotate_left_(root);
		else
			return root; // else no change needed
	}

	static node_ptr_t rotate_left_(node_ptr_t oldroot)
	{
		//Left rotation
		node_ptr_t newroot = oldroot->right;

		newroot->parent = oldroot->parent; // Parenting
		oldroot->parent = newroot;
		if (newroot->left != NIL)
			newroot->left->parent = oldroot;

		oldroot->right       = newroot->left;
		newroot->left        = oldroot;

		// promote newroot to next higher level
		newroot->level += 1;

		// return pointer of the node that came out on top
		return newroot;
	}

	static void update_level_(node_ptr_t node)
	{
		int ideal_level = 1 + std::min(node->left->level, node->right->level);
		// node's level above ideal ?
		if (node->level > ideal_level) // necessary test bc must not be touched if below ideal level (red node)
		{
			node->level = ideal_level;
			if (node->right->level > ideal_level) // node's right child red ?
				node->right->level = ideal_level;
		}
	}

	// Update node's level and then three skews and two splits do the trick
	static node_ptr_t fixup_after_delete_(node_ptr_t node)
	{
		update_level_(node);
		node               = skew_(node);
		node->right        = skew_(node->right);
		node->right->right = skew_(node->right->right);
		node               = split_(node);
		node->right        = split_(node->right);
		return (node);
	}

	/*INSERTION & DELETION*/

	iterator insert_(Key const& k, Value const& v)
	{
		node_ptr_t ret = NULL;;

		root_ = insert_(k, v, NIL, root_, &ret);
		// Need this line if we want root to be its own parent, need to change update_root if commmented out
		root_->parent = root_;
		return iterator(root_, ret);
	}

	node_ptr_t insert_(Key const& k, Value const& v, node_ptr_t parent, node_ptr_t current_node, node_ptr_t *ret)
	{
		if (current_node == NIL) // fell out of the tree?
		{
			++size_;
			// create a new leaf node
			current_node = node_alloc_.allocate(1);
			node_alloc_.construct(current_node, node_t(k, v, parent));
			*ret = current_node;
		}
		else if (compare_func_(k, current_node->key()))        // key is smaller?
			current_node->left = insert_(k, v, current_node, current_node->left, ret);   // ->insert left
		else if (compare_func_(current_node->key(), k))         // key is larger?
			current_node->right = insert_(k, v, current_node, current_node->right, ret); // ->insert right
		else
		{
			current_node->value() = v;
			*ret = current_node;
		}
		return split_(skew_(current_node)); // restructure and return result
	}

	// This implementation does not bother with special cases that do not need rebalancing
	node_ptr_t remove_(Key k, node_ptr_t node)
	{
		if (node == NIL) // Fell out of tree, key does not exist
			return node; // No-op
		else if (compare_func_(k, node->key()))
			node->left = remove_(k, node->left); // Look in left subtree
		else if (compare_func_(node->key(), k))
			node->right = remove_(k, node->right); // Look in right subtree
		else                                       // Found it !
		{
			if (node->right == NIL && node->left == NIL) // It's a leaf node, remove it
			{
				node_alloc_.destroy(node);
				node_alloc_.deallocate(node, 1);
				--size_;
				return NIL;
			}
			else if (node->left == NIL) // No left child ? Means it is a black leaf node. Replace with its red child
			{
				node->key()   = node->right->key();
				node->value() = node->right->value();
				node->right   = remove_(node->right->key(), node->right);
			}
			else // Find succesor, copy its values and remove successor instead
			{
				node_ptr_t successor   = in_order_successor_(node);
				node->key()              = successor->key();
				node->value()            = successor->value();
				node->right              = remove_(successor->key(), node->right);
			}
		}
		return fixup_after_delete_(node);
	}

	node_ptr_t clear_(node_ptr_t node)
	{
		if (node->left != NIL)
			node->left = clear_(node->left);
		if (node->right != NIL)
			node->right = clear_(node->right);
		// If both have returned NIL, our node is now a leaf node
		if (node != NIL && node->right == NIL && node->left == NIL) // It's a leaf node, remove it
		{
			delete node;
			--size_;
			return NIL;
		}
		return NIL;
	}

	/* INTERFACE */

  public:
	/*Constructor*/ map(Alloc alloc = Alloc()) :
		root_(NIL),
		size_(0),
		node_alloc_(alloc) // node_alloc_ and alloc are different types, implicit conversion thanks to allocator's special ctor
	{ }

	/*Destructor*/ ~map()
	{
		this->clear();
	}

	map operator=(map const& rhs)
	{
		if (this != &rhs)
		{
			this->clear();
			insert(rhs.begin(), rhs.last); // There must be a better way though
		}
	}

	mapped_type& operator[]( const Key& key )
	{
		// Using operator[] requires that the mapped type be default constructible
		// If that is not the case use this->at() or in C++11 this->emplace()
		iterator it = lower_bound(key); // (*it).first is *not less* than key
		if ( it == this->end() || compare_func_(key, (*it).first) ) // if (*i).first is greater than key i.e key does not exist
			it = insert(it, pair_type_t(key, mapped_type())); // Insert default value with that key
		return (*it).second;
	}

	// MODIFIERS

	void clear()
	{
		root_ = clear_(root_);
	}

	ft::pair<iterator, bool> insert(pair_type_t const& pair)
	{
		size_type size_before = size_;
		iterator it = insert_(pair.first, pair.second);

		if (size_before == size_)
			return ft::make_pair(it, false);
		else
			return ft::make_pair(it, true);
	}

	void insert(iterator first, iterator last)
	{
		for (; first != last; ++first)
			insert(*first);
	}

	iterator insert(iterator hint, pair_type_t const &new_val)
	{
		(void)hint;
		return insert(new_val).first;
	}

	size_type erase(Key const& k)
	{
		size_type size_before = size_;
		root_ = remove_(k, root_);
		if (size_before == size_)
			return 0;
		return 0;
	}

	void erase( iterator it )
	{
		erase(*it->key());
	}

	void erase( iterator first, iterator last )
	{
		for (; first != last ; ++first)
			erase(first);
	}

	void swap( map& other )
	{
		if (this != &other)
		{
			node_ptr_t tmp_root = root_;
			size_type tmp_size = size_;

			root_ = other.root_;
			size_ = other.size_;

			other.root_ = tmp_root;
			other.size_ = tmp_size;
		}
	}

	/* CAPACITY */

	bool empty() const
	{
		if (size_ == 0)
			return true;
		else
			return false;
	}

	size_type size() const
	{
		return size_;
	}

	size_type max_size() const
	{
		return node_alloc_.max_size();
	}

	/* LOOKUP */

	size_type count( const Key& key ) const
	{
		if (find(key) == this->end())
			return 0;
		return 1;
	}

	iterator find( const Key& key )
	{
		node_ptr_t current;
		bool searched_is_strictly_less;
		bool searched_is_strictly_greater;

		current = root_;
		while (current != NIL)
		{
			searched_is_strictly_less = compare_func_(key, current->key());
			searched_is_strictly_greater = compare_func_(current->key, key);
			if (searched_is_strictly_less)
				current = current->_left;
			else if (searched_is_strictly_greater)
				current = current->_right;
			else // they're equal
				return iterator(root_, current);
		}
		return this->end();
	}

	const_iterator find( const Key& key ) const
	{
		node_ptr_t current;
		bool searched_is_strictly_less;
		bool searched_is_strictly_greater; 

		current = root_;
		while (current != NIL)
		{
			searched_is_strictly_less = compare_func_(key, current->key());
			searched_is_strictly_greater = compare_func_(current->key, key);
			if (searched_is_strictly_less)
				current = current->_left;
			else if (searched_is_strictly_greater)
				current = current->_right;
			else // they're equal
				return const_iterator(root_, current);
		}
		return this->end();
	}

	ft::pair<iterator,iterator> equal_range( const Key& key )
	{
		return ft::make_pair(lower_bound(key), upper_bound(key));
	}

	ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const
	{
		return ft::make_pair(lower_bound(key), upper_bound(key));
	}

	iterator lower_bound( const Key& key )
	{
		bool searched_is_strictly_less;  
		bool searched_is_strictly_greater; 
		node_ptr_t current;
		node_ptr_t key_successor;

		key_successor = NULL;
		current = root_;
		while (current != NIL)
		{
			searched_is_strictly_less = compare_func_(key, current->key());
			searched_is_strictly_greater = compare_func_(current->key(), key);
			
			if (searched_is_strictly_less) // than current's key
				current = current->left;
			else if (searched_is_strictly_greater) // than current's key
			{
				key_successor = current;
				current = current->left;
			}
			else
				return iterator(root_, key_successor);
		}
		return iterator(root_, key_successor);

	}

	const_iterator lower_bound( const Key& key ) const
	{
		bool searched_is_strictly_less;  
		bool searched_is_strictly_greater; 
		node_ptr_t current;
		node_ptr_t key_successor;

		key_successor = NULL;
		current = root_;
		while (current != NIL)
		{
			searched_is_strictly_less = compare_func_(key, current->key());
			searched_is_strictly_greater = compare_func_(current->key, key);
			
			if (searched_is_strictly_less) // than current's key
				current = current->_left;
			else if (searched_is_strictly_greater) // than current's key
			{
				key_successor = current;
				current = current->_left;
			}
			else
				return const_iterator(root_, key_successor);
		}
		return const_iterator(root_, key_successor);
	}

	iterator upper_bound( const Key& key )
	{
		bool searched_is_strictly_less;  
		bool searched_is_strictly_greater; 
		node_ptr_t current;
		node_ptr_t key_successor;

		key_successor = NULL;
		current = root_;
		while (current != NIL)
		{
			searched_is_strictly_less = compare_func_(key, current->key());
			searched_is_strictly_greater = compare_func_(current->key, key);
			
			if (searched_is_strictly_less) // than current's key
				current = current->_left;
			else if (searched_is_strictly_greater) // than current's key
			{
				key_successor = current;
				current = current->_left;
			}
			else
				break;
		}
		return iterator(root_, key_successor);
	}

	const_iterator key_successor( const Key& key ) const
	{
		bool searched_is_strictly_less;  
		bool searched_is_strictly_greater; 
		node_ptr_t current;
		node_ptr_t key_successor;

		key_successor = NULL;
		current = root_;
		while (current != NIL)
		{
			searched_is_strictly_less = compare_func_(key, current->key());
			searched_is_strictly_greater = compare_func_(current->key, key);
			
			if (searched_is_strictly_less) // than current's key
				current = current->_left;
			else if (searched_is_strictly_greater) // than current's key
			{
				key_successor = current;
				current = current->_left;
			}
			else
				break;
		}
		return const_iterator(root_, key_successor);
	}

	/* OBSERVERS */

	allocator_type get_allocator() const
	{
		return allocator_type(node_alloc_); // Implicit conversion
	}

	key_compare key_comp() const
	{
		return compare_func_;
	}

	/* NESTED ITERATOR CLASSES */

  protected:
	template <typename MaybeConstValue>
	class aat_iterator
	{
	  public:
		typedef typename
		map<const Key, MaybeConstValue, KeyCmpFn, Alloc>::value_type value_type;
		typedef value_type&                                           reference;
		typedef value_type*                                             pointer;
		typedef bidirectional_iterator_tag                    iterator_category;
		typedef std::ptrdiff_t                                  difference_type;
	  protected:
		typedef
		map<Key, Value, KeyCmpFn, Alloc>::node_ptr_t         node_ptr_t;

		/* STATE */
		node_ptr_t             root_;
		node_ptr_t             current_;

	  public:

		/* Constructor */ aat_iterator(node_ptr_t root, node_ptr_t current)
			: root_(root), current_(current)
		{ }

		/* Copy Constructor */ aat_iterator(aat_iterator const &other)
			: root_(other.root_), current_(other.current_)
		{ }

		/* Conversion */ operator map<Key, Value, KeyCmpFn, Alloc>::const_iterator()
		{
			return map<Key, Value, KeyCmpFn, Alloc>::const_iterator(root_, current_);
		}

		aat_iterator &operator=(iterator const &rhs)
		{
			root_ = rhs.root_;
			current_ = rhs.current_;
			return *this;
		}
		pointer operator->() const { return &(this->operator*()); }

		reference operator*() const { return reinterpret_cast<reference>(current_->pair); }
		//reference operator*() const { return current_->pair; }

		bool operator==(aat_iterator const &rhs) const { return current_ == rhs.current_; }

		bool operator!=(aat_iterator const &rhs) const { return current_ != rhs.current_; }

		// iterator will cycle forward passing through an end's marker
		aat_iterator &operator++()
		{
			root_ = update_root(root_);
			if (root_ == NIL) // Tree empty ?
				current_ = NULL;
			else if (current_ == NULL) 
				current_ = leftmost_(root_);
			// If tree was empty but stuff got in since last call
			 // If has successor...
			else if (current_->right != NIL)
				current_ = leftmost_(current_->right); // ... goes to successor
			// Else if it has no successor under itself
			else if (current_ == current_->parent->left) // If it is its parent's left child
				current_ = current_->parent; // ... it becomes its parent
			// Then it's its parent's right child
			else 
			{
				while (current_ == current_->parent->right) // Go up the succession of right children
					current_ = current_->parent;
				if (current_ == current_->parent->left) // if it is its parent's left child
					current_ = current_->parent; // ... it becomes its parent
				else
					current_ = NULL; // ..else NULL, end is reached
			}
			return *this;
		}

		// iterator will cycle backward passing through an end's marker
		aat_iterator &operator--()
		{
			root_ = update_root(root_);
			if (root_ == NIL) // Tree empty ?
				current_ = NULL;
			else if (current_ == NULL) // Reached the end ?
				current_ = rightmost_(root_);
			else if (current_->left != NIL)
				current_ = rightmost_(current_->left);
			else if (current_ == current_->parent->right)
				current_ = current_->parent;
			else 
			{
				while (current_ == current_->parent->left) // Go up the succession of left children
					current_ = current_->parent;
				if (current_ == current_->parent->right) // if it is its parent's right child
					current_ = current_->parent; // ... it becomes its parent
				else
					current_ = NULL; // ..else NULL, end is reached
			}
			return *this;
		}

		aat_iterator & operator++(int)
		{
			aat_iterator tmp = *this;
			operator++();
			return tmp;
		}

		aat_iterator & operator--(int)
		{
			aat_iterator tmp = *this;
			operator--();
			return tmp;
		}
	};

  public:
	iterator begin()
	{
		return ++iterator(root_, NULL);
	}

	iterator end()
	{
		return iterator(root_, NULL);
	}

	const_iterator begin() const
	{
		return ++const_iterator(root_, NULL);
	}

	const_iterator end() const
	{
		return const_iterator(root_, NULL);
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(this->end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(this->begin());
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(this->end());
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(this->begin());
	}

	/* VALUE COMPARE */

	/* This exists only for forwarding the key_comp function*/
	class value_compare : public std::binary_function<pair_type_t, pair_type_t, bool>
	{
		protected:
		KeyCmpFn comp;
		value_compare(KeyCmpFn c) : comp(c)
		{ }

		public:
		value_compare() : comp()
		{ }
		bool operator()(pair_type_t const& x, pair_type_t const& y) const { return comp(x.first, y.first); }
	};

	map<Key, Value, KeyCmpFn, Alloc>::value_compare value_comp() const
	{
		return value_compare(compare_func_);

	}

#ifdef DEBUG
	/* DEBUG */

	void print_node(std::stringstream &ss, node_ptr_t node)
	{
		if (node != NIL)
		{
			ss << node->key() << " [label=< <b>" << node->key() << "</b><br/> <sub>" << node->parent->key() << "</sub>>]\n\t";
			if (node->left != NIL)
			{
				if (node->left->level == node->level)
				{
					ss << "{rank=same; " << node->key() << "; " << node->left->key() << "}\n\t";
					ss << node->left->key() << " [color=red]\n\t";
				}
				ss << node->key() << " -> " << node->left->key() << "\n\t";
				print_node(ss, node->left);
			}
			if (node->right != NIL)
			{
				if (node->right->level == node->level)
				{
					ss << "{rank=same; " << node->key() << "; " << node->right->key() << "}\n\t";
					ss << node->right->key() << " [color=red]\n\t";
				}
				ss << node->key() << " -> " << node->right->key() << "\n\t";
				print_node(ss, node->right);
			}
		}
	}

# define DEFAULT_FILENAME "tree"
	void print_dot()
	{
		static int n;
		print_dot(++n);
	}

	void print_dot(int n)
	{
		std::stringstream ss;
		print_node(ss, root_);

		std::stringstream title;
		title << DEFAULT_FILENAME << n << ".dot";
		std::ofstream file(title.str().c_str());
		file << "digraph {\n\n\t";
		file << ss.str();
		file << "\n}";
	}
# undef DEFAULT_FILENAME
#endif /* ifdef DEBUG */
#undef NIL
}; // class AA_tree
   
template< class Key, class T, class Compare, class Allocator >
bool	operator==( map< Key, T, Compare, Allocator > const & x, map< Key, T, Compare, Allocator> const & y )
{
	if ( x.size() != y.size() )
		return false;
	return ft::equal( x.begin(), x.end(), y.begin() );
}

template< class Key, class T, class Compare, class Allocator >
bool	operator<( map< Key, T, Compare, Allocator > const & x, map< Key, T, Compare, Allocator> const & y )
{
	return ft::lexicographical_compare( x.begin(), x.end(), y.begin(), y.end() );
}

template< class Key, class T, class Compare, class Allocator >
bool	operator!=( map< Key, T, Compare, Allocator > const & x, map< Key, T, Compare, Allocator> const & y )
{
	return !( x == y );
}

template< class Key, class T, class Compare, class Allocator >
bool	operator>( map< Key, T, Compare, Allocator > const & x, map< Key, T, Compare, Allocator> const & y )
{
	return !( x <= y );
}

template< class Key, class T, class Compare, class Allocator >
bool	operator>=( map< Key, T, Compare, Allocator > const & x, map< Key, T, Compare, Allocator> const & y )
{
	return !( x < y );
}

template< class Key, class T, class Compare, class Allocator >
bool	operator<=( map< Key, T, Compare, Allocator > const & x, map< Key, T, Compare, Allocator> const & y )
{
	return !( y < x );
}

} // namespace ft

// specialized algorithms
namespace std {
template< class Key, class T, class Compare, class Allocator >
void	swap( ft::map< Key, T, Compare, Allocator > & x, ft::map< Key, T, Compare, Allocator > & y )
{
	x.swap( y );
	return ;
}
} // namespace std

#endif /* MAP_HPP */
