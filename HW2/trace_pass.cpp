// Pass for tracing "User <- Operand" of IR instructions.
// Before every instruction (except phi) we insert calls logUse(user, operand)
// for each operand that is an instruction. Applied to app.c after -O2.
//
// Star task: if the operand is a phi, we log the operands of this phi instead
// (shl <- phi  =>  shl <- add, shl <- sub). TRACE_EXPAND_PHI=0 turns it off.
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include <cstdlib>
#include <map>
#include <vector>
using namespace llvm;

// clang can't pass options to a plugin, so use an environment variable
bool expandPhi() {
  char *s = getenv("TRACE_EXPAND_PHI");
  if (s && atoi(s) == 0)
    return false;
  return true;
}

// Collect instructions that are used through the value V.
// For a phi go through its incoming values (phi can use another phi).
void collect(Value *V, std::vector<Instruction *> &res,
             std::vector<PHINode *> &visited) {
  if (auto *phi = dyn_cast<PHINode>(V)) {
    if (!expandPhi()) {
      res.push_back(phi);
      return;
    }
    for (PHINode *p : visited)
      if (p == phi)
        return;
    visited.push_back(phi);
    for (Value *in : phi->incoming_values())
      collect(in, res, visited);
  } else if (auto *inst = dyn_cast<Instruction>(V)) {
    // the same instruction can come from several phi edges, log it once
    for (Instruction *i : res)
      if (i == inst)
        return;
    res.push_back(inst);
  }
}

struct TracePass : public PassInfoMixin<TracePass> {
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    Function *app = M.getFunction("app");
    if (!app || app->isDeclaration())
      return PreservedAnalyses::all();

    LLVMContext &Ctx = M.getContext();
    IRBuilder<> builder(Ctx);
    Type *voidTy = Type::getVoidTy(Ctx);
    Type *int32Ty = Type::getInt32Ty(Ctx);
    Type *ptrTy = PointerType::getUnqual(Ctx);

    FunctionCallee logUse = M.getOrInsertFunction(
        "logUse", FunctionType::get(voidTy, {int32Ty, int32Ty}, false));
    FunctionCallee logFrame =
        M.getOrInsertFunction("logFrame", FunctionType::get(voidTy, false));
    FunctionCallee registerOp = M.getOrInsertFunction(
        "registerOp", FunctionType::get(voidTy, {int32Ty, ptrTy}, false));

    // opcode -> name, to print names in the statistics
    std::map<unsigned, std::string> names;

    for (Function &F : M) {
      if (F.isDeclaration())
        continue;

      // collect first, because we insert new instructions while going
      std::vector<Instruction *> users;
      for (BasicBlock &BB : F)
        for (Instruction &I : BB)
          if (!isa<PHINode>(I))
            users.push_back(&I);

      for (Instruction *user : users) {
        builder.SetInsertPoint(user);

        // one frame is finished before simFlush
        if (auto *call = dyn_cast<CallInst>(user)) {
          Function *callee = call->getCalledFunction();
          if (callee && callee->getName() == "simFlush")
            builder.CreateCall(logFrame);
        }

        for (Value *operand : user->operands()) {
          std::vector<Instruction *> ops;
          std::vector<PHINode *> visited;
          collect(operand, ops, visited);
          for (Instruction *op : ops) {
            names[user->getOpcode()] = user->getOpcodeName();
            names[op->getOpcode()] = op->getOpcodeName();
            Value *args[] = {builder.getInt32(user->getOpcode()),
                             builder.getInt32(op->getOpcode())};
            builder.CreateCall(logUse, args);
          }
        }
      }
    }

    // register names at the start of app()
    builder.SetInsertPoint(&app->getEntryBlock().front());
    for (auto &p : names) {
      Value *args[] = {builder.getInt32(p.first),
                       builder.CreateGlobalString(p.second)};
      builder.CreateCall(registerOp, args);
    }

    verifyModule(M, &errs());
    return PreservedAnalyses::none();
  }
};

PassPluginLibraryInfo getPassPluginInfo() {
  const auto callback = [](PassBuilder &PB) {
    // run at the end of the pipeline, so we see the final -O2 IR
    PB.registerOptimizerLastEPCallback(
        [](ModulePassManager &MPM, OptimizationLevel, ThinOrFullLTOPhase) {
          MPM.addPass(TracePass{});
        });
  };
  return {LLVM_PLUGIN_API_VERSION, "TracePass", "0.1.0", callback};
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getPassPluginInfo();
}
