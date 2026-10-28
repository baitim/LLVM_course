// IR generator for the logic module of HW1 (app.c)

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include <cstring>
using namespace llvm;

extern "C" {
#include "../HW1/sim.h"
}

// define i32 @heatColor(i32 %0)
static void genHeatColor(Module *module, IRBuilder<> &builder) {
  LLVMContext &context = module->getContext();
  Type *int32Type = builder.getInt32Ty();
  Type *int64Type = builder.getInt64Ty();

  // @__const.heatColor.pal = private unnamed_addr constant [7 x i32]
  //   [i32 -16777212, i32 -14152876, i32 -10152594, i32 -6346141,
  //    i32 -2865086, i32 -422902, i32 -196700], align 16
  ArrayType *palType = ArrayType::get(int32Type, 7);
  Constant *palInit = ConstantDataArray::get(
      context, ArrayRef<uint32_t>{0xFF000004, 0xFF280B54, 0xFF65156E,
                                  0xFF9F2A63, 0xFFD44842, 0xFFF98C0A,
                                  0xFFFCFFA4});
  GlobalVariable *pal =
      new GlobalVariable(*module, palType, true, GlobalValue::PrivateLinkage,
                         palInit, "__const.heatColor.pal");
  pal->setUnnamedAddr(GlobalValue::UnnamedAddr::Global);
  pal->setAlignment(Align(16));

  FunctionType *funcType = FunctionType::get(int32Type, {int32Type}, false);
  Function *func =
      Function::Create(funcType, Function::ExternalLinkage, "heatColor", module);
  Value *val0 = func->getArg(0);

  // BasicBlocks:
  BasicBlock *BB1 = BasicBlock::Create(context, "", func);
  BasicBlock *BB14 = BasicBlock::Create(context, "", func);
  BasicBlock *BB15 = BasicBlock::Create(context, "", func);

  // 1:
  builder.SetInsertPoint(BB1);
  // %2 = mul nsw i32 %0, 6
  Value *val2 = builder.CreateMul(val0, builder.getInt32(6), "", false, true);
  // %3 = ashr i32 %2, 16
  Value *val3 = builder.CreateAShr(val2, 16);
  // %4 = call i32 @llvm.smin.i32(i32 %3, i32 5)
  Value *val4 =
      builder.CreateBinaryIntrinsic(Intrinsic::smin, val3, builder.getInt32(5));
  // %5 = mul i32 %4, -65536
  Value *val5 = builder.CreateMul(val4, builder.getInt32(-65536));
  // %6 = add i32 %5, %2
  Value *val6 = builder.CreateAdd(val5, val2);
  // %7 = sext i32 %4 to i64
  Value *val7 = builder.CreateSExt(val4, int64Type);
  // %8 = getelementptr inbounds [7 x i32], ptr @__const.heatColor.pal, i64 0, i64 %7
  Value *val8 =
      builder.CreateInBoundsGEP(palType, pal, {builder.getInt64(0), val7});
  // %9 = load i32, ptr %8, align 4
  Value *val9 = builder.CreateLoad(int32Type, val8);
  // %10 = add nsw i32 %4, 1
  Value *val10 = builder.CreateAdd(val4, builder.getInt32(1), "", false, true);
  // %11 = sext i32 %10 to i64
  Value *val11 = builder.CreateSExt(val10, int64Type);
  // %12 = getelementptr inbounds [7 x i32], ptr @__const.heatColor.pal, i64 0, i64 %11
  Value *val12 =
      builder.CreateInBoundsGEP(palType, pal, {builder.getInt64(0), val11});
  // %13 = load i32, ptr %12, align 4
  Value *val13 = builder.CreateLoad(int32Type, val12);
  // br label %15
  builder.CreateBr(BB15);

  // 15:                                               ; preds = %1, %15
  builder.SetInsertPoint(BB15);
  // %16 = phi i32 [ -16777216, %1 ], [ %27, %15 ]
  PHINode *val16 = builder.CreatePHI(int32Type, 2);
  // %17 = phi i32 [ 0, %1 ], [ %28, %15 ]
  PHINode *val17 = builder.CreatePHI(int32Type, 2);
  // %18 = ashr i32 %9, %17
  Value *val18 = builder.CreateAShr(val9, val17);
  // %19 = and i32 %18, 255
  Value *val19 = builder.CreateAnd(val18, 255);
  // %20 = ashr i32 %13, %17
  Value *val20 = builder.CreateAShr(val13, val17);
  // %21 = and i32 %20, 255
  Value *val21 = builder.CreateAnd(val20, 255);
  // %22 = sub nsw i32 %21, %19
  Value *val22 = builder.CreateSub(val21, val19, "", false, true);
  // %23 = mul nsw i32 %22, %6
  Value *val23 = builder.CreateMul(val22, val6, "", false, true);
  // %24 = ashr i32 %23, 16
  Value *val24 = builder.CreateAShr(val23, 16);
  // %25 = add nsw i32 %24, %19
  Value *val25 = builder.CreateAdd(val24, val19, "", false, true);
  // %26 = shl i32 %25, %17
  Value *val26 = builder.CreateShl(val25, val17);
  // %27 = or i32 %26, %16
  Value *val27 = builder.CreateOr(val26, val16);
  // %28 = add nuw nsw i32 %17, 8
  Value *val28 = builder.CreateAdd(val17, builder.getInt32(8), "", true, true);
  // %29 = icmp ult i32 %17, 16
  Value *val29 = builder.CreateICmpULT(val17, builder.getInt32(16));
  // br i1 %29, label %15, label %14
  builder.CreateCondBr(val29, BB15, BB14);

  // 14:                                               ; preds = %15
  builder.SetInsertPoint(BB14);
  // ret i32 %27
  builder.CreateRet(val27);

  // Link PHI nodes
  // %16 = phi i32 [ -16777216, %1 ], [ %27, %15 ]
  val16->addIncoming(builder.getInt32(0xFF000000), BB1);
  val16->addIncoming(val27, BB15);
  // %17 = phi i32 [ 0, %1 ], [ %28, %15 ]
  val17->addIncoming(builder.getInt32(0), BB1);
  val17->addIncoming(val28, BB15);
}

// define void @diffuseStep(ptr %0, ptr %1)
static void genDiffuseStep(Module *module, IRBuilder<> &builder) {
  LLVMContext &context = module->getContext();
  Type *voidType = builder.getVoidTy();
  Type *int32Type = builder.getInt32Ty();
  Type *int64Type = builder.getInt64Ty();
  Type *ptrType = PointerType::get(context, 0);

  FunctionType *funcType =
      FunctionType::get(voidType, {ptrType, ptrType}, false);
  Function *func = Function::Create(funcType, Function::ExternalLinkage,
                                    "diffuseStep", module);
  Value *val0 = func->getArg(0);
  Value *val1 = func->getArg(1);

  // BasicBlocks:
  BasicBlock *BB2 = BasicBlock::Create(context, "", func);
  BasicBlock *BB3 = BasicBlock::Create(context, "", func);
  BasicBlock *BB6 = BasicBlock::Create(context, "", func);
  BasicBlock *BB7 = BasicBlock::Create(context, "", func);
  BasicBlock *BB10 = BasicBlock::Create(context, "", func);

  // 2:
  builder.SetInsertPoint(BB2);
  // br label %3
  builder.CreateBr(BB3);

  // 3:                                                ; preds = %2, %7
  builder.SetInsertPoint(BB3);
  // %4 = phi i64 [ 1, %2 ], [ %8, %7 ]
  PHINode *val4 = builder.CreatePHI(int64Type, 2);
  // %5 = shl nsw i64 %4, 8
  Value *val5 = builder.CreateShl(val4, 8, "", false, true);
  // br label %10
  builder.CreateBr(BB10);

  // 6:                                                ; preds = %7
  builder.SetInsertPoint(BB6);
  // ret void
  builder.CreateRetVoid();

  // 7:                                                ; preds = %10
  builder.SetInsertPoint(BB7);
  // %8 = add nuw nsw i64 %4, 1
  Value *val8 = builder.CreateAdd(val4, builder.getInt64(1), "", true, true);
  // %9 = icmp eq i64 %8, 127
  Value *val9 = builder.CreateICmpEQ(val8, builder.getInt64(127));
  // br i1 %9, label %6, label %3
  builder.CreateCondBr(val9, BB6, BB3);

  // 10:                                               ; preds = %3, %10
  builder.SetInsertPoint(BB10);
  // %11 = phi i64 [ 1, %3 ], [ %33, %10 ]
  PHINode *val11 = builder.CreatePHI(int64Type, 2);
  // %12 = add nuw nsw i64 %11, %5
  Value *val12 = builder.CreateAdd(val11, val5, "", true, true);
  // %13 = getelementptr inbounds i32, ptr %0, i64 %12
  Value *val13 = builder.CreateInBoundsGEP(int32Type, val0, val12);
  // %14 = load i32, ptr %13, align 4
  Value *val14 = builder.CreateLoad(int32Type, val13);
  // %15 = add nsw i64 %12, -1
  Value *val15 = builder.CreateAdd(val12, builder.getInt64(-1), "", false, true);
  // %16 = getelementptr inbounds i32, ptr %0, i64 %15
  Value *val16 = builder.CreateInBoundsGEP(int32Type, val0, val15);
  // %17 = load i32, ptr %16, align 4
  Value *val17 = builder.CreateLoad(int32Type, val16);
  // %18 = add nsw i32 %17, %14
  Value *val18 = builder.CreateAdd(val17, val14, "", false, true);
  // %19 = add nuw nsw i64 %12, 1
  Value *val19 = builder.CreateAdd(val12, builder.getInt64(1), "", true, true);
  // %20 = getelementptr inbounds i32, ptr %0, i64 %19
  Value *val20 = builder.CreateInBoundsGEP(int32Type, val0, val19);
  // %21 = load i32, ptr %20, align 4
  Value *val21 = builder.CreateLoad(int32Type, val20);
  // %22 = add nsw i32 %18, %21
  Value *val22 = builder.CreateAdd(val18, val21, "", false, true);
  // %23 = add nsw i64 %12, -256
  Value *val23 =
      builder.CreateAdd(val12, builder.getInt64(-256), "", false, true);
  // %24 = getelementptr inbounds i32, ptr %0, i64 %23
  Value *val24 = builder.CreateInBoundsGEP(int32Type, val0, val23);
  // %25 = load i32, ptr %24, align 4
  Value *val25 = builder.CreateLoad(int32Type, val24);
  // %26 = add nsw i32 %22, %25
  Value *val26 = builder.CreateAdd(val22, val25, "", false, true);
  // %27 = add nuw nsw i64 %12, 256
  Value *val27 = builder.CreateAdd(val12, builder.getInt64(256), "", true, true);
  // %28 = getelementptr inbounds i32, ptr %0, i64 %27
  Value *val28 = builder.CreateInBoundsGEP(int32Type, val0, val27);
  // %29 = load i32, ptr %28, align 4
  Value *val29 = builder.CreateLoad(int32Type, val28);
  // %30 = add nsw i32 %26, %29
  Value *val30 = builder.CreateAdd(val26, val29, "", false, true);
  // %31 = sdiv i32 %30, 5
  Value *val31 = builder.CreateSDiv(val30, builder.getInt32(5));
  // %32 = getelementptr inbounds i32, ptr %1, i64 %12
  Value *val32 = builder.CreateInBoundsGEP(int32Type, val1, val12);
  // store i32 %31, ptr %32, align 4
  builder.CreateStore(val31, val32);
  // %33 = add nuw nsw i64 %11, 1
  Value *val33 = builder.CreateAdd(val11, builder.getInt64(1), "", true, true);
  // %34 = icmp eq i64 %33, 255
  Value *val34 = builder.CreateICmpEQ(val33, builder.getInt64(255));
  // br i1 %34, label %7, label %10
  builder.CreateCondBr(val34, BB7, BB10);

  // Link PHI nodes
  // %4 = phi i64 [ 1, %2 ], [ %8, %7 ]
  val4->addIncoming(builder.getInt64(1), BB2);
  val4->addIncoming(val8, BB7);
  // %11 = phi i64 [ 1, %3 ], [ %33, %10 ]
  val11->addIncoming(builder.getInt64(1), BB3);
  val11->addIncoming(val33, BB10);

}

// define void @applyDisc(ptr %0, i32 %1, i32 %2, i32 %3, i32 %4)
static void genApplyDisc(Module *module, IRBuilder<> &builder) {
  LLVMContext &context = module->getContext();
  Type *voidType = builder.getVoidTy();
  Type *int32Type = builder.getInt32Ty();
  Type *int64Type = builder.getInt64Ty();
  Type *ptrType = PointerType::get(context, 0);

  FunctionType *funcType = FunctionType::get(
      voidType, {ptrType, int32Type, int32Type, int32Type, int32Type}, false);
  Function *func = Function::Create(funcType, Function::ExternalLinkage,
                                    "applyDisc", module);
  Value *val0 = func->getArg(0);
  Value *val1 = func->getArg(1);
  Value *val2 = func->getArg(2);
  Value *val3 = func->getArg(3);
  Value *val4 = func->getArg(4);

  // BasicBlocks:
  BasicBlock *BB5 = BasicBlock::Create(context, "", func);
  BasicBlock *BB8 = BasicBlock::Create(context, "", func);
  BasicBlock *BB10 = BasicBlock::Create(context, "", func);
  BasicBlock *BB17 = BasicBlock::Create(context, "", func);
  BasicBlock *BB18 = BasicBlock::Create(context, "", func);
  BasicBlock *BB21 = BasicBlock::Create(context, "", func);
  BasicBlock *BB33 = BasicBlock::Create(context, "", func);
  BasicBlock *BB37 = BasicBlock::Create(context, "", func);

  // 5:
  builder.SetInsertPoint(BB5);
  // %6 = sub i32 0, %3
  Value *val6 = builder.CreateSub(builder.getInt32(0), val3);
  // %7 = icmp slt i32 %3, 0
  Value *val7 = builder.CreateICmpSLT(val3, builder.getInt32(0));
  // br i1 %7, label %17, label %8
  builder.CreateCondBr(val7, BB17, BB8);

  // 8:                                                ; preds = %5
  builder.SetInsertPoint(BB8);
  // %9 = mul nsw i32 %3, %3
  Value *val9 = builder.CreateMul(val3, val3, "", false, true);
  // br label %10
  builder.CreateBr(BB10);

  // 10:                                               ; preds = %18, %8
  builder.SetInsertPoint(BB10);
  // %11 = phi i32 [ %6, %8 ], [ %19, %18 ]
  PHINode *val11 = builder.CreatePHI(int32Type, 2);
  // %12 = add nsw i32 %11, %2
  Value *val12 = builder.CreateAdd(val11, val2, "", false, true);
  // %13 = mul nsw i32 %11, %11
  Value *val13 = builder.CreateMul(val11, val11, "", false, true);
  // %14 = icmp sgt i32 %12, 0
  Value *val14 = builder.CreateICmpSGT(val12, builder.getInt32(0));
  // %15 = icmp slt i32 %12, 127
  Value *val15 = builder.CreateICmpSLT(val12, builder.getInt32(127));
  // %16 = shl nuw nsw i32 %12, 8
  Value *val16 = builder.CreateShl(val12, 8, "", true, true);
  // br label %21
  builder.CreateBr(BB21);

  // 17:                                               ; preds = %18, %5
  builder.SetInsertPoint(BB17);
  // ret void
  builder.CreateRetVoid();

  // 18:                                               ; preds = %37
  builder.SetInsertPoint(BB18);
  // %19 = add i32 %11, 1
  Value *val19 = builder.CreateAdd(val11, builder.getInt32(1));
  // %20 = icmp eq i32 %11, %3
  Value *val20 = builder.CreateICmpEQ(val11, val3);
  // br i1 %20, label %17, label %10
  builder.CreateCondBr(val20, BB17, BB10);

  // 21:                                               ; preds = %10, %37
  builder.SetInsertPoint(BB21);
  // %22 = phi i32 [ %6, %10 ], [ %38, %37 ]
  PHINode *val22 = builder.CreatePHI(int32Type, 2);
  // %23 = add nsw i32 %22, %1
  Value *val23 = builder.CreateAdd(val22, val1, "", false, true);
  // %24 = mul nsw i32 %22, %22
  Value *val24 = builder.CreateMul(val22, val22, "", false, true);
  // %25 = add nuw nsw i32 %24, %13
  Value *val25 = builder.CreateAdd(val24, val13, "", true, true);
  // %26 = icmp ule i32 %25, %9
  Value *val26 = builder.CreateICmpULE(val25, val9);
  // %27 = icmp sgt i32 %23, 0
  Value *val27 = builder.CreateICmpSGT(val23, builder.getInt32(0));
  // %28 = select i1 %26, i1 %27, i1 false
  Value *val28 = builder.CreateSelect(val26, val27, builder.getFalse());
  // %29 = icmp slt i32 %23, 255
  Value *val29 = builder.CreateICmpSLT(val23, builder.getInt32(255));
  // %30 = select i1 %28, i1 %29, i1 false
  Value *val30 = builder.CreateSelect(val28, val29, builder.getFalse());
  // %31 = select i1 %30, i1 %14, i1 false
  Value *val31 = builder.CreateSelect(val30, val14, builder.getFalse());
  // %32 = select i1 %31, i1 %15, i1 false
  Value *val32 = builder.CreateSelect(val31, val15, builder.getFalse());
  // br i1 %32, label %33, label %37
  builder.CreateCondBr(val32, BB33, BB37);

  // 33:                                               ; preds = %21
  builder.SetInsertPoint(BB33);
  // %34 = add nuw nsw i32 %23, %16
  Value *val34 = builder.CreateAdd(val23, val16, "", true, true);
  // %35 = zext i32 %34 to i64
  Value *val35 = builder.CreateZExt(val34, int64Type);
  // %36 = getelementptr inbounds i32, ptr %0, i64 %35
  Value *val36 = builder.CreateInBoundsGEP(int32Type, val0, val35);
  // store i32 %4, ptr %36, align 4
  builder.CreateStore(val4, val36);
  // br label %37
  builder.CreateBr(BB37);

  // 37:                                               ; preds = %33, %21
  builder.SetInsertPoint(BB37);
  // %38 = add i32 %22, 1
  Value *val38 = builder.CreateAdd(val22, builder.getInt32(1));
  // %39 = icmp eq i32 %22, %3
  Value *val39 = builder.CreateICmpEQ(val22, val3);
  // br i1 %39, label %18, label %21
  builder.CreateCondBr(val39, BB18, BB21);

  // Link PHI nodes
  // %11 = phi i32 [ %6, %8 ], [ %19, %18 ]
  val11->addIncoming(val6, BB8);
  val11->addIncoming(val19, BB18);
  // %22 = phi i32 [ %6, %10 ], [ %38, %37 ]
  val22->addIncoming(val6, BB10);
  val22->addIncoming(val38, BB37);

}

// define void @rotate(ptr %0, ptr %1, ptr %2, i32 %3)
static void genRotate(Module *module, IRBuilder<> &builder) {
  LLVMContext &context = module->getContext();
  Type *voidType = builder.getVoidTy();
  Type *int32Type = builder.getInt32Ty();
  Type *int64Type = builder.getInt64Ty();
  Type *ptrType = PointerType::get(context, 0);

  FunctionType *funcType = FunctionType::get(
      voidType, {ptrType, ptrType, ptrType, int32Type}, false);
  Function *func =
      Function::Create(funcType, Function::ExternalLinkage, "rotate", module);
  Value *val0 = func->getArg(0);
  Value *val1 = func->getArg(1);
  Value *val2 = func->getArg(2);
  Value *val3 = func->getArg(3);

  // BasicBlocks:
  BasicBlock *BB4 = BasicBlock::Create(context, "", func);
  BasicBlock *BB6 = BasicBlock::Create(context, "", func);
  BasicBlock *BB8 = BasicBlock::Create(context, "", func);
  BasicBlock *BB9 = BasicBlock::Create(context, "", func);

  // 4:
  builder.SetInsertPoint(BB4);
  // %5 = icmp sgt i32 %3, 0
  Value *val5 = builder.CreateICmpSGT(val3, builder.getInt32(0));
  // br i1 %5, label %6, label %8
  builder.CreateCondBr(val5, BB6, BB8);

  // 6:                                                ; preds = %4
  builder.SetInsertPoint(BB6);
  // %7 = zext i32 %3 to i64
  Value *val7 = builder.CreateZExt(val3, int64Type);
  // br label %9
  builder.CreateBr(BB9);

  // 8:                                                ; preds = %9, %4
  builder.SetInsertPoint(BB8);
  // ret void
  builder.CreateRetVoid();

  // 9:                                                ; preds = %6, %9
  builder.SetInsertPoint(BB9);
  // %10 = phi i64 [ 0, %6 ], [ %23, %9 ]
  PHINode *val10 = builder.CreatePHI(int64Type, 2);
  // %11 = getelementptr inbounds i32, ptr %1, i64 %10
  Value *val11 = builder.CreateInBoundsGEP(int32Type, val1, val10);
  // %12 = load i32, ptr %11, align 4
  Value *val12 = builder.CreateLoad(int32Type, val11);
  // %13 = getelementptr inbounds i32, ptr %2, i64 %10
  Value *val13 = builder.CreateInBoundsGEP(int32Type, val2, val10);
  // %14 = load i32, ptr %13, align 4
  Value *val14 = builder.CreateLoad(int32Type, val13);
  // %15 = ashr i32 %12, %14
  Value *val15 = builder.CreateAShr(val12, val14);
  // %16 = getelementptr inbounds i32, ptr %0, i64 %10
  Value *val16 = builder.CreateInBoundsGEP(int32Type, val0, val10);
  // %17 = load i32, ptr %16, align 4
  Value *val17 = builder.CreateLoad(int32Type, val16);
  // %18 = sub nsw i32 %17, %15
  Value *val18 = builder.CreateSub(val17, val15, "", false, true);
  // store i32 %18, ptr %16, align 4
  builder.CreateStore(val18, val16);
  // %19 = load i32, ptr %13, align 4
  Value *val19 = builder.CreateLoad(int32Type, val13);
  // %20 = ashr i32 %18, %19
  Value *val20 = builder.CreateAShr(val18, val19);
  // %21 = load i32, ptr %11, align 4
  Value *val21 = builder.CreateLoad(int32Type, val11);
  // %22 = add nsw i32 %21, %20
  Value *val22 = builder.CreateAdd(val21, val20, "", false, true);
  // store i32 %22, ptr %11, align 4
  builder.CreateStore(val22, val11);
  // %23 = add nuw nsw i64 %10, 1
  Value *val23 = builder.CreateAdd(val10, builder.getInt64(1), "", true, true);
  // %24 = icmp eq i64 %23, %7
  Value *val24 = builder.CreateICmpEQ(val23, val7);
  // br i1 %24, label %8, label %9
  builder.CreateCondBr(val24, BB8, BB9);

  // Link PHI nodes
  // %10 = phi i64 [ 0, %6 ], [ %23, %9 ]
  val10->addIncoming(builder.getInt64(0), BB6);
  val10->addIncoming(val23, BB9);

}

// define void @drawField(ptr %0)
static void genDrawField(Module *module, IRBuilder<> &builder) {
  LLVMContext &context = module->getContext();
  Type *voidType = builder.getVoidTy();
  Type *int32Type = builder.getInt32Ty();
  Type *int64Type = builder.getInt64Ty();
  Type *ptrType = PointerType::get(context, 0);

  FunctionType *funcType = FunctionType::get(voidType, {ptrType}, false);
  Function *func =
      Function::Create(funcType, Function::ExternalLinkage, "drawField", module);
  Function *heatColorFunc = module->getFunction("heatColor");
  Function *simPutPixelFunc = module->getFunction("simPutPixel");
  Value *val0 = func->getArg(0);

  // BasicBlocks:
  BasicBlock *BB1 = BasicBlock::Create(context, "", func);
  BasicBlock *BB2 = BasicBlock::Create(context, "", func);
  BasicBlock *BB7 = BasicBlock::Create(context, "", func);
  BasicBlock *BB8 = BasicBlock::Create(context, "", func);
  BasicBlock *BB11 = BasicBlock::Create(context, "", func);
  BasicBlock *BB19 = BasicBlock::Create(context, "", func);
  BasicBlock *BB22 = BasicBlock::Create(context, "", func);
  BasicBlock *BB25 = BasicBlock::Create(context, "", func);
  BasicBlock *BB28 = BasicBlock::Create(context, "", func);

  // 1:
  builder.SetInsertPoint(BB1);
  // br label %2
  builder.CreateBr(BB2);

  // 2:                                                ; preds = %1, %8
  builder.SetInsertPoint(BB2);
  // %3 = phi i64 [ 0, %1 ], [ %9, %8 ]
  PHINode *val3 = builder.CreatePHI(int64Type, 2);
  // %4 = shl nsw i64 %3, 8
  Value *val4 = builder.CreateShl(val3, 8, "", false, true);
  // %5 = trunc i64 %3 to i32
  Value *val5 = builder.CreateTrunc(val3, int32Type);
  // %6 = shl i32 %5, 2
  Value *val6 = builder.CreateShl(val5, 2);
  // br label %11
  builder.CreateBr(BB11);

  // 7:                                                ; preds = %8
  builder.SetInsertPoint(BB7);
  // ret void
  builder.CreateRetVoid();

  // 8:                                                ; preds = %22
  builder.SetInsertPoint(BB8);
  // %9 = add nuw nsw i64 %3, 1
  Value *val9 = builder.CreateAdd(val3, builder.getInt64(1), "", true, true);
  // %10 = icmp eq i64 %9, 128
  Value *val10 = builder.CreateICmpEQ(val9, builder.getInt64(128));
  // br i1 %10, label %7, label %2
  builder.CreateCondBr(val10, BB7, BB2);

  // 11:                                               ; preds = %2, %22
  builder.SetInsertPoint(BB11);
  // %12 = phi i64 [ 0, %2 ], [ %23, %22 ]
  PHINode *val12 = builder.CreatePHI(int64Type, 2);
  // %13 = add nuw nsw i64 %12, %4
  Value *val13 = builder.CreateAdd(val12, val4, "", true, true);
  // %14 = getelementptr inbounds i32, ptr %0, i64 %13
  Value *val14 = builder.CreateInBoundsGEP(int32Type, val0, val13);
  // %15 = load i32, ptr %14, align 4
  Value *val15 = builder.CreateLoad(int32Type, val14);
  // %16 = call i32 @heatColor(i32 %15)
  Value *val16 = builder.CreateCall(heatColorFunc, {val15});
  // %17 = trunc i64 %12 to i32
  Value *val17 = builder.CreateTrunc(val12, int32Type);
  // %18 = shl i32 %17, 2
  Value *val18 = builder.CreateShl(val17, 2);
  // br label %19
  builder.CreateBr(BB19);

  // 19:                                               ; preds = %11, %25
  builder.SetInsertPoint(BB19);
  // %20 = phi i32 [ 0, %11 ], [ %26, %25 ]
  PHINode *val20 = builder.CreatePHI(int32Type, 2);
  // %21 = add nuw nsw i32 %20, %6
  Value *val21 = builder.CreateAdd(val20, val6, "", true, true);
  // br label %28
  builder.CreateBr(BB28);

  // 22:                                               ; preds = %25
  builder.SetInsertPoint(BB22);
  // %23 = add nuw nsw i64 %12, 1
  Value *val23 = builder.CreateAdd(val12, builder.getInt64(1), "", true, true);
  // %24 = icmp eq i64 %23, 256
  Value *val24 = builder.CreateICmpEQ(val23, builder.getInt64(256));
  // br i1 %24, label %8, label %11
  builder.CreateCondBr(val24, BB8, BB11);

  // 25:                                               ; preds = %28
  builder.SetInsertPoint(BB25);
  // %26 = add nuw nsw i32 %20, 1
  Value *val26 = builder.CreateAdd(val20, builder.getInt32(1), "", true, true);
  // %27 = icmp eq i32 %26, 4
  Value *val27 = builder.CreateICmpEQ(val26, builder.getInt32(4));
  // br i1 %27, label %22, label %19
  builder.CreateCondBr(val27, BB22, BB19);

  // 28:                                               ; preds = %19, %28
  builder.SetInsertPoint(BB28);
  // %29 = phi i32 [ 0, %19 ], [ %31, %28 ]
  PHINode *val29 = builder.CreatePHI(int32Type, 2);
  // %30 = add nuw nsw i32 %29, %18
  Value *val30 = builder.CreateAdd(val29, val18, "", true, true);
  // call void @simPutPixel(i32 %30, i32 %21, i32 %16)
  builder.CreateCall(simPutPixelFunc, {val30, val21, val16});
  // %31 = add nuw nsw i32 %29, 1
  Value *val31 = builder.CreateAdd(val29, builder.getInt32(1), "", true, true);
  // %32 = icmp eq i32 %31, 4
  Value *val32 = builder.CreateICmpEQ(val31, builder.getInt32(4));
  // br i1 %32, label %25, label %28
  builder.CreateCondBr(val32, BB25, BB28);

  // Link PHI nodes
  // %3 = phi i64 [ 0, %1 ], [ %9, %8 ]
  val3->addIncoming(builder.getInt64(0), BB1);
  val3->addIncoming(val9, BB8);
  // %12 = phi i64 [ 0, %2 ], [ %23, %22 ]
  val12->addIncoming(builder.getInt64(0), BB2);
  val12->addIncoming(val23, BB22);
  // %20 = phi i32 [ 0, %11 ], [ %26, %25 ]
  val20->addIncoming(builder.getInt32(0), BB11);
  val20->addIncoming(val26, BB25);
  // %29 = phi i32 [ 0, %19 ], [ %31, %28 ]
  val29->addIncoming(builder.getInt32(0), BB19);
  val29->addIncoming(val31, BB28);

}

// define void @app()
static void genApp(Module *module, IRBuilder<> &builder) {
  LLVMContext &context = module->getContext();
  Type *voidType = builder.getVoidTy();
  Type *int8Type = builder.getInt8Ty();
  Type *int32Type = builder.getInt32Ty();
  Type *int64Type = builder.getInt64Ty();
  Type *ptrType = PointerType::get(context, 0);
  ArrayType *fieldType = ArrayType::get(int32Type, 32768);
  ArrayType *srcType = ArrayType::get(int32Type, 6);

  FunctionType *funcType = FunctionType::get(voidType, false);
  Function *func =
      Function::Create(funcType, Function::ExternalLinkage, "app", module);
  Function *diffuseStepFunc = module->getFunction("diffuseStep");
  Function *applyDiscFunc = module->getFunction("applyDisc");
  Function *rotateFunc = module->getFunction("rotate");
  Function *drawFieldFunc = module->getFunction("drawField");
  Function *simRandFunc = module->getFunction("simRand");
  Function *simFlushFunc = module->getFunction("simFlush");

  // BasicBlocks:
  BasicBlock *BB0 = BasicBlock::Create(context, "", func);
  BasicBlock *BB10 = BasicBlock::Create(context, "", func);
  BasicBlock *BB39 = BasicBlock::Create(context, "", func);
  BasicBlock *BB43 = BasicBlock::Create(context, "", func);
  BasicBlock *BB47 = BasicBlock::Create(context, "", func);
  BasicBlock *BB50 = BasicBlock::Create(context, "", func);
  BasicBlock *BB53 = BasicBlock::Create(context, "", func);
  BasicBlock *BB67 = BasicBlock::Create(context, "", func);
  BasicBlock *BB74 = BasicBlock::Create(context, "", func);

  // 0:
  builder.SetInsertPoint(BB0);
  // %1 = alloca [32768 x i32], align 16
  AllocaInst *val1 = builder.CreateAlloca(fieldType);
  val1->setAlignment(Align(16));
  // %2 = alloca [32768 x i32], align 16
  AllocaInst *val2 = builder.CreateAlloca(fieldType);
  val2->setAlignment(Align(16));
  // %3 = alloca [6 x i32], align 16
  AllocaInst *val3 = builder.CreateAlloca(srcType);
  val3->setAlignment(Align(16));
  // %4 = alloca [6 x i32], align 16
  AllocaInst *val4 = builder.CreateAlloca(srcType);
  val4->setAlignment(Align(16));
  // %5 = alloca [6 x i32], align 16
  AllocaInst *val5 = builder.CreateAlloca(srcType);
  val5->setAlignment(Align(16));
  // %6 = alloca [6 x i32], align 16
  AllocaInst *val6 = builder.CreateAlloca(srcType);
  val6->setAlignment(Align(16));
  // %7 = alloca [6 x i32], align 16
  AllocaInst *val7 = builder.CreateAlloca(srcType);
  val7->setAlignment(Align(16));
  // %8 = alloca [6 x i32], align 16
  AllocaInst *val8 = builder.CreateAlloca(srcType);
  val8->setAlignment(Align(16));
  // %9 = alloca [6 x i32], align 16
  AllocaInst *val9 = builder.CreateAlloca(srcType);
  val9->setAlignment(Align(16));
  // call void @llvm.memset.p0.i64(ptr align 16 %1, i8 0, i64 131072, i1 false)
  builder.CreateMemSet(val1, builder.getInt8(0), builder.getInt64(131072),
                       MaybeAlign(16));
  // call void @llvm.memset.p0.i64(ptr align 16 %2, i8 0, i64 131072, i1 false)
  builder.CreateMemSet(val2, builder.getInt8(0), builder.getInt64(131072),
                       MaybeAlign(16));
  // br label %10
  builder.CreateBr(BB10);

  // 10:                                               ; preds = %0, %10
  builder.SetInsertPoint(BB10);
  // %11 = phi i64 [ 0, %0 ], [ %37, %10 ]
  PHINode *val11 = builder.CreatePHI(int64Type, 2);
  // %12 = call i32 @simRand()
  Value *val12 = builder.CreateCall(simRandFunc);
  // %13 = srem i32 %12, 64
  Value *val13 = builder.CreateSRem(val12, builder.getInt32(64));
  // %14 = mul nsw i32 %13, -256
  Value *val14 =
      builder.CreateMul(val13, builder.getInt32(-256), "", false, true);
  // %15 = add nsw i32 %14, 31232
  Value *val15 =
      builder.CreateAdd(val14, builder.getInt32(31232), "", false, true);
  // %16 = getelementptr inbounds [6 x i32], ptr %3, i64 0, i64 %11
  Value *val16 =
      builder.CreateInBoundsGEP(srcType, val3, {builder.getInt64(0), val11});
  // store i32 %15, ptr %16, align 4
  builder.CreateStore(val15, val16);
  // %17 = getelementptr inbounds [6 x i32], ptr %4, i64 0, i64 %11
  Value *val17 =
      builder.CreateInBoundsGEP(srcType, val4, {builder.getInt64(0), val11});
  // store i32 0, ptr %17, align 4
  builder.CreateStore(builder.getInt32(0), val17);
  // %18 = call i32 @simRand()
  Value *val18 = builder.CreateCall(simRandFunc);
  // %19 = srem i32 %18, 3
  Value *val19 = builder.CreateSRem(val18, builder.getInt32(3));
  // %20 = add nsw i32 %19, 4
  Value *val20 = builder.CreateAdd(val19, builder.getInt32(4), "", false, true);
  // %21 = getelementptr inbounds [6 x i32], ptr %5, i64 0, i64 %11
  Value *val21 =
      builder.CreateInBoundsGEP(srcType, val5, {builder.getInt64(0), val11});
  // store i32 %20, ptr %21, align 4
  builder.CreateStore(val20, val21);
  // %22 = getelementptr inbounds [6 x i32], ptr %6, i64 0, i64 %11
  Value *val22 =
      builder.CreateInBoundsGEP(srcType, val6, {builder.getInt64(0), val11});
  // store i32 0, ptr %22, align 4
  builder.CreateStore(builder.getInt32(0), val22);
  // %23 = call i32 @simRand()
  Value *val23 = builder.CreateCall(simRandFunc);
  // %24 = srem i32 %23, 32
  Value *val24 = builder.CreateSRem(val23, builder.getInt32(32));
  // %25 = mul nsw i32 %24, -256
  Value *val25 =
      builder.CreateMul(val24, builder.getInt32(-256), "", false, true);
  // %26 = add nsw i32 %25, 14848
  Value *val26 =
      builder.CreateAdd(val25, builder.getInt32(14848), "", false, true);
  // %27 = getelementptr inbounds [6 x i32], ptr %7, i64 0, i64 %11
  Value *val27 =
      builder.CreateInBoundsGEP(srcType, val7, {builder.getInt64(0), val11});
  // store i32 %26, ptr %27, align 4
  builder.CreateStore(val26, val27);
  // %28 = call i32 @simRand()
  Value *val28 = builder.CreateCall(simRandFunc);
  // %29 = srem i32 %28, 3
  Value *val29 = builder.CreateSRem(val28, builder.getInt32(3));
  // %30 = add nsw i32 %29, 4
  Value *val30 = builder.CreateAdd(val29, builder.getInt32(4), "", false, true);
  // %31 = getelementptr inbounds [6 x i32], ptr %8, i64 0, i64 %11
  Value *val31 =
      builder.CreateInBoundsGEP(srcType, val8, {builder.getInt64(0), val11});
  // store i32 %30, ptr %31, align 4
  builder.CreateStore(val30, val31);
  // %32 = trunc i64 %11 to i8
  Value *val32 = builder.CreateTrunc(val11, int8Type);
  // %33 = urem i8 %32, 3
  Value *val33 = builder.CreateURem(val32, builder.getInt8(3));
  // %34 = icmp eq i8 %33, 2
  Value *val34 = builder.CreateICmpEQ(val33, builder.getInt8(2));
  // %35 = select i1 %34, i32 0, i32 65535
  Value *val35 =
      builder.CreateSelect(val34, builder.getInt32(0), builder.getInt32(65535));
  // %36 = getelementptr inbounds [6 x i32], ptr %9, i64 0, i64 %11
  Value *val36 =
      builder.CreateInBoundsGEP(srcType, val9, {builder.getInt64(0), val11});
  // store i32 %35, ptr %36, align 4
  builder.CreateStore(val35, val36);
  // %37 = add nuw nsw i64 %11, 1
  Value *val37 = builder.CreateAdd(val11, builder.getInt64(1), "", true, true);
  // %38 = icmp eq i64 %37, 6
  Value *val38 = builder.CreateICmpEQ(val37, builder.getInt64(6));
  // br i1 %38, label %39, label %10
  builder.CreateCondBr(val38, BB39, BB10);

  // 39:                                               ; preds = %10, %74
  builder.SetInsertPoint(BB39);
  // %40 = phi ptr [ %44, %74 ], [ %2, %10 ]
  PHINode *val40 = builder.CreatePHI(ptrType, 2);
  // %41 = phi i32 [ %75, %74 ], [ 0, %10 ]
  PHINode *val41 = builder.CreatePHI(int32Type, 2);
  // %42 = phi ptr [ %46, %74 ], [ %1, %10 ]
  PHINode *val42 = builder.CreatePHI(ptrType, 2);
  // br label %43
  builder.CreateBr(BB43);

  // 43:                                               ; preds = %39, %50
  builder.SetInsertPoint(BB43);
  // %44 = phi ptr [ %42, %39 ], [ %46, %50 ]
  PHINode *val44 = builder.CreatePHI(ptrType, 2);
  // %45 = phi i32 [ 0, %39 ], [ %51, %50 ]
  PHINode *val45 = builder.CreatePHI(int32Type, 2);
  // %46 = phi ptr [ %40, %39 ], [ %44, %50 ]
  PHINode *val46 = builder.CreatePHI(ptrType, 2);
  // br label %53
  builder.CreateBr(BB53);

  // 47:                                               ; preds = %50
  builder.SetInsertPoint(BB47);
  // %48 = and i32 %41, 15
  Value *val48 = builder.CreateAnd(val41, 15);
  // %49 = icmp eq i32 %48, 0
  Value *val49 = builder.CreateICmpEQ(val48, builder.getInt32(0));
  // br i1 %49, label %67, label %74
  builder.CreateCondBr(val49, BB67, BB74);

  // 50:                                               ; preds = %53
  builder.SetInsertPoint(BB50);
  // call void @diffuseStep(ptr %44, ptr %46)
  builder.CreateCall(diffuseStepFunc, {val44, val46});
  // %51 = add nuw nsw i32 %45, 1
  Value *val51 = builder.CreateAdd(val45, builder.getInt32(1), "", true, true);
  // %52 = icmp eq i32 %51, 4
  Value *val52 = builder.CreateICmpEQ(val51, builder.getInt32(4));
  // br i1 %52, label %47, label %43
  builder.CreateCondBr(val52, BB47, BB43);

  // 53:                                               ; preds = %43, %53
  builder.SetInsertPoint(BB53);
  // %54 = phi i64 [ 0, %43 ], [ %65, %53 ]
  PHINode *val54 = builder.CreatePHI(int64Type, 2);
  // %55 = getelementptr inbounds [6 x i32], ptr %3, i64 0, i64 %54
  Value *val55 =
      builder.CreateInBoundsGEP(srcType, val3, {builder.getInt64(0), val54});
  // %56 = load i32, ptr %55, align 4
  Value *val56 = builder.CreateLoad(int32Type, val55);
  // %57 = ashr i32 %56, 8
  Value *val57 = builder.CreateAShr(val56, 8);
  // %58 = add nsw i32 %57, 128
  Value *val58 =
      builder.CreateAdd(val57, builder.getInt32(128), "", false, true);
  // %59 = getelementptr inbounds [6 x i32], ptr %7, i64 0, i64 %54
  Value *val59 =
      builder.CreateInBoundsGEP(srcType, val7, {builder.getInt64(0), val54});
  // %60 = load i32, ptr %59, align 4
  Value *val60 = builder.CreateLoad(int32Type, val59);
  // %61 = ashr i32 %60, 8
  Value *val61 = builder.CreateAShr(val60, 8);
  // %62 = add nsw i32 %61, 64
  Value *val62 = builder.CreateAdd(val61, builder.getInt32(64), "", false, true);
  // %63 = getelementptr inbounds [6 x i32], ptr %9, i64 0, i64 %54
  Value *val63 =
      builder.CreateInBoundsGEP(srcType, val9, {builder.getInt64(0), val54});
  // %64 = load i32, ptr %63, align 4
  Value *val64 = builder.CreateLoad(int32Type, val63);
  // call void @applyDisc(ptr %44, i32 %58, i32 %62, i32 4, i32 %64)
  builder.CreateCall(applyDiscFunc,
                     {val44, val58, val62, builder.getInt32(4), val64});
  // %65 = add nuw nsw i64 %54, 1
  Value *val65 = builder.CreateAdd(val54, builder.getInt64(1), "", true, true);
  // %66 = icmp eq i64 %65, 6
  Value *val66 = builder.CreateICmpEQ(val65, builder.getInt64(6));
  // br i1 %66, label %50, label %53
  builder.CreateCondBr(val66, BB50, BB53);

  // 67:                                               ; preds = %47
  builder.SetInsertPoint(BB67);
  // %68 = call i32 @simRand()
  Value *val68 = builder.CreateCall(simRandFunc);
  // %69 = srem i32 %68, 254
  Value *val69 = builder.CreateSRem(val68, builder.getInt32(254));
  // %70 = add nsw i32 %69, 1
  Value *val70 = builder.CreateAdd(val69, builder.getInt32(1), "", false, true);
  // %71 = call i32 @simRand()
  Value *val71 = builder.CreateCall(simRandFunc);
  // %72 = srem i32 %71, 126
  Value *val72 = builder.CreateSRem(val71, builder.getInt32(126));
  // %73 = add nsw i32 %72, 1
  Value *val73 = builder.CreateAdd(val72, builder.getInt32(1), "", false, true);
  // call void @applyDisc(ptr %46, i32 %70, i32 %73, i32 8, i32 65535)
  builder.CreateCall(applyDiscFunc, {val46, val70, val73, builder.getInt32(8),
                                     builder.getInt32(65535)});
  // br label %74
  builder.CreateBr(BB74);

  // 74:                                               ; preds = %67, %47
  builder.SetInsertPoint(BB74);
  // call void @rotate(ptr %3, ptr %4, ptr %5, i32 6)
  builder.CreateCall(rotateFunc, {val3, val4, val5, builder.getInt32(6)});
  // call void @rotate(ptr %6, ptr %7, ptr %8, i32 6)
  builder.CreateCall(rotateFunc, {val6, val7, val8, builder.getInt32(6)});
  // call void @drawField(ptr %46)
  builder.CreateCall(drawFieldFunc, {val46});
  // call void @simFlush()
  builder.CreateCall(simFlushFunc);
  // %75 = add nuw nsw i32 %41, 1
  Value *val75 = builder.CreateAdd(val41, builder.getInt32(1), "", true, true);
  // br label %39
  builder.CreateBr(BB39);

  // Link PHI nodes
  // %11 = phi i64 [ 0, %0 ], [ %37, %10 ]
  val11->addIncoming(builder.getInt64(0), BB0);
  val11->addIncoming(val37, BB10);
  // %40 = phi ptr [ %44, %74 ], [ %2, %10 ]
  val40->addIncoming(val44, BB74);
  val40->addIncoming(val2, BB10);
  // %41 = phi i32 [ %75, %74 ], [ 0, %10 ]
  val41->addIncoming(val75, BB74);
  val41->addIncoming(builder.getInt32(0), BB10);
  // %42 = phi ptr [ %46, %74 ], [ %1, %10 ]
  val42->addIncoming(val46, BB74);
  val42->addIncoming(val1, BB10);
  // %44 = phi ptr [ %42, %39 ], [ %46, %50 ]
  val44->addIncoming(val42, BB39);
  val44->addIncoming(val46, BB50);
  // %45 = phi i32 [ 0, %39 ], [ %51, %50 ]
  val45->addIncoming(builder.getInt32(0), BB39);
  val45->addIncoming(val51, BB50);
  // %46 = phi ptr [ %40, %39 ], [ %44, %50 ]
  val46->addIncoming(val40, BB39);
  val46->addIncoming(val44, BB50);
  // %54 = phi i64 [ 0, %43 ], [ %65, %53 ]
  val54->addIncoming(builder.getInt64(0), BB43);
  val54->addIncoming(val65, BB53);

}

int main() {
  LLVMContext context;
  // ; ModuleID = 'app.c'
  // source_filename = "app.c"
  Module *module = new Module("app.c", context);
  IRBuilder<> builder(context);

  Type *voidType = Type::getVoidTy(context);
  Type *int32Type = Type::getInt32Ty(context);

  // declare void @simPutPixel(i32, i32, i32)
  FunctionType *simPutPixelType =
      FunctionType::get(voidType, {int32Type, int32Type, int32Type}, false);
  module->getOrInsertFunction("simPutPixel", simPutPixelType);
  // declare i32 @simRand()
  FunctionType *simRandType = FunctionType::get(int32Type, false);
  module->getOrInsertFunction("simRand", simRandType);
  // declare void @simFlush()
  FunctionType *simFlushType = FunctionType::get(voidType, false);
  module->getOrInsertFunction("simFlush", simFlushType);

  genHeatColor(module, builder);
  genDiffuseStep(module, builder);
  genApplyDisc(module, builder);
  genRotate(module, builder);
  genDrawField(module, builder);
  genApp(module, builder);

  // Dump LLVM IR
  module->print(outs(), nullptr);
  outs() << '\n';
  bool verif = verifyModule(*module, &outs());
  outs() << "[VERIFICATION] " << (verif ? "FAIL\n\n" : "OK\n\n");

  // LLVM IR Interpreter
  outs() << "[EE] Run\n";
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  Function *appFunc = module->getFunction("app");
  ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
  ee->InstallLazyFunctionCreator([](const std::string &fnName) -> void * {
    if (fnName == "simPutPixel")
      return reinterpret_cast<void *>(simPutPixel);
    if (fnName == "simRand")
      return reinterpret_cast<void *>(simRand);
    if (fnName == "simFlush")
      return reinterpret_cast<void *>(simFlush);
    if (fnName == "memset")
      return reinterpret_cast<void *>(memset);
    outs() << "[ExecutionEngine] Can't find function " << fnName << '\n';
    return nullptr;
  });
  ee->finalizeObject();
  outs().flush();

  simInit();

  ArrayRef<GenericValue> noargs;
  ee->runFunction(appFunc, noargs);

  simExit();
  return EXIT_SUCCESS;
}
