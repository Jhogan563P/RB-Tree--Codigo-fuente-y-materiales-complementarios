#include <iostream>
#include <vector>

using namespace std;

class splay_tree
{
public:
    //Return number of nodes in the Splay Tree
    virtual int get_num_nodes() = 0;

    //Splay the node to the root of the tree.
    //If node is present in tree, splay the node and return 1.
    //If the node is not present in the tree, return 0 and
    //splay the last seen node (before you fell off) to the root.
    virtual int find(int) = 0;

    //Insert a node into the splay tree.
    //If the node is already present, do not insert anything else
    //and splay the node.
    //If the node isn't present, insert it like a normal BST
    //and splay the inserted node.
    virtual void insert(int) = 0;

    //Delete an element from the splay tree.
    //If the node is present, delete it like how it is done in a BST.
    //https://en.wikipedia.org/wiki/Binary_search_tree#Deletion
    //Case 1. If there are no children, it can be removed directly.
    //Case 2. If it has only one child, that child takes the nodes place.
    //i.e. if 4 has to be deleted and 4 has 5 as a parent and 6 as a child,
    //6's parent becomes 5(5's child becomes 6) and 4 is removed.
    //The children of 6 are still it's children.
    //Case 3. If there are 2 children, the inorder successor takes the nodes place.
    //In all cases of successful deletion,
    //the parent of the node to be deleted is splayed.
    //If the node to be deleted is the root,
    //i.e. no parent, then no splay has to be done.
    //If the node to be deleted is not found,
    //the last seen node is splayed(to be consistent with find).
    virtual void remove(int) = 0;

    //Return a vector of the post order traversal of tree elements
    virtual vector<int> post_order() = 0;

    //Return a vector of the post order traversal of tree elements
    virtual vector<int> pre_order() = 0;
};


struct node
{
	int key;
	node* left,*right;
};

class splay_tree_implementation : public splay_tree
{
private:
	int number_of_nodes; // keep track of number of nodes
	node* root; // current root of the tree

public:
	splay_tree_implementation(); // constructor
	node* getNewNode(int); // creates new node
	node* rotateLeft(node*); // helper function
	node* rotateRight(node*); // helper function
	node* splay(node*,int); // // helper function (brings given node to root)
	int find(int); // find the node with the given value
	void insert(int); // insert a node into the tree
	void remove(int); // removes the node
	node* delete_parent(node*,int); // helper function
	node* delete_root(node*); // helper function
	vector<int> post_order(); // print the tree
	vector<int> pre_order(); // print the tree
	vector<int> in_order(); // print the tree
	void post(node*,vector<int>&); // helper function
	void pre(node*,vector<int>&); // helper function
	void in(node*,vector<int>&); // helper function
	int get_num_nodes(); // returns the number of nodes
	~splay_tree_implementation();
};

int splay_tree_implementation::get_num_nodes()
{
	return number_of_nodes;
}


splay_tree_implementation::splay_tree_implementation()
{
	root = NULL;
	number_of_nodes = 0;
}

node* splay_tree_implementation::getNewNode(int data)
{
	node* newNode = (node*)malloc(sizeof(node)*1);
	newNode -> key = data;
	newNode -> left = NULL;
	newNode -> right = NULL;
	//printf("made");
	return newNode;
}

node* splay_tree_implementation::rotateLeft(node* x)
{
	if(!x || !x->right)
	{
		//printf("error");
		return x;
	}
	else
	{
		node* temp = x -> right;
		x -> right = temp -> left;
		temp -> left = x;
		return temp;
	}
}

node* splay_tree_implementation::rotateRight(node* y)
{
	if(!y || !y->left)
	{
		//printf("error");
		return y;
	}
	else
	{
		node* temp = y -> left;
		y -> left = temp -> right;
		temp -> right = y;
		return temp;
	}
}


node* splay_tree_implementation::splay(node* r, int key)
{
	if(r == NULL || r -> key == key) // already splayed
	{
		return r;
	}

	if(key < r -> key)
	{
		if(!r->left) return r;
		if(key < r->left->key)		//left left
		{
			r->left->left = splay(r->left->left, key);
			r = rotateRight(r);
		}
		else if(key > r->left->key)  //left right
		{
			r->left->right = splay(r->left->right,key);
			r->left = rotateLeft(r->left);
		}
		r = rotateRight(r);

	}

	else
	{
		if(!r->right) return r;

		if(key > r->right->key)		//right rihgt
		{
			r->right->right = splay(r->right->right,key);
			r = rotateLeft(r);
		}
		else if(key < r->right->key)	//right left
		{
			r->right->left = splay(r->right->left,key);
			r->right = rotateRight(r->right);
		}
		r = rotateLeft(r);
	}
	return r;
}

int splay_tree_implementation::find(int key)
{
	//if(root == NULL) return 0;

	root = splay(root, key);

	if(root->key == key)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void splay_tree_implementation::insert(int key)
{
	node* root_cpy = root;

	if(root_cpy == NULL)// only one element
	{
		root =  getNewNode(key);
		number_of_nodes += 1;
	}
	else
	{
		node* parent = NULL;
		//parent = get_parent(root_cpy,key);
		while(root_cpy && root_cpy->key!=key)
		{
			parent = root_cpy;
			if(key < root_cpy->key) root_cpy = root_cpy->left;
			else root_cpy = root_cpy->right;
		}


		if(root_cpy == NULL)
		{

			node* newNode = getNewNode(key);
			if(key < parent->key) parent->left = newNode;
			else parent->right = newNode;

			number_of_nodes++;
		}
		root = splay(root, key);  // to bring nenode / parent to root

	}
}

node* splay_tree_implementation::delete_parent(node* r, int key)
{
	if (r == NULL) return NULL;
	node* parent = NULL;
	while(r != NULL && r->key!=key)
	{
		parent = r;
		if(key > r->key) r = r->right;
		else r = r->left;
	}


	if(r == NULL) return parent;

	if(!(r->left && r->right))
	{
		node* temp;
		if(!r->left) temp = r->right;
		else temp = r->left;

		if(parent->left == r) parent->left = temp;
		else parent->right = temp;

		free(r);
	}

	else
	{
		node* p = NULL;    				//
		node* successor = r->right; 	//we know successor isn't NULL;

		while(successor->left)
		{
			p = successor;
			successor = successor->left;
		}

		r->key = successor->key;

		if(!p) r->right = successor->right;
		else p->left = successor->right;
		free(successor);
	}
	number_of_nodes -= 1;
	return parent;
}

node* splay_tree_implementation::delete_root(node* r)
{
	if (root == NULL) return NULL;
	if(r->left == NULL)// go right
	{
		node* temp = r->right;
		free(r);
		return temp;
	}
	if(r->right == NULL)// go left
	{
		node* temp = r->left;
		free(r);
		return temp;
	}

	else // do rotations 2 childerens
	{

	node* parent = NULL;
	node* successor = r->right; // get inorder successor
	//sucessor = in_order_scucessor(sucessor)
	while(successor->left)
	{
		parent = successor;
		successor = successor->left;
	}

	r->key = successor->key;

	if(parent != NULL)
	{
		parent->left = successor->right;
	}
	else
	{
		 r->right = successor->right;
	}
	free(successor);
	return r;
	}

}

void splay_tree_implementation::remove(int key)
{
	if(root == NULL) return; // empty
	//node * parent == NULL;
	//parent get_parent(root_cpy,key);
	if(root->key == key) // root has data
	{
		root = delete_root(root);
		number_of_nodes--;
	}
	else
	{
		node* parent = delete_parent(root,key); // get parents data
		if (parent == NULL) printf("error!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		else root = splay(root, parent->key);
	}
}



void splay_tree_implementation::post(node* r, vector<int> &result)
{
	if(r != NULL)
	{
		post(r->left, result);
		post(r->right, result);
		result.push_back(r->key);
	}
	else;
}

vector<int> splay_tree_implementation::post_order()
{
	vector<int> result;
	if (root == NULL) return result;
	else{
	post(root,result);
	return result;}
}


void splay_tree_implementation::pre(node* r, vector<int> &result)
{
	if(r != NULL)
	{
		result.push_back(r->key);
		pre(r->left, result);
		pre(r->right, result);
	}
	else;
}

vector<int> splay_tree_implementation::pre_order()
{
	vector<int> result;
	if (root == NULL) return result;
	else{
	pre(root,result);
	return result;}
}

void splay_tree_implementation::in(node* r, vector<int> &result)
{
	if(r != NULL)
	{
		in(r->left, result);
		result.push_back(r->key);
		in(r->right, result);
	}
	else;
}

vector<int> splay_tree_implementation::in_order()
{
	vector<int> result;
	if (root == NULL) return result;
	else{
	in(root,result);
	return result;}
}

void del (node *head)
{
	if (head == NULL) return;
	else
	{
		del(head->right);
		del(head->left);
		free(head);
	}
	return;
}

splay_tree_implementation::~splay_tree_implementation()
{
    if (root)
    {
        del(root);
    }
    number_of_nodes = 0;
}
