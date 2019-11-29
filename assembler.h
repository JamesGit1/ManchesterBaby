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
	char binaryArray[32][32];
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
};

void Assembler::readfile(){
	activeLine = 0;
	totalNoOfLines = 0;
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
			// remove the commenting
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
		cout << "There are " << variableArray.size() << " variables" << endl;
		for(int i = 0 ; i < variableArray.size() ; i++){
			cout << variableArray.at(i).name << " was found on lines ";
			for (int c = 0 ; c < variableArray.at(i).usedInLine.size() ; c++){
				cout << variableArray.at(i).usedInLine.at(c) << " ";
			}
			cout << endl;
		}
	}
	// close the file
	file.close();
}

void Assembler::intToBinary(string variableName, int number){
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
	// cout << binaryLine << endl;

	// copy the binary value into the active line
	for(int i = 0 ; i < 32 ; i++){
		binaryArray[activeLine][i] = binaryLine[i];
	}

	// find the variable and add the value to it
	for (int i = 0 ; i < variableArray.size() ; i++){
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
	string binaryLine = "00000000000000000000000000000000";
	bool found;

	if (machineCode.find("VAR") != string::npos){

		while(machineCode[position] != ':' && machineCode[position] == ' '){
			variableName += machineCode[position];
			position++;
		}

		// variableArray.at(numberOfVariables).definedOnLine = activeLine;
		machineCode.erase(0,machineCode.find("VAR"));
		cout << machineCode << endl;
		position = 4;

		while (machineCode[position] != ' '){
			stringNumber += machineCode[position];
			position++;
		}

		// now we have the operand, convert it to an integer
		int number = stoi(stringNumber);
		intToBinary(variableName, number);

		for (int i = 0 ; i < variableArray.size() ; i++){
			if(variableArray.at(i).name == variableName){
				variableArray.at(i).definedOnLine = activeLine;
				cout << variableName << " is defined on line " << activeLine << endl;
				break;
			}
		}
	}
	else if(machineCode.find("LDN") != string::npos){
		operand = "010";
		machineCode.erase(0,machineCode.find("LDN"));
		cout << machineCode << endl;
		position = 4;
		while (machineCode[position] != ' '){
			variableName += machineCode[position];
			position++;
		}
		for (int i = 0 ; i < variableArray.size() ; i++){
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
		
		// TODO correct value of binary line.
		binaryLine = "00000000001000000000000000000000";

	}
	else if(machineCode.find("STO") != string::npos){
		operand = "110";
		machineCode.erase(0,machineCode.find("STO"));
		cout << machineCode << endl;
		position = 4;
		while (machineCode[position] != ' '){
			variableName += machineCode[position];
			position++;
		}
		for (int i = 0 ; i < variableArray.size() ; i++){
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
		
		// TODO correct value of binary line.
		binaryLine = "00000000001000000000000000000000";
	}
	else if(machineCode.find("JRP") != string::npos){
		operand = "100";
		machineCode.erase(0,machineCode.find("JRP"));
		cout << machineCode << endl;
		position = 4;
		while (machineCode[position] != ' '){
			variableName += machineCode[position];
			position++;
		}
		for (int i = 0 ; i < variableArray.size() ; i++){
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
		
		// TODO correct value of binary line.
		binaryLine = "00000000001000000000000000000000";
	}
	else if(machineCode.find("STP") != string::npos){
		operand = "111";
		machineCode.erase(0,machineCode.find("STP"));
		cout << machineCode << endl;
		
		// TODO correct value of binary line.
		binaryLine = "00000000001000000000000000000000";
	}
	else if(machineCode.find("SUB") != string::npos){
		operand = "001";
		machineCode.erase(0,machineCode.find("SUB"));
		cout << machineCode << endl;
		position = 4;
		while (machineCode[position] != ' '){
			variableName += machineCode[position];
			position++;
		}
		for (int i = 0 ; i < variableArray.size() ; i++){
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
		
		// TODO correct value of binary line.
		binaryLine = "00000000001000000000000000000000";
	}
	else if(machineCode.find("CMP") != string::npos){
		operand = "011";
		machineCode.erase(0,machineCode.find("CMP"));
		cout << machineCode << endl;
		position = 4;
		while (machineCode[position] != ' '){
			variableName += machineCode[position];
			position++;
		}
		for (int i = 0 ; i < variableArray.size() ; i++){
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
		
		// TODO correct value of binary line.
		binaryLine = "00000000001000000000000000000000";
	}
	else if(machineCode.find("JMP") != string::npos){
		operand = "000";
		machineCode.erase(0,machineCode.find("JMP"));
		cout << machineCode << endl;
		position = 4;
		while (machineCode[position] != ' '){
			variableName += machineCode[position];
			position++;
		}
		for (int i = 0 ; i < variableArray.size() ; i++){
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
		
		// TODO correct value of binary line.
		binaryLine = "00000000001000000000000000000000";
	}
	else{
		cout << "Unrecognised command found on line: " << activeLine << endl;
		return;
	}
	// copy the binary value into the active line
	for(int i = 0 ; i < 32 ; i++){
		binaryArray[activeLine][i] = binaryLine[i];
	}
}

#endif