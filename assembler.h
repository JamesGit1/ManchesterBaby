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
};

void Assembler::readfile(){
    filename = "BabyTest1-Assembler.txt";
    ifstream file;
	string temp;
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
            cout << temp << endl;
		}
	}
	// close the file
	file.close();
}

#endif