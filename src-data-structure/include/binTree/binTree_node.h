/******************************************************************************************
 * File Name: binTree_node.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019. All rights reserved.
 * 
 * Interface List:
 * 
 * 
 * Description: Definition/Declaration of BinTreeNode struct.
 * 
 ******************************************************************************************/

#pragma once

#include "queue/queue.h"

typedef int Rank;

template<typename T>
struct BinTreeNode {
	T data;
	BinTreeNode<T> *parent, *lc, *rc;
	int height;
	BinTreeNode() :
		parent(NULL), lc(NULL), rc(NULL), height(0) { }
	BinTreeNode(T e, BinTreeNode<T> *p = NULL, 
		BinTreeNode<T> *lc = NULL, BinTreeNode<T> *rc = NULL, 
		int h = 0) : data(e), parent(p), lc(lc), rc(rc), height(h) { }
	Rank size();
	BinTreeNode<T> * succ();

	bool operator<(BinTreeNode const &bn) { 
		return data < bn.data;
	}
	bool operator>(BinTreeNode const &bn) { 
		return data > bn.data;
	}
	bool operator==(BinTreeNode const &bn) {
		return data == bn.data;
	}

	BinTreeNode<T> * insertAsLC(T const &e); // insert left child
	BinTreeNode<T> * insertAsRC(T const &e); // insert right child
	template<typename OP> void travPre(OP &visit); // preorder traversal
	template<typename OP> void travIn(OP &visit); // inorder traversal
	template<typename OP> void travPost(OP &visit); // postorder traversal
	template<typename OP> void travLevel(OP &visit); // Level traversal
};

template <typename T>
Rank BinTreeNode<T>::size() {
	Rank s = 1;
	if(lc) s += lc->size();
	if(rc) s += rc->size();
	return s;
}

template<typename T>
BinTreeNode<T> * BinTreeNode<T>::succ() {
	BinTreeNode<T> *s = this;
	if(rc) {
		s = rc;
		while((*s).lc) s = s->lc;
	} else {
		while (!IsRoot(*s) && (&(*s) == (*s).parent->rc)) s = s->parent;
		s = s->parent;
	}
	return s;
}

template<typename T>
BinTreeNode<T> * BinTreeNode<T>::insertAsLC(T const &e) { 
	return lc = new BinTreeNode(e, this);
}

template<typename T>
BinTreeNode<T> * BinTreeNode<T>::insertAsRC(T const &e) { 
	return rc = new BinTreeNode(e, this);
}

template<typename T> template<typename OP>
void BinTreeNode<T>::travPre(OP &visit) {
	travPre_R(this, visit);
}

template<typename T> template<typename OP>
void BinTreeNode<T>::travIn(OP &visit) {
	travIn_R(this, visit);
}

template<typename T> template<typename OP>
void BinTreeNode<T>::travPost(OP &visit) {
	travPost_R(this, visit);
}

template<typename T> template<typename OP>
void BinTreeNode<T>::travLevel(OP &visit) {
	Queue<BinTreeNode<T>*> Q;
	Q.push(this);
	while(!Q.empty()) {
		BinTreeNode<T> *x = Q.front();
		Q.pop();
		visit(x->data);
		if((*x).lc) Q.push(x->lc);
		if((*x).rc) Q.push(x->rc);
	}
}

#include "traverse.h"