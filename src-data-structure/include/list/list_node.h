/******************************************************************************************
 * File Name: list_node.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019. All rights reserved.
 * 
 * Interface List:
 * 
 * 
 * Description: Definition/Declaration of ListNode struct.
 * 
 ******************************************************************************************/

#pragma once

template<typename T>
struct ListNode {
	T data;
	ListNode<T> *pred;
	ListNode<T> *succ;
	ListNode() {}
	ListNode(T e, ListNode<T> *p = NULL, ListNode<T> *s = NULL)
		: data(e), pred(p), succ(s) {}
	ListNode<T> * insertAsPred(T const &e);
	ListNode<T> * insertAsSucc(T const &e);
};

template<typename T>
ListNode<T> * ListNode<T>::insertAsPred(T const &e) {
	ListNode<T> *x = new ListNode(e, pred, this);
	pred->succ = x;
	pred = x;
	return x;
}

template<typename T>
ListNode<T> * ListNode<T>::insertAsSucc(T const &e) {
	ListNode<T> *x = new ListNode(e, this, succ);
	succ->pred = x;
	succ = x;
	return x;
}