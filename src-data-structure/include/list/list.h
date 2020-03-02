/******************************************************************************************
 * Class Name: list.h
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

#include "list_node.h"

typedef int Rank;

template<typename T>
class List {
private:
	Rank _size;
	ListNode<T> *header;
	ListNode<T> *trailer;

protected:
	void init();
	Rank clear();
	void copyNodes(ListNode<T> *p, int n);
	void merge(ListNode<T> *&p, int n, List<T>& L, ListNode<T> *q, int m);
	void mergeSort(ListNode<T> *&p, int n);
	void selectionSort(ListNode<T> *p, int n);
	void insertionSort(ListNode<T> *p, int n);

public:
	List(){
		init();
	}
	List(List<T> const &L) {
		copyNodes(L.first(), L._size);
	}
	List(ListNode<T> *p, int n) {
		copyNodes(p, n);
	}
	List(List<T> const &L, Rank r, int n) {
		copyNodes(L[r], n);
	}
	~List() {
		clear();
		delete header;
		delete trailer;
	}

	// only read ADT
	Rank size() const {
		return _size;
	}
	bool empty() const {
		return _size <= 0;
	}

	T & operator[](int r) const;

	ListNode<T> * first() const {
		return header->succ;
	}
	ListNode<T> * last() const {
		return trailer->pred;
	}
	bool valid(ListNode<T> *p) {
		return p && (trailer != p) && (header != p);
	}
	int disordered() const;
	ListNode<T> * find(T const &e) const {
		return find(e, _size, trailer);
	}
	ListNode<T> * find(T const &e, int n, ListNode<T> *p) const;
	ListNode<T> * search(T const &e) const {
		return search(e, _size, trailer);
	}
	ListNode<T> * search(T const &e, int n, ListNode<T> *p) const;
	ListNode<T> * selectMax(ListNode<T> *p, int n);
	ListNode<T> * selectMax() {
		return selectMax(header->succ, _size);
	}

	// can read ADT
	ListNode<T> * insertAsFirst(T const &e);
	ListNode<T> * insertAsLast(T const &e);
	ListNode<T> * insertS(ListNode<T> *p, T const &e);
	ListNode<T> * insertP(ListNode<T> *p, T const &e);
	T remove(ListNode<T> *p);
	void merge(List<T> &L);
	void sort(ListNode<T> *p, int n);
	void sort(){
		sort(first(), _size);
	}
	Rank deduplicate();
	Rank uniquify();
	void reverse();

	void traverse(void(*) (T&));
    template<typename OP> void traverse(OP&);
};

#include "list_implementation.h"