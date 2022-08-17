#ifndef AA_TREE_HPP
#define AA_TREE_HPP

#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <utility>

namespace ft
{

template <typename Key, typename Value, typename KeyCmpFn = std::less<Key>,
          typename Alloc = std::allocator<std::pair<const Key, Value> > >
class AA_tree
{
  protected:
	struct AA_base_node;
	struct AA_node;

  public:
	class iterator;
	class const_iterator;

	typedef Key            key_type;
	typedef Value          mapped_type;
	typedef AA_node        node_type;
	typedef AA_node *      node_pointer;
	typedef std::size_t    size_type;

	// the template keyword is only here so that the < can be correctly parsed
	typedef typename Alloc::template rebind<node_type>::other node_allocator_type;
	// Were we able to use c++11, we would have used std::allocator_traits like this
	// typedef typename std::allocator_traits<Alloc>::template rebind_alloc<node_type>		node_allocator_type;
	
  protected:

	/* DATA */
	node_pointer        root_;
	size_type           size_;
	node_allocator_type node_alloc_;
	KeyCmpFn            compare_func_;

	// Singleton for the nil node. Easy because benefits from the class templating
#define NIL get_nil_()
	static AA_node *get_nil_()
	{
		static AA_base_node nil; 
		return static_cast<AA_node *>(&nil);
	}

	/* NESTED NODE CLASSES */

	// This allows us to use our tree with non default-constructible Key and Value
	// because using an having a Key and Vlaue as members would force the use of Key() and Value()
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
		Key      key;
		Value    value;
		AA_node *parent;

		/*Default ctor*/ AA_node() :
			AA_base_node(NIL, NIL, 1),
			key(Key()),
			value(Value())
		{ }

		/*Constructor*/ AA_node(Key k, Value v, AA_node *p) :
			AA_base_node(NIL, NIL, 1),
			key(k),
			value(v), parent(p)
		{ }
	};

	/* HELPERS */

	static node_pointer update_root(node_pointer root)
	{
		// We chose for the root to be its own parent
		//while (root != root->parent)
		//    root = root->parent;
		return root;
	}

	static node_pointer leftmost_(node_pointer node)
	{
		while (node->left != NIL)
			node = node->left;
		return node;
	}

	static node_pointer rightmost_(node_pointer node)
	{
		while (node->right != NIL)
			node = node->right;
		return node;
	}

	static node_pointer in_order_successor_(node_pointer node)
	{
		if (node->right == NIL)
			return node;
		else
			node = node->right;
		while (node->left != NIL)
			node = node->left;
		return node;
	}

	static node_pointer in_order_predecessor_(node_pointer node)
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

	static node_pointer skew_(node_pointer root)
	{
		if (root->level && root->left->level == root->level) // red node to our left?
			return rotate_right_(root);
		else
			return root; // else no change neeeded
	}

	static node_pointer rotate_right_(node_pointer oldroot)
	{
		// Right rotation
		node_pointer newroot = oldroot->left;

		newroot->parent = oldroot->parent; // Parenting
		oldroot->parent = newroot;
		newroot->right->parent = oldroot;

		oldroot->left        = newroot->right;
		newroot->right       = oldroot;

		// return pointer of the node that came out on top
		return newroot;
	}

	static node_pointer split_(node_pointer root)
	{
		if (root->level && root->right->right->level == root->level) // 2 red nodes on our right ?
			return rotate_left_(root);
		else
			return root; // else no change needed
	}

	static node_pointer rotate_left_(node_pointer oldroot)
	{
		//Left rotation
		node_pointer newroot = oldroot->right;

		newroot->parent = oldroot->parent; // Parenting
		oldroot->parent = newroot;
		newroot->left->parent = oldroot;

		oldroot->right       = newroot->left;
		newroot->left        = oldroot;

		// promote newroot to next higher level
		newroot->level += 1;

		// return pointer of the node that came out on top
		return newroot;
	}

	static void update_level_(node_pointer node)
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
	static node_pointer fixup_after_delete_(node_pointer node)
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

	node_pointer insert_(Key k, Value v, node_pointer parent, node_pointer current_node)
	{
		if (current_node == NIL) // fell out of the tree?
		{
			++size_;
			// create a new leaf node
			current_node = node_alloc_.allocate(1);
			node_alloc_.construct(current_node, node_type(k, v, parent));
		}
		else if (compare_func_(k, current_node->key))        // key is smaller?
			current_node->left = insert_(k, v, current_node, current_node->left);   // ->insert left
		else if (compare_func_(current_node->key, k))         // key is larger?
			current_node->right = insert_(k, v, current_node, current_node->right); // ->insert right
		else
			current_node->value = v;
		return split_(skew_(current_node)); // restructure and return result
	}

	// This implementation does not bother with special cases that do not need rebalancing
	node_pointer remove_(Key k, node_pointer node)
	{
		if (node == NIL) // Fell out of tree, key does not exist
			return node; // No-op
		else if (compare_func_(k, node->key))
			node->left = remove_(k, node->left); // Look in left subtree
		else if (compare_func_(node->key, k))
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
				node->key   = node->right->key;
				node->value = node->right->value;
				node->right = remove_(node->right->key, node->right);
			}
			else // Find succesor, copy its values and remove successor instead
			{
				node_pointer successor = in_order_successor_(node);
				node->key              = successor->key;
				node->value            = successor->value;
				node->right            = remove_(successor->key, node->right);
			}
		}
		return fixup_after_delete_(node);
	}

	node_pointer clear_(node_pointer node)
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

  public:
	/*Constructor*/ AA_tree(Alloc alloc = Alloc()) :
		root_(NIL),
		size_(0),
		node_alloc_(alloc) // node_alloc_ and alloc are different types, implicit conversion thanks to allocator's special ctor
	{ }

	/*Destructor*/ ~AA_tree()
	{
		this->clear();
	}

	void clear()
	{
		root_ = clear_(root_);
	}

	void insert(Key k, Value v)
	{
		root_ = insert_(k, v, root_, root_);
		root_->parent = root_;
	}

	void remove(Key k)
	{
		root_ = remove_(k, root_);
	}

	/* NESTED ITERATOR CLASSES */

	class const_iterator
	{
	  protected:
		/* STATE */
		node_pointer             root_;
		node_pointer             current_;

	  public:
		/* Constructor */ const_iterator(node_pointer root, node_pointer current = NULL)
			: root_(root), current_(current)
		{ }

		/* Copy Constructor */ const_iterator(const_iterator const &other)
			: root_(other.root_), current_(other.current_)
		{ }

		const_iterator &operator=(const_iterator const &rhs)
		{
			root_ = rhs.root_;
			current_ = rhs.current_;
			return *this;
		}

		// const_iterator will cycle forward passing through an end's marker
		const_iterator &operator++()
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

		// const_iterator will cycle backward passing through an end's marker
		const_iterator &operator--()
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

		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			operator++();
			return tmp;
		}

		const_iterator operator--(int)
		{
			const_iterator tmp = *this;
			operator--();
			return tmp;
		}

		const mapped_type * operator->() const
		{
			return &(current_->value);
		}

		mapped_type operator*() const
		{
			return current_->value;
		}

		bool operator==(const_iterator const &rhs) const
		{
			return current_ == rhs.current_;
		}

		bool operator!=(const_iterator const &rhs) const
		{
			return current_ != rhs.current_;
		}
	};

	class iterator
	{
	  protected:
		/* STATE */
		node_pointer             root_;
		node_pointer             current_;

	  public:

		/* Constructor */ iterator(node_pointer root, node_pointer current = NULL)
			: root_(root), current_(current)
		{ }

		/* Copy Constructor */ iterator(iterator const &other)
			: root_(other.root_), current_(other.current_)
		{ }

		/* Conversion */ operator const_iterator()
		{
			return const_iterator(root_, current_);
		}

		iterator &operator=(iterator const &rhs)
		{
			root_ = rhs.root_;
			current_ = rhs.current_;
			return *this;
		}

		// iterator will cycle forward passing through an end's marker
		iterator &operator++()
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
		iterator &operator--()
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

		iterator operator++(int)
		{
			iterator tmp = *this;
			operator++();
			return tmp;
		}

		iterator operator--(int)
		{
			iterator tmp = *this;
			operator--();
			return tmp;
		}

		mapped_type * operator->() const
		{
			return &(current_->value);
		}

		mapped_type operator*() const
		{
			return current_->value;
		}

		bool operator==(iterator const &rhs) const
		{
			return current_ == rhs.current_;
		}

		bool operator!=(iterator const &rhs) const
		{
			return current_ != rhs.current_;
		}
	};

	const_iterator begin() const
	{
		return ++const_iterator(root_);
	}

	const_iterator end() const
	{
		return const_iterator(root_);
	}

	iterator begin()
	{
		return ++iterator(root_);
	}

	iterator end()
	{
		return iterator(root_);
	}

	/* DEBUG */

	void print_node(std::stringstream &ss, node_pointer node)
	{
		if (node != NIL)
		{
			ss << node->key << " [label=< <b>" << node->key << "</b><br/> <sub>" << node->parent->key << "</sub>>]\n\t";
			if (node->left != NIL)
			{
				if (node->left->level == node->level)
				{
					ss << "{rank=same; " << node->key << "; " << node->left->key << "}\n\t";
					ss << node->left->key << " [color=red]\n\t";
				}
				ss << node->key << " -> " << node->left->key << "\n\t";
				print_node(ss, node->left);
			}
			if (node->right != NIL)
			{
				if (node->right->level == node->level)
				{
					ss << "{rank=same; " << node->key << "; " << node->right->key << "}\n\t";
					ss << node->right->key << " [color=red]\n\t";
				}
				ss << node->key << " -> " << node->right->key << "\n\t";
				print_node(ss, node->right);
			}
		}
	}

#define DEFAULT_FILENAME "tree"
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
#undef DEFAULT_FILENAME

#undef NIL
}; // class AA_tree
} // namespace ft

#endif /* AA_TREE_HPP */
