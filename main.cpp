#include <cstdio>
#include <cstdlib>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem {
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];
    void Initialise() {
        for (u32 i = 0; i < MAX_MEM; i++) {
            Data[i] = 0;
        }
    }

    // this is cool, read 1 byte
    Byte operator[](u32 Address) const { return Data[Address]; }

    // and this, write 1 byte
    Byte& operator[](u32 Address) { return Data[Address]; }
};

struct CPU {

    Word PC; // Program Counter
    Word SP; // Stack Pointer

    Byte A, X, Y;

    Byte C : 1; // Status flag
    Byte Z : 1;
    Byte I : 1;
    Byte D : 1;
    Byte B : 1;
    Byte V : 1;
    Byte N : 1;

    void Reset(Mem& memory) {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
        memory.Initialise();
    }

    Byte FetchByte(u32& Cycles, Mem& memory) {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    static constexpr Byte INS_LDA_IM = 0xA9;

    void Execute(u32 Cycles, Mem& memory) {
        while (Cycles > 0) {
            Byte Ins = FetchByte(Cycles, memory);
            switch (Ins) {
            case INS_LDA_IM: {
                Byte Value = FetchByte(Cycles, memory);
                A = Value;
                Z = (A == 0);
                N = (A & 0b10000000) > 0;
            } break;
            default: {
                printf("instruction not handled %d", Ins);
            } break;
            }
        }
    }
};

int main() {
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x42;
    cpu.Execute(2, mem);
    return 0;
}
