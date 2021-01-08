/******************************************************************************************
 * File Name: traverse.h
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-06
 * Copyright (c) 2019. All rights reserved.
 * 
 * Interface List:
 * 
 * 
 * Description: Definition/Declaration of Traverse Functions
 * 
 ******************************************************************************************/

#include <stack>

// Preorder traversal

template<typename T, typename OP>
void travPre_R(BinTreeNode<T> *x, OP &visit) {
	if(!x) return;
	visit(x->data);
	travPre_R(x->lc, visit);
	travPre_R(x->rc, visit);
}

template<typename T, typename OP>
void travPre_I1 (BinTreeNode<T> *x, OP& visit) {
	std::stack<BinTreeNode<T>*> S;
	if(x) S.push(x);
	while(!S.empty()) {
		x = S.pop(); visit(x->data);
		if((*x).rc) S.push ( x->rc );
		if((*x).lc) S.push (x->lc);
	}
}

template<typename T, typename OP>
static void visitTD(BinTreeNode<T> *x, OP& visit, std::stack<BinTreeNode<T>*> &S) {
	while(x) {
		visit(x->data);
		if(x->rc != NULL) S.push(x->rc);
		x = x->lc;
	}
}

template<typename T, typename OP>
void travPre_I2(BinTreeNode<T> *x, OP &visit) {
	std::stack<BinTreeNode<T>*> S;
	while(true) {
		visitTD(x, visit, S);
		if (S.empty()) break;
		x = S.pop();
	}
}

// Inorder traversal

template<typename T, typename OP>
void travIn_R(BinTreeNode<T> *x, OP &visit) {
	if(!x) return;
	travIn_R(x->lc, visit);
	visit(x->data);
	travIn_R(x->rc, visit);
}

template<typename T>
static void goLongest(BinTreeNode<T> *x, std::stack<BinTreeNode<T>*> &S) {
	while(x) {
		S.push(x);
		x = x->lc; 
	}
}

template<typename T, typename OP>
void travIn_I1(BinTreeNode<T> *x, OP& visit) {
	std::stack<BinTreeNode<T>*> S;
	while(true) {
		goLongest(x, S);
		if(S.empty()) break;
		x = S.pop();
		visit(x->data);
		x = x->rc;
	}
}

template<typename T, typename OP>
void travIn_I2(BinTreeNode<T> *x, OP &visit) {
	std::stack<BinTreeNode<T>*> S; //辅助栈
	while(true) {
		if(x) {
			S.push(x);
			x = x->lc;
		} else if(!S.empty()) {
			x = S.pop();
			visit(x->data);
			x = x->rc;
		} else break;
	}
}

template<typename T, typename OP>
void travIn_I3(BinTreeNode<T> *x, OP &visit) {
	bool backFlag = false;
	while(true) {
		if(!backFlag && (*x).lc) x = x->lc;
		else {
			visit(x->data);
			if((*x).rc) {
				x = x->rc;
				backFlag = false;
			} else {
				if(!(x = x->succ())) break;
				backFlag = true;
			}
		}
	}
}

template<typename T, typename OP>
void travIn_I4(BinTreeNode<T> *x, OP &visit) {
	while(true) {
		if((*x).lc) x = x->lc;
		else {
			visit(x->data);
			while (!(*x).rc )
				if (!(x = x->succ())) return;
				else visit (x->data);
			x = x->rc;
		}
	}
}

// Postorder traversal

template<typename T, typename OP>
void travPost_R(BinTreeNode<T> *x, OP &visit) {
	if(!x) return;
	travPost_R(x->lc, visit);
	travPost_R(x->rc, visit);
	visit(x->data);
}

template<typename T>
static void getLeftmostLeaf(std::stack<BinTreeNode<T>*> &S) {
	while(BinTreeNode<T> *x = S.top())
	if((*x).lc) {
		if((*x).rc) S.push(x->rc);
		S.push(x->lc);
	} else S.push(x->rc);
	S.pop();
}

template<typename T, typename OP>
void travPost_I(BinTreeNode<T> *x, OP &visit) { 
	std::stack<BinTreeNode<T>*> S;
	if(x) S.push (x);
	while(!S.empty()) {
		if(S.top() != x->parent) getLeftmostLeaf(S);
		x = S.pop();
		visit(x->data);
	}
}