#ifndef MY_ASSEMBLER
#define MY_ASSEMBLER

#include <string>
#include <istream>
#include <iostream>
#include <fstream>

using namespace std;

class Assembler {
    public:
    string filename;
    void readfile();
	void readCommand();
	int activeLine;
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
				cout << temp << endl;
			}
			activeLine++;
		}
	}
	// close the file
	file.close();
}

void readCommand(string machineCode){
	string binaryLine = "00000000000000000000000000000000";
	int remainder;
	int power;
	string stringNumber;
	int number;
	if (machineCode.find("VAR") != string::npos){
		int position = machineCode.find("VAR") + 4;
		while (machineCode[position] != ' '){
			stringNumber += machineCode[position];
		}
		int number = stoi(stringNumber);
		while (remainder != 0){
			remainder = machineCode % power;
		}
	}
}

#endif