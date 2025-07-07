#include <stdio.h>
#include <stdint.h>

#define INSTRUCTION_LENGTH 14
#define MEMORY_SIZE 4096
typedef unsigned int uint;

struct Operation
{
    uint opcode;
    char mnemonic[7];
    uint opcodeLength;
};

struct Instruction
{
    const struct Operation *operation;
};

const struct Operation OPERATION_TABLE[] = 
{
    {0b000111u,         "ADDWF",   6},
    {0b111101u,         "ADDWFC",  6},
    {0b000101u,         "ANDWF",   6},
    {0b110111u,         "ASRF",    6},
    {0b110101u,         "LSLF",    6},
    {0b110110u,         "LSRF",    6},
    {0b0000011u,        "CLRF",    7},
    {0b00000100000u,    "CLRW",   11},
    {0b001001u,         "COMF",    6},
    {0b000011u,         "DECF",    6},
    {0b001010u,         "INCF",    6},
    {0b000100u,         "IORWF",   6},
    {0b001000u,         "MOVF",    6},
    {0b0000001u,        "MOVWF",   7},
    {0b001101u,         "RLF",     6},
    {0b001100u,         "RRF",     6},
    {0b000010u,         "SUBWF",   6},
    {0b111011u,         "SUBWFB",  6},
    {0b001110u,         "SWAPF",   6},
    {0b000110u,         "XORWF",   6},
    {0b001011u,         "DECFSZ",  6},
    {0b001111u,         "INCFSZ",  6},
    {0b0100u,           "BCF" ,    4},
    {0b0101u,           "BSF" ,    4},
    {0b100u ,           "CALL",    3},
    {0b101u ,           "GOTO",    3},
}; 
const int OPERATION_TABLE_LENGTH = sizeof(OPERATION_TABLE) / sizeof(struct Operation);

const struct Operation *getOperation(uint bytes)
{
    for(int i = 0; i < OPERATION_TABLE_LENGTH; ++i)
    {
        const struct Operation *operation = &OPERATION_TABLE[i];
        printf("instruction: %x\n", bytes);
        uint truncatedInstruction = bytes & (0xffffu << (INSTRUCTION_LENGTH - operation->opcodeLength));
        printf("truncated instruction: %x\n", truncatedInstruction);
        uint offsetOpcode = operation->opcode << (INSTRUCTION_LENGTH - operation->opcodeLength);
        printf("offset opcode: %x\n", offsetOpcode);
        if(offsetOpcode == truncatedInstruction)
        {
            return operation;
        }
    }
    return NULL;
}

//Ignores the 7th and 8th least significant bit, and returns the instruction as an unsigned 14-bit integer
uint formatBytes(uint bytes)
{
    uint low = 0x3f00u & bytes;
    //printf("low: %x\n", low);
    uint high = 0x00ffu & bytes;
    //printf("high: %x\n", high);
    high = high << 6;
    low = low >> 8;
    return high | low;
}

//Read an instruction from stream
struct Instruction readInstruction(FILE *stream)
{
    uint bytes;
    fread(&bytes, 2, 1, stream);
    //printf("raw: %x\n", bytes);
    bytes = formatBytes(bytes);
    //printf("formatted: %x\n", bytes);
    struct Instruction instruction =
    {
        getOperation(bytes)
    };
    return instruction;
}

//Write an instruction to stream
void writeInstruction(struct Instruction *instruction, FILE *stream)
{
    if(!instruction->operation)
    {
        //puts("aawawawa");
        fputs("ERROR\n", stream);
        //puts("awa2");
    }
    else
    {
        fputs(instruction->operation->mnemonic, stream);
        fputc('\n', stream);
    }
}

int main(int argc, char *argv[])
{
    switch(argc) {
        case 1:
        case 2:
        fputs("pic12f1850-disassembler\n\nUsage:\npic12f1850-disassembler (input filename) (output filename)\n", stdout);
        return 0;
        break;
    }
    FILE *inFile = fopen(argv[1], "r+");
    if(!inFile)
    {
        fputs("Can't open file\n", stderr);
        return -1;
    }
    FILE *outFile = fopen(argv[2], "w+");
    for(int i = 0; i < MEMORY_SIZE/2; ++i) 
    {
        struct Instruction instruction = readInstruction(inFile);
        writeInstruction(&instruction, outFile);
    }
    fclose(inFile);
    fclose(outFile);
}