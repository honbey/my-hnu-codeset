/******************************************************************************************
 * File Name: binTree_external.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019. All rights reserved.
 * 
 * Description: 
 * 
 ******************************************************************************************/

#pragma once

#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))
#define FromParentTo(x) (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc))

template<typename T>
static int removeAt(BinTreeNode<T> *x) {
	if(!x) return 0;
	int n = 1 + removeAt(x->lc) + removeAt(x->rc);
	// free(x->data);
	if(x) delete x;
	return n;
}

template<typename T>
BinTreeNode<T> * BinTree<T>::insertAsRoot(T const &e) {
	_size = 1;
	return _root = new BinTreeNode<T>(e);
}

template<typename T>
BinTreeNode<T> * BinTree<T>::insertAsLC(BinTreeNode<T> *x, T const &e) {
	_size++;
	x->insertAsLC(e);
	updateHeightAbove(x);
	return x->lc;
}

template<typename T>
BinTreeNode<T> * BinTree<T>::insertAsRC(BinTreeNode<T> *x, T const &e) {
	_size++;
	x->insertAsRC(e);
	updateHeightAbove(x);
	return x->rc;
}

template<typename T>
BinTreeNode<T> * BinTree<T>::attachAsLC(BinTreeNode<T> *x, BinTree<T>* &bT) {
	if(x->lc = bT->_root) x->lc->parent = x;
	_size += bT->_size;
	updateHeightAbove(x);
	bT->_root = NULL;
	bT->_size = 0;
	if(bT) delete bT;
	bT = NULL;
	return x;
}

template<typename T>
BinTreeNode<T> * BinTree<T>::attachAsRC(BinTreeNode<T> *x, BinTree<T>* &bT) {
	if(x->rc = bT->_root) x->rc->parent = x;
	_size += bT->_size;
	updateHeightAbove(x);
	bT->_root = NULL;
	bT->_size = 0;
	if(bT) delete bT;
	bT = NULL;
	return x;
}

template<typename T>
Rank BinTree<T>::remove(BinTreeNode<T> *x) {
	FromParentTo(*x) = NULL;
	updateHeightAbove(x->parent);
	Rank n = removeAt(x);
	_size -= n;
	return n;
}

template<typename T>
BinTree<T> * BinTree<T>::secede(BinTreeNode<T> *x) {
   FromParentTo(*x) = NULL;
   updateHeightAbove(x->parent);
   BinTree<T> *bT = new BinTree<T>;
   bT->_root = x;
   x->parent = NULL;
   bT->_size = x->size();
   _size -= bT->_size;
   return bT;
}

