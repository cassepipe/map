#ifndef SIMPLE_AA_TREE_HPP
#define SIMPLE_AA_TREE_HPP

#include <memory>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace ft
{


template <typename Key, typename Value>
class AA_tree;


// On the other hand, we don't care about the key and value arguments. Could it lead to issues ?
//#define NIL &nil_node
//template <typename Key, typename Value>
//static typename AA_tree<Key, Value>::AA_node nil_node(0, 0, 0); // Definition with constructor

template <typename Key, typename Value>
class AA_tree
{
  public:
	struct AA_node
	{
		Key      key;
		Value    value;
		int      level;
		AA_node* left;
		AA_node* right;

		/*Default ctor*/ AA_node() :
			//left(&nil_node),
			//right(&nil_node),
			key(0),
			value(0),
			level(0),
			left(0),
			right(0) {}

		/*Constructor*/ AA_node(Key k, Value v, int lvl, AA_node *l, AA_node *r) :
			//left(&nil_node),
			//right(&nil_node),
			key(k),
			value(v),
			level(lvl),
			left(l),
			right(r) {}

		/*Constructor*/ AA_node(Key k, Value v, int lvl) :
			//left(&nil_node),
			//right(&nil_node),
			key(k),
			value(v),
			level(lvl),
			left(get_nil_()),
			right(get_nil_()) {}
	};

	typedef AA_node                 node_type;
	typedef AA_node*                node_pointer;
	typedef std::size_t             size_type;

  protected:

	node_pointer           root_;
	size_type              size_;

	// Singleton for the nil node ? To be sure we have the same template arguments ?
	static node_pointer get_nil_() {
#define NIL get_nil_()
		// Only runs once
		static AA_node nil; // Only works if Key and Value are default constructible...
		// Then we redirect nil's left and right in the tree ctor
		return &nil;
	}

	/*TREE BALANCING*/

	node_pointer skew_(node_pointer root)
	{
		if (root->left->level == root->level) // red node to our left?
			return rotate_right_(root);
		else
			return root; // else no change neeeded
	}

	node_pointer rotate_right_(node_pointer old_root)
	{
		node_pointer new_root = old_root->left;
		old_root->left  = new_root->right;
		new_root->right = old_root;

		// return pointer of the new upper node
		return new_root;
	}

	node_pointer split_(node_pointer root)
	{
		if (root->right->right->level == root->level) // 2 red nodes on our right ?
			return rotate_left_(root);
		else
			return root; //else no change needed
	}

	node_pointer rotate_left_(node_pointer old_root)
	{
		node_pointer new_root = old_root->right;
		old_root->right = new_root->left;
		new_root->left  = old_root;

		// promote new_root to next higher level
		new_root->level += 1;

		// return pointer of the new upper node
		return new_root;
	}

	void  update_level_(node_pointer node)
	{
		int ideal_level = 1 + min(node->left->level, node->right->level);
		// node's level above ideal ? 
		if (node->level > ideal_level) // necessary test bc must not be touched if below ideal level (red node)
		{
			node->level = ideal_level;
			if (node->right->level > ideal_level) // node's right child red ?
				node->right->level = ideal_level;
		}
	}

	// Update node's level and then three skews and two splits do the trick
	node_pointer fixup_after_delete_(node_pointer node)
	{
		update_level_(node);
		node = skew_(node);
		node->right = skew_(node->right);
		node->right->right = skew_(node->right->right);
		node = split_(node);
		node->right = split_(node->right);
	}

	node_pointer in_order_successor_(node_pointer node)
	{
		while (node->left != NIL)
			node = node->left;
		return node;
	}

	node_pointer in_order_predecessor_(node_pointer node)
	{
		while (node->right != NIL)
			node = node->right;
		return node;
	}

	/*INSERTION & DELETION*/

	node_pointer insert_(Key k, Value v, node_pointer current_node)
	{
		if (current_node == NIL) // fell out of the tree?
		{
			++size_;
			current_node = new node_type(k, v, 1); // -> create a new leaf node here
		}
		else if (k < current_node->key) // key is smaller?
			current_node->left = insert_(k, v, current_node->left); // ->insert left
		else if (k > current_node->key) // key is larger?
			current_node->right = insert_(k, v, current_node->right); // ->insert right
		else
			current_node->value = v;
		return split_(skew_(current_node)); // restructure and return result
	}

	// This implementation does not bother with special cases that do not need rebalancing
	node_pointer remove_(Key k, node_pointer node)
	{
		if (node == NIL) // fell out of tree, key does not exist
			return node; // result in a no-op
		else if (k < node->key)
			node = remove_(k, node->left);
		else if (k > node->key)
			node = remove_(k, node->right);
		else
		{
			if (node->right == NIL && node->left == NIL) // It's a leaf node, remove it
			{
				delete node;
				--size_;
				return NIL;
			}
			else // Find successor, copy its values and remove successor instead
			{
				node_pointer successor = in_order_successor(node);
				node->key = successor->key;
				node->value = successor->value;
				remove_(successor->key, node->right);
			}
		}
		return fixup_after_delete_(node);
	}

	node_pointer clear_(node_pointer node)
	{
		if (node->left != NIL)
			node = clear_(node->left);
		if (node->left != NIL)
			node = clear_(node->right);
		// If both have returned NIL, our node is now a leaf nodek
		if (node != NIL && node->right == NIL && node->left == NIL) // It's a leaf node, remove it
		{
			delete node;
			--size_;
			return NIL;
		}
		return NIL;
	}

  public:
	/*Constructor*/ AA_tree() : root_(get_nil_()), size_(0)
	{
		get_nil_()->left = get_nil_();
		get_nil_()->right = get_nil_();
	}

	/*Destructor*/ ~AA_tree()
	{
		this->clear();
	}

	void clear()
	{
		root_ = clear_(root_);
	}

	// Because caller should not have access to structure's raw data
	void insert(Key k, Value v)
	{
		root_ = insert_(k, v, root_);
	}

	// Because caller should not have access to structure's raw data
	void remove(Key k)
	{
		root_ = remove_(k, root_);
	}

	void print_node(std::stringstream& ss, node_pointer node)
	{
		if (node->left != NIL)
		{
			ss << node->key << " -> " << node->left->key << '\n';
			print_node(ss, node->left);
		}
		if (node->right != NIL)
		{
			if (node->right->level == node->level)
			{
				ss << "{rank=same; " << node->key << "; " << node->right->key << "}\n";
				ss << node->key << " [color=red]\n";
			}
			ss << node->key << " -> " << node->right->key << '\n';
			print_node(ss, node->right);
		}
	}

# undef NIL

#define DEFAULT_FILENAME "tree.dot"
	void print_dot()
	{
		std::stringstream ss;
		print_node(ss, root_);

		std::ofstream file(DEFAULT_FILENAME);
		file << "digraph {\n";
		file << ss.str();
		file << "}\n";
	}
}; // class AA_tree

# undef DEFAULT_FILENAME

} // namespace ft

#endif /* SIMPLE_AA_TREE_HPP */
