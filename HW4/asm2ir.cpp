// ASM2IR: runs a program of our ISA (see ISA.md) in one of three modes:
//   sim  - simple interpreter, no LLVM IR
//   ext  - IR with calls of functions do_<NAME> which emulate instructions
//          (control flow is real IR), run in ExecutionEngine
//   full - IR equivalents of all instructions, run in ExecutionEngine
//
// Usage: asm2ir <sim|ext|full> <program.s> [dump.ll]
//        dump.ll - file for the generated IR (only for ext and full)

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace llvm;

extern "C" {
#include "../HW1/sim.h"
}

const int REG_NUM = 32;
const int MEM_SIZE = 65600; // words

uint32_t REG[REG_NUM];
uint32_t MEM[MEM_SIZE];

void die(const std::string &msg) {
  fprintf(stderr, "[ERROR] %s\n", msg.c_str());
  exit(1);
}

// ---------------------------------------------------------------------
// Functions which emulate the instructions.
// Arguments are register numbers and immediates from the assembler.
// ---------------------------------------------------------------------

void do_ADD(int d, int a, int b) { REG[d] = REG[a] + REG[b]; }
void do_ADDi(int d, int a, int imm) { REG[d] = REG[a] + imm; }
void do_SUB(int d, int a, int b) { REG[d] = REG[a] - REG[b]; }
void do_SUBi(int d, int a, int imm) { REG[d] = REG[a] - imm; }
void do_MUL(int d, int a, int b) { REG[d] = REG[a] * REG[b]; }
void do_MULi(int d, int a, int imm) { REG[d] = REG[a] * imm; }
void do_DIV(int d, int a, int b) { REG[d] = (int32_t)REG[a] / (int32_t)REG[b]; }
void do_DIVi(int d, int a, int imm) { REG[d] = (int32_t)REG[a] / imm; }
void do_REM(int d, int a, int b) { REG[d] = (int32_t)REG[a] % (int32_t)REG[b]; }
void do_REMi(int d, int a, int imm) { REG[d] = (int32_t)REG[a] % imm; }
void do_AND(int d, int a, int b) { REG[d] = REG[a] & REG[b]; }
void do_ANDi(int d, int a, int imm) { REG[d] = REG[a] & imm; }
void do_OR(int d, int a, int b) { REG[d] = REG[a] | REG[b]; }
void do_ORi(int d, int a, int imm) { REG[d] = REG[a] | imm; }
void do_XOR(int d, int a, int b) { REG[d] = REG[a] ^ REG[b]; }
void do_XORi(int d, int a, int imm) { REG[d] = REG[a] ^ imm; }
void do_SHL(int d, int a, int b) { REG[d] = REG[a] << REG[b]; }
void do_SHLi(int d, int a, int imm) { REG[d] = REG[a] << imm; }
void do_SHR(int d, int a, int b) { REG[d] = (int32_t)REG[a] >> REG[b]; }
void do_SHRi(int d, int a, int imm) { REG[d] = (int32_t)REG[a] >> imm; }

void do_MOV(int d, int s) { REG[d] = REG[s]; }
void do_MOVi(int d, int imm) { REG[d] = imm; }

uint32_t *cell(int base, int imm) {
  uint32_t addr = REG[base] + imm;
  if (addr >= MEM_SIZE)
    die("memory address is out of range: " + std::to_string(addr));
  return &MEM[addr];
}

void do_LD(int d, int a, int imm) { REG[d] = *cell(a, imm); }
void do_LDADD(int d, int a, int imm) { REG[d] += *cell(a, imm); }
void do_ST(int s, int a, int imm) { *cell(a, imm) = REG[s]; }

// d = a + (((b - a) * f) >> 16)
void do_LERP(int d, int a, int b, int f) {
  int32_t diff = (int32_t)((REG[b] - REG[a]) * REG[f]);
  REG[d] = REG[a] + (diff >> 16);
}

// d = (a >> b) & 255
void do_EXTB(int d, int a, int b) { REG[d] = (REG[a] >> REG[b]) & 255; }

// d = d | (a << b)
void do_ORSHL(int d, int a, int b) { REG[d] |= REG[a] << REG[b]; }

void do_PUT_PIXEL(int x, int y, int c) { simPutPixel(REG[x], REG[y], REG[c]); }
void do_RAND(int d) { REG[d] = simRand(); }
void do_FLUSH() { simFlush(); }

// ---------------------------------------------------------------------
// Instruction set
// ---------------------------------------------------------------------

// The order is the same as in OPS
enum Op {
  OP_ADD, OP_ADDi, OP_SUB, OP_SUBi, OP_MUL, OP_MULi, OP_DIV, OP_DIVi,
  OP_REM, OP_REMi, OP_AND, OP_ANDi, OP_OR, OP_ORi, OP_XOR, OP_XORi,
  OP_SHL, OP_SHLi, OP_SHR, OP_SHRi,
  OP_MOV, OP_MOVi,
  OP_LD, OP_LDADD, OP_ST,
  OP_LERP, OP_EXTB, OP_ORSHL,
  OP_PUT_PIXEL, OP_RAND, OP_FLUSH,
  // control flow, is not emulated
  OP_BEQ, OP_BNE, OP_BLT, OP_BGE, OP_BEQi, OP_BNEi, OP_BLTi, OP_BGEi,
  OP_JMP, OP_EXIT,
  OP_NUM
};

// fmt: r - register, i - immediate, l - label
struct OpInfo {
  const char *name;
  const char *fmt;
  void *emu; // do_<NAME>
};

const OpInfo OPS[] = {
    {"ADD", "rrr", (void *)do_ADD},     {"ADDi", "rri", (void *)do_ADDi},
    {"SUB", "rrr", (void *)do_SUB},     {"SUBi", "rri", (void *)do_SUBi},
    {"MUL", "rrr", (void *)do_MUL},     {"MULi", "rri", (void *)do_MULi},
    {"DIV", "rrr", (void *)do_DIV},     {"DIVi", "rri", (void *)do_DIVi},
    {"REM", "rrr", (void *)do_REM},     {"REMi", "rri", (void *)do_REMi},
    {"AND", "rrr", (void *)do_AND},     {"ANDi", "rri", (void *)do_ANDi},
    {"OR", "rrr", (void *)do_OR},       {"ORi", "rri", (void *)do_ORi},
    {"XOR", "rrr", (void *)do_XOR},     {"XORi", "rri", (void *)do_XORi},
    {"SHL", "rrr", (void *)do_SHL},     {"SHLi", "rri", (void *)do_SHLi},
    {"SHR", "rrr", (void *)do_SHR},     {"SHRi", "rri", (void *)do_SHRi},
    {"MOV", "rr", (void *)do_MOV},      {"MOVi", "ri", (void *)do_MOVi},
    {"LD", "rri", (void *)do_LD},       {"LDADD", "rri", (void *)do_LDADD},
    {"ST", "rri", (void *)do_ST},       {"LERP", "rrrr", (void *)do_LERP},
    {"EXTB", "rrr", (void *)do_EXTB},   {"ORSHL", "rrr", (void *)do_ORSHL},
    {"PUT_PIXEL", "rrr", (void *)do_PUT_PIXEL},
    {"RAND", "r", (void *)do_RAND},     {"FLUSH", "", (void *)do_FLUSH},
    {"BEQ", "rrl", nullptr},            {"BNE", "rrl", nullptr},
    {"BLT", "rrl", nullptr},            {"BGE", "rrl", nullptr},
    {"BEQi", "ril", nullptr},           {"BNEi", "ril", nullptr},
    {"BLTi", "ril", nullptr},           {"BGEi", "ril", nullptr},
    {"JMP", "l", nullptr},              {"EXIT", "", nullptr},
};

static_assert(sizeof(OPS) / sizeof(OPS[0]) == OP_NUM, "OPS and Op differ");

bool isFlow(int op) { return op >= OP_BEQ; }
bool isCondBranch(int op) { return op >= OP_BEQ && op <= OP_BGEi; }
// the second operand of a branch is a number
bool isImmBranch(int op) { return op >= OP_BEQi && op <= OP_BGEi; }

enum { COND_EQ, COND_NE, COND_LT, COND_GE };

int findOp(const std::string &name) {
  for (int i = 0; i < OP_NUM; i++)
    if (name == OPS[i].name)
      return i;
  return -1;
}

// ---------------------------------------------------------------------
// Assembler
// ---------------------------------------------------------------------

struct Instr {
  int op;
  int arg[4];
  int n;    // number of arguments
  int cond; // for branches
  std::string label;
  int line;
};

std::vector<Instr> prog;
std::map<std::string, int> labels;  // label -> index of the instruction
std::map<int, std::string> labelAt; // index -> first label there

int parseReg(const std::string &s, int line) {
  if (s.size() > 1 && s[0] == 'x') {
    char *end;
    long n = strtol(s.c_str() + 1, &end, 10);
    if (*end == 0 && n >= 0 && n < REG_NUM)
      return n;
  }
  die("line " + std::to_string(line) + ": bad register " + s);
  return 0;
}

int parseImm(const std::string &s, int line) {
  char *end;
  long long n = strtoll(s.c_str(), &end, 0);
  if (end == s.c_str() || *end != 0)
    die("line " + std::to_string(line) + ": bad number " + s);
  return (int32_t)(uint32_t)n;
}

void parseAsm(const char *path) {
  std::ifstream input(path);
  if (!input.is_open())
    die(std::string("can't open ") + path);

  std::string text;
  int line = 0;
  while (std::getline(input, text)) {
    line++;
    // remove the comment and commas
    text = text.substr(0, text.find('#'));
    for (char &c : text)
      if (c == ',')
        c = ' ';
    std::istringstream words(text);
    std::vector<std::string> tok;
    std::string w;
    while (words >> w)
      tok.push_back(w);
    if (tok.empty())
      continue;

    int op = findOp(tok[0]);
    if (op < 0) {
      // not an instruction, so it is a label
      if (tok.size() != 1)
        die("line " + std::to_string(line) + ": unknown instruction " + tok[0]);
      if (labels.count(tok[0]))
        die("line " + std::to_string(line) + ": label " + tok[0] + " again");
      labels[tok[0]] = prog.size();
      if (!labelAt.count(prog.size()))
        labelAt[prog.size()] = tok[0];
      continue;
    }

    const char *fmt = OPS[op].fmt;
    int n = strlen(fmt);
    if ((int)tok.size() - 1 != n)
      die("line " + std::to_string(line) + ": " + tok[0] + " needs " +
          std::to_string(n) + " arguments");
    Instr in = {op, {0, 0, 0, 0}, n, 0, "", line};
    for (int k = 0; k < n; k++) {
      if (fmt[k] == 'r')
        in.arg[k] = parseReg(tok[k + 1], line);
      else if (fmt[k] == 'i')
        in.arg[k] = parseImm(tok[k + 1], line);
      else
        in.label = tok[k + 1];
    }
    if (isCondBranch(op))
      in.cond = (op - OP_BEQ) % 4; // EQ, NE, LT, GE
    prog.push_back(in);
  }

  if (prog.empty())
    die("empty program");
  if (prog.back().op != OP_JMP && prog.back().op != OP_EXIT)
    die("the last instruction must be JMP or EXIT");

  // replace labels with instruction indexes
  for (Instr &in : prog) {
    if (in.label.empty())
      continue;
    if (!labels.count(in.label))
      die("line " + std::to_string(in.line) + ": no label " + in.label);
    if (labels[in.label] >= (int)prog.size())
      die("label " + in.label + " is after the last instruction");
    in.arg[in.n - 1] = labels[in.label];
  }
}

// ---------------------------------------------------------------------
// Mode sim
// ---------------------------------------------------------------------

void callEmu(void *f, int n, const int *a) {
  if (n == 0)
    ((void (*)())f)();
  else if (n == 1)
    ((void (*)(int))f)(a[0]);
  else if (n == 2)
    ((void (*)(int, int))f)(a[0], a[1]);
  else if (n == 3)
    ((void (*)(int, int, int))f)(a[0], a[1], a[2]);
  else
    ((void (*)(int, int, int, int))f)(a[0], a[1], a[2], a[3]);
}

bool branchTaken(const Instr &in) {
  int32_t x = REG[in.arg[0]];
  int32_t y = isImmBranch(in.op) ? in.arg[1] : REG[in.arg[1]];
  if (in.cond == COND_EQ)
    return x == y;
  if (in.cond == COND_NE)
    return x != y;
  if (in.cond == COND_LT)
    return x < y;
  return x >= y;
}

void runSim() {
  size_t pc = 0;
  while (true) {
    const Instr &in = prog[pc];
    if (in.op == OP_EXIT)
      return;
    if (in.op == OP_JMP)
      pc = in.arg[0];
    else if (isCondBranch(in.op))
      pc = branchTaken(in) ? in.arg[2] : pc + 1;
    else {
      callEmu(OPS[in.op].emu, in.n, in.arg);
      pc++;
    }
  }
}

// ---------------------------------------------------------------------
// Modes ext and full: IR generation
// ---------------------------------------------------------------------

LLVMContext Ctx;
IRBuilder<> B(Ctx);
Module *M;
Type *I32;
ArrayType *RegType;
ArrayType *MemType;
GlobalVariable *RegFile;
GlobalVariable *Mem;
FunctionCallee PutPixelFunc, RandFunc, FlushFunc;

Value *getReg(int n) {
  Value *p = B.CreateConstGEP2_32(RegType, RegFile, 0, n);
  return B.CreateLoad(I32, p);
}

void setReg(int n, Value *v) {
  B.CreateStore(v, B.CreateConstGEP2_32(RegType, RegFile, 0, n));
}

// pointer to MEM[reg[base] + imm]
Value *memPtr(int base, int imm) {
  Value *addr = B.CreateAdd(getReg(base), B.getInt32(imm));
  Value *idx = B.CreateZExt(addr, B.getInt64Ty());
  return B.CreateInBoundsGEP(MemType, Mem, {B.getInt64(0), idx});
}

Value *aluResult(int op, Value *x, Value *y) {
  if (op == OP_ADD || op == OP_ADDi)
    return B.CreateAdd(x, y);
  if (op == OP_SUB || op == OP_SUBi)
    return B.CreateSub(x, y);
  if (op == OP_MUL || op == OP_MULi)
    return B.CreateMul(x, y);
  if (op == OP_DIV || op == OP_DIVi)
    return B.CreateSDiv(x, y);
  if (op == OP_REM || op == OP_REMi)
    return B.CreateSRem(x, y);
  if (op == OP_AND || op == OP_ANDi)
    return B.CreateAnd(x, y);
  if (op == OP_OR || op == OP_ORi)
    return B.CreateOr(x, y);
  if (op == OP_XOR || op == OP_XORi)
    return B.CreateXor(x, y);
  if (op == OP_SHL || op == OP_SHLi)
    return B.CreateShl(x, y);
  return B.CreateAShr(x, y); // SHR, SHRi
}

// IR equivalent of an instruction (mode full)
void emitFull(const Instr &in) {
  const int *a = in.arg;
  if (in.op <= OP_SHRi) {
    Value *y = OPS[in.op].fmt[2] == 'i' ? B.getInt32(a[2]) : getReg(a[2]);
    setReg(a[0], aluResult(in.op, getReg(a[1]), y));
  } else if (in.op == OP_MOV) {
    setReg(a[0], getReg(a[1]));
  } else if (in.op == OP_MOVi) {
    setReg(a[0], B.getInt32(a[1]));
  } else if (in.op == OP_LD) {
    setReg(a[0], B.CreateLoad(I32, memPtr(a[1], a[2])));
  } else if (in.op == OP_LDADD) {
    Value *mem = B.CreateLoad(I32, memPtr(a[1], a[2]));
    setReg(a[0], B.CreateAdd(getReg(a[0]), mem));
  } else if (in.op == OP_ST) {
    B.CreateStore(getReg(a[0]), memPtr(a[1], a[2]));
  } else if (in.op == OP_LERP) {
    Value *x = getReg(a[1]);
    Value *diff = B.CreateMul(B.CreateSub(getReg(a[2]), x), getReg(a[3]));
    setReg(a[0], B.CreateAdd(x, B.CreateAShr(diff, 16)));
  } else if (in.op == OP_EXTB) {
    Value *shifted = B.CreateLShr(getReg(a[1]), getReg(a[2]));
    setReg(a[0], B.CreateAnd(shifted, 255));
  } else if (in.op == OP_ORSHL) {
    Value *shifted = B.CreateShl(getReg(a[1]), getReg(a[2]));
    setReg(a[0], B.CreateOr(getReg(a[0]), shifted));
  } else if (in.op == OP_PUT_PIXEL) {
    B.CreateCall(PutPixelFunc, {getReg(a[0]), getReg(a[1]), getReg(a[2])});
  } else if (in.op == OP_RAND) {
    setReg(a[0], B.CreateCall(RandFunc));
  } else if (in.op == OP_FLUSH) {
    B.CreateCall(FlushFunc);
  }
}

// call of the emulating function (mode ext)
void emitExt(const Instr &in) {
  std::vector<Type *> types(in.n, I32);
  FunctionType *type = FunctionType::get(B.getVoidTy(), types, false);
  FunctionCallee func =
      M->getOrInsertFunction(std::string("do_") + OPS[in.op].name, type);
  std::vector<Value *> args;
  for (int k = 0; k < in.n; k++)
    args.push_back(B.getInt32(in.arg[k]));
  B.CreateCall(func, args);
}

void emitFlow(const Instr &in, size_t pc, std::vector<BasicBlock *> &block) {
  if (in.op == OP_EXIT) {
    B.CreateRetVoid();
  } else if (in.op == OP_JMP) {
    B.CreateBr(block[in.arg[0]]);
  } else {
    Value *x = getReg(in.arg[0]);
    Value *y = isImmBranch(in.op) ? B.getInt32(in.arg[1]) : getReg(in.arg[1]);
    Value *cond;
    if (in.cond == COND_EQ)
      cond = B.CreateICmpEQ(x, y);
    else if (in.cond == COND_NE)
      cond = B.CreateICmpNE(x, y);
    else if (in.cond == COND_LT)
      cond = B.CreateICmpSLT(x, y);
    else
      cond = B.CreateICmpSGE(x, y);
    // if the condition is false we go to the next instruction
    B.CreateCondBr(cond, block[in.arg[2]], block[pc + 1]);
  }
}

Function *genIR(bool full) {
  M = new Module("top", Ctx);
  I32 = B.getInt32Ty();
  RegType = ArrayType::get(I32, REG_NUM);
  MemType = ArrayType::get(I32, MEM_SIZE);

  if (full) {
    // registers and memory are in the module
    RegFile = new GlobalVariable(*M, RegType, false,
                                 GlobalValue::PrivateLinkage,
                                 ConstantAggregateZero::get(RegType), "regFile");
    Mem = new GlobalVariable(*M, MemType, false, GlobalValue::PrivateLinkage,
                             ConstantAggregateZero::get(MemType), "mem");
    PutPixelFunc = M->getOrInsertFunction(
        "simPutPixel", FunctionType::get(B.getVoidTy(), {I32, I32, I32}, false));
    RandFunc = M->getOrInsertFunction("simRand", FunctionType::get(I32, false));
    FlushFunc = M->getOrInsertFunction(
        "simFlush", FunctionType::get(B.getVoidTy(), false));
  } else {
    // registers are used by do_* functions, IR reads them only for branches
    RegFile = new GlobalVariable(*M, RegType, false,
                                 GlobalValue::ExternalLinkage, nullptr, "regFile");
  }

  Function *app = Function::Create(FunctionType::get(B.getVoidTy(), false),
                                   Function::ExternalLinkage, "app", M);
  BasicBlock *start = BasicBlock::Create(Ctx, "start", app);

  // A basic block starts at the first instruction, at every label and after
  // every branch
  std::vector<bool> leader(prog.size(), false);
  leader[0] = true;
  for (size_t i = 0; i < prog.size(); i++) {
    if (labelAt.count(i))
      leader[i] = true;
    if (isFlow(prog[i].op) && i + 1 < prog.size())
      leader[i + 1] = true;
  }
  std::vector<BasicBlock *> block(prog.size(), nullptr);
  for (size_t i = 0; i < prog.size(); i++) {
    if (!leader[i])
      continue;
    std::string name = labelAt.count(i) ? labelAt[i] : "bb" + std::to_string(i);
    block[i] = BasicBlock::Create(Ctx, name, app);
  }

  B.SetInsertPoint(start);
  B.CreateBr(block[0]);
  bool open = false; // the block has no terminator yet
  for (size_t i = 0; i < prog.size(); i++) {
    if (block[i]) {
      if (open)
        B.CreateBr(block[i]);
      B.SetInsertPoint(block[i]);
      open = true;
    }
    const Instr &in = prog[i];
    if (isFlow(in.op)) {
      emitFlow(in, i, block);
      open = false;
    } else if (full) {
      emitFull(in);
    } else {
      emitExt(in);
    }
  }

  if (verifyModule(*M, &errs()))
    die("wrong IR was generated");
  return app;
}

// address of a function from this program
void *hostFunction(const std::string &name) {
  if (name == "simPutPixel")
    return (void *)simPutPixel;
  if (name == "simRand")
    return (void *)simRand;
  if (name == "simFlush")
    return (void *)simFlush;
  if (name.compare(0, 3, "do_") == 0) {
    int op = findOp(name.substr(3));
    if (op >= 0 && OPS[op].emu)
      return OPS[op].emu;
  }
  die("unknown function " + name);
  return nullptr;
}

void runIR(bool full, const char *dumpPath) {
  Function *app = genIR(full);
  if (dumpPath) {
    std::error_code ec;
    raw_fd_ostream out(dumpPath, ec);
    if (ec)
      die(std::string("can't write ") + dumpPath);
    M->print(out, nullptr);
  }

  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  std::string err;
  ExecutionEngine *ee =
      EngineBuilder(std::unique_ptr<Module>(M)).setErrorStr(&err).create();
  if (!ee)
    die("can't create ExecutionEngine: " + err);

  for (Function &f : *M)
    if (f.isDeclaration())
      ee->addGlobalMapping(&f, hostFunction(std::string(f.getName())));
  if (!full)
    ee->addGlobalMapping(RegFile, (void *)REG);
  ee->finalizeObject();

  void (*appFunc)() = (void (*)())ee->getFunctionAddress(app->getName().str());
  simInit();
  appFunc();
  simExit();
}

int main(int argc, char *argv[]) {
  if (argc < 3 || argc > 4)
    die("usage: asm2ir <sim|ext|full> <program.s> [dump.ll]");
  std::string mode = argv[1];
  if (mode != "sim" && mode != "ext" && mode != "full")
    die("mode must be sim, ext or full");

  parseAsm(argv[2]);
  fprintf(stderr, "[asm2ir] %s: %zu instructions, mode %s\n", argv[2],
          prog.size(), mode.c_str());

  if (mode == "sim") {
    simInit();
    runSim();
    simExit();
  } else {
    runIR(mode == "full", argc == 4 ? argv[3] : nullptr);
  }
  return 0;
}
