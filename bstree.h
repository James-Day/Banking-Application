#ifndef BSTREE_H_
#define BSTREE_H_
#include "account.h"

class BSTree
{
public:
	BSTree();
	BSTree(const BSTree& source);
	~BSTree();
	bool Insert(Account* acc);
	bool Remove(Account* acc);
	bool Retrieve(const int& id, Account*& acc) const;
	void Display() const;
	void Empty();
	bool isEmpty() const;
	int getHeight()const;

	bool operator == (const BSTree& rhs) const;
	bool operator!= (const BSTree& rhs) const;
	BSTree operator+(const BSTree& rhs)const;
	BSTree& operator+=(const BSTree& rhs);
	BSTree& operator= (const BSTree& rhs);
private:
	struct Node {
		Account* pAcct;
		Node* right = nullptr;
		Node* left = nullptr;
	};
	void CopyBST(const Node* tree_original, Node*& tree_new);
	void InOrderHelper(const Node* node) const;
	int getHeight(const Node* node)const;
	bool EqualsRecursive(Node* root1, Node* root2) const;
	void PlusEqualsHelper(const Node* root, BSTree& insert_tree) const;
	Node* root_ = nullptr;
};
#endif

