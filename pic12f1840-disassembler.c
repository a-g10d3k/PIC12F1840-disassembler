#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define INSTRUCTION_LENGTH 14
#define MEMORY_SIZE 4096
#define PROGRAM_MEMORY_START 5
typedef unsigned int uint;

enum Notation {
    NOTATION_NO_OPERANDS,
    NOTATION_SINGLE_OPERAND,
    NOTATION_NORMAL,
    NOTATION_REVERSE,
    NOTATION_FLAG,
    NOTATION_FLAG2,
    NOTATION_FLAG_REVERSE,
    NOTATION_INDEX,
};

struct Operation
{
    uint opcode;
    char mnemonic[7];
    uint opcodeLength;
    uint operand1Length;
    uint operand2Length;
    enum Notation notation;
};

struct Instruction
{
    const struct Operation *operation;
    uint operand1;
    uint operand2;
};

const struct Operation OPERATION_TABLE[] = 
{
    {0b000111u,         "ADDWF",   6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b111101u,         "ADDWFC",  6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b000101u,         "ANDWF",   6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b110111u,         "ASRF",    6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b110101u,         "LSLF",    6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b110110u,         "LSRF",    6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b0000011u,        "CLRF",    7, 7, 0, NOTATION_SINGLE_OPERAND},
    {0b000001000000u,   "CLRW",   12, 0, 0, NOTATION_NO_OPERANDS},
    {0b001001u,         "COMF",    6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b000011u,         "DECF",    6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b001010u,         "INCF",    6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b000100u,         "IORWF",   6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b001000u,         "MOVF",    6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b0000001u,        "MOVWF",   7, 7, 0, NOTATION_SINGLE_OPERAND},
    {0b001101u,         "RLF",     6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b001100u,         "RRF",     6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b000010u,         "SUBWF",   6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b111011u,         "SUBWFB",  6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b001110u,         "SWAPF",   6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b000110u,         "XORWF",   6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b001011u,         "DECFSZ",  6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b001111u,         "INCFSZ",  6, 1, 7, NOTATION_FLAG_REVERSE},
    {0b0100u,           "BCF" ,    4, 3, 7, NOTATION_REVERSE},
    {0b0101u,           "BSF" ,    4, 3, 7, NOTATION_REVERSE},
    {0b0110u,           "BTFSC",   4, 3, 6, NOTATION_REVERSE},
    {0b0111u,           "BTFSS",   4, 3, 7, NOTATION_REVERSE},
    {0b111110u,         "ADDLW",   6, 8, 0, NOTATION_SINGLE_OPERAND},
    {0b111001u,         "ANDLW",   6, 8, 0, NOTATION_SINGLE_OPERAND},
    {0b111000u,         "IORLW",   6, 8, 0, NOTATION_SINGLE_OPERAND},
    {0b000000001u,      "MOVLB",   9, 5, 0, NOTATION_SINGLE_OPERAND},
    {0b1100011u,        "MOVLP",   7, 7, 0, NOTATION_SINGLE_OPERAND},
    {0b110000u,         "MOVLW",   6, 8, 0, NOTATION_SINGLE_OPERAND},
    {0b111100u,         "SUBLW",   6, 8, 0, NOTATION_SINGLE_OPERAND},
    {0b111010u,         "XORLW",   6, 8, 0, NOTATION_SINGLE_OPERAND},
    {0b11001u,          "BRA",     5, 9, 0, NOTATION_SINGLE_OPERAND},
    {0b00000000001011u, "BRW",    14, 0, 0, NOTATION_NO_OPERANDS},
    {0b100u,            "CALL",    3,11, 0, NOTATION_SINGLE_OPERAND},
    {0b00000000001010u, "CALLW",  14, 0, 0, NOTATION_NO_OPERANDS},
    {0b101u,            "GOTO",    3,11, 0, NOTATION_SINGLE_OPERAND},
    {0b00000000001001u, "RETFIE", 14, 0, 0, NOTATION_NO_OPERANDS},
    {0b110100u,         "RETLW",   6, 8, 0, NOTATION_SINGLE_OPERAND},
    {0b00000000001000u, "RETURN", 14, 0, 0, NOTATION_NO_OPERANDS},
    {0b00000001100100u, "CLRWDT", 14, 0, 0, NOTATION_NO_OPERANDS},
    {0b00000000000000u, "NOP",    14, 0, 0, NOTATION_NO_OPERANDS},
    {0b00000001100010u, "OPTION", 14, 0, 0, NOTATION_NO_OPERANDS},
    {0b00000000000001u, "RESET",  14, 0, 0, NOTATION_NO_OPERANDS},
    {0b00000001100011u, "SLEEP",  14, 0, 0, NOTATION_NO_OPERANDS},
    {0b00000001100u,    "TRIS",   11, 3, 0, NOTATION_SINGLE_OPERAND},
    {0b1100010u,        "ADDFSR",  7, 1, 6, NOTATION_FLAG},
    {0b00000000010u,    "MOVIW",  11, 1, 2, NOTATION_FLAG2},
    {0b1111110u,        "MOVIW",   7, 1, 6, NOTATION_INDEX},
    {0b00000000011u,    "MOVWI",  11, 1, 2, NOTATION_FLAG2},
    {0b1111111u,        "MOVWI",   7, 1, 6, NOTATION_INDEX}
}; 
const int OPERATION_TABLE_LENGTH = sizeof(OPERATION_TABLE) / sizeof(struct Operation);

//Convert big-endian 16-bit unsigned integer to unsigned int
uint be2le16u(uint num)
{
    return (num >> 8) | (num << 8);
}

const struct Operation *getOperation(uint bytes)
{
    for(int i = 0; i < OPERATION_TABLE_LENGTH; ++i)
    {
        const struct Operation *operation = &OPERATION_TABLE[i];
        uint truncatedInstruction = bytes & (0xffffu << (INSTRUCTION_LENGTH - operation->opcodeLength));
        uint offsetOpcode = operation->opcode << (INSTRUCTION_LENGTH - operation->opcodeLength);
        if(offsetOpcode == truncatedInstruction)
        {
            return operation;
        }
    }
    return NULL;
}

uint getOperand(uint bytes, uint offset, uint length)
{
    uint mask = 0x3fffu;
    mask = mask >> (INSTRUCTION_LENGTH - length);
    mask = mask << (INSTRUCTION_LENGTH - offset - length);
    uint operand = bytes & mask;
    operand = operand >> (INSTRUCTION_LENGTH - offset - length);
    return operand;
}

//Ignores the 7th and 8th least significant bit, and returns the instruction as an unsigned 14-bit integer
uint formatBytes(char *bytes)
{
    uint low = 0xffu & bytes[0] ;
    uint high = 0x3fu & bytes[1];
    high = high << 8;
    return high | low;
}

//Read an instruction from stream
struct Instruction readInstruction(char *buffer, int index)
{
    uint bytes = formatBytes(buffer + index);
    const struct Operation *operation = getOperation(bytes);
    bool hasOperand1 = operation->operand1Length != 0;
    bool hasOperand2 = operation->operand2Length != 0;
    uint operand1 = hasOperand1 ? getOperand(bytes, operation->opcodeLength, operation->operand1Length) : 0u;
    uint operand2 = hasOperand2 ? getOperand(bytes, operation->opcodeLength + operation->operand1Length, operation->operand1Length) : 0u;
    struct Instruction instruction =
    {
        operation,
        operand1,
        operand2
    };
    return instruction;
}

const char *getFlag2Prefix(uint flags)
{
    switch (flags)
    {
        case 0b00u:
            return "++";
        case 0b01u:
            return "--";
        default:
            return "";
    }
}

const char *getFlag2Suffix(uint flags)
{
    switch (flags)
    {
        case 0b10u:
            return "++";
        case 0b11u:
            return "--";
        default:
            return "";
    }
}

//Convert a 6-bit two's complement signed number to an int
int _6b2c(uint num) 
{
    if((num & (1u << 5)) == 0) return (int)num;
    num = ~(num - 1) & 0b111111;
    return -((int)num);
}

//Write an instruction to stream
void writeInstruction(struct Instruction *instruction, FILE **stream)
{
    if(!instruction->operation)
        fputs("??????\n", *stream);
    else
    {
        const char *mnemonic = instruction->operation->mnemonic;
        uint operand1 = instruction->operand1;
        uint operand2 = instruction->operand2;
        switch(instruction->operation->notation)
        {
            case NOTATION_NO_OPERANDS:
                fprintf(*stream, "%s\n", mnemonic);
                break;
            case NOTATION_SINGLE_OPERAND:
                fprintf(*stream, "%s %u", mnemonic, operand1);
                if(strcmp(mnemonic, "GOTO") == 0 || strcmp(mnemonic, "CALL") == 0)
                    fprintf(*stream, " ;%s line %u", mnemonic, (operand1 - PROGRAM_MEMORY_START) / 2 + 1);
                fputc('\n', *stream);
                break;
            case NOTATION_NORMAL:
                fprintf(*stream, "%s 0x%x, 0x%x\n", mnemonic, operand1, operand2);
                break;
            case NOTATION_REVERSE:
                fprintf(*stream, "%s %u, %u\n", mnemonic, operand2, operand1);
                break;
            case NOTATION_FLAG:
                fprintf(*stream, "%s FSR%u, %i\n", mnemonic, operand1, _6b2c(operand2));
                break;
            case NOTATION_FLAG_REVERSE:
                fprintf(*stream, "%s %u, %u\n", mnemonic, operand2, operand1);
                break;
            case NOTATION_INDEX:
                fprintf(*stream, "%s %u[FSR%u]\n", mnemonic, operand2, operand1);
                break;
            case NOTATION_FLAG2:
                fprintf(*stream, "%s %sFSR%u%s\n", mnemonic, getFlag2Prefix(operand2), operand1, getFlag2Suffix(operand2));
                break;
        }
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
    char buffer[MEMORY_SIZE];
    fread(buffer, 1, MEMORY_SIZE, inFile);
    fclose(inFile);
    for(int i = 0; i < MEMORY_SIZE; i += 2) 
    {
        struct Instruction instruction = readInstruction(buffer, i);
        writeInstruction(&instruction, &outFile);
    }
    fclose(outFile);
}