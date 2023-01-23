/*
Write your Rope implementation in this file. 
You do not have to keep any of the code that is already in this file.
Functions that are required that you have not implemented have an assert 0
meaning they will always crash. This should help you make sure you implement
everything.
*/

#include <cassert>
#include "Rope.h"

//Node destructor
Node::~Node() {
	if(!left) {
		delete left;
	}

	if(!right) {
		delete right;
	}
}

//helpler function for destructor
void destroy_tree(Node* p) {
    if (!p) return;
    destroy_tree(p->right);
    destroy_tree(p->left);
    delete p;
}

// Copy node with every rope
Node* deepClone(Node* n) {
	if(!n) {
		return nullptr;
	}

	Node* p = new Node;
	p->value = n->value;
	p->weight = n->weight;

	if(n->left) {
		Node* l = deepClone(n->left);
		l->parent = p;
		p->left = l;
	}

	if(n->right) {
		Node* r = deepClone(n->right);
		r->parent = p;
		p->right = r;
	}

	return p;
}

//Should advance to the next Node using in-order traversal
//It can point at any Node, not just leaves
rope_iterator& rope_iterator::operator++(){
	if(ptr_->right) {
		ptr_ = ptr_->right;
		while(ptr_->left) {
			ptr_ = ptr_->left;
		}
	} else {
		while(ptr_->parent && ptr_->parent->right == ptr_) {
			ptr_ = ptr_->parent;
		}
		ptr_ = ptr_->parent;
	}

	return *this;
}

//Point to the first Node for in-order traversal
rope_iterator Rope::begin() const{
	if(!root) {
		return rope_iterator();
	}

	Node* p = root;
	while(p->left) {
		p = p->left;
	}

	return rope_iterator(p);
}

//default constructor
Rope::Rope(): size_(0), root(nullptr){
}

int Rope::calcWeight(Node* r) {
	if(!r) {
		return 0;
	}

	if(is_leaf(r)) { // root
		r->weight = r->value.size();
		return r->weight;
	}

	int ret = 0;
	if(r->left) {
		r->weight = calcWeight(r->left);
		ret += r->weight;
	}

	if(r->right) {
		ret += calcWeight(r->right);
	}

	return ret;
}

//whether is the left
bool Rope::is_left(Node* n) {
	return n && n->parent && n == n->parent->left;
}

//whether is the right
bool Rope::is_right(Node* n) {
	return n && n->parent && n == n->parent->right;
}

//The nearest left leaf
Node* Rope::left_leaf(Node* n) {
	Node* pre = NULL;
	rope_iterator it = begin();
	while(it.ptr_) {
		if(n == it.ptr_) {
			return pre;
		}
		if(is_leaf(it.ptr_)) {
			pre = it.ptr_;
		}
		++it;
	}

	return NULL;
}

//The nearest right leaf
Node* Rope::right_side_leaf() {
	Node* ret = NULL;
	rope_iterator it = begin();
	while(it.ptr_) {
		if(is_leaf(it.ptr_))
		{
			ret = it.ptr_;
		}
		++it;
	}

	return ret;
}

//The nearest common parent
Node* Rope::nearest_parent(Node* n1, Node* n2) {
	std::vector<Node*> parents1;
	std::vector<Node*> parents2;
	while(n1) {
		parents1.push_back(n1);
		n1 = n1->parent;
	}

	while(n2) {
		parents2.push_back(n2);
		n2 = n2->parent;
	}

	assert(parents1.back() == parents2.back());
	Node* result = NULL;
	while(!parents1.empty() && !parents2.empty() && parents1.back() == parents2.back()) {
		result = parents1.back();
		parents1.pop_back();
		parents2.pop_back();
	}

	return result;
}

// find the begin point of index i+1
Node* Rope::ithNode(int i) {
	if(!root || i >= size_) {
		return NULL;
	}

	rope_iterator it = begin();
	while(i >= 0) {
		if(is_leaf(it.ptr_)) {
			if(i == 0) {
				return it.ptr_;
			}

			if(i >= int(it.ptr_->value.size())) {
				i -= int(it.ptr_->value.size());
			} else { // split
				Node* left = new Node;
				left->parent = it.ptr_;
				it.ptr_->left = left;
				left->value = it.ptr_->value.substr(0, i);
				Node* right = new Node;
				right->parent = it.ptr_;
				it.ptr_->right = right;
				right->value = it.ptr_->value.substr(i, int(it.ptr_->value.size())-i);
				it.ptr_->value = "";
				return right;
			}
		}
		++it;
	}

	return it.ptr_;
}

// recursive to calculate size
int Rope::sizeOf(Node* r) {
	if(!r) {
		return 0;
	}

	if(is_leaf(r)) {
		return r->value.size();
	}

	return sizeOf(r->left) + sizeOf(r->right);
}

// recalculate size
void Rope::calcSize() {
	size_ = 0;
	rope_iterator it = begin();
	while(it.ptr_) {
		if(is_leaf(it.ptr_)) { // leaf
			size_ += it.ptr_->value.size();
		}
		++it;
	}
}

//Should make the root = p and adjust any member variables
//This should not make a copy of the rope with root at p,
//it should just "steal" the Node*
Rope::Rope(Node* p): size_(0), root(p){
	calcSize();
}

Rope::~Rope(){
	destroy_tree(root);
	root = nullptr;
}

Rope::Rope(const Rope& r):size_(0), root(NULL){
	if(!r.root) {
		return;
	}

	root = deepClone(r.root);
	calcSize();
}

Rope& Rope::operator= (const Rope& r){
	if(!r.root) {
		return *this;
	}

	root = deepClone(r.root);
	calcSize();

	return *this;
}

//MUST BE ITERATIVE
//Get a single character at index i
bool Rope::index(int i, char& c) const{
	Node* p = root;
	if(!p) {
		return false;
	}

	while(!is_leaf(p)) { // find until the leaf
		if(i >= p->weight) { // to the right
			i -= p->weight;
			p = p->right;
			if(!p) {
				return false;
			}
		} else { // to the left
			p = p->left;
			if(!p) {
				return false;
			}
		}
	}

	// remain the leaf rope
	c = p->value[i];
	return true;
}

//Add the other rope (r)'s string to the end of my string
void Rope::concat(const Rope& r){
	if(!r.root) {
		return;
	}

	Node* new_root = new Node;
	new_root->left = root;
	new_root->right = deepClone(r.root);
	new_root->weight = size_;
	root = new_root;
	root->left->parent = root;
	root->right->parent = root;
	calcSize();
}

//Get a substring from index i to index j.
//Includes both the characters at index i and at index j.
//String can be one character if i and j match
//Returns true if a string was returned, and false otherwise
//Function should be written with efficient running time.
bool Rope::report(int i, int j, std::string& s) const{
	std::string all = ""; //calculate the whole string
	rope_iterator it = begin();
	while(it.ptr_) {
		if(!it.ptr_->left && !it.ptr_->right) { //leaf
			all += it.ptr_->value;
		}
		++it;
	}
	if(i >= int(all.size()) || j >= int(all.size())) {
		return false;
	}
	s = all.substr(i, j-i+1);
	return true;
}

//The first i characters should stay in the current rope, while a new
//Rope (rhs) should contain the remaining size_-i characters.
//A valid split should always result in two ropes of non-zero length.
//If the split would not be valid, this rope should not be changed,
//and the rhs rope should be an empty rope.
//The return value should be this rope (the "left") part of the string
//This function should move the nodes to the rhs instead of making new copies.
Rope& Rope::split(int i, Rope& rhs){
	if(i <= 0 || i>=size_) {
		return *this;
	}
	Node* p = ithNode(i);
	Node* ll = left_leaf(p); // current node's left node
	Node* rsl = right_side_leaf(); // the tree's rightest node

	assert(ll != NULL);
	if(rsl == p) {// the current node is the last one
		p->parent->right = NULL;
		p->parent = NULL;
		rhs.root = p;
	} else { // the current node isn't the last one
		Node* lp = nearest_parent(p, ll); // left parent
		Node* rp = nearest_parent(p, rsl); // right parent
		if(lp == rp) {
			lp->parent->right = NULL;
			rhs.root = lp;
		} else {
			Node* rhs_left = lp->right; //left
			rhs_left->parent->right = NULL;
			Node* rhs_right = rp->right; //right
			rhs_right->parent->right = NULL;
			// Creat a new node to connect the 2 broken rope
			Node* new_root = new Node;
			new_root->left = rhs_left;
			rhs_left->parent = new_root;
			new_root->right = rhs_right;
			rhs_right->parent = new_root;
			rhs.root = new_root;
		}
	}

	this->calcWeight(root);
	rhs.calcWeight(rhs.root);

	calcSize();
	rhs.calcSize();

	return *this;
}
