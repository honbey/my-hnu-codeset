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

template<typename T>
int BinTree<T>::updateHeight(BinTreeNode<T> *x) {
	return x->height = 1 + 
		std::max(((x->lc) ? (x->lc)->height : -1), ((x->rc) ? (x->rc)->height : -1));
}

template<typename T>
void BinTree<T>::updateHeightAbove(BinTreeNode<T> *x) {
	while(x) {
		updateHeight(x);
		x = x->parent;
	}
}