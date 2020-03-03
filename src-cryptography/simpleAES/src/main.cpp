/******************************************************************************************
 * Project Name: simpleAES
 * 
 * Honbey, honbey@honbey.com
 * Created On 2019-11-11
 * Copyright (c) 2019. All rights reserved.
 * 
 * Date: 2019-12-13
 * Version: 0.4.0
 * 
 * Change Log: Overload the operation in Galois Field.
 * 
 * Description: For the 5th cryptography experiment.
 * Stability impact on source files: 
 *   the file will have a blank line added.
 *  
 * History: 
 * <author> <time> <version > <desc>
 * Honbey 2019-11-11 0.1.0 Create project and normalize code
 * Honbey 2019-11-13 0.2.0 Debug and optimization
 * Honbey 2019-11-15 0.3.0 Print information when encrypting/decrypting
 ******************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

#include "simple_aes.h"

int main(int argc, char const *argv[]) {
	simpleAES def;
	ifstream inFile(argv[1]); // open the FILE
	string plainText = "", temp = "";
	while (getline(inFile, temp)) plainText += temp + '\n'; // copy text to plainText and add '\n' to the end of each row
	inFile.close(); // close FILE

	// some information about Encrypt/Decrypt process
	string cipherText = def.encryption(plainText);
	cout << " Encrypted Text: " << endl << cipherText.substr(0, 10) << endl;
	string deCipherText = def.decryption(cipherText);
	cout << " Decrypted Text: " << endl << deCipherText.substr(0, 10) << endl;

	ofstream outFile("EncryptedFile.txt"); // open or create a txt FILE 
	outFile << cipherText; // output the encrypted text to FILE
	outFile.close(); // close FILE

	outFile.open("DecryptedFile.txt", ios::out); // open or create a txt FILE 
	outFile << deCipherText; // output the decrypted text to FILE
	outFile.close(); // close FILE
    GFPoly a(0x7F), b(0x17);
    cout << "\nThe Greatest common factor of x^6+x^5+x^4+x^3+x^2+x+1 and x^4+x^2+x+1 is: " 
         << setiosflags(ios::uppercase) << hex << (Euclid(b, a)).data() << "." << endl;
	return 0;
}