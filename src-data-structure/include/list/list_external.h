/******************************************************************************************
 * File Name: list_external.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019. All rights reserved.
 * 
 * Description: 
 * 
 ******************************************************************************************/

#pragma once

template<typename T>
static void swap(T &x, T &y) {
	T temp;
	temp = x;
	x = y;
	y = temp;
}

template<typename T>
int List<T>::disordered() const {
	int n = 0;
	ListNode<T> *p = first();
	for(int i = 0; i < _size - 1; p = p->succ, i++)
		if(p->succ->data < p->data) n++;
	return n;
}

template<typename T>
ListNode<T> * List<T>::find(T const &e, int n, ListNode<T> *p) const {
	for( ; 0 < n; n--)
		if(e == (p = p->pred)->data) return p;
	return NULL;
}

template<typename T>
ListNode<T> * List<T>::search(T const &e, int n, ListNode<T> *p) const {
	for(p = p->pred; (0 < n) && (e < p->data); n--) p = p->pred;
	return p;
}

template<typename T>
ListNode<T> * List<T>::selectMax(ListNode<T> *p, int n){
	ListNode<T> *M = p;
	for(ListNode<T> *temp = p; 1 < n; n--) {
		temp = temp->succ;
		if(temp->data > M->data) M = temp;
	}
	return M;
}

template<typename T>
ListNode<T> * List<T>::insertAsFirst(T const &e) {
	_size++;
	return header->insertAsSucc(e);
}

template<typename T>
ListNode<T> * List<T>::insertAsLast(T const &e) {
	_size++;
	return trailer->insertAsPred(e);
}

template<typename T>
ListNode<T> * List<T>::insertS(ListNode<T> *p, T const &e) {
	_size++;
	return p->insertAsSucc(e);
}

template<typename T>
ListNode<T> * List<T>::insertP(ListNode<T> *p, T const &e) {
	_size++;
	return p->insertAsPred(e);
}

template<typename T>
T List<T>::remove(ListNode<T> *p){
	T e = p->data;
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	delete p;
	_size--;
	return e;
}

template<typename T>
void List<T>::sort(ListNode<T> *p, int n) {
	switch(rand() % 3){
		case 1: {
			printf("InsertSort\n");
			insertionSort(p, n); 
			break;
		}
		case 2: {
			printf("SelectionSort\n");
			selectionSort(p, n); 
			break;
		}
		default: {
			printf("MergeSort\n");
			mergeSort(p, n); 
			break;
		}
	}
}

template<typename T>
Rank List<T>::deduplicate() {
	if(_size < 2) return 0;
	Rank oldSize = _size;
	ListNode<T> *p = header;
	Rank r = 0;
	while(trailer != (p = p->succ)){
		ListNode<T> *q = find(p->data, r, p);
		q ? remove(q) : r++;
	}
	return oldSize - _size;
}

template<typename T>
Rank List<T>::uniquify() {
	if(_size < 2) return 0;
	Rank oldSize= _size;
	ListNode<T> *p = first();
	ListNode<T> *q;
	while(trailer != (q = p->succ))
		if(p->data != q->data) p = q;
		else remove(q);
	return oldSize - _size;
}

template<typename T>
void List<T>::reverse() {
	if(_size < 2) return;
	ListNode<T> *p;
	ListNode<T> *q;
	for(p = header, q = p->succ; p != trailer; p = q, q = p->succ)
		p->pred = q;
	trailer->pred = NULL;
	for(p = header, q = p->pred; p != trailer; p = q, q = p->pred)
		q->succ = p;
	header->succ = NULL;
	swap(header, trailer);
	/*
	ListNode<T> *p = header;
	ListNode<T> *q = trailer;
	for ( int i = 1; i < _size; i += 2 ) 
		swap ( ( p = p->succ )->data, ( q = q->pred )->data );
	*/
}

template <typename T>
void List<T>::traverse(void(*visit)(T&)) {
	for(ListNode<T> *p = header->succ; p != trailer; p = p->succ) visit(p->data);
}

template <typename T> template <typename OP>
void List<T>::traverse(OP& visit) {
	for(ListNode<T> *p = header->succ; p != trailer; p = p->succ) visit(p->data);
}