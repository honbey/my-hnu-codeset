/******************************************************************************************
 * Files Name: lzw.cpp
 *
 * Honbey, honbey@honbey.com
 * Created On 2019-10-24
 * Copyright (c) 2019. All rights reserved.
 *
 * Date: 2019-12-15
 *
 * Description: For experiment 6.
 *   C++ program for LZW(Lempel-Ziv-Welch) Coding.
 * Reference Info: https://blog.csdn.net/abcjennifer/article/details/7995426
 *
 ******************************************************************************************/
#include <iostream>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

map<string, int> dictionary;

int main(int argc, char const *argv[]) {
    string inputStr = "ABABBABCABABBA", outputStr = "";
    int rank = 1;
    // first scan
    for (int i = 0; i < inputStr.length(); i++) {
    	string testChar = inputStr.substr(i, 1);
    	if (dictionary.find(testChar) == dictionary.end()) {
    		dictionary[testChar] = rank++;
    	}
    }

    // build dictionary and output
    string temp = "A"; // init
    for (int i = 0; i < inputStr.length(); ) {
    	// cout << temp << endl;
    	if (dictionary.find(temp) != dictionary.end()) {
    		temp += inputStr.substr(++i, 1);
    	}else {
    		dictionary[temp] = rank++;
		outputStr += to_string(dictionary.find(temp.substr(0, temp.length() - 1))->second);
    		temp = inputStr.substr(i, 1);
    	}
    }
    // last character
    outputStr += to_string(dictionary.find(temp)->second);

    // output
    cout << "Output: " << outputStr << endl << "Compression Ratio: " << fixed << setprecision(2) << (float)outputStr.length() / (float)inputStr.length() << endl << "Dictionary" << endl;
    for (map<string, int>::iterator it = dictionary.begin(); it != dictionary.end(); it++) cout << it->first << "\t" << setiosflags(ios::uppercase) << hex << it->second << endl;

    return 0;
}



