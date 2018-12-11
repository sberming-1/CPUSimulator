#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#define MAX_LENGTH 5000

using namespace std;

short registers[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char screen[16][16] = {
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',},
    {'O','O','O','O','O','O','O','O','O','O','O','O','O','O','O','O',}
};

signed short ALU(signed short reg1, signed short reg2, int opcode){
    signed short result;
    switch(opcode){
        case 0: // and
            result = reg1 & reg2;
            break;
        case 1: // or
            result = reg1 | reg2;
            break;
        case 2: // xor
            result = reg1 ^ reg2;
            break;
        case 3: // not
            result = ~reg1;
            break;
        case 4: // add
            result = reg1+reg2;
            break;
        case 5: // subtract
            result = reg1-reg2;
            break;
        case 6: // multiply
            result = reg1 * reg2;
            break;
        case 7: // set less than
            result = (reg1<reg2) ? 32767 : 0;
            break;
        case 8: // shift left
            result = reg1<<1;
            break;
        case 9: // shift right
            result = reg1>>1;
            break;
        case 10: // blank
            result = reg1;
            break;
        case 11: // blank
            result = reg1;
            break;
        case 12: // blank
            result = reg2;
            break;
        case 13: // blank
            result = 1;
            break;
        case 14: // == 0
            result = (reg1 == 0) ? 32767 : 0;
            break;
        case 15: // != 0
            result = (reg1 != 0) ? 32767 : 0;
            break;
    }
    return result;
}

short * registerFile(bool clear, short writeData, int writeAddress, bool writeEnable, int read1, int read2){
    if(clear){
        for(int i = 0; i<16; i++){
            registers[i] = 0;
        }
    }

    if(writeEnable){
        registers[writeAddress] = writeData;
    }

    static short array[2] = {registers[read1], registers[read2]};

    return array;
} 

void screenMemory(bool clear, char writeData[16], int writeAddress, bool writeEnable){
    if(clear){
        for(int i=0;i<16;i++){
            for(int j=0;j<16;j++){
                screen[i][j] = 'O';
            }
        }
    }

    if(writeEnable){
        for(int i=0; i<16; i++){
            screen[writeAddress][i] = writeData[i];
        }
    }
}


int main(int argc, char const *argv[])
{
    signed short** instructions = new signed short*[MAX_LENGTH];
    for(int i = 0; i < MAX_LENGTH; i++){
        instructions[i] = new signed short[4]; 
    }


    string line; // line read in from file
    signed short PC;

    if(argc < 2){
        cout<<"Please select a file to execute"<<endl;
    }

    //open the file
    ifstream file(argv[1]);
    
    //make sure the file is open
    if(!file.is_open()){
        cout<<"There was an error opening the file. The program will now exit"<<endl;
        return -1;
    }
    //make sure th file is the correct format;
    getline(file, line);
    if(line.compare("v2.0 raw") != 0){
        cout << "The proper file type to execute is a .hex file\n";
        return -1;
    }

    int numItems = 0;
    std::stringstream ss;
    while(getline(file, line)){
        bool immed = false;
        for(int i = 0; i < 4; i++){
            short result;
            if(immed && i == 2){
                string temp = "0x";
                temp += line[i];
                temp += line[i+1];
                signed char c = std::stoi(temp, nullptr, 0);
                result = c;
                
            }else{
                ss<<std::hex<<line[i];
                ss >> result;
                ss.clear();
            }
            if(i == 0 && result >= 12){
                immed = true;
            }
            //cout<< "result: " << result << endl;;
            instructions[numItems][i] = result;
        }
        numItems++;
        //cout << "\n";
    }
    for(int i = 0; i < numItems; i++){
        cout<< "Instructions " << instructions[i][0] << instructions[i][1] << instructions[i][2] << instructions[i][3] <<endl;

    }
    PC = 0;
    cout<<numItems<<endl;
    while(PC < numItems+1){
        cout<< "PC: " << PC << ", Instructions " << instructions[PC][0] << instructions[PC][1] << instructions[PC][2] << instructions[PC][3] <<endl;
        short immed = instructions[PC][2];
        cout << immed << endl;
        if(instructions[PC][0] < 10){
            registers[(instructions[PC][1])] = ALU(instructions[PC][2], instructions[PC][3], instructions[PC][0]);
            PC++;
        }
        if(instructions[PC][0] == 10){
            registers[(instructions[PC][1])] = registers[(instructions[PC][2])];
            PC++;
        }
        if(instructions[PC][0] == 11){
            registers[(instructions[PC][2])] = instructions[PC][3];
            PC++;
        }
        if(instructions[PC][0] == 12){
            registers[(instructions[PC][1])] = immed;
            PC++;
        }
        if(instructions[PC][0] == 13){
            PC += immed;
        }
        if(instructions[PC][0] == 14){
            if((instructions[PC][1]) == 0){
                PC += immed;
            }
        }
        if(instructions[PC][0] == 15){
            if((instructions[PC][1]) != 0){
                PC += immed;
            }
        }
        
        cout<<"PC after process: "<< PC <<endl;
    }      
    return 0;
}
