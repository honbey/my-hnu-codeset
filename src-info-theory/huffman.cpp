/******************************************************************************************
 * Files Name: huffman.cpp
 *
 * Honbey, honbey@honbey.com
 * Created On 2019-11-21
 * Copyright (c) 2019. All rights reserved.
 *
 * Date: 2019-12-15
 *
 * Description: For experiment 9.
 *   C++ program for Huffman Coding.
 *
 ******************************************************************************************/
#include <iostream>
#include <vector>
#include <queue>

#define N 7

using namespace std;

struct BTreeNode {
	char symb;
	int freq;
	BTreeNode *lc, *rc;

	BTreeNode(char symb, int freq) {
		lc = rc = NULL;
		this->symb = symb;
		this->freq = freq;
	}
	~BTreeNode() {
		delete lc;
		delete rc;
	}
};

void printCode(struct BTreeNode*, string);
void toHuffman(char*, int*, int);

int main(int argc, char const *argv[]) {
	float p[N] = { 0.2, 0.19, 0.18,
		0.17, 0.15, 0.10, 0.01 };
	int n[N];
	for (int i = 0; i < N; i++) n[i] = (int)(p[i] * 100);
	char symbol[N] = { '0', '1', '2', '3', '4', '5', '6' };
	toHuffman(symbol, n, N);

	return 0;
}

void printCode(struct BTreeNode *root, string str) {
	if (!root) return;
	if (root->symb != '#') cout << root->symb << ": " << str << endl;
	printCode(root->lc, str + "0");
	printCode(root->rc, str + "1");
}

void toHuffman(char *symb, int *freq, int size) {
	struct BTreeNode *lc, *rc, *top;
	struct compare { 
		bool operator()(BTreeNode *lc, BTreeNode *rc) { return (lc->freq > rc->freq); }
	};
	priority_queue<BTreeNode*, vector<BTreeNode*>, compare> HuffmanTree;

	for (int i = 0; i < size; ++i) HuffmanTree.push(new BTreeNode(symb[i], freq[i]));
	while (HuffmanTree.size() != 1) {
		lc = HuffmanTree.top();
		HuffmanTree.pop();
		rc = HuffmanTree.top();
		HuffmanTree.pop();
		top = new BTreeNode('#', lc->freq + rc->freq);
		top->lc = lc;
		top->rc = rc;
		HuffmanTree.push(top);
	}
	printCode(HuffmanTree.top(), "");
	delete top;
}