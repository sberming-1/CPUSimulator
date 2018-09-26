#include <string.h>
#include <iostream>

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
    /* code */
    return 0;
}
