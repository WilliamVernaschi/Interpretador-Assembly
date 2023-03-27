#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <sstream>

using namespace std;

enum InstructionType{ ARITMETICA, DESVIO, MEMORIA, MOVIMENTACAO};

enum InstructionName{ ADD, ADDI, SUB, SUBI, MUL, DIV, BLT, BGT, BEQ, J, JR, JAL,
                      LW, SW, MOV, MOVI};


struct Instruction{
  InstructionName name;
  int vimm, rd, rs, rt;
  InstructionType type;
};

class Parser{
private:

  // recebe uma string que representa um registro do tipo "rxx"
  // e retorna apenas o número "xx"
  static int getRegisterId(const string &reg){
    return stoi(reg.substr(1, reg.size()-1));
  }

  
  // recebe um vetor que foram recebidos da entrada e retrona uma instrução.
  static Instruction parseInstruction(const vector<string> &values){
    Instruction instruction;

    string name = values[0];

    if(name == "add"){
      instruction.rd = getRegisterId(values[1]);
      instruction.rs = getRegisterId(values[2]);
      instruction.rt = getRegisterId(values[3]);
      instruction.type = ARITMETICA;
      instruction.name = ADD;
    }
    else if(name == "addi"){
      instruction.rd = getRegisterId(values[1]);
      instruction.rs = getRegisterId(values[2]);
      instruction.vimm = stoi(values[3]);
      instruction.type = ARITMETICA;
      instruction.name = ADDI;
    }
    else if(name == "sub"){
      instruction.rd = getRegisterId(values[1]);
      instruction.rs = getRegisterId(values[2]);
      instruction.rt = getRegisterId(values[3]);
      instruction.type = ARITMETICA;
      instruction.name = SUB;
    }
    else if(name == "subi"){
      instruction.rd = getRegisterId(values[1]);
      instruction.rs = getRegisterId(values[2]);
      instruction.vimm = stoi(values[3]);
      instruction.type = ARITMETICA;
      instruction.name = SUBI;
    }
    else if(name == "mul"){
      instruction.rd = getRegisterId(values[1]);
      instruction.rs = getRegisterId(values[2]);
      instruction.rt = getRegisterId(values[3]);
      instruction.type = ARITMETICA;
      instruction.name = MUL;
    }
    else if(name == "div"){
      instruction.rd = getRegisterId(values[1]);
      instruction.rs = getRegisterId(values[2]);
      instruction.rt = getRegisterId(values[3]);
      instruction.type = ARITMETICA;
      instruction.name = DIV;
    }
    else if(name == "blt"){
      instruction.rs = getRegisterId(values[1]);
      instruction.rt = getRegisterId(values[2]);
      instruction.vimm = stoi(values[3]);
      instruction.type = DESVIO;
      instruction.name = BLT;
    }
    else if(name == "bgt"){
      instruction.rs = getRegisterId(values[1]);
      instruction.rt = getRegisterId(values[2]);
      instruction.vimm = stoi(values[3]);
      instruction.type = DESVIO;
      instruction.name = BGT;
    }
    else if(name == "beq"){
      instruction.rs = getRegisterId(values[1]);
      instruction.rt = getRegisterId(values[2]);
      instruction.vimm = stoi(values[3]);
      instruction.type = DESVIO;
      instruction.name = BEQ;
    }
    else if(name == "j"){
      instruction.vimm = stoi(values[1]);
      instruction.type == DESVIO;
      instruction.name == J;
    }
    else if(name == "jr"){
      instruction.rd = getRegisterId(values[1]);
      instruction.type = DESVIO;
      instruction.name = JR;
    }
    else if(name == "jal"){
      instruction.vimm = stoi(values[1]);
      instruction.type = DESVIO;
      instruction.name = JAL;
    }
    else if(name == "lw"){
      instruction.rd = getRegisterId(values[1]);
      instruction.vimm = stoi(values[2]);
      instruction.rs = getRegisterId(values[3]);
      instruction.type = MEMORIA;
      instruction.name = LW;
    }
    else if(name == "sw"){
      instruction.rd = getRegisterId(values[1]);
      instruction.vimm = stoi(values[2]);
      instruction.rs = getRegisterId(values[3]);
      instruction.type = MEMORIA;
      instruction.name = SW;
    }
    else if(name == "mov"){
      instruction.rd = getRegisterId(values[1]);
      instruction.rs = getRegisterId(values[2]);
      instruction.type = MOVIMENTACAO;
      instruction.name = MOV;
    }
    else if(name == "movi"){
      instruction.rd = getRegisterId(values[1]);
      instruction.vimm = stoi(values[2]);
      instruction.type = MOVIMENTACAO;
      instruction.name = MOVI;
    }

    return instruction;
  }

  // lê as linhas da entrada padrão e retorna um vetor de valores 
  // separados por espaço e vírgula.
  //
  // por exemplo, para a entrada
  //
  // mov r01,r05
  // addi r01,r05,9
  //
  // a saída seria
  //
  // {{"mov", "r01", "r05"},
  //  {"addi", "r01", "r05", "9"}}
  static vector<vector<String>> getInputValues(){

    string line;
    vector<vector<string>> values;
    while(getline(cin, line)){


      istringstream iss(line);
      string token;
      string no_commas;

      // retira as vírgulas da linha de input
      while (std::getline(iss, token, ',')) {
        no_commas += token + ' ';
      }

      vector<string> value;


      // retira os espacos da linha de input
      iss = std::istringstream(no_commas);
      while(std::getline(iss, token, ' ')){
        value.push_back(token);
      }

      values.push_back(value);
    }

    return values;
  }
public:
  // recebe os valores da entrada e retorna um vetor de instruções 
  // correspondentes.
  static vector<Instruction> parseInstructions(){
    vector<Instruction> instructions;

    vector<string> inputValues = getInputValues();

    for(const string &values: inputValues){
      instructions.push_back(parseInstruction(values));
    }

    
    return instructions;
};

class Interpreter{
private:

  vector<int> r;
  vector<int> data_memory;
  vector<Instruction> instruction_memory;
  map<InstructionName, function<void(const Instruction)>> instruction_func;
  int PC=0, RA=0, SP=0;

public:
  Interpreter(){
    r.assign(32, 0);
    data_memory.assign(2048, 0);

    // atribui uma função para cada nome de instrução.
    instruction_func[ADD] = [this](const Instruction& instruction){ add(instruction); };
    instruction_func[ADDI] = [this](const Instruction& instruction){ addi(instruction); };
    instruction_func[SUB] = [this](const Instruction& instruction){ sub(instruction); };
    instruction_func[SUBI] = [this](const Instruction& instruction){ subi(instruction); };
    instruction_func[MUL] = [this](const Instruction& instruction){ mul(instruction); };
    instruction_func[DIV] = [this](const Instruction& instruction){ div(instruction); };
    instruction_func[BLT] = [this](const Instruction& instruction){ blt(instruction);};
    instruction_func[BGT] = [this](const Instruction& instruction){ bgt(instruction);};
    instruction_func[BEQ] = [this](const Instruction& instruction){ beq(instruction);};
    instruction_func[J] = [this](const Instruction& instruction){ j(instruction);};
    instruction_func[JR] = [this](const Instruction& instruction){ jr(instruction);};
    instruction_func[JAL] = [this](const Instruction& instruction){ jal(instruction);};
    instruction_func[LW] = [this](const Instruction& instruction){ lw(instruction);};
    instruction_func[SW] = [this](const Instruction& instruction){ sw(instruction);};
    instruction_func[MOV] = [this](const Instruction& instruction){ mov(instruction);};
    instruction_func[MOVI] = [this](const Instruction& instruction){ movi(instruction);};
  }

  void setInstructions(const vector<Instruction> &instruction_memory){
    this->instruction_memory = instruction_memory;
  }

  vector<Instruction> getInstructions(){
    return instruction_memory;
  }

  void printMemory(){
    for(int i = 0; i < data_memory.size(); i++){
      clog << "data_memory[" << i << "] = " << data_memory[i] << endl;
    }
    
  }

  void printRegisters(){
    clog << "PC = " << PC << endl;
    for(int i = 0; i < r.size(); i++){
      clog << "r[" << i << "] = " << r[i] << endl;
    }
  }

  bool executeNextCycle(){

    bool executed = executeInstruction();

    return executed == true;
  }

private:

  bool executeInstruction(){
    if(PC >= instruction_memory.size()) return false;

    InstructionName instruction_name = instruction_memory[PC].name;

    instruction_func[instruction_name](instruction_memory[PC]);

    if(instruction_memory[PC].type != DESVIO) PC++;

    return true;
  }

  // ARITMÉTICAS
  void add(const Instruction &instruction) { r[instruction.rd] = r[instruction.rs] + r[instruction.rt]; }

  void addi(const Instruction &instruction) { r[instruction.rd] = r[instruction.rs] + instruction.vimm; }

  void sub(const Instruction &instruction) { r[instruction.rd] = r[instruction.rs] - r[instruction.rt]; }

  void subi(const Instruction &instruction) { r[instruction.rd] = r[instruction.rs] - instruction.vimm; }

  void mul(const Instruction &instruction) { r[instruction.rd] = r[instruction.rs] * r[instruction.rt]; }

  void div(const Instruction &instruction) { r[instruction.rd] = r[instruction.rs] / r[instruction.rt]; }

  // DESVIOS
  void blt(const Instruction &instruction) {

    if(r[instruction.rs] < r[instruction.rt])
      PC = instruction.vimm;
    else
      PC++;
  }

  void bgt(const Instruction &instruction) {

    if(r[instruction.rs] > r[instruction.rt])
      PC = instruction.vimm;
    else
      PC++;
  }

  void beq(const Instruction &instruction) {

    if(r[instruction.rs] == r[instruction.rt])
      PC = instruction.vimm;
    else
      PC++;
  }

  void j(const Instruction &instruction) { PC = instruction.vimm; }

  void jr(const Instruction &instruction) { PC = r[instruction.rd]; }

  void jal(const Instruction &instruction) { RA = PC+1, PC = instruction.vimm; }

  // MEMÓRIA
  void lw(const Instruction &instruction) { r[instruction.rd] = data_memory[instruction.vimm + r[instruction.rs]]; }

  void sw(const Instruction &instruction) { data_memory[instruction.vimm + r[instruction.rt]] = r[instruction.rs]; }

  // MOVIMENTAÇÃO
  void mov(const Instruction &instruction) { r[instruction.rd] = r[instruction.rs]; }

  void movi(const Instruction &instruction) { r[instruction.rd] = instruction.vimm; }

};

int main(){
  Interpreter i;

  i.setInstructions(Parser::parseInstructions());

  while(i.executeNextCycle()){
    i.printMemory();
    i.printRegisters();
  }

}

