#ifndef MY_ASSEMBLER
#define MY_ASSEMBLER

#include <string>
#include <istream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

class Assembler {
    public:
    string filename;
    void readfile();
	void readCommand(string);
	int activeLine;
	char binaryArray[8191][32];
	void intToBinary(string, int);
	struct variable{
		string name;
		vector <int> usedInLine;
		int definedOnLine;
		string value;
	};
	vector <variable> variableArray;
	int numberOfVariables;
	int totalNoOfLines;
	int startLine;
	int endLine;
	void placeValues();
	void printBinary();
	void initialiseBinaryArray();
};

void Assembler::initialiseBinaryArray(){
	for(int i = 0 ; i < 32 ; i++){
		// This might have to change
		for (int c = 0 ; c < 32 ; c++){
			binaryArray[i][c] = '0';
		}
	}
}

void Assembler::readfile(){
	activeLine = 0;
	totalNoOfLines = 0;
    filename = "BabyTest1-Assembler.txt";
    ifstream file;
	string temp;
	int restOfLine;
	initialiseBinaryArray();
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
			// remove the commenting
            for(unsigned i = 0 ; i < temp.size() ; i++){
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
			// find the start and end points
			if(temp.find("START: ") != string::npos){
				startLine = activeLine;
			}
			else if(temp.find("END: ") != string::npos){
				endLine = activeLine;
			}
			if (temp.compare("") != 0){
				// temp.erase(0,10);
				totalNoOfLines++;
				readCommand(temp);
				activeLine++;
			}
		}
		placeValues();
		printBinary();
	}
	// close the file
	file.close();
}

void Assembler::intToBinary(string variableName, int number){
	string binaryLine = "00000000000000000000000000000000";
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

	// copy the binary value into the active line
	for(int i = 0 ; i < 32 ; i++){
		binaryArray[activeLine][i] = binaryLine[i];
	}

	// find the variable and add the value to it
	for (unsigned i = 0 ; i < variableArray.size() ; i++){
		if(variableArray.at(i).name == variableName){
			variableArray.at(i).value = binaryLine;
			break;
		}
	}
}

void Assembler::readCommand(string machineCode){

	string stringNumber;
	int position = 0;
	string operand;
	string variableName;
	bool found;

    //string operandList[] = {"LDN", "STO", "JRP", "STP", "SUB", "CMP", "JMP", "VAR"};
	//string operandCode[] = {"010", "100", "111", "001", "011", "000", "110", ""};
	string operandBoth[2][8] = 
	{
		{"LDN", "STO", "JRP", "STP", "SUB", "CMP", "JMP", "VAR"},
		{"010", "110", "100", "111", "001", "011", "000", ""}
	};
	int loopResult;

	for(int i=0; i<8; i++){
		if (machineCode.find(operandBoth[0][i]) != string::npos){
			loopResult = i;
			break;
		}
		else if(i>6){
			cout << "Unrecognised command found on line: " << activeLine << endl;
			return;
		}
	}
	if(operandBoth[0][loopResult] == operandBoth[0][7]){
		while(machineCode[position] != ':' && machineCode[position] != ' '){
			variableName += machineCode[position];
			position++;
		}

		machineCode.erase(0,machineCode.find(operandBoth[0][loopResult]));

		position = 4;

		cout << machineCode << endl;

		while (machineCode[position] != ' '){
			stringNumber += machineCode[position];
			position++;
		}

		// now we have the operand, convert it to an integer
		int number = stoi(stringNumber);
		intToBinary(variableName, number);

		for (unsigned i = 0 ; i < variableArray.size() ; i++){
			if(variableArray.at(i).name == variableName){
				variableArray.at(i).definedOnLine = activeLine;
				break;
			}
		}
		return;
	} else{
		operand=operandBoth[1][loopResult];
		machineCode.erase(0,machineCode.find(operandBoth[0][loopResult]));

	}

	cout << machineCode << endl;
	if(operand != "111"){
		position = 4;
		while (machineCode[position] != ' '){
			variableName += machineCode[position];
			position++;
		}
		for (unsigned i = 0 ; i < variableArray.size() ; i++){
			if(variableArray.at(i).name == variableName){
				variableArray.at(i).usedInLine.push_back(activeLine);
				found = true;
				break;
			}
		}
		if(found == false){
			variable temp;
			temp.name = variableName;
			temp.usedInLine.push_back(activeLine);
			variableArray.push_back(temp);
		}
	}
	// copy the binary value into the active line
	for(int i = 0 ; i < 32 ; i++){
		if (i == 13 || i == 14 || i == 15){
			binaryArray[activeLine][i] = operand[(i-13)];
		} 
		else{
			binaryArray[activeLine][i] = '0';
		}
	}
}

void Assembler::placeValues(){
	for(unsigned i = 0 ; i < variableArray.size() ; i++){
		for (unsigned c = 0 ; c < variableArray.at(i).usedInLine.size() ; c++){
			// change the "definedOnLine variable to binary"
			int number = variableArray.at(i).definedOnLine;
			for (int v = 12 ; v >= 0 ; v--){
				if (number >= pow(2,v)){
					binaryArray[variableArray.at(i).usedInLine.at(c)][v] = '1';
					number -= pow(2,v);
				}
				else{
					binaryArray[variableArray.at(i).usedInLine.at(c)][v] = '0';
				}
			}
		}
	}
}

void Assembler::printBinary(){
	
	ofstream myfile ("binary.txt");
	if (myfile.is_open())
	{
		for(int i = 0 ; i < totalNoOfLines ; i++){
			// This might have to change
			for (int c = 0 ; c < 32 ; c++){
				myfile << binaryArray[i][c];
			}
			myfile << "\n";
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file";
	}
	return;
}

#endif