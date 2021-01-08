/******************************************************************************************
 * File Name: stack.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019. All rights reserved.
 * 
 * Interface List:
 * 
 * 
 * Description: Definition/Declaration of Stack struct.
 * 
 ******************************************************************************************/

#pragma once

#include "list/list.h"

template<typename T>
class Stack: public List<T> {
public:
	void push(T const &e) {
		this->insertAsLast(e);
	}
	T pop() {
		return this->remove(this->last());
	}
	T & top() {
		return this->last()->data;
	}
};