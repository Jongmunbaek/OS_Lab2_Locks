#include <iostream.h>

enum Boolean {FALSE, TRUE};
template <class Type>
class Element {
public:
    Type key;
};

template <class Type>
class BstNode {
friend class BST<Type>;
public:
private:
   Element<Type> data;
   BstNode *LeftChild, *RightChild;
   void display(int i);
   void treeprint(int i);
};

template <class Type>
class BST {
public:
   BST(BstNode<Type> *init = 0) {root = init;};

//   BST& operator=(const BST&);

   Boolean Insert(const Element<Type>& x);
//   Boolean Delete(const Element<Type>&);
   BstNode<Type>* Search(BstNode<Type>*, const Element<Type>&);
   BstNode<Type>* Search(const Element<Type>&);
   BstNode<Type>* IterSearch(const Element<Type>&);

   void treeprint() { cout << "\n"; root->treeprint(1); }

   void display() {cout << "\n";
		   if (root) root->display(1);
		   else cout << "0\n";};
   Element<Type>* Split(Type i, BST& B, BST& C, Element<Type> &x);
private:
   BstNode<Type> *root;
};

template <class Type>
void BstNode<Type>::display(int i)
{
   cout << "Position " << i << ": data.key " << data.key << "\n";
   if (LeftChild) LeftChild->display(2*i);
   if (RightChild) RightChild->display(2*i + 1);
};

template <class Type>
Element<Type>* BST<Type>::Split(Type i, BST<Type>& B, BST<Type>& C,
Element<Type> &x)
// Split the binary search tree with respect to key @i@
{
    if (!root) {B.root = C.root = 0; return 0;} // empty tree
    BstNode<Type> *Y = new BstNode<Type>; BstNode<Type> *L = Y;
    BstNode<Type> *Z = new BstNode<Type>; BstNode<Type> *R = Z;
    BstNode<Type> *t = root;
    while (t)
	if (i == t->data.key) {  // split at @t@
	    L->RightChild = t->LeftChild;
	    R->LeftChild = t->RightChild;
	    x = t->data;
	    B.root = Y->RightChild; delete Y;
	    C.root = Z->LeftChild; delete Z;
	    return &x;
	}
	else if (i < t->data.key) {
	    R->LeftChild = t;
	    R = t; t = t->LeftChild;
	}
	else {
	    L->RightChild = t;
	    L = t; t = t->RightChild;
	}
    L->RightChild = R->LeftChild = 0;
    B.root = Y->RightChild; delete Y;
    C.root = Z->LeftChild; delete Z;
    return 0;
}

template <class Type>
void BstNode<Type>::treeprint(int l)
{
   if (this) {
      (this->RightChild)->treeprint(l+1);
      for (int i = 1; i <= l; i++) cout << "   ";
      cout << this->data.key << "\n";
      (this->LeftChild)->treeprint(l+1);
   };
};

template <class Type>
BstNode<Type>* BST<Type>::Search(BstNode<Type>* b, const Element<Type> &x)
{
   if (!b) return 0;
   if (x.key == b->data.key) return b;
   if  (x.key < b->data.key) return Search(b->LeftChild,x);
   return Search(b->RightChild,x);
};

template <class Type>
BstNode<Type>* BST<Type>::Search(const Element<Type>& x)
{
   return Search(root, x);
}

template <class Type>
BstNode<Type>* BST<Type>::IterSearch(const Element<Type>& x)
{
   for (BstNode<Type> *t = root;  t; )
   {
      if (x.key == t->data.key) return t;
      if (x.key < t->data.key) t = t->LeftChild;
      else t = t->RightChild;
   }
   return 0;
}

template <class Type>
Boolean BST<Type>::Insert(const Element<Type>& x)
{
   BstNode<Type> *p = root;  BstNode<Type> *q = 0;
   while (p) {
      q = p;
      if (x.key == p->data.key) return FALSE; //x.key is already in t
      if (x.key < p->data.key) p = p->LeftChild;
      else p = p->RightChild;
   };

   p = new BstNode<Type>;
   p->LeftChild = p->RightChild = 0; p->data = x;
   if (!root) root = p;
   else if (x.key < q->data.key) q->LeftChild = p;
	else q->RightChild = p;
   return TRUE;
}

main()
{
   BST<int> m;
   Element<int> a, b, c, d, e, f, g, h, i, j, k, l;
   a.key = 5; b.key = 3; c.key = 11; d.key = 3; e.key = 15;
   f.key = 2; g.key = 8; h.key = 22; i.key = 20; j.key = 9;
   cout << "\n" << m.Insert(a);
   cout << "\n" << m.Insert(b);
//   m.treeprint();
   cout << "\n" << m.Insert(c);
//   m.treeprint();
   cout << "\n" << m.Insert(d);
//   m.treeprint();

   cout << "\n" << m.Insert(e);
//   m.treeprint();
   cout << "\n" << m.Insert(f);
//   m.treeprint();
   cout << "\n" << m.Insert(g);
//   m.treeprint();
   cout << "\n" << m.Insert(h);
//   m.treeprint();
   cout << "\n" << m.Insert(i);
//   m.treeprint();
   cout << "\n" << m.Insert(j);
   m.treeprint();
BST<int> n, o;
Element<int> el;
cout << (m.Split(22, n, o, el))->key << endl;
n.treeprint();
o.treeprint();
   /*
   cout << "\n" << (m.Search(j) == m.IterSearch(j));
//   m.treeprint();
   cout << "\n" << (m.Search(i) == m.IterSearch(i));
//   m.treeprint();
   cout << "\n" << (m.Search(b) == m.IterSearch(b));
//   m.treeprint();
   cout << "\n" << (m.Search(a) == m.IterSearch(a));
//   m.treeprint();
   cout << "\n" << (m.Search(d) == m.IterSearch(d));
//   m.treeprint();
   cout << "\n" << (m.Search(c) == m.IterSearch(c));
//   m.treeprint();
   cout << "\n" << (m.Search(f) == m.IterSearch(f));
//   m.treeprint();
   cout << "\n" << (m.Search(e) == m.IterSearch(e));
//   m.treeprint();
   cout << "\n" << (m.Search(g) == m.IterSearch(g));
//   m.treeprint();
   cout << "\n" << (m.Search(h) == m.IterSearch(h));
//   m.treeprint();
*/
}



template <typename T> 
Node<T>*& BinarySearchTree<T>::find_left_most_child(Node<T>*& current) 
{ while (current->left) { current = current->left; } return current; } 

template <typename T> 
void BinarySearchTree<T>::replace_node(Node<T>*& replaced, Node<T>*& newone)
{ if (replaced == nullptr || newone == nullptr) { return; } 
if (replaced->left != nullptr && replaced->right != nullptr) 
{ newone->left = replaced->left; if (replaced->right != newone) newone->right = replaced->right; } 
Node<T>* temp = replaced; replaced = newone; 
delete temp; return; } 

template <typename T> 
bool BinarySearchTree<T>::remove(T value, function<int(T&, T&)>& comp) 
{ return internal_remove(root, value, comp); } 

template <typename T> 
bool BinarySearchTree<T>::internal_remove(Node<T>*& current, T value, function<int(T&, T&)>& comp)
{
	if (comp(current->data, value) == 0) {
		if (current->left == nullptr && current->right == nullptr) {
			// if leaf node, 
			delete current; current = nullptr; } 
		else if (current->left != nullptr && current->right != nullptr) 
		{ // if has two children 
			replace_node(current, find_left_most_child(current->right)); } 
		else if (current->left != nullptr && current->right == nullptr) { 
			// only has a left child; 
			replace_node(current, current->left); } 
		else if (current->left == nullptr && current->right != nullptr) { 
			// only has a right child 
			replace_node(current, current->right); } return true; }
	else if (comp(value, current->data) == 1) { // if the variable value is greater than current node's data 
		return internal_remove(current->right, value, comp); } 
	else { // if the variable value is less than current node's data
		return internal_remove(current->left, value, comp); } return false; }


