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
void List<T>::init() {
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->succ = trailer;
	header->pred = NULL;
	trailer->pred = header;
	trailer->succ = NULL;
	_size = 0;
}

template<typename T>
Rank List<T>::clear() {
	int oldSize = _size;
	while(0 < _size) remove(header->succ);
	return oldSize;
}

template<typename T>
void List<T>::copyNodes(ListNode<T> *p, int n) {
	init();
	while(n--){
		insertAsLast(p->data);
		p = p->succ;
	}
}

template<typename T>
void List<T>::merge(ListNode<T> *&p, int n, List<T> &L, ListNode<T> *q, int m) {
	// this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
	// L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
	ListNode<T> *pp = p->pred;
	while(0 < m)
		if((0 < n) && (p->data <= q->data)) {
			if(q == (p = p->succ)) break;
			n--;
		} else {
			insertP(p, L.remove((q = q->succ)->pred));
			m--;
		}
	p = pp->succ;
}

template<typename T>
void List<T>::mergeSort(ListNode<T> *&p, int n) {
	if(n < 2) return;
	int m = n >> 1;
	ListNode<T> *q = p;
	for(int i = 0; i < m; i++) q = q->succ;
	mergeSort(p, m);
	mergeSort(q, n - m);
	merge(p, m, *this, q, n - m);
}

template<typename T>
void List<T>::selectionSort(ListNode<T> *p, int n) {
	ListNode<T> *head = p->pred;
	ListNode<T> *tail = p;
	for(int i = 0; i < n; i++) tail = tail->succ;
	for( ; 1 < n; n--) {
		ListNode<T> *M = selectMax(head->succ, n);
		insertP(tail, remove(M));
		tail = tail->pred;
	}
}

template<typename T>
void List<T>::insertionSort(ListNode<T> *p, int n) {
	for(int i = 0; i < n; i++) {
		insertS(search(p->data, i, p), p->data);
		p = p->succ;
		remove(p->pred);
	}
}