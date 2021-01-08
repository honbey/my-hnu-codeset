/******************************************************************************************
 * Class Name: binTree.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019. All rights reserved.
 * 
 * Interface List:
 * 
 * 
 * Description: Definition/Declaration of macro constant and main class and function interface.
 * 
 ******************************************************************************************/

#pragma once

#include "binTree_node.h"

template<typename T> 
class BinTree {
private:
	Rank _size; 
	BinTreeNode<T> *_root;

protected:
	virtual int updateHeight(BinTreeNode<T> *x);
	void updateHeightAbove(BinTreeNode<T> *x);

public:
	BinTree() : _size(0), _root(NULL) { }
	~BinTree() {
		if(0 < _size) remove(_root); 
	}
	Rank size() const {
		return _size; 
	}
	bool empty() const {
		return !_root; 
	}

	bool operator<(BinTree<T> const &t) {
		return _root && t._root && (_root < t._root);
	}
	bool operator>(BinTree<T> const &t) {
		return _root && t._root && (_root > t._root);
	}
	bool operator== ( BinTree<T> const &t ) {
		return _root && t._root && ( _root == t._root );
	}

	BinTreeNode<T> * root() const {
		return _root;
	}
	BinTreeNode<T> * insertAsRoot(T const &e);
	BinTreeNode<T> * insertAsLC(BinTreeNode<T> *x, T const &e);
	BinTreeNode<T> * insertAsRC(BinTreeNode<T> *x, T const &e);
	BinTreeNode<T> * attachAsLC(BinTreeNode<T> *x, BinTree<T> * &bT);
	BinTreeNode<T> * attachAsRC(BinTreeNode<T> *x, BinTree<T> * &bT);
	Rank remove (BinTreeNode<T> *x);
	BinTree<T> * secede(BinTreeNode<T> *x);
	template<typename OP>
	void travPre(OP &visit) {
		if(_root) _root->travPre(visit);
	}
	template<typename OP>
	void travIn(OP &visit) {
		if(_root) _root->travIn(visit);
	}
	template<typename OP>
	void travPost(OP &visit) {
		if(_root) _root->travPost(visit);
	}
	template<typename OP>
	void travLevel(OP &visit) {
		if(_root) _root->travLevel(visit);
	}
};

#include "binTree_implementation.h"