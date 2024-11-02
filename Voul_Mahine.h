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
    int size = 256;
    std::vector<std::string> memory;

    public:
    Memory();
    std::string getCell(int address) const;
    void setCell(int address, const std::string& val);
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
    void OR(int R, int S, int T, Register&reg);
    void AND(int R, int S, int T, Register&reg);
    void XOR(int R, int S, int T, Register&reg);
    
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
    void loadFile(std::ifstream& file , int m);
    void loadProgram(std::string s);
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