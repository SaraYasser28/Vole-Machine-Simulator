#include "Voul_Mahine.h"
#include <iostream>

void MainUI::displayMenu() {
    std::cout << "\nMenu:\nA) Load a Program from a File\nB) Enter Instructions Manually\nC) Exit";
    std::cout << "\nChoose an option: ";
    std::string choice;
    std::cin >> choice;

    if (choice == "A" || choice == "a") {
        inputFile();
    }else if (choice == "B" || choice == "b") {
        inputInstruction();
    }else if (choice == "C" || choice == "c") {
        std::cout << "\nExiting program...\n";
    } else {
        std::cerr << RED << "\n*Invalid choice*\n" << RESET;
        displayMenu();
    }
    
}

void MainUI::inputFile() {
    std::cout << "Enter the file name: ";
    std::string fileName;
    std::cin >> fileName;
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << RED << "\nError: Unable to open file : " << fileName << RESET << std::endl;
        std::cerr << RED << "Please check the file path and extension\n\n" << fileName << RESET;
        inputFile();
    }
    int m = 10;
    std::cout << "\nWhat index number would you like to begin with in memory: ";
    std::cin >> m;
    machine.loadFile(file, m);
    machine.outputState();
}

void MainUI::inputInstruction() {
    std::string instruction;
    do{
        std::cout << "Enter the instruction (or 'C000' to HALT): ";
        std::cin >> instruction;
        machine.loadProgram(instruction);
    }while(instruction != "C000");

}

//Machine
void Machine::loadFile(std::ifstream& file , int m){
    std::string val;
    std::string struc;
    for( int i = m ; file >> val; i++) {
        processor.setMem().setCell(i++, val);
        if (file >> struc) {
            processor.setMem().setCell(i, struc);
        }else{
            std::cout << "\n**The memory must be even numbers check the input and try agein**\n";
            std::exit(EXIT_FAILURE);
        }
        struc = val + struc;
        loadProgram(struc);
    }
    file.close();
}

void Machine::loadProgram(std::string s) {
   
    if(isValid(s)){
        processor.runNextStep();
    }else{
        std::cerr << RED << "**Invalid instructions**" << RESET << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Machine::outputState() {
    
    ALU a;
    std::cout << "\n MEMORY \n";
    for(int i = 0 ; i< 15 ; i++){
        //int cellValue = std::soi(memory.getCell(i));
        std::cout << a.decToHex(i)  <<" -> "<< processor.setMem().getCell(i) << std::endl;
    }
    std::cout << "\nREGISTER\n";
    for(int i = 0 ; i< 16 ; i++){
        int cellValue = processor.getReg().getCell(i);
        std::cout << a.decToHex(i)  <<" -> "<< a.decToHex(cellValue) << std::endl;
    }
}

bool Machine::isValid(const std::string& s) {
    std::cout << "Validating input..." << std::endl;
        if(s.size() != 4){
            return false;
        }

        if((s[0] >= '1' && s[0] <= '9') || (s[0] >= 'A' && s[0] <= 'D')){
            return true;
        }
        for(int i = 1 ; i < s.size() ; i++){

            if((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'A' && s[i] <= 'F')){
                return true;
            }
        }
    return false;
}

//CPU
Memory& CPU::setMem(){
    return memory;
}

Register& CPU::getReg() {
    return reg;
}

void CPU::fetch() {
    instruc = memory.getCell(pCounter) + memory.getCell(pCounter+1);
    pCounter += 2;
    std::cout << instruc <<std::endl;
}

void CPU::execute() {
    opCode = instruc[0];
    //op RXY
    int XY = alu.hexToDec(std::string(1, instruc[2]) + std::string(1, instruc[3]));
    int R = alu.hexToDec(std::string(1, instruc[1]));
    int S = alu.hexToDec(std::string(1, instruc[2]));
    int T = alu.hexToDec(std::string(1, instruc[3]));

    switch (opCode) {
        case '1':
            cu.load(R, XY, reg, memory);
            break;
        case '2':
            std::cout <<"b";
            cu.load(R, XY, reg);
            break;
        case '3':
            cu.store(R, XY, reg, memory);
            break;
        case '4':
            cu.move(R, S, reg);
            break;
        case '5':
            alu.add(R, S, T, reg);
            break;
        case '6':
            alu.add(R, S, T,reg);
            break;
        case '7':
            alu.OR(R, S, T, reg);
            break;
        case '8':
            alu.AND(R, S, T, reg);
            break;
        case '9':
            alu.XOR(R, S, T, reg);
            break;
        case 'A':
            cu.jump(R, XY, reg, pCounter, opCode);
            std::cout << pCounter;
            break;
        case 'B':
            cu.jump(R, XY, reg, pCounter, opCode);
            std::cout <<"jump to "<< pCounter <<std::endl;
            break;
        case 'C':
            std::cout << "\nProgram halted...\n";
            break;
        case 'D':
            cu.jump(R, XY, reg, pCounter, opCode);
            break;
        default:
            std::cerr << RED << "\n*Invalid opcode in memory[ " << pCounter-2 << " ]\n"<< RESET;
            std::exit(EXIT_FAILURE);
    }
    
}


void CPU::runNextStep() {
    fetch();
    execute();
}

//CU
void CU::load(int R, int XY, Register& reg, Memory& mem) {
    std::cout << "Loading from memory to register..." << std::endl;
    int val = std::stoi(mem.getCell(XY),nullptr,16);
    reg.setCell(R, val);
}

void CU::load(int R, int val, Register& reg) {
    std::cout << "Loading value into register..." << std::endl;
    reg.setCell(R, val);
}

void CU::store(int R, int XY, Register& reg, Memory& mem) {
    std::cout << "Storing from register to memory..." << std::endl;
    mem.setCell(XY, std::to_string(reg.getCell(R)));
}

void CU::move(int R, int S, Register& reg) {
    std::cout << "Moving data between registers..." << std::endl;
    int t = reg.getCell(R);
    int val = reg.getCell(S);
    reg.setCell(R, val);
    reg.setCell(S, t);
}

void CU::jump(int R, int XY, Register& reg, int& PC , char& op) {
    bool cond1 = op == 'B' && reg.getCell(R) == reg.getCell(0);
    bool cond2 = op == 'D' && reg.getCell(R) > reg.getCell(0);
    if(cond1 || cond2){
        std::cout << "Jumping to memory location..." << std::endl;
        if(XY % 2 == 0){
            PC = XY;
        }else{
            PC = XY - 1;
        }
    }
}

int rotl(int value, int shift, int bit_width = 8) {
    // Mask the value to 8 bits
    value &= 0xFF;
    // Perform the rotation
    shift %= bit_width;
    return ((value >> shift) | (value << (bit_width - shift))) & 0xFF;
}
void CU::rotate(int R, int X, Register& reg){
    int val = rotl(reg.getCell(R),X);
    reg.setCell(R, val);
}
//ALU
int ALU::hexToDec(const std::string& hex) {
    int result;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> result;
    return result;
    return 0;
}

std::string ALU::decToHex(int& dec) {
   std::stringstream ss;
    ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << dec;
    return ss.str();
}

void ALU::add(int R, int S, int T, Register& reg) {
     // Extract sign, exponent, and mantissa for both numbers
    uint8_t num1 = reg.getCell(S);
    uint8_t num2 = reg.getCell(T);
    
    int sign1 = (num1 >> 7) & 1;
    int exponent1 = (num1 >> 4) & 0b111;
    int mantissa1 = num1 & 0b1111;

    int sign2 = (num2 >> 7) & 1;
    int exponent2 = (num2 >> 4) & 0b111;
    int mantissa2 = num2 & 0b1111;

    // Align exponents by shifting mantissa of the smaller exponent
    if (exponent1 > exponent2) {
        mantissa2 >>= (exponent1 - exponent2);
        exponent2 = exponent1;
    } else if (exponent2 > exponent1) {
        mantissa1 >>= (exponent2 - exponent1);
        exponent1 = exponent2;
    }

    // Perform addition or subtraction on mantissas
    int result_mantissa;
    int result_sign;
    if (sign1 == sign2) {
        // Same signs, add mantissas
        result_mantissa = mantissa1 + mantissa2;
        result_sign = sign1;
    } else {
        // Different signs, subtract smaller from larger
        if (mantissa1 > mantissa2) {
            result_mantissa = mantissa1 - mantissa2;
            result_sign = sign1;
        } else {
            result_mantissa = mantissa2 - mantissa1;
            result_sign = sign2;
        }
    }

    // Normalize if needed
    int result_exponent = exponent1;
    while (result_mantissa > 0b1111) { // If mantissa exceeds 4 bits
        result_mantissa >>= 1;
        result_exponent += 1;
    }

    // Reassemble the result in 8-bit format
    uint8_t result = (result_sign << 7) | (result_exponent << 4) | (result_mantissa & 0b1111);
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(result);
    // Convert the string in ss to an integer
    std::string hexString = ss.str(); // Get the formatted hex string
    int integerResult = std::stoi(hexString, nullptr, 16); // Convert hex string to integer

    // Store result in Register R (assuming reg is a valid object with setCell method)
    reg.setCell(R, integerResult);
    
}
void ALU::OR(int R, int S, int T, Register&reg){
    int val = reg.getCell(S) | reg.getCell(T);
    reg.setCell(R, val);
}
void ALU::AND(int R, int S, int T, Register&reg){
    int val = reg.getCell(S) & reg.getCell(T);
    reg.setCell(R, val);
}
void ALU::XOR(int R, int S, int T, Register&reg){
    int val = reg.getCell(S) ^ reg.getCell(T);
    reg.setCell(R, val);
}
//memory
Memory::Memory() : memory(size, "00") {}

std::string Memory::getCell(int address) const {
    if (address >= 0 && address <= 256) {
        return memory[address];
    }

    std::cerr << RED << "\n**Invalid memory address**\n" << RESET;
    std::exit(EXIT_FAILURE);
}

void Memory::setCell(int address, const std::string& val) {
    if (address >= 0 && address <= 256) {
        memory[address] = val;
    }else {
        std::cerr << RED << "\n**Invalid memory address**\n" << RESET;
    }
}
//register
int Register::getCell(const int& address) const {
    if (address >= 0 && address < 16) {
        return memory[address];
    }
    std::cerr << RED << "\n**Invalid register address**\n" << RESET;
    return -1;
}

void Register::setCell(int& address, int& val) {
    if (address >= 0 && address < 16) {
        memory[address] = val;
    } else {
        std::cerr << RED << "\n**Invalid register address**\n" << RESET;
    }
}

