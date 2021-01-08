/******************************************************************************************
 * Files Name: caesar.cpp
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-09-22
 * Copyright (c) 2019. All rights reserved.
 * 
 * Date: 2019-12-05
 * 
 * Description: For experiment 2.
 * Encryption, decryption and brute force hacking of Caesar code.
 * 
 *
 ******************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* caecarEncryhtion(const char*, int); // 利用字符数组实现凯撒加密
void hackerEncryhtion(const char*);

int main(int argc, char const* argv[]) {
char test[27] = "abcdefghijklmnopqrstuvwxyz";
    for(int i = 0; i < 26; i++) {    // 测试加密是否正常工作
    char* cipherText = caecarEncryhtion(test, i);  // 接收返回的堆内存地址
        printf("%s\n", cipherText);
        free(cipherText); // 释放堆内存
    }
    char t[30] = "guvf vf zl frperg zrffntr";
    hackerEncryhtion(t);

return 0;
}

char* caecarEncryhtion(const char* _plainText, int _key) {
    int len = strlen(_plainText);
    char* cipherText = (char*)malloc(sizeof(char) * (len + 1)); // 方便返回堆内存的地址
    cipherText[len] = '\0';
    for(int i = 0; _plainText[i] != '\0'; i++){
        if(_plainText[i] + _key > 'z') cipherText[i] = _plainText[i] - 26 + _key; // 判断字符是否大于'z'的ASCII码
        else cipherText[i] = _plainText[i] + _key;
    }
    return cipherText;  // 返回加密后的字符
}

void hackerEncryhtion(const char* _cipherText) {
    int len = strlen(_cipherText);
    char* plainText = (char*)malloc(sizeof(char) * (len + 1)); // 方便返回堆内存的地址
    plainText[len] = '\0';
    for(int j = 1; j < 26; j++) {
        for(int i = 0; _cipherText[i] != '\0'; i++){
            if(_cipherText[i] + j > 'z') plainText[i] = _cipherText[i] - 26 + j; // 判断字符是否大于'z'的ASCII码
            else if(_cipherText[i] == ' ') plainText[i] = ' ';
            else plainText[i] = _cipherText[i] + j;
        }
        printf("key: %d, plainText: %s\n", j, plainText);
    }
    free(plainText);
}