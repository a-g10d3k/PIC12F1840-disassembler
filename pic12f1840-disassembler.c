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
    {0b100u , "CALL", 3},
    {0b101u , "GOTO", 3},
    {0b0100u, "BCF" , 4},
};
const int OPERATION_TABLE_LENGTH = sizeof(OPERATION_TABLE) / sizeof(struct Operation);

const struct Operation *getOperation(uint bytes)
{
    for(int i = 0; i < OPERATION_TABLE_LENGTH; ++i)
    {
        const struct Operation *operation = &OPERATION_TABLE[i];
        if(operation->opcode << (INSTRUCTION_LENGTH - operation->opcodeLength) == bytes & (0xffffu << (INSTRUCTION_LENGTH - operation->opcodeLength)))
        {
            return operation;
        }
    }
    return NULL;
}

//Ignores the 7th and 8th least significant bit, and returns the instruction as an unsigned 14-bit integer
uint formatBytes(uint bytes)
{
    uint high = 0xff00u & bytes;
    uint low = UINT8_C(0x003fu) & bytes;
    high = high >> 2;
    return high | low;
}

//Read an instruction from stream
struct Instruction readInstruction(FILE *stream)
{
    uint bytes;
    const int offset = __INT_WIDTH__ / 8 - 2;
    fread(&bytes + offset, 2, 1, stream);
    bytes = formatBytes(bytes);
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
        puts("aawawawa");
        fputs("ERROR\n", stream);
        puts("awa2");
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
    for(int i = 0; i < MEMORY_SIZE; ++i) 
    {
        fputs("whatever", outFile);
        struct Instruction instruction = readInstruction(inFile);
        //fputs("whatever", outFile);
        writeInstruction(&instruction, outFile);
    }
    fclose(inFile);
    fclose(outFile);
}

