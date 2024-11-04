#ifndef VOUL_MACHIN_H
#define VOUL_MACHIN_H

#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <bit>
#include <cmath>
#include <iostream>
#define RED "\033[31m"
#define RESET "\033[0m"

class Register {
    private:
    int memory[16] = {0};
    int size = 16;

    public:
    int getCell(const int& address) const;
    void setCell(int& address, int& val);

};

class Memory {
    private:
     int memory[256] = {};

    public:
    Memory();
    std::string getCell(int address) const;
    void setCell(int address, const std::string& val);
void clearm();
};

class CU{
public:
    void load(int R, int XY, Register& reg, Memory& mem);
    void load(int R, int val, Register& reg);
    void store(int R, int XY, Register& reg, Memory& mem);
    void move(int R, int S, Register& reg);
    void jump(int R, int XY, Register& reg, int& PC, char& op);
    void rotate(int R, int X, Register& reg);

};

class ALU {
    public:
    std::string decToHex(int& dec);
    int hexToDec(const std::string& hex);
    void add(int R, int S, int T, Register&reg);
void addf(int R, int S, int T, Register& reg);
    void OR(int R, int S, int T, Register&reg);
    void AND(int R, int S, int T, Register&reg);
    void XOR(int R, int S, int T, Register&reg);
     float hexToFloat(const string& hex);
std::string floatToHex(float value);
};
class CPU {
    protected:
    int pCounter;
    std::string instruc;
    char opCode;
    Register reg;
    Memory memory;
    ALU alu;
    CU cu;

    void fetch();
    void execute();

public:
    CPU() : pCounter(0){};
    Memory& setMem();
    void runNextStep();
    Register& getReg();
};

class Machine {
    private:
    CPU processor;

    public:
Machine();
    Machine(int iMemory[], int iRegisters[], int iPc, int iIr, const std::string& iBuffer = "");

    void loadFile(std::ifstream& file , int m);
     void loadProgram(const std::string& filename);
    bool isValid(const std::string& s);
    void outputState();

};

class MainUI {
private:
    Machine machine;
   

public:
    void displayMenu();
    void inputFile();
    void inputInstruction();
};
#endif
