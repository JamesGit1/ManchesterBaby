/*
	Manchester Baby
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
using namespace std;

class Baby {

  int store[8191][32];			//  8191 is max with space The machine code input from file
  int accumulator[32];			// Result of arithmetic operations
  int controlInstruction[32];	// ie Program Counter, typically instruciton address
  int presentInstruction[32];
  int counter;
  bool stop;
  int opInt;
  int arrayLen;

public:
	Baby(); //Constructor

	void getFileIn();
	void printArray();

	void increaseCounter();
	void fetch();
	void decodeOperate();

	void JMP();
	void JRP();
	void LDN();
	void STO();
	void SUB();
	void CMP();
	void STP();

	void printValues();

	void opcodeInt(int *);
	int binToInt(int *);
	bool getStop(){return stop;}
};

Baby::Baby(){
	counter=0;
	stop=false;
	arrayLen = 10;

	for (int i = 0; i < 32; ++i){
		controlInstruction[i]=0;
	}
}

void Baby::printArray(){
	for (int x = 0; x < arrayLen; ++x){
		cout<<x<<"| ";
		for (int y = 0; y < 32; ++y){
			printf("%d", store[x][y]);
		}
		cout<<endl;
	}
	cout<<endl;
}

void Baby::JMP(){
	cout<<"| JMP INSTRUCTION SETTING CI TO CONTENT OF STORE | STORE LOCATION "<<opInt<<endl;
	for (int y = 0; y < 32; ++y){
		controlInstruction[y] = store[counter][y];
	}
}

void Baby::JRP(){
	cout<<"| JRP INSTRUCTION | ADDING CONTENT OF STORE TO CI | STORE LOCATION "<<opInt<<endl;

	bool carry=false;
	for (int y = 0; y < 32; ++y){
		if(controlInstruction[y]==1 && store[opInt][y]==1 && carry==true){
			carry=true;
			controlInstruction[y]=1;
		}
		else if(controlInstruction[y]==1 || store[opInt][y]==1 && carry==true){
			carry=true;
			controlInstruction[y]=0;
		}
		else if(controlInstruction[y]==1 && store[opInt][y]==1){
			carry=true;
			controlInstruction[y]=0;
		}
		else if(controlInstruction[y]==1 || store[opInt][y]==1){
			carry=false;
			controlInstruction[y]=1;
		}
		else if(carry==true){
			carry=false;
			controlInstruction[y]=1;
		}
		else{
			carry=false;
			controlInstruction[y]=0;
		}
	}
}

void Baby::LDN(){
	cout<<"| LDN INSTRUCTION | LOADING NEGITIVE STORE VALUE TO ACCUMULATOR | STORE LOCATION "<<opInt<<endl;

	for (int i = 0; i < 32; ++i){
		accumulator[i] = store[opInt][i];
		if(accumulator[i]==0){
			accumulator[i]=1;
		}
		else{
			accumulator[i]=0;
		}
	}

	int pos=0;
	bool first = true;
	while(accumulator[pos]==1){
		first = false;
		accumulator[pos]=0;
		pos++;
	}
	if (first==true){
		accumulator[0]=1;
	}
	else{
		accumulator[pos]=1;
	}
}

void Baby::STO(){
	cout<<"| STO INSTRUCTION | COPYING ACCUMULATOR TO STORE | STORE LOCATION "<<opInt<<endl;
	for (int y = 0; y < 32; ++y){
		store[opInt][y] = accumulator[y] ;
	}
}

void Baby::SUB(){
	cout<<"| SUB INSTRUCTION | SUBTRACTING CONTENT OF STORE FROM ACCUMULATOR | STORE LOCATION "<<opInt<<endl;
	int negitiveStore[32];

	for (int i = 0; i < 32; ++i){
		negitiveStore[i] = store[opInt][i];
		if(negitiveStore[i]==0){
			negitiveStore[i]=1;
		}
		else{
			negitiveStore[i]=0;
		}
	}
	bool carry = true;
	int pos=0;
	while(carry){
		if(negitiveStore[pos]==1){
			carry=true;
			negitiveStore[pos]=0;
		}
		else{
			negitiveStore[pos]=1;
			carry=false;
		}	
		pos++;
	}

	carry=false;
	for (int y = 0; y < 32; ++y){
		if(accumulator[y]==1 && negitiveStore[y]==1 && carry==true){
			carry=true;
			accumulator[y]=1;
		}
		else if(accumulator[y]==1 || negitiveStore[y]==1 && carry==true){
			carry=true;
			accumulator[y]=0;
		}
		else if(accumulator[y]==1 && negitiveStore[y]==1){
			carry=true;
			accumulator[y]=0;
		}
		else if(accumulator[y]==1 || negitiveStore[y]==1){
			carry=false;
			accumulator[y]=1;
		}
		else if(carry==true){
			carry=false;
			accumulator[y]=1;
		}
		else{
			carry=false;
			accumulator[y]=0;
		}
	}
}

void Baby::CMP(){
	cout<<"| CMP INSTRUCTION | INCREMENT COUNTER IF ACCUMULATOR NEGITIVE"<<endl;
	if(accumulator[31]==1){
		increaseCounter();
	}
}

void Baby::STP(){
	cout<<"| STP INSTRUCTION | SENDING STOP"<<endl;
	stop = true;
}

void Baby::getFileIn(){
	string filename = "BabyTest1-MC.txt";
	//string filename = "tester.txt";

    //READ IN FILE
    string line;
    
    ifstream reader( filename );
    if( ! reader ) {
        cout << "Error opening input file, filename not BabyTest1-MC.txt?" << endl;
    }
    int x=0;
    while(getline(reader, line)) {
    	for (int y = 0; y < 32; ++y){
    		char binNum = line[y];
    		int tmp = int(binNum);
    		if (tmp==48){
    			store[x][y] = 0;
    		}
    		else{
    			store[x][y] = 1;
    		}
    	}
        x++;
    }
    reader.close();
	cout<<"FILE LOADED SUCCESSFULLY"<<endl<<endl;
}

void Baby::increaseCounter(){
	bool carry = true;
	int pos=0;

	while(carry){
		if(controlInstruction[pos]==1){
			carry=true;
			controlInstruction[pos]=0;
		}
		else{
			controlInstruction[pos]=1;
			carry=false;
		}	
		pos++;
	}

	counter=binToInt(controlInstruction);
}

void Baby::fetch(){
	for (int y = 0; y < 32; ++y)
	{
		presentInstruction[y] = store[counter][y];
	}
}

int Baby::binToInt(int arrayIn[32]){
	int power=1;
	int intOut=0;
	for (int i=0; i < 32; ++i){
		if(arrayIn[i]==1){
			intOut+=power;
		}
		power=power*2;
	}
	return intOut;
}

void Baby::opcodeInt(int *operand){
	int power=1;
	for (int i=0; i < 13; ++i){
		if(operand[i]==1){
			opInt+=power;
		}
		power=power*2;
	}
}

void Baby::decodeOperate(){
	int operand[13] = {0};
	int functionNumber[3] = {0};

	for (int i = 0; i < 13; ++i){
		operand[i] = presentInstruction[i];
		if(i < 3){
			functionNumber[i] = presentInstruction[i+13];
		}
	}

	opInt=0;
	opcodeInt(operand);

	//Instruction Sets
	if(functionNumber[0]==0 && functionNumber[1]==0 && functionNumber[2]==0){
		JMP();
	}
	if(functionNumber[0]==1 && functionNumber[1]==0 && functionNumber[2]==0){
		JRP();
	}
	if(functionNumber[0]==0 && functionNumber[1]==1 && functionNumber[2]==0){
		LDN();
	}
	if(functionNumber[0]==1 && functionNumber[1]==1 && functionNumber[2]==0){
		STO();
	}
	if(functionNumber[0]==0 && functionNumber[1]==0 && functionNumber[2]==1){
		SUB();
	}
	if(functionNumber[0]==1 && functionNumber[1]==0 && functionNumber[2]==1){
		SUB();
	}
	if(functionNumber[0]==0 && functionNumber[1]==1 && functionNumber[2]==1){
		CMP();
	}
	if(functionNumber[0]==1 && functionNumber[1]==1 && functionNumber[2]==1){
		STP();
	}
}

void Baby::printValues(){
	if(counter>0){
		cout<<"| CONTROL INSTRUCTION: ";
		for (int x = 0; x < 32; ++x){
			printf("%d", controlInstruction[x]);
		}
		cout<<" | COUNT: "<<counter;
		cout<<endl<<"| ACCUMULATOR VALUE ";
		for (int x = 0; x < 32; ++x){
			printf("%d", accumulator[x]);
		}
		cout<<"| DECIMAL VALUE ";
		cout<<binToInt(accumulator)<<endl<<endl;
	}
}

int main(){
	Baby babyTest;

	babyTest.getFileIn();
	cout<<"| ATTEMPTING TO OPERATE |"<<endl;
	babyTest.printArray();
	while(babyTest.getStop()==false){
		babyTest.printValues();
		babyTest.increaseCounter();
		babyTest.fetch();
		babyTest.decodeOperate();
	}
	cout<<endl<<"| FINAL OUTPUT OF STORE |"<<endl;
	babyTest.printArray();
	return 0;
}
