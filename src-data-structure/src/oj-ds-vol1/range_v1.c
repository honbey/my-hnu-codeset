/******************************************************************************************
 * Project Name: Range
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-08-19
 * Copyright (c) 2019. All rights reserved.
 * 
 * Date: 2020-08-20
 * Version: 1.4.0
 * 
 * Change Log: Merge/Quick Sort and Binary-search
 * 
 * Description: Learning DSA(Data Structure & Algorithms).
 *   Tsinghua Online Judge: 
 *     Course: Data Structure (Volume I) (Autonomous Mode) (MOOC)
 *     Programming Assignment #1: Range
 * 
 * 
 * History: 
 * <author> <time> <version > <desc>
 * Honbey 2019-08-19 1.0.0 Main frame
 * Honbey 2019-08-19 1.1.0 Some modifications
 * Honbey 2019-08-19 1.2.0 Some modifications
 * Honbey 2019-08-19 1.3.0 Merge Sort and Binary-search
 ******************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef long elemType; // element type is long

void mergeSort(elemType*, int, int); // version 1.0-1.3
void merge(elemType*, int, int, int);
elemType binSearch(elemType*, elemType, int, int);
void swap(elemType*, elemType*);
int partition(elemType*, int, int);
void quickSort(elemType*, int, int); // version 1.4

int main(int argc, char** argv) {
    int n, m;
    scanf("%d %d", &n, &m); // input n and m
    elemType* A = (elemType*)malloc(sizeof(elemType) * n);
    int* count = (int*)malloc(sizeof(int) * m);

    elemType temp, a, b;
    for(int i = 0; i < n; i++) { // input number of point 
        scanf("%ld", &temp);
        A[i] = temp;
    }

    //mergeSort(A, 0, n);
    quickSort(A, 0, n);

    for(int i = 0; i < m; i++) { // input the range
        scanf("%ld %ld", &a, &b);
        int la = binSearch(A, a, 0, n);
        int lb = binSearch(A, b, 0, n);
        count[i] = lb - la;
        if(A[la] == a) // border
            count[i]++;
    }
    
    for(int i = 0; i < m; i++) // output the result
        printf("%d\n", count[i]);

    free(A);
    free(count);
    //char c = getchar();
    return 0;
}

void mergeSort(elemType* A, int lo, int hi) {
    if(hi - lo < 2)
        return;
    int mi = (lo + hi) >> 1;
    mergeSort(A, lo, mi);
    mergeSort(A, mi, hi);
    merge(A, lo, mi, hi);
}

void merge(elemType* A, int lo, int mi, int hi) {
    elemType* T = A;
    A = T + lo;

    int lb = mi - lo;
    elemType* B = (elemType*)malloc(sizeof(elemType) * lb);
    for(int i = 0; i < lb; i++)
        B[i] = A[i];

    int lc = hi - mi;
    elemType* C = T + mi;

    for(int i = 0, j = 0, k = 0; j < lb; ) {
        if((k < lc)  && (C[k] <  B[j])) A[i++] = C[k++];
        if((lc <= k) || (B[j] <= C[k])) A[i++] = B[j++];
    }

    free(B);
}

elemType binSearch(elemType* A, elemType e, int lo, int hi) {
    for(; lo < hi; ){
        int mi = (lo + hi) >> 1;
        (e < A[mi]) ? (hi = mi) : (lo = mi + 1);
    }
    return --lo;
}

void swap(elemType* x, elemType* y) {
    elemType temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

int partition(elemType* A, int lo, int hi) {
    swap(&A[lo], &A[lo + rand() % (hi - lo + 1)]);
    elemType pivot =  A[lo];
    int mi = lo;
    for(int k = lo + 1; k <= hi; k++)
        if(A[k] < pivot)
            swap(&A[++mi], &A[k]);
    swap(&A[lo], &A[mi]);
    return mi;
}

void quickSort(elemType* A, int lo, int hi) {
    if (hi - lo < 2)
        return;
    int mi = partition(A, lo, hi - 1);
    quickSort(A, lo, mi);
    quickSort(A, mi + 1, hi);
}