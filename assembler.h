#ifndef MY_ASSEMBLER
#define MY_ASSEMBLER

#include <string>
#include <istream>
#include <iostream>
#include <fstream>
#include<cmath>

using namespace std;

class Assembler {
    public:
    string filename;
    void readfile();
	void readCommand(string);
	int activeLine;
	char binaryArray[32][32];
	void intToBinary(int);
};

void Assembler::readfile(){
	activeLine = 0;
    filename = "BabyTest1-Assembler.txt";
    ifstream file;
	string temp;
	int restOfLine;
	try{
		// open the file
		file.open(filename);
		cout << "Opening file: " << filename << endl;
	}
	catch(...){
		cerr << "failure to open file" << endl;
	}
	// if the file cannot be opened, send an error message
	if (!file){
		cerr << "This file could not be opened" << endl;
		exit(100);
	}
	else{
		// get the lines of text from the file
		while(getline(file, temp)){
			// do something with the file
            for(int i = 0 ; i < temp.length() ; i++){
                if (temp[i] == ';'){
					if (i==0){
						temp.clear();
					}
					else{
						restOfLine = temp.length() - i;
						temp.erase(i, restOfLine);
						break;
					}
                }
            }
			if (temp.compare("") != 0){
				// temp.erase(0,10);
				readCommand(temp);
			}
			activeLine++;
		}
	}
	// close the file
	file.close();
}

void Assembler::intToBinary(int number){
	string binaryLine = "00000000000000000000000000000000";
	int remainder;
	// to find the operand, we first find the command, then add 4
	// to find the first number in the operand
	for (int i = 31 ; i >= 0 ; i--){
		if (number >= pow(2,i)){
			binaryLine[i] = '1';
			number -= pow(2,i);
		}
		else{
			binaryLine[i] = '0';
		}
	}
	cout << binaryLine << endl;
}

void Assembler::readCommand(string machineCode){
	string stringNumber;
	int position = 4;
	string operand;
	if (machineCode.find("VAR") != string::npos){
		machineCode.erase(0,machineCode.find("VAR"));
		cout << machineCode << endl;
		while (machineCode[position] != ' '){
			stringNumber += machineCode[position];
			position++;
		}
		// now we have the operand, convert it to an integer
		int number = stoi(stringNumber);
		intToBinary(number);
	}
	else if(machineCode.find("LDN") != string::npos){
		operand = "101";
		machineCode.erase(0,machineCode.find("LDN"));
		cout << machineCode << endl;
		while (machineCode[position] != ' '){
			stringNumber += machineCode[position];
			position++;
		}

	}
	else{
		return;
	}
	
}

#endif