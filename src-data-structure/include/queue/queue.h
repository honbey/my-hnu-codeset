/******************************************************************************************
 * File Name: queue.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019. All rights reserved.
 * 
 * Interface List:
 * 
 * 
 * Description: Definition/Declaration of Queue struct.
 * 
 ******************************************************************************************/

#pragma once

#include "list/list.h"

template<typename T> 
class Queue: public List<T> {
public:
	void enqueue (T const &e) {
		this->insertAsLast(e);
	}
	T dequeue() {
		return this->remove(this->first());
	}
	T & front() {
		return this->first()->data;
	}
	void push(T const &e) {
		this->insertAsLast(e);
	}
	void pop() {
		this->remove(this->first());
	}
};