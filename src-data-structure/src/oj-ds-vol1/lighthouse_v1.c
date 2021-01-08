/******************************************************************************************
 * Project Name: LightHouse
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-08-20
 * Copyright (c) 2019. All rights reserved.
 * 
 * Date: 2020-08-20
 * Version: 1.3.0
 * 
 * Change Log: Some modifications
 * 
 * Description: Learning DSA(Data Structure & Algorithms).
 *   Tsinghua Online Judge: 
 *     Course: Data Structure (Volume I) (Autonomous Mode) (MOOC)
 *     Programming Assignment #1: LightHouse
 * 
 * 
 * History: 
 * <author> <time> <version > <desc>
 * Honbey 2019-08-20 1.0.0 Main frame
 * Honbey 2019-08-20 1.1.0 Based Range
 * Honbey 2019-08-20 1.2.0 Some modifications
 ******************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//typedef long elemType;
typedef struct {
    int x;
    int y;
}elemType;

long long g_count = 0;

void swap(elemType*, elemType*);
int partition(elemType*, int, int);
void quickSort(elemType*, int, int);
void mergeSort(int*, int, int);
void merge(int*, int, int, int);

int main(int argc, char** argv) {
    int n;
    scanf("%d", &n); // input how many points
    elemType* P = (elemType*)malloc(sizeof(elemType) * n);
    for(int i = 0; i < n; i++) {
        scanf("%d %d", &(P[i].x), &(P[i].y)); // input each point
        //P[i].x = rand() % 100; P[i].y = rand() % 100;
    }

    int* A = (int*)malloc(sizeof(int) * n);
    quickSort(P, 0, n); // sort points
    for(int i = 0; i < n; i++) {
        A[i] = P[i].y; 
        //printf("%d %d\n", P[i].x, P[i].y);
    }
    mergeSort(A, 0, n); // calculate result
    printf("%lld", g_count); // output result

    //printf("\n*%ld*\n", count);
    free(P);
    free(A);
    //char c1 = getchar(); char c2 = getchar();
    return 0;
}

void swap(elemType* x, elemType* y) {
    elemType temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

int partition(elemType* P, int lo, int hi) {
    swap(&P[lo], &P[lo + rand() % (hi - lo + 1)]);
    int pivot =  P[lo].x;
    int mi = lo;
    for(int k = lo + 1; k <= hi; k++)
        if(P[k].x < pivot)
            swap(&P[++mi], &P[k]);
    swap(&P[lo], &P[mi]);
    return mi;
}

void quickSort(elemType* P, int lo, int hi) {
    if (hi - lo < 2) return;
    int mi = partition(P, lo, hi - 1);
    quickSort(P, lo, mi);
    quickSort(P, mi + 1, hi);
}

void mergeSort(int* A, int lo, int hi) {
    if(hi - lo < 2) return;
    int mi = (lo + hi) >> 1;
    mergeSort(A, lo, mi);
    mergeSort(A, mi, hi);
    merge(A, lo, mi, hi);
}

void merge(int* A, int lo, int mi, int hi) { // normal 2-way merge
    int* T = A;
    A = T + lo;

    int lb = mi - lo;
    int* B = (int*)malloc(sizeof(int) * lb);
    for(int i = 0; i < lb; i++)
        B[i] = A[i];

    int lc = hi - mi;
    int* C = T + mi;

    for(int i = 0, j = 0, k = 0; j < lb; ){
        if((k < lc)  && (C[k] <  B[j])) {
            A[i++] = C[k++];
        }
        if((lc <= k) || (B[j] <= C[k])) {
            A[i++] = B[j++];
            if(k < lc) g_count += (long long)lc - (long long)k; // calculate the sequence
        }
    }

    free(B);
}