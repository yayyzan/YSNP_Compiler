#ifndef ast_context_hpp
#define ast_context_hpp

#include <bitset>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stack>

struct regfile
{

  std::bitset<32> registers{0b00001111111111111111111100011111}; // lsb on the right

  void free_reg(int bitnum)
  { // set reg to 0 (free)
    if (bitnum != -1)
    {
      registers.set(bitnum, 0);
    }
  }
  void occ_reg(int bitnum)
  { // set reg to 1 (occupy)
    if (bitnum != -1)
    {
      registers.set(bitnum, 1);
    }
  }
  int check_reg()
  { // return a free reg
    for (int j = 0; j < 32; j++)
    {
      if (!registers[j])
      {
        return j;
      }
    }
    return -1;
  }
};

struct saved_regfile
{
  std::vector<int> saved;
  std::bitset<32> registers{0b11110000000000111111110111111111}; // lsb on the right

  void free_reg(int bitnum)
  { // set reg to 0 (free)
    if (bitnum != -1)
    {
      registers.set(bitnum, 0);
    }
  }
  void occ_reg(int bitnum)
  { // set reg to 1 (occupy)
    if (bitnum != -1)
    {
      registers.set(bitnum, 1);
    }
  }
  int check_reg()
  { // return a free reg
    for (int j = 0; j < 32; j++)
    {
      if (!registers[j])
      {
        return j;
      }
    }
    return -1;
  }
};

struct func_regfile
{

  std::bitset<32> func_registers{0b11111111111111000000001111111111}; // function register

  void free_all()
  { // set reg to 0 (free)
    func_registers = {0b11111111111111000000001111111111};
  }

  void occ_reg(int bitnum)
  { // set reg to 1 (occupy)
    func_registers.set(bitnum, 1);
  }

  int check_reg()
  { // return a free reg
    for (int j = 0; j < 32; j++)
    {
      if (!func_registers[j])
      {
        return j;
      }
    }
    return -1;
  }
};



struct symbol
{                   // what each entry consists off
  double val;       // for optimizations
  int offset;       // offset from initial stack pointer
  std::string type; // type
};

struct symbol_table
{

  std::unordered_map<std::string, symbol> table; // entry

  void add_symbol(std::string name, double value, int offset, std::string type)
  {
    symbol sym;
    sym.val = value;
    sym.offset = offset;
    sym.type = type;
    table[name] = sym;
  }

  symbol *find_symbol(std::string name)
  {
    auto it = table.find(name);
    if (it != table.end())
    {
      return &(it->second);
    }
    return nullptr;
  }
};

struct symbol_table_stack
{

  std::vector<symbol_table> stack; // stack of entries, as vector to access inner components

  void new_scope()
  {
    symbol_table table;
    stack.push_back(table);
  }

  void leave_scope()
  {
    stack.pop_back();
  }

  void add_symbol(std::string name, double value, int offset, std::string type)
  { // add entry
    if (!stack.empty())
    {
      symbol_table &table = stack.back();
      table.add_symbol(name, value, offset, type);
    }
    else
    {
      std::cerr << "Error: no scope declared in stack" << std::endl;
      exit(1);
    }
  }

  symbol *find_symbol(std::string name)
  { // find entry
    for (auto it = stack.rbegin(); it != stack.rend(); it++)
    {
      symbol *sym = it->find_symbol(name);
      if (sym != nullptr)
        return sym;
    }
    std::cerr << "Error: Unable to find symbol " << name << " in symbol tables" << std::endl;
    return nullptr;
  }

  bool is_global()
  {
    return stack.size() == 1;
  }

  bool check_global(std::string name){
    int i = stack.size();
      for (auto it = stack.rbegin(); it != stack.rend(); it++)
    {
      symbol *sym = it->find_symbol(name);
      if (sym != nullptr)
        return i == 1;
      else{
        i--;
      }
    }
    std::cerr << "Error: Unable to find symbol " << name << " in symbol tables" << std::endl;
    return 0;
  }

};

struct Context
{

  symbol_table_stack stack;
  regfile cregfile;
  func_regfile func_cregfile;
  saved_regfile saved_cregfile;
  std::string return_label;
  std::vector<std::string> break_labels = {};
  std::vector<std::string> cont_labels = {};
  std::vector<std::vector<std::string>> case_labels = {{}};
  int curr_offset = 0;
  int stack_offset = 0;
  std::string make_name(std::string name)
  {
    static int unique = 0;
    return name + std::to_string(unique++);
  }

      void print_symbol_table_stack(symbol_table_stack& stackhehe) {
      std::cout << "-----------------------------------" << std::endl;
      for (int i = stack.stack.size() - 1; i >= 0; i--) {
          std::cout << "Symbol table #" << i + 1 << ":" << std::endl;
          for (auto& entry : stack.stack[i].table) {
              std::cout << "  " << entry.first << ": val=" << entry.second.val
                        << ", offset=" << entry.second.offset << ", type=" << entry.second.type << std::endl;

          }
          std::cout << "-----------------------------------" << std::endl;
      }
      std::cout  << std::endl << std::endl;

  }
  // int stack_size = -12
};

#endif
