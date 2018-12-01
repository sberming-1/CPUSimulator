#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

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
            result = reg1;
            break;
        case 13: // blank
            result = reg1;
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
    
    string line;
    if(argc < 2){
        cout<<"Please select a file to execute"<<endl;
    }

    ifstream file (argv[1]);
    if(!file.is_open()){
        cout<<"There was an error opening the file. The program will now exit"<<endl;
        return -1;
    }
    else{
        getline(file, line);
        if(line.compare("v2.0 raw") != 0){
            cout << "The proper file type to execute is a .hex file\n";
        }
        else{
            int counter = 0;
            short** ROM = new short*[10000];
            for(int i = 0; i<10000; i++){
                ROM[i] = new short[4];
            }
            
            int numberOfItems = 0;
            while(getline(file, line)){
                numberOfItems++;   
            }
            file.clear();
            file.seekg(0, ios::beg);
            string contents[numberOfItems];
            getline(file, line);
            int i = 0;
            while(getline(file,line)){
                contents[i] = line;
                i++;
            }
            while(true){
                int position;
                std::stringstream ss;
                short opcode, dest, src1, src2, immediate;
                line = contents[counter]; 
                string im = "";
                switch(line[0]){
                    //the r opcodes
                    case 0: case 1: case 2: case 3: case 4:                        
                    case 5: case 6: case 7: case 8: case 9:                        
                    case 'a': case 'b':
                        ss<<std::hex<<line[0];
                        ss>>ROM[position][0];
                        ss.clear();
                        ss<<std::hex<<line[1];
                        ss>>ROM[position][1];
                        ss.clear();
                        ss<<std::hex<<line[2];
                        ss>>ROM[position][2];
                        ss.clear();
                        ss<<std::hex<<line[3];
                        ss>>ROM[position][3];
                        ss.clear();
                        position++;
                        counter++;
                        break;
                    //The i codes
                    case 'c': 
                        ss<<std::hex<<line[0];
                        ss>>opcode;
                        ss.clear();
                        ss<<std::hex<<line[1];
                        ss>>dest;
                        ss.clear();
                        im = "";
                        im+=line[2];
                        im+=line[3];
                        ss<<std::hex<<im;
                        ss>>immediate;
                        ss.clear();
                        registers[dest] = immediate;
                        break;
                    case 'd':
                        ss<<std::hex<<line[0];
                        ss>>opcode;
                        ss.clear();
                        ss<<std::hex<<line[1];
                        ss>>dest;
                        ss.clear();
                        im = "";
                        im+=line[2];
                        im+=line[3];
                        ss<<std::hex<<im;
                        ss>>immediate;
                        ss.clear();
                        counter += immediate;
                        break; 
                    case 'e': 
                        ss<<std::hex<<line[0];
                        ss>>opcode;
                        ss.clear();
                        ss<<std::hex<<line[1];
                        ss>>dest;
                        ss.clear();
                        im = "";
                        im+=line[2];
                        im+=line[3];
                        ss<<std::hex<<im;
                        ss>>immediate;
                        ss.clear();
                        if(dest == 0) counter += immediate;
                        break;
                    case 'f':
                        ss<<std::hex<<line[0];
                        ss>>opcode;
                        ss.clear();
                        ss<<std::hex<<line[1];
                        ss>>dest;
                        ss.clear();
                        im = "";
                        im+=line[2];
                        im+=line[3];
                        ss<<std::hex<<im;
                        ss>>immediate;
                        ss.clear();
                        if(dest != 0) counter += immediate;
                        break;
                }
            }
        }// end of compare else

    }//end of file else
    return 0;
}
