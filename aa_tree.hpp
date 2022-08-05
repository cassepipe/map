#ifndef SIMPLE_AA_TREE_HPP
#define SIMPLE_AA_TREE_HPP

#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace ft
{

template <typename Key, typename Value>
class AA_tree;

template <typename Key, typename Value>
class AA_tree
{
# define NIL get_nil_()
  protected:
	struct AA_node;

	// This allows us to use our tree with non default-constructible Key and Value
	struct AA_base_node
	{
		AA_node* left;
		AA_node* right;
		int      level;

		/*Default ctor*/ AA_base_node() :
			left(NULL),
			right(NULL),
			level(0) {}

		protected:
		/*Level ctor*/ AA_base_node(AA_node *left, AA_node * right, int lvl) :
			left(left),
			right(right), 
			level(lvl) {}
	};

	struct AA_node : public AA_base_node
	{
		Key      key;
		Value    value;

		/*Default ctor*/ AA_node() :
			AA_base_node(NIL, NIL, 1),
			key(Key()),
			value(Value()) {}

		/*Constructor*/ AA_node(Key k, Value v) :
			AA_base_node(NIL, NIL, 1),
			key(k),
			value(v) {}
	};

	typedef AA_node                 node_type;
	typedef AA_node*                node_pointer;
	typedef std::size_t             size_type;

	/* AA_tree<Key, Value> class */

	node_pointer           root_;
	size_type              size_;

	// Singleton for the nil node ? To be sure we have the same template arguments ?
	static AA_node *get_nil_() {
		// Only run once ftw !
		static AA_base_node nil; // We'll redirect nil's left and right in AA_tree's constructor
		return static_cast<AA_node *>(&nil);
	}

	/*TREE BALANCING*/

	node_pointer skew_(node_pointer root)
	{
		if (root->level &&
				root->left->level == root->level) // red node to our left?
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
		if (root->level &&
				root->right->right->level == root->level) // 2 red nodes on our right ?
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
	node_pointer fixup_after_delete_(node_pointer node)
	{
		update_level_(node);
		node = skew_(node);
		node->right = skew_(node->right);
		node->right->right = skew_(node->right->right);
		node = split_(node);
		node->right = split_(node->right);
		return (node);
	}

	node_pointer in_order_successor_(node_pointer node)
	{
		if (node->right == NIL)
			return node;
		else
			node = node->right;
		while (node->left != NIL)
			node = node->left;
		return node;
	}

	node_pointer in_order_predecessor_(node_pointer node)
	{
		if (node->left == NIL)
			return node;
		else
			node = node->left;
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
			current_node = new node_type(k, v); // -> create a new leaf node here
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
		if (node == NIL)                // Fell out of tree, key does not exist
			return node;                                               // No-op
		else if (k < node->key) 
			node->left = remove_(k, node->left);        // Look in left subtree
		else if (k > node->key)
			node->right = remove_(k, node->right);     // Look in right subtree
		else                                                       //Found it !
		{
			if (node->right == NIL && node->left == NIL) // It's a leaf node, remove it
			{
				delete node;
				--size_;
				return NIL;
			}
			else if (node->left == NIL) // No left child ? Means it is a black leaf node. Replace with its red child
			{
				node->key = node->right->key;
				node->value = node->right->value;
				node->right = remove_(node->right->key, node->right);
			}
			else // Find succesor, copy its values and remove succesor instead
			{
				node_pointer successor = in_order_successor_(node);
				node->key = successor->key;
				node->value = successor->value;
				node->right = remove_(successor->key, node->right);
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
	/*Constructor*/ AA_tree() : root_(NIL), size_(0)
	{
		// Set NIL to point back to itself because it can't be done at instantiation
		NIL->left = NIL;
		NIL->right = NIL;
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
		if (node != NIL)
		{
			ss << node->key << " [label=< <b>" << node->key << "</b><br/> <sub>" << node->level << "</sub>>]\n\t";
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

# undef NIL

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
}; // class AA_tree

# undef DEFAULT_FILENAME

} // namespace ft

#endif /* SIMPLE_AA_TREE_HPP */
