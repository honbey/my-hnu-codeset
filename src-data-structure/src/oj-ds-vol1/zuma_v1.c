/******************************************************************************************
 * Project Name: Zuma
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
 *     Programming Assignment #1: Zuma
 * 
 * 
 * History: 
 * <author> <time> <version > <desc>
 * Honbey 2019-08-20 1.0.0 Main frame
 * Honbey 2019-08-20 1.1.0 Char array
 * Honbey 2019-08-20 1.2.0 Some modifications
 ******************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 20005 // the MAX_LEN set twice as maximum n because the last case is a large string

int len = 0, m = 0;
int removeBead(char*, char*, int); // remove beads by char array

int main(int argc, char** argv) {
    char* olist = (char*)malloc(sizeof(char) * MAX_LEN);
    char* tlist = (char*)malloc(sizeof(char) * MAX_LEN);
    //memset(olist, 0, sizeof(char) * MAX_LEN);
    int n = 0;
    //scanf("%s", olist);// input the original bead sequence
    gets(olist);         // use gets() because the second case is a empty string 
    scanf("%d", &n);     // input how many operations to do
    len = strlen(olist); // length of olist

    char c[n];
    int k[n];
    for(int i = 0; i < n; i++) {
        scanf("%d %c", &k[i], &c[i]); // input each operation
    }

    for(int i = 0; i < n; i++) { // calculate results
        m = k[i];
        strcpy(tlist, olist + m);
        strcpy(olist + m + 1, tlist); // move one back from olist[m/k[i]]
        olist[m] = c[i]; // have formed new bead sequence 
        len++;  // length need addition one
        for(; removeBead(olist, tlist, m) && len; ); // judge which beads need be removed
            
        if(len) // output results of each operation
            printf("%s\n", olist);
        else printf("-\n"); // olist is empty string
    }

    free(olist);
    free(tlist);
    //char c1 = getchar(); char c2 = getchar();
    return 0;
}

int removeBead(char* olist, char* tlist, int k) {
    int start = k, end = k; // init
    char c = olist[k]; // bead/character of each operation

    for(; (olist[start] == c) && start; start--); // judge the beads/characters before olist[k]
    if(start || (olist[start] != c)) // border/error correction
        start++;
    for(; (olist[end] == c) && end < len; end++); // judge the beads/characters after olist[k]
    
    if(end - start > 2) { // reach the condition of removeBead
        strcpy(tlist, olist + end);
        strcpy(olist + start, tlist); // move start addition end from olist[start]
        len = len + start - end; // length have changed
        m = start; // judge the new start
        return 1; // if removed some beads/characters, return one
    }
    return 0; // no remove
}