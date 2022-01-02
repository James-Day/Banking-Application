#include "bstree.h"

BSTree::BSTree()
{
	root_ = nullptr;
}
BSTree::BSTree(const BSTree& source)
{
	*this = source;
}
BSTree::~BSTree()
{
	Empty();
}

bool BSTree::Insert(Account* acc)
{
	if (root_ == nullptr)
	{
		root_ = new Node;
		root_->pAcct = new Account(*acc);
		return true;
	}
	Node* curr = root_;
	Node* parent = nullptr;

	while (curr != nullptr && *(curr->pAcct) != *(acc))
	{
		parent = curr;
		if (*(acc) < *(curr->pAcct))
		{
			curr = curr->left;
		}
		else
		{
			curr = curr->right;
		}
	}
	//no duplicates
	if (parent == nullptr)
	{
		return false;
	}
	else if ((curr != nullptr) && (*(curr->pAcct) == *(acc)))
	{
		return false;
	}
	else if (*(acc) < *(parent->pAcct))
	{
		parent->left = new Node;
		parent->left->pAcct = new Account(*acc);
		return true;
	}
	else
	{
		parent->right = new Node;
		parent->right->pAcct = new Account(*acc);
		return true;
	}
	return false;
}
bool BSTree::Remove(Account* acc)
{
	if (root_ == nullptr)
	{
		return false;
	}

	Node* curr = root_;
	Node* parent = curr;
	//search for parent
	while (curr != nullptr && *(curr->pAcct) != *(acc))
	{
		parent = curr;
		if (*(acc) < *(curr->pAcct))
		{
			curr = curr->left;
		}
		else
		{
			curr = curr->right;
		}
	}
	//couldn't find account to remove
	if (curr == nullptr)
	{
		return false;
	}
	//leaf node
	//if (parent->left == nullptr && parent->right == nullptr) {
	if (curr->left == nullptr && curr->right == nullptr)
	{
		if (curr != root_)
		{
			if (parent->left == curr)
			{
				parent->left = nullptr;
			}
			else
			{
				parent->right = nullptr;
			}
		}
		else
		{
			root_ = nullptr;
		}
		delete curr->pAcct;
		delete curr;
		return true;
	}
	// two child node
	else if (curr->left != nullptr && curr->right != nullptr)
	{
		//find successor
		Node* successor = curr->right;
		while (successor->left != nullptr)
		{
			successor = successor->left;
		}

		Account* copy = new Account(*successor->pAcct);
		//recursively call remove with successor, this successor can only have one child
		Remove(successor->pAcct);
		delete curr->pAcct;
		curr->pAcct = copy;
		return true;
	}
	//one child
	else {
		Node* child = nullptr;
		if (curr->left != nullptr)
		{
			child = curr->left;
		}
		else
		{
			child = curr->right;
		}
		if (curr != root_)
		{
			if (curr == parent->left)
			{
				delete curr->pAcct;
				delete curr;
				parent->left = child;
			}
			else
			{
				delete curr->pAcct;
				delete curr;
				parent->right = child;
			}
		}
		else
		{
			delete curr->pAcct;
			delete curr;
			root_ = child;
		}
		return true;
	}
	return false;
}
bool BSTree::Retrieve(const int& id, Account*& acc) const
{
	Node* curr = root_;
	while (curr != nullptr && curr->pAcct->GetId() != id)
	{
		if (id < curr->pAcct->GetId())
		{
			curr = curr->left;
		}
		else
		{
			curr = curr->right;
		}
	}
	if (curr == nullptr)
	{
		return false;
	}
	else
	{
		acc = curr->pAcct;
		return true;
	}

}
void BSTree::Display() const
{
	//In order traversal
	InOrderHelper(root_);
}
void BSTree::Empty()
{
	while (!isEmpty())
	{
		Remove(root_->pAcct);
	}
	root_ = nullptr;
}
bool BSTree::isEmpty() const
{
	if (root_ == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int BSTree::getHeight() const
{
	return getHeight(root_);
}

bool BSTree::operator==(const BSTree& rhs) const
{
	Node* curr = root_;
	Node* rhs_curr = rhs.root_;
	bool same = true;
	same = EqualsRecursive(curr, rhs_curr);
	return same;
}
bool BSTree::operator!=(const BSTree& rhs) const
{
	return !(*this == rhs);
}
BSTree BSTree::operator+(const BSTree& rhs) const
{
	BSTree result = *this;
	result += rhs;
	return result;
}
BSTree& BSTree::operator+=(const BSTree& rhs)
{
	PlusEqualsHelper(rhs.root_, *this);
	return *this;
}
BSTree& BSTree::operator=(const BSTree& rhs)
{
	root_ = new Node();
	CopyBST(rhs.root_, root_);
	return *this;
}

void BSTree::InOrderHelper(const Node* node) const
{
	if (node == nullptr)
	{
		return;
	}

	InOrderHelper(node->left);
	cout << *(node->pAcct);
	InOrderHelper(node->right);
}
void BSTree::CopyBST(const Node* treeOriginal, Node*& treeNew)
{

	if (treeOriginal == nullptr) //base case to end recursion when at tree end
		treeNew = nullptr;
	else
	{
		treeNew = new Node;
		treeNew->pAcct = new Account(treeOriginal->pAcct->GetId(), treeOriginal->pAcct->GetFirstName(), treeOriginal->pAcct->GetLastName());

		// Just call recursively to copy the subtrees:
		CopyBST(treeOriginal->left, treeNew->left);
		CopyBST(treeOriginal->right, treeNew->right);
	}
}
int BSTree::getHeight(const Node* node)const
{
	if (node == nullptr)
	{
		return -1;
	}
	int l_depth = getHeight(node->left);
	int r_depth = getHeight(node->right);
	if (l_depth > r_depth)
	{
		return (l_depth + 1);
	}
	else
	{
		return (r_depth + 1);
	}
}
bool BSTree::EqualsRecursive(Node* root1, Node* root2) const
{
	if (root1 == nullptr && root2 == nullptr)
	{
		return true;
	}
	if (root1 != nullptr && root2 != nullptr)
	{
		return
			(root1->pAcct == root2->pAcct && EqualsRecursive(root1->left, root2->left) && EqualsRecursive(root1->right, root2->right));
	}
	return false;
}
void BSTree::PlusEqualsHelper(const Node* root, BSTree& insert_tree) const
{
	if (root == nullptr)
	{
		return;
	}
	PlusEqualsHelper(root->left, insert_tree);
	insert_tree.Insert(root->pAcct);
	PlusEqualsHelper(root->right, insert_tree);
}
