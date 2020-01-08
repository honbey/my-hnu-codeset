/******************************************************************************************
 * Files Name: transposition.cpp
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-10-11
 * Copyright (c) 2019. All rights reserved.
 * 
 * Date: 2019-10-11
 * 
 * Description: For experiment 3.
 * Simple Transposition encryption and decryption, can handle TXT file.
 * 
 *
 ******************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int const key = 10; // KEY is 10
string transpositionEncrypht(string ); // encrypt function 
string transpositionDecrypht(string ); // decrypt function

int main(int argc, char const *argv[]) {
	cout << "The default KEY is 10." << endl;
	if(argc != 2) { 
		cout << "Error:the program only need two argument.\nUsage: ./a [File Name]" << endl;
		return 0; // end program
	}

	ifstream inFile(argv[1]); // open the FILE
	string plainText = "", temp = ""; 
	while(getline(inFile, temp)) plainText += temp + '\n'; // copy text to plainText and add '\n' to the end of each row
	inFile.close(); // close FILE

	// some information about Encrypt/Decrypt process
	cout << "Text to Encrypt: " << endl << plainText.substr(0, 100) << endl;
	string cipherText = transpositionEncrypht(plainText);
	cout << "Encrypted Text: " << endl << cipherText.substr(0, 100) << endl;
	string deCipherText = transpositionDecrypht(cipherText);
	cout << "Decrypted Text: " << endl << deCipherText.substr(0, 100) << endl;

	ofstream outFile("EncryptedFile.txt"); // open or create a txt FILE 
	outFile << cipherText; // output the encrypted text to FILE
	outFile.close(); // close FILE

	ofstream outFile_("DecryptedFile.txt"); // open or create a txt FILE 
	outFile_ << deCipherText; // output the decrypted text to FILE
	outFile_.close(); // close FILE

	return 0;
}

string transpositionEncrypht(string _plainText) {
	int row;
	row = _plainText.length() / key;
	if(_plainText.length() % key) row += 1; // can't divisible

	char plainMat[row][key];
	for(int i = 0, k = 0; i < row; i++) 
		for(int j = 0; j < key; j++) plainMat[i][j] = _plainText[k++];

	string cipherText = "";
	for(int i = 0; i < key; i++) 
		for(int j = 0; j < row; j++) cipherText += plainMat[j][i];

	return cipherText;
}

string transpositionDecrypht(string _cipherText) {
	int row = _cipherText.length() / key;

	char cipherMat[row][key];
	for (int i = 0, k = 0; i < key; i++)
		for (int j = 0; j < row; j++) cipherMat[j][i] = _cipherText[k++];

	string plainText = "";
	for (int i = 0; i < row; i++) 
		for(int j = 0; j < key; j++) plainText += cipherMat[i][j];

	return plainText;
}