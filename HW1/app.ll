; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@__const.heatColor.pal = private unnamed_addr constant [7 x i32] [i32 -16777212, i32 -14152876, i32 -10152594, i32 -6346141, i32 -2865086, i32 -422902, i32 -196700], align 16

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local range(i32 -16777216, 0) i32 @heatColor(i32 noundef %0) local_unnamed_addr #0 {
  %2 = mul nsw i32 %0, 6
  %3 = ashr i32 %2, 16
  %4 = tail call i32 @llvm.smin.i32(i32 %3, i32 5)
  %5 = shl nsw i32 %4, 16
  %6 = sub nsw i32 %2, %5
  %7 = sext i32 %4 to i64
  %8 = getelementptr inbounds [7 x i32], ptr @__const.heatColor.pal, i64 0, i64 %7
  %9 = load i32, ptr %8, align 4, !tbaa !5
  %10 = add nsw i32 %4, 1
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds [7 x i32], ptr @__const.heatColor.pal, i64 0, i64 %11
  %13 = load i32, ptr %12, align 4, !tbaa !5
  %14 = and i32 %9, 255
  %15 = and i32 %13, 255
  %16 = sub nsw i32 %15, %14
  %17 = mul nsw i32 %16, %6
  %18 = ashr i32 %17, 16
  %19 = add nsw i32 %18, %14
  %20 = or i32 %19, -16777216
  %21 = lshr i32 %9, 8
  %22 = and i32 %21, 255
  %23 = lshr i32 %13, 8
  %24 = and i32 %23, 255
  %25 = sub nsw i32 %24, %22
  %26 = mul nsw i32 %25, %6
  %27 = ashr i32 %26, 16
  %28 = add nsw i32 %27, %22
  %29 = shl nsw i32 %28, 8
  %30 = or i32 %29, %20
  %31 = lshr i32 %9, 16
  %32 = and i32 %31, 255
  %33 = lshr i32 %13, 16
  %34 = and i32 %33, 255
  %35 = sub nsw i32 %34, %32
  %36 = mul nsw i32 %35, %6
  %37 = shl nuw nsw i32 %32, 16
  %38 = add i32 %36, %37
  %39 = and i32 %38, 16711680
  %40 = or i32 %39, %30
  ret i32 %40
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @diffuseStep(ptr noundef readonly captures(none) %0, ptr noundef writeonly captures(none) %1) local_unnamed_addr #2 {
  %3 = getelementptr nuw i8, ptr %1, i64 1028
  %4 = getelementptr i8, ptr %1, i64 130044
  %5 = getelementptr nuw i8, ptr %0, i64 4
  %6 = getelementptr i8, ptr %0, i64 131068
  %7 = icmp ult ptr %3, %6
  %8 = icmp ult ptr %5, %4
  %9 = and i1 %7, %8
  br label %10

10:                                               ; preds = %2, %54
  %11 = phi i64 [ 1, %2 ], [ %55, %54 ]
  %12 = shl nuw nsw i64 %11, 8
  br i1 %9, label %13, label %15

13:                                               ; preds = %15, %10
  %14 = phi i64 [ 1, %10 ], [ 249, %15 ]
  br label %57

15:                                               ; preds = %10, %15
  %16 = phi i64 [ %51, %15 ], [ 0, %10 ]
  %17 = or disjoint i64 %16, 1
  %18 = add nuw nsw i64 %17, %12
  %19 = getelementptr inbounds nuw i32, ptr %0, i64 %18
  %20 = getelementptr inbounds nuw i8, ptr %19, i64 16
  %21 = load <4 x i32>, ptr %19, align 4, !tbaa !5, !alias.scope !9
  %22 = load <4 x i32>, ptr %20, align 4, !tbaa !5, !alias.scope !9
  %23 = getelementptr i8, ptr %19, i64 -4
  %24 = getelementptr i8, ptr %19, i64 12
  %25 = load <4 x i32>, ptr %23, align 4, !tbaa !5, !alias.scope !9
  %26 = load <4 x i32>, ptr %24, align 4, !tbaa !5, !alias.scope !9
  %27 = add nsw <4 x i32> %25, %21
  %28 = add nsw <4 x i32> %26, %22
  %29 = getelementptr inbounds nuw i8, ptr %19, i64 4
  %30 = getelementptr inbounds nuw i8, ptr %19, i64 20
  %31 = load <4 x i32>, ptr %29, align 4, !tbaa !5, !alias.scope !9
  %32 = load <4 x i32>, ptr %30, align 4, !tbaa !5, !alias.scope !9
  %33 = add nsw <4 x i32> %27, %31
  %34 = add nsw <4 x i32> %28, %32
  %35 = getelementptr i8, ptr %19, i64 -1024
  %36 = getelementptr i8, ptr %19, i64 -1008
  %37 = load <4 x i32>, ptr %35, align 4, !tbaa !5, !alias.scope !9
  %38 = load <4 x i32>, ptr %36, align 4, !tbaa !5, !alias.scope !9
  %39 = add nsw <4 x i32> %33, %37
  %40 = add nsw <4 x i32> %34, %38
  %41 = getelementptr inbounds nuw i8, ptr %19, i64 1024
  %42 = getelementptr inbounds nuw i8, ptr %19, i64 1040
  %43 = load <4 x i32>, ptr %41, align 4, !tbaa !5, !alias.scope !9
  %44 = load <4 x i32>, ptr %42, align 4, !tbaa !5, !alias.scope !9
  %45 = add nsw <4 x i32> %39, %43
  %46 = add nsw <4 x i32> %40, %44
  %47 = sdiv <4 x i32> %45, splat (i32 5)
  %48 = sdiv <4 x i32> %46, splat (i32 5)
  %49 = getelementptr inbounds nuw i32, ptr %1, i64 %18
  %50 = getelementptr inbounds nuw i8, ptr %49, i64 16
  store <4 x i32> %47, ptr %49, align 4, !tbaa !5, !alias.scope !12, !noalias !9
  store <4 x i32> %48, ptr %50, align 4, !tbaa !5, !alias.scope !12, !noalias !9
  %51 = add nuw i64 %16, 8
  %52 = icmp eq i64 %51, 248
  br i1 %52, label %13, label %15, !llvm.loop !14

53:                                               ; preds = %54
  ret void

54:                                               ; preds = %57
  %55 = add nuw nsw i64 %11, 1
  %56 = icmp eq i64 %55, 127
  br i1 %56, label %53, label %10, !llvm.loop !18

57:                                               ; preds = %13, %57
  %58 = phi i64 [ %76, %57 ], [ %14, %13 ]
  %59 = add nuw nsw i64 %58, %12
  %60 = getelementptr inbounds nuw i32, ptr %0, i64 %59
  %61 = load i32, ptr %60, align 4, !tbaa !5
  %62 = getelementptr i8, ptr %60, i64 -4
  %63 = load i32, ptr %62, align 4, !tbaa !5
  %64 = add nsw i32 %63, %61
  %65 = getelementptr inbounds nuw i8, ptr %60, i64 4
  %66 = load i32, ptr %65, align 4, !tbaa !5
  %67 = add nsw i32 %64, %66
  %68 = getelementptr i8, ptr %60, i64 -1024
  %69 = load i32, ptr %68, align 4, !tbaa !5
  %70 = add nsw i32 %67, %69
  %71 = getelementptr inbounds nuw i8, ptr %60, i64 1024
  %72 = load i32, ptr %71, align 4, !tbaa !5
  %73 = add nsw i32 %70, %72
  %74 = sdiv i32 %73, 5
  %75 = getelementptr inbounds nuw i32, ptr %1, i64 %59
  store i32 %74, ptr %75, align 4, !tbaa !5
  %76 = add nuw nsw i64 %58, 1
  %77 = icmp eq i64 %76, 255
  br i1 %77, label %54, label %57, !llvm.loop !19
}

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: write) uwtable
define dso_local void @applyDisc(ptr noundef writeonly captures(none) %0, i32 noundef %1, i32 noundef %2, i32 noundef %3, i32 noundef %4) local_unnamed_addr #3 {
  %6 = sub nsw i32 0, %3
  %7 = add i32 %2, -1
  %8 = icmp slt i32 %3, 0
  br i1 %8, label %109, label %9

9:                                                ; preds = %5
  %10 = add i32 %1, -1
  %11 = mul nuw nsw i32 %3, %3
  %12 = shl nuw i32 %3, 1
  %13 = shl i32 %2, 8
  %14 = add i32 %1, %13
  %15 = mul i32 %3, -257
  %16 = add i32 %15, %14
  %17 = icmp ult i32 %3, 2
  %18 = and i32 %12, -4
  %19 = sub i32 %18, %3
  %20 = insertelement <4 x i32> poison, i32 %11, i64 0
  %21 = shufflevector <4 x i32> %20, <4 x i32> poison, <4 x i32> zeroinitializer
  %22 = insertelement <4 x i32> poison, i32 %10, i64 0
  %23 = shufflevector <4 x i32> %22, <4 x i32> poison, <4 x i32> zeroinitializer
  %24 = insertelement <4 x i32> poison, i32 %6, i64 0
  %25 = shufflevector <4 x i32> %24, <4 x i32> poison, <4 x i32> zeroinitializer
  %26 = add <4 x i32> %25, <i32 0, i32 1, i32 2, i32 3>
  br label %27

27:                                               ; preds = %110, %9
  %28 = phi i32 [ %113, %110 ], [ 0, %9 ]
  %29 = phi i32 [ %111, %110 ], [ %6, %9 ]
  %30 = add nsw i32 %29, %2
  %31 = mul nsw i32 %29, %29
  %32 = add i32 %29, %7
  %33 = icmp ult i32 %32, 126
  %34 = shl nuw nsw i32 %30, 8
  %35 = add i32 %1, %34
  br i1 %17, label %36, label %58

36:                                               ; preds = %105, %58, %27
  %37 = phi i32 [ %6, %58 ], [ %6, %27 ], [ %19, %105 ]
  %38 = add i32 %3, %37
  %39 = and i32 %38, 1
  %40 = icmp eq i32 %39, 0
  br i1 %40, label %41, label %55

41:                                               ; preds = %36
  %42 = mul nsw i32 %37, %37
  %43 = add nuw nsw i32 %42, %31
  %44 = icmp samesign ule i32 %43, %11
  %45 = add i32 %37, %10
  %46 = icmp ult i32 %45, 254
  %47 = and i1 %44, %46
  %48 = and i1 %47, %33
  br i1 %48, label %49, label %53

49:                                               ; preds = %41
  %50 = add i32 %37, %35
  %51 = zext nneg i32 %50 to i64
  %52 = getelementptr inbounds nuw i32, ptr %0, i64 %51
  store i32 %4, ptr %52, align 4, !tbaa !5
  br label %53

53:                                               ; preds = %49, %41
  %54 = add i32 %37, 1
  br label %55

55:                                               ; preds = %53, %36
  %56 = phi i32 [ %37, %36 ], [ %54, %53 ]
  %57 = icmp eq i32 %3, %37
  br i1 %57, label %110, label %114

58:                                               ; preds = %27
  %59 = shl i32 %28, 8
  %60 = add i32 %16, %59
  %61 = xor i32 %60, -1
  %62 = icmp ugt i32 %12, %61
  br i1 %62, label %36, label %63

63:                                               ; preds = %58
  %64 = insertelement <4 x i32> poison, i32 %31, i64 0
  %65 = shufflevector <4 x i32> %64, <4 x i32> poison, <4 x i32> zeroinitializer
  %66 = insertelement <4 x i1> poison, i1 %33, i64 0
  %67 = shufflevector <4 x i1> %66, <4 x i1> poison, <4 x i32> zeroinitializer
  %68 = add i32 1, %35
  %69 = add i32 2, %35
  %70 = add i32 3, %35
  br label %71

71:                                               ; preds = %105, %63
  %72 = phi i32 [ 0, %63 ], [ %106, %105 ]
  %73 = phi <4 x i32> [ %26, %63 ], [ %107, %105 ]
  %74 = sub i32 %72, %3
  %75 = mul nsw <4 x i32> %73, %73
  %76 = add nuw nsw <4 x i32> %75, %65
  %77 = icmp samesign ule <4 x i32> %76, %21
  %78 = add <4 x i32> %73, %23
  %79 = icmp ult <4 x i32> %78, splat (i32 254)
  %80 = and <4 x i1> %77, %79
  %81 = and <4 x i1> %80, %67
  %82 = extractelement <4 x i1> %81, i64 0
  br i1 %82, label %83, label %87

83:                                               ; preds = %71
  %84 = add i32 %74, %35
  %85 = zext nneg i32 %84 to i64
  %86 = getelementptr inbounds nuw i32, ptr %0, i64 %85
  store i32 %4, ptr %86, align 4, !tbaa !5
  br label %87

87:                                               ; preds = %83, %71
  %88 = extractelement <4 x i1> %81, i64 1
  br i1 %88, label %89, label %93

89:                                               ; preds = %87
  %90 = add i32 %74, %68
  %91 = zext nneg i32 %90 to i64
  %92 = getelementptr inbounds nuw i32, ptr %0, i64 %91
  store i32 %4, ptr %92, align 4, !tbaa !5
  br label %93

93:                                               ; preds = %89, %87
  %94 = extractelement <4 x i1> %81, i64 2
  br i1 %94, label %95, label %99

95:                                               ; preds = %93
  %96 = add i32 %74, %69
  %97 = zext nneg i32 %96 to i64
  %98 = getelementptr inbounds nuw i32, ptr %0, i64 %97
  store i32 %4, ptr %98, align 4, !tbaa !5
  br label %99

99:                                               ; preds = %95, %93
  %100 = extractelement <4 x i1> %81, i64 3
  br i1 %100, label %101, label %105

101:                                              ; preds = %99
  %102 = add i32 %74, %70
  %103 = zext nneg i32 %102 to i64
  %104 = getelementptr inbounds nuw i32, ptr %0, i64 %103
  store i32 %4, ptr %104, align 4, !tbaa !5
  br label %105

105:                                              ; preds = %101, %99
  %106 = add nuw i32 %72, 4
  %107 = add <4 x i32> %73, splat (i32 4)
  %108 = icmp eq i32 %106, %18
  br i1 %108, label %36, label %71, !llvm.loop !20

109:                                              ; preds = %110, %5
  ret void

110:                                              ; preds = %140, %55
  %111 = add i32 %29, 1
  %112 = icmp eq i32 %29, %3
  %113 = add i32 %28, 1
  br i1 %112, label %109, label %27, !llvm.loop !21

114:                                              ; preds = %55, %140
  %115 = phi i32 [ %141, %140 ], [ %56, %55 ]
  %116 = mul nsw i32 %115, %115
  %117 = add nuw nsw i32 %116, %31
  %118 = icmp samesign ule i32 %117, %11
  %119 = add i32 %115, %10
  %120 = icmp ult i32 %119, 254
  %121 = and i1 %118, %120
  %122 = and i1 %121, %33
  br i1 %122, label %123, label %127

123:                                              ; preds = %114
  %124 = add i32 %115, %35
  %125 = zext nneg i32 %124 to i64
  %126 = getelementptr inbounds nuw i32, ptr %0, i64 %125
  store i32 %4, ptr %126, align 4, !tbaa !5
  br label %127

127:                                              ; preds = %123, %114
  %128 = add i32 %115, 1
  %129 = mul nsw i32 %128, %128
  %130 = add nuw nsw i32 %129, %31
  %131 = icmp samesign ule i32 %130, %11
  %132 = add i32 %115, %1
  %133 = icmp ult i32 %132, 254
  %134 = and i1 %131, %133
  %135 = and i1 %134, %33
  br i1 %135, label %136, label %140

136:                                              ; preds = %127
  %137 = add i32 %128, %35
  %138 = zext nneg i32 %137 to i64
  %139 = getelementptr inbounds nuw i32, ptr %0, i64 %138
  store i32 %4, ptr %139, align 4, !tbaa !5
  br label %140

140:                                              ; preds = %136, %127
  %141 = add i32 %115, 2
  %142 = icmp eq i32 %128, %3
  br i1 %142, label %110, label %114, !llvm.loop !22
}

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @rotate(ptr noundef captures(none) %0, ptr noundef captures(none) %1, ptr noundef readonly captures(none) %2, i32 noundef %3) local_unnamed_addr #2 {
  %5 = icmp sgt i32 %3, 0
  br i1 %5, label %6, label %46

6:                                                ; preds = %4
  %7 = zext nneg i32 %3 to i64
  %8 = icmp ult i32 %3, 12
  br i1 %8, label %44, label %9

9:                                                ; preds = %6
  %10 = shl nuw nsw i64 %7, 2
  %11 = getelementptr i8, ptr %0, i64 %10
  %12 = getelementptr i8, ptr %1, i64 %10
  %13 = getelementptr i8, ptr %2, i64 %10
  %14 = icmp ult ptr %0, %12
  %15 = icmp ult ptr %1, %11
  %16 = and i1 %14, %15
  %17 = icmp ult ptr %0, %13
  %18 = icmp ult ptr %2, %11
  %19 = and i1 %17, %18
  %20 = or i1 %16, %19
  %21 = icmp ult ptr %1, %13
  %22 = icmp ult ptr %2, %12
  %23 = and i1 %21, %22
  %24 = or i1 %20, %23
  br i1 %24, label %44, label %25

25:                                               ; preds = %9
  %26 = and i64 %7, 2147483644
  br label %27

27:                                               ; preds = %27, %25
  %28 = phi i64 [ 0, %25 ], [ %40, %27 ]
  %29 = getelementptr inbounds nuw i32, ptr %1, i64 %28
  %30 = load <4 x i32>, ptr %29, align 4, !tbaa !5, !alias.scope !23, !noalias !26
  %31 = getelementptr inbounds nuw i32, ptr %2, i64 %28
  %32 = load <4 x i32>, ptr %31, align 4, !tbaa !5, !alias.scope !26
  %33 = ashr <4 x i32> %30, %32
  %34 = getelementptr inbounds nuw i32, ptr %0, i64 %28
  %35 = load <4 x i32>, ptr %34, align 4, !tbaa !5, !alias.scope !28, !noalias !30
  %36 = sub nsw <4 x i32> %35, %33
  store <4 x i32> %36, ptr %34, align 4, !tbaa !5, !alias.scope !28, !noalias !30
  %37 = ashr <4 x i32> %36, %32
  %38 = load <4 x i32>, ptr %29, align 4, !tbaa !5, !alias.scope !23, !noalias !26
  %39 = add nsw <4 x i32> %38, %37
  store <4 x i32> %39, ptr %29, align 4, !tbaa !5, !alias.scope !23, !noalias !26
  %40 = add nuw i64 %28, 4
  %41 = icmp eq i64 %40, %26
  br i1 %41, label %42, label %27, !llvm.loop !31

42:                                               ; preds = %27
  %43 = icmp eq i64 %26, %7
  br i1 %43, label %46, label %44

44:                                               ; preds = %9, %6, %42
  %45 = phi i64 [ 0, %9 ], [ 0, %6 ], [ %26, %42 ]
  br label %47

46:                                               ; preds = %47, %42, %4
  ret void

47:                                               ; preds = %44, %47
  %48 = phi i64 [ %61, %47 ], [ %45, %44 ]
  %49 = getelementptr inbounds nuw i32, ptr %1, i64 %48
  %50 = load i32, ptr %49, align 4, !tbaa !5
  %51 = getelementptr inbounds nuw i32, ptr %2, i64 %48
  %52 = load i32, ptr %51, align 4, !tbaa !5
  %53 = ashr i32 %50, %52
  %54 = getelementptr inbounds nuw i32, ptr %0, i64 %48
  %55 = load i32, ptr %54, align 4, !tbaa !5
  %56 = sub nsw i32 %55, %53
  store i32 %56, ptr %54, align 4, !tbaa !5
  %57 = load i32, ptr %51, align 4, !tbaa !5
  %58 = ashr i32 %56, %57
  %59 = load i32, ptr %49, align 4, !tbaa !5
  %60 = add nsw i32 %59, %58
  store i32 %60, ptr %49, align 4, !tbaa !5
  %61 = add nuw nsw i64 %48, 1
  %62 = icmp eq i64 %61, %7
  br i1 %62, label %46, label %47, !llvm.loop !32
}

; Function Attrs: nounwind uwtable
define dso_local void @drawField(ptr noundef readonly captures(none) %0) local_unnamed_addr #4 {
  br label %2

2:                                                ; preds = %1, %15
  %3 = phi i64 [ 0, %1 ], [ %16, %15 ]
  %4 = shl nuw nsw i64 %3, 2
  %5 = shl nuw nsw i64 %3, 10
  %6 = getelementptr inbounds nuw i8, ptr %0, i64 %5
  %7 = trunc nuw nsw i64 %4 to i32
  %8 = trunc i64 %4 to i32
  %9 = or disjoint i32 %8, 1
  %10 = trunc i64 %4 to i32
  %11 = or disjoint i32 %10, 2
  %12 = trunc i64 %4 to i32
  %13 = or disjoint i32 %12, 3
  br label %18

14:                                               ; preds = %15
  ret void

15:                                               ; preds = %18
  %16 = add nuw nsw i64 %3, 1
  %17 = icmp eq i64 %16, 128
  br i1 %17, label %14, label %2, !llvm.loop !33

18:                                               ; preds = %2, %18
  %19 = phi i64 [ 0, %2 ], [ %69, %18 ]
  %20 = getelementptr inbounds nuw i32, ptr %6, i64 %19
  %21 = load i32, ptr %20, align 4, !tbaa !5
  %22 = mul nsw i32 %21, 6
  %23 = ashr i32 %22, 16
  %24 = tail call i32 @llvm.smin.i32(i32 %23, i32 5)
  %25 = shl nsw i32 %24, 16
  %26 = sub nsw i32 %22, %25
  %27 = sext i32 %24 to i64
  %28 = getelementptr inbounds [7 x i32], ptr @__const.heatColor.pal, i64 0, i64 %27
  %29 = load i32, ptr %28, align 4, !tbaa !5
  %30 = add nsw i32 %24, 1
  %31 = sext i32 %30 to i64
  %32 = getelementptr inbounds [7 x i32], ptr @__const.heatColor.pal, i64 0, i64 %31
  %33 = load i32, ptr %32, align 4, !tbaa !5
  %34 = and i32 %29, 255
  %35 = and i32 %33, 255
  %36 = sub nsw i32 %35, %34
  %37 = mul nsw i32 %26, %36
  %38 = ashr i32 %37, 16
  %39 = add nsw i32 %38, %34
  %40 = lshr i32 %29, 8
  %41 = and i32 %40, 255
  %42 = lshr i32 %33, 8
  %43 = and i32 %42, 255
  %44 = sub nsw i32 %43, %41
  %45 = mul nsw i32 %44, %26
  %46 = ashr i32 %45, 16
  %47 = add nsw i32 %46, %41
  %48 = shl nsw i32 %47, 8
  %49 = lshr i32 %29, 16
  %50 = and i32 %49, 255
  %51 = lshr i32 %33, 16
  %52 = and i32 %51, 255
  %53 = sub nsw i32 %52, %50
  %54 = mul nsw i32 %53, %26
  %55 = shl nuw nsw i32 %50, 16
  %56 = add i32 %54, %55
  %57 = and i32 %56, 16711680
  %58 = or i32 %39, %57
  %59 = or i32 %58, %48
  %60 = or i32 %59, -16777216
  %61 = shl nuw nsw i64 %19, 2
  %62 = trunc nuw nsw i64 %61 to i32
  tail call void @simPutPixel(i32 noundef %62, i32 noundef %7, i32 noundef %60) #9
  %63 = trunc i64 %61 to i32
  %64 = or disjoint i32 %63, 1
  tail call void @simPutPixel(i32 noundef %64, i32 noundef %7, i32 noundef %60) #9
  %65 = trunc i64 %61 to i32
  %66 = or disjoint i32 %65, 2
  tail call void @simPutPixel(i32 noundef %66, i32 noundef %7, i32 noundef %60) #9
  %67 = trunc i64 %61 to i32
  %68 = or disjoint i32 %67, 3
  tail call void @simPutPixel(i32 noundef %68, i32 noundef %7, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %62, i32 noundef %9, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %64, i32 noundef %9, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %66, i32 noundef %9, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %68, i32 noundef %9, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %62, i32 noundef %11, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %64, i32 noundef %11, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %66, i32 noundef %11, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %68, i32 noundef %11, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %62, i32 noundef %13, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %64, i32 noundef %13, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %66, i32 noundef %13, i32 noundef %60) #9
  tail call void @simPutPixel(i32 noundef %68, i32 noundef %13, i32 noundef %60) #9
  %69 = add nuw nsw i64 %19, 1
  %70 = icmp eq i64 %69, 256
  br i1 %70, label %15, label %18, !llvm.loop !34
}

declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #5

; Function Attrs: noreturn nounwind uwtable
define dso_local void @app() local_unnamed_addr #6 {
  %1 = alloca [32768 x i32], align 16
  %2 = alloca [32768 x i32], align 16
  %3 = alloca [6 x i32], align 16
  %4 = alloca [6 x i32], align 16
  %5 = alloca [6 x i32], align 16
  %6 = alloca [6 x i32], align 16
  %7 = alloca [6 x i32], align 16
  %8 = alloca [6 x i32], align 16
  %9 = alloca [6 x i32], align 16
  call void @llvm.lifetime.start.p0(i64 131072, ptr nonnull %1) #9
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(131072) %1, i8 0, i64 131072, i1 false)
  call void @llvm.lifetime.start.p0(i64 131072, ptr nonnull %2) #9
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 16 dereferenceable(131072) %2, i8 0, i64 131072, i1 false)
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %3) #9
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %4) #9
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %5) #9
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %6) #9
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %7) #9
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %8) #9
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %9) #9
  br label %10

10:                                               ; preds = %0, %10
  %11 = phi i64 [ 0, %0 ], [ %39, %10 ]
  %12 = trunc i64 %11 to i32
  %13 = tail call i32 (...) @simRand() #9
  %14 = srem i32 %13, 64
  %15 = shl nsw i32 %14, 8
  %16 = sub nsw i32 31232, %15
  %17 = getelementptr inbounds nuw [6 x i32], ptr %3, i64 0, i64 %11
  store i32 %16, ptr %17, align 4, !tbaa !5
  %18 = getelementptr inbounds nuw [6 x i32], ptr %4, i64 0, i64 %11
  store i32 0, ptr %18, align 4, !tbaa !5
  %19 = tail call i32 (...) @simRand() #9
  %20 = srem i32 %19, 3
  %21 = add nsw i32 %20, 4
  %22 = getelementptr inbounds nuw [6 x i32], ptr %5, i64 0, i64 %11
  store i32 %21, ptr %22, align 4, !tbaa !5
  %23 = getelementptr inbounds nuw [6 x i32], ptr %6, i64 0, i64 %11
  store i32 0, ptr %23, align 4, !tbaa !5
  %24 = tail call i32 (...) @simRand() #9
  %25 = srem i32 %24, 32
  %26 = shl nsw i32 %25, 8
  %27 = sub nsw i32 14848, %26
  %28 = getelementptr inbounds nuw [6 x i32], ptr %7, i64 0, i64 %11
  store i32 %27, ptr %28, align 4, !tbaa !5
  %29 = tail call i32 (...) @simRand() #9
  %30 = srem i32 %29, 3
  %31 = add nsw i32 %30, 4
  %32 = getelementptr inbounds nuw [6 x i32], ptr %8, i64 0, i64 %11
  store i32 %31, ptr %32, align 4, !tbaa !5
  %33 = add nsw i32 %12, -3
  %34 = icmp samesign ult i64 %11, 3
  %35 = select i1 %34, i32 %12, i32 %33
  %36 = icmp eq i32 %35, 2
  %37 = select i1 %36, i32 0, i32 65535
  %38 = getelementptr inbounds nuw [6 x i32], ptr %9, i64 0, i64 %11
  store i32 %37, ptr %38, align 4, !tbaa !5
  %39 = add nuw nsw i64 %11, 1
  %40 = icmp eq i64 %39, 6
  br i1 %40, label %41, label %10, !llvm.loop !35

41:                                               ; preds = %10
  %42 = getelementptr inbounds nuw i8, ptr %4, i64 4
  %43 = getelementptr inbounds nuw i8, ptr %5, i64 4
  %44 = getelementptr inbounds nuw i8, ptr %3, i64 4
  %45 = getelementptr inbounds nuw i8, ptr %4, i64 8
  %46 = getelementptr inbounds nuw i8, ptr %5, i64 8
  %47 = getelementptr inbounds nuw i8, ptr %3, i64 8
  %48 = getelementptr inbounds nuw i8, ptr %4, i64 12
  %49 = getelementptr inbounds nuw i8, ptr %5, i64 12
  %50 = getelementptr inbounds nuw i8, ptr %3, i64 12
  %51 = getelementptr inbounds nuw i8, ptr %4, i64 16
  %52 = getelementptr inbounds nuw i8, ptr %5, i64 16
  %53 = getelementptr inbounds nuw i8, ptr %3, i64 16
  %54 = getelementptr inbounds nuw i8, ptr %4, i64 20
  %55 = getelementptr inbounds nuw i8, ptr %5, i64 20
  %56 = getelementptr inbounds nuw i8, ptr %3, i64 20
  %57 = getelementptr inbounds nuw i8, ptr %7, i64 4
  %58 = getelementptr inbounds nuw i8, ptr %8, i64 4
  %59 = getelementptr inbounds nuw i8, ptr %6, i64 4
  %60 = getelementptr inbounds nuw i8, ptr %7, i64 8
  %61 = getelementptr inbounds nuw i8, ptr %8, i64 8
  %62 = getelementptr inbounds nuw i8, ptr %6, i64 8
  %63 = getelementptr inbounds nuw i8, ptr %7, i64 12
  %64 = getelementptr inbounds nuw i8, ptr %8, i64 12
  %65 = getelementptr inbounds nuw i8, ptr %6, i64 12
  %66 = getelementptr inbounds nuw i8, ptr %7, i64 16
  %67 = getelementptr inbounds nuw i8, ptr %8, i64 16
  %68 = getelementptr inbounds nuw i8, ptr %6, i64 16
  %69 = getelementptr inbounds nuw i8, ptr %7, i64 20
  %70 = getelementptr inbounds nuw i8, ptr %8, i64 20
  %71 = getelementptr inbounds nuw i8, ptr %6, i64 20
  br label %72

72:                                               ; preds = %41, %474
  %73 = phi ptr [ %77, %474 ], [ %2, %41 ]
  %74 = phi i32 [ %559, %474 ], [ 0, %41 ]
  %75 = phi ptr [ %79, %474 ], [ %1, %41 ]
  br label %76

76:                                               ; preds = %72, %152
  %77 = phi ptr [ %75, %72 ], [ %79, %152 ]
  %78 = phi i32 [ 0, %72 ], [ %153, %152 ]
  %79 = phi ptr [ %73, %72 ], [ %77, %152 ]
  %80 = ptrtoint ptr %79 to i64
  %81 = ptrtoint ptr %77 to i64
  br label %155

82:                                               ; preds = %152
  %83 = and i32 %74, 15
  %84 = icmp eq i32 %83, 0
  br i1 %84, label %284, label %474

85:                                               ; preds = %265, %128
  %86 = phi i64 [ %129, %128 ], [ 1, %265 ]
  %87 = shl nuw nsw i64 %86, 8
  br i1 %283, label %88, label %90

88:                                               ; preds = %90, %85
  %89 = phi i64 [ 1, %85 ], [ 249, %90 ]
  br label %131

90:                                               ; preds = %85, %90
  %91 = phi i64 [ %126, %90 ], [ 0, %85 ]
  %92 = or disjoint i64 %91, 1
  %93 = add nuw nsw i64 %92, %87
  %94 = getelementptr inbounds nuw i32, ptr %77, i64 %93
  %95 = getelementptr inbounds nuw i8, ptr %94, i64 16
  %96 = load <4 x i32>, ptr %94, align 4, !tbaa !5
  %97 = load <4 x i32>, ptr %95, align 4, !tbaa !5
  %98 = getelementptr i8, ptr %94, i64 -4
  %99 = getelementptr i8, ptr %94, i64 12
  %100 = load <4 x i32>, ptr %98, align 4, !tbaa !5
  %101 = load <4 x i32>, ptr %99, align 4, !tbaa !5
  %102 = add nsw <4 x i32> %100, %96
  %103 = add nsw <4 x i32> %101, %97
  %104 = getelementptr inbounds nuw i8, ptr %94, i64 4
  %105 = getelementptr inbounds nuw i8, ptr %94, i64 20
  %106 = load <4 x i32>, ptr %104, align 4, !tbaa !5
  %107 = load <4 x i32>, ptr %105, align 4, !tbaa !5
  %108 = add nsw <4 x i32> %102, %106
  %109 = add nsw <4 x i32> %103, %107
  %110 = getelementptr i8, ptr %94, i64 -1024
  %111 = getelementptr i8, ptr %94, i64 -1008
  %112 = load <4 x i32>, ptr %110, align 4, !tbaa !5
  %113 = load <4 x i32>, ptr %111, align 4, !tbaa !5
  %114 = add nsw <4 x i32> %108, %112
  %115 = add nsw <4 x i32> %109, %113
  %116 = getelementptr inbounds nuw i8, ptr %94, i64 1024
  %117 = getelementptr inbounds nuw i8, ptr %94, i64 1040
  %118 = load <4 x i32>, ptr %116, align 4, !tbaa !5
  %119 = load <4 x i32>, ptr %117, align 4, !tbaa !5
  %120 = add nsw <4 x i32> %114, %118
  %121 = add nsw <4 x i32> %115, %119
  %122 = sdiv <4 x i32> %120, splat (i32 5)
  %123 = sdiv <4 x i32> %121, splat (i32 5)
  %124 = getelementptr inbounds nuw i32, ptr %79, i64 %93
  %125 = getelementptr inbounds nuw i8, ptr %124, i64 16
  store <4 x i32> %122, ptr %124, align 4, !tbaa !5
  store <4 x i32> %123, ptr %125, align 4, !tbaa !5
  %126 = add nuw i64 %91, 8
  %127 = icmp eq i64 %126, 248
  br i1 %127, label %88, label %90, !llvm.loop !36

128:                                              ; preds = %131
  %129 = add nuw nsw i64 %86, 1
  %130 = icmp eq i64 %129, 127
  br i1 %130, label %152, label %85, !llvm.loop !18

131:                                              ; preds = %88, %131
  %132 = phi i64 [ %150, %131 ], [ %89, %88 ]
  %133 = add nuw nsw i64 %132, %87
  %134 = getelementptr inbounds nuw i32, ptr %77, i64 %133
  %135 = load i32, ptr %134, align 4, !tbaa !5
  %136 = getelementptr i8, ptr %134, i64 -4
  %137 = load i32, ptr %136, align 4, !tbaa !5
  %138 = add nsw i32 %137, %135
  %139 = getelementptr inbounds nuw i8, ptr %134, i64 4
  %140 = load i32, ptr %139, align 4, !tbaa !5
  %141 = add nsw i32 %138, %140
  %142 = getelementptr i8, ptr %134, i64 -1024
  %143 = load i32, ptr %142, align 4, !tbaa !5
  %144 = add nsw i32 %141, %143
  %145 = getelementptr inbounds nuw i8, ptr %134, i64 1024
  %146 = load i32, ptr %145, align 4, !tbaa !5
  %147 = add nsw i32 %144, %146
  %148 = sdiv i32 %147, 5
  %149 = getelementptr inbounds nuw i32, ptr %79, i64 %133
  store i32 %148, ptr %149, align 4, !tbaa !5
  %150 = add nuw nsw i64 %132, 1
  %151 = icmp eq i64 %150, 255
  br i1 %151, label %128, label %131, !llvm.loop !37

152:                                              ; preds = %128
  %153 = add nuw nsw i32 %78, 1
  %154 = icmp eq i32 %153, 4
  br i1 %154, label %82, label %76, !llvm.loop !38

155:                                              ; preds = %76, %262
  %156 = phi i64 [ 0, %76 ], [ %263, %262 ]
  %157 = getelementptr inbounds nuw [6 x i32], ptr %3, i64 0, i64 %156
  %158 = load i32, ptr %157, align 4, !tbaa !5
  %159 = ashr i32 %158, 8
  %160 = add nsw i32 %159, 128
  %161 = getelementptr inbounds nuw [6 x i32], ptr %7, i64 0, i64 %156
  %162 = load i32, ptr %161, align 4, !tbaa !5
  %163 = ashr i32 %162, 8
  %164 = add nsw i32 %163, 64
  %165 = getelementptr inbounds nuw [6 x i32], ptr %9, i64 0, i64 %156
  %166 = load i32, ptr %165, align 4, !tbaa !5
  %167 = add nsw i32 %163, 63
  %168 = add nsw i32 %159, 127
  %169 = add nsw i32 %159, 123
  %170 = icmp ult i32 %169, 254
  %171 = add nsw i32 %159, 124
  %172 = icmp ult i32 %171, 254
  %173 = add nsw i32 %159, 125
  %174 = icmp ult i32 %173, 254
  %175 = add nsw i32 %159, 126
  %176 = icmp ult i32 %175, 254
  %177 = icmp ult i32 %168, 254
  %178 = icmp ult i32 %160, 254
  %179 = add nsw i32 %159, 129
  %180 = icmp ult i32 %179, 254
  %181 = add nsw i32 %159, 130
  %182 = icmp ult i32 %181, 254
  %183 = add nsw i32 %159, 131
  %184 = icmp ult i32 %183, 254
  br label %185

185:                                              ; preds = %259, %155
  %186 = phi i32 [ -4, %155 ], [ %260, %259 ]
  %187 = add nsw i32 %164, %186
  %188 = mul nsw i32 %186, %186
  %189 = add nsw i32 %167, %186
  %190 = icmp ult i32 %189, 126
  %191 = shl nuw nsw i32 %187, 8
  %192 = add i32 %160, %191
  %193 = icmp eq i32 %186, 0
  %194 = and i1 %170, %193
  %195 = and i1 %190, %194
  br i1 %195, label %196, label %200

196:                                              ; preds = %185
  %197 = add i32 %192, -4
  %198 = zext nneg i32 %197 to i64
  %199 = getelementptr inbounds nuw i32, ptr %77, i64 %198
  store i32 %166, ptr %199, align 4, !tbaa !5
  br label %200

200:                                              ; preds = %196, %185
  %201 = icmp samesign ult i32 %188, 8
  %202 = and i1 %172, %201
  %203 = and i1 %190, %202
  br i1 %203, label %204, label %208

204:                                              ; preds = %200
  %205 = add i32 %192, -3
  %206 = zext nneg i32 %205 to i64
  %207 = getelementptr inbounds nuw i32, ptr %77, i64 %206
  store i32 %166, ptr %207, align 4, !tbaa !5
  br label %208

208:                                              ; preds = %204, %200
  %209 = icmp samesign ult i32 %188, 13
  %210 = and i1 %174, %209
  %211 = and i1 %190, %210
  br i1 %211, label %212, label %216

212:                                              ; preds = %208
  %213 = add i32 %192, -2
  %214 = zext nneg i32 %213 to i64
  %215 = getelementptr inbounds nuw i32, ptr %77, i64 %214
  store i32 %166, ptr %215, align 4, !tbaa !5
  br label %216

216:                                              ; preds = %212, %208
  %217 = icmp samesign ult i32 %188, 16
  %218 = and i1 %176, %217
  %219 = and i1 %190, %218
  br i1 %219, label %220, label %224

220:                                              ; preds = %216
  %221 = add i32 %192, -1
  %222 = zext nneg i32 %221 to i64
  %223 = getelementptr inbounds nuw i32, ptr %77, i64 %222
  store i32 %166, ptr %223, align 4, !tbaa !5
  br label %224

224:                                              ; preds = %220, %216
  %225 = icmp samesign ult i32 %188, 17
  %226 = and i1 %177, %225
  %227 = and i1 %190, %226
  br i1 %227, label %228, label %231

228:                                              ; preds = %224
  %229 = zext nneg i32 %192 to i64
  %230 = getelementptr inbounds nuw i32, ptr %77, i64 %229
  store i32 %166, ptr %230, align 4, !tbaa !5
  br label %231

231:                                              ; preds = %228, %224
  %232 = and i1 %178, %217
  %233 = and i1 %190, %232
  br i1 %233, label %234, label %238

234:                                              ; preds = %231
  %235 = add nuw i32 %192, 1
  %236 = zext nneg i32 %235 to i64
  %237 = getelementptr inbounds nuw i32, ptr %77, i64 %236
  store i32 %166, ptr %237, align 4, !tbaa !5
  br label %238

238:                                              ; preds = %234, %231
  %239 = and i1 %180, %209
  %240 = and i1 %190, %239
  br i1 %240, label %241, label %245

241:                                              ; preds = %238
  %242 = add i32 %192, 2
  %243 = zext nneg i32 %242 to i64
  %244 = getelementptr inbounds nuw i32, ptr %77, i64 %243
  store i32 %166, ptr %244, align 4, !tbaa !5
  br label %245

245:                                              ; preds = %241, %238
  %246 = and i1 %182, %201
  %247 = and i1 %190, %246
  br i1 %247, label %248, label %252

248:                                              ; preds = %245
  %249 = add i32 %192, 3
  %250 = zext nneg i32 %249 to i64
  %251 = getelementptr inbounds nuw i32, ptr %77, i64 %250
  store i32 %166, ptr %251, align 4, !tbaa !5
  br label %252

252:                                              ; preds = %248, %245
  %253 = and i1 %184, %193
  %254 = and i1 %190, %253
  br i1 %254, label %255, label %259

255:                                              ; preds = %252
  %256 = add i32 %192, 4
  %257 = zext nneg i32 %256 to i64
  %258 = getelementptr inbounds nuw i32, ptr %77, i64 %257
  store i32 %166, ptr %258, align 4, !tbaa !5
  br label %259

259:                                              ; preds = %255, %252
  %260 = add nsw i32 %186, 1
  %261 = icmp eq i32 %186, 4
  br i1 %261, label %262, label %185, !llvm.loop !21

262:                                              ; preds = %259
  %263 = add nuw nsw i64 %156, 1
  %264 = icmp eq i64 %263, 6
  br i1 %264, label %265, label %155, !llvm.loop !39

265:                                              ; preds = %262
  %266 = sub i64 %80, %81
  %267 = add i64 %80, 4
  %268 = sub i64 %267, %81
  %269 = add i64 %80, -4
  %270 = sub i64 %269, %81
  %271 = add i64 %80, 1024
  %272 = sub i64 %271, %81
  %273 = add i64 %80, -1024
  %274 = sub i64 %273, %81
  %275 = icmp ult i64 %266, 32
  %276 = icmp ult i64 %268, 32
  %277 = or i1 %275, %276
  %278 = icmp ult i64 %270, 32
  %279 = or i1 %277, %278
  %280 = icmp ult i64 %272, 32
  %281 = or i1 %279, %280
  %282 = icmp ult i64 %274, 32
  %283 = or i1 %281, %282
  br label %85

284:                                              ; preds = %82
  %285 = tail call i32 (...) @simRand() #9
  %286 = srem i32 %285, 254
  %287 = add nsw i32 %286, 1
  %288 = tail call i32 (...) @simRand() #9
  %289 = srem i32 %288, 126
  %290 = add nsw i32 %289, 1
  %291 = add i32 %286, -1799
  %292 = insertelement <4 x i32> poison, i32 %286, i64 0
  %293 = shufflevector <4 x i32> %292, <4 x i32> poison, <4 x i32> zeroinitializer
  %294 = add nsw <4 x i32> %293, <i32 -8, i32 -7, i32 -6, i32 -5>
  %295 = icmp ult <4 x i32> %294, splat (i32 254)
  %296 = add nsw <4 x i32> %293, <i32 -4, i32 -3, i32 -2, i32 -1>
  %297 = icmp ult <4 x i32> %296, splat (i32 254)
  %298 = add nsw <4 x i32> %293, <i32 0, i32 1, i32 2, i32 3>
  %299 = icmp ult <4 x i32> %298, splat (i32 254)
  %300 = add nsw <4 x i32> %293, <i32 4, i32 5, i32 6, i32 7>
  %301 = icmp ult <4 x i32> %300, splat (i32 254)
  br label %302

302:                                              ; preds = %441, %284
  %303 = phi i32 [ %444, %441 ], [ 0, %284 ]
  %304 = phi i32 [ %442, %441 ], [ -8, %284 ]
  %305 = add nsw i32 %290, %304
  %306 = mul nsw i32 %304, %304
  %307 = add nsw i32 %304, %289
  %308 = icmp ult i32 %307, 126
  %309 = shl nuw nsw i32 %305, 8
  %310 = add i32 %287, %309
  %311 = add i32 %289, %303
  %312 = shl i32 %311, 8
  %313 = add i32 %312, %291
  %314 = icmp ult i32 %313, -16
  br i1 %314, label %330, label %315

315:                                              ; preds = %435, %437, %302
  %316 = phi i32 [ -8, %302 ], [ 8, %437 ], [ 8, %435 ]
  %317 = mul nsw i32 %316, %316
  %318 = add nuw nsw i32 %317, %306
  %319 = icmp samesign ult i32 %318, 65
  %320 = add nsw i32 %316, %286
  %321 = icmp ult i32 %320, 254
  %322 = and i1 %321, %319
  %323 = and i1 %308, %322
  br i1 %323, label %324, label %328

324:                                              ; preds = %315
  %325 = add i32 %310, %316
  %326 = zext nneg i32 %325 to i64
  %327 = getelementptr inbounds nuw i32, ptr %79, i64 %326
  store i32 65535, ptr %327, align 4, !tbaa !5
  br label %328

328:                                              ; preds = %324, %315
  %329 = or disjoint i32 %316, 1
  br i1 %314, label %441, label %445

330:                                              ; preds = %302
  %331 = insertelement <4 x i32> poison, i32 %306, i64 0
  %332 = shufflevector <4 x i32> %331, <4 x i32> poison, <4 x i32> zeroinitializer
  %333 = insertelement <4 x i1> poison, i1 %308, i64 0
  %334 = shufflevector <4 x i1> %333, <4 x i1> poison, <4 x i32> zeroinitializer
  %335 = icmp samesign ult <4 x i32> %332, <i32 1, i32 16, i32 29, i32 40>
  %336 = and <4 x i1> %295, %335
  %337 = and <4 x i1> %334, %336
  %338 = extractelement <4 x i1> %337, i64 0
  br i1 %338, label %339, label %343

339:                                              ; preds = %330
  %340 = add i32 %310, -8
  %341 = zext nneg i32 %340 to i64
  %342 = getelementptr inbounds nuw i32, ptr %79, i64 %341
  store i32 65535, ptr %342, align 4, !tbaa !5
  br label %343

343:                                              ; preds = %339, %330
  %344 = extractelement <4 x i1> %337, i64 1
  br i1 %344, label %345, label %349

345:                                              ; preds = %343
  %346 = add i32 %310, -7
  %347 = zext nneg i32 %346 to i64
  %348 = getelementptr inbounds nuw i32, ptr %79, i64 %347
  store i32 65535, ptr %348, align 4, !tbaa !5
  br label %349

349:                                              ; preds = %345, %343
  %350 = extractelement <4 x i1> %337, i64 2
  br i1 %350, label %351, label %355

351:                                              ; preds = %349
  %352 = add i32 %310, -6
  %353 = zext nneg i32 %352 to i64
  %354 = getelementptr inbounds nuw i32, ptr %79, i64 %353
  store i32 65535, ptr %354, align 4, !tbaa !5
  br label %355

355:                                              ; preds = %351, %349
  %356 = extractelement <4 x i1> %337, i64 3
  br i1 %356, label %357, label %361

357:                                              ; preds = %355
  %358 = add i32 %310, -5
  %359 = zext nneg i32 %358 to i64
  %360 = getelementptr inbounds nuw i32, ptr %79, i64 %359
  store i32 65535, ptr %360, align 4, !tbaa !5
  br label %361

361:                                              ; preds = %357, %355
  %362 = icmp samesign ult <4 x i32> %332, <i32 49, i32 56, i32 61, i32 64>
  %363 = and <4 x i1> %297, %362
  %364 = and <4 x i1> %334, %363
  %365 = extractelement <4 x i1> %364, i64 0
  br i1 %365, label %366, label %370

366:                                              ; preds = %361
  %367 = add i32 %310, -4
  %368 = zext nneg i32 %367 to i64
  %369 = getelementptr inbounds nuw i32, ptr %79, i64 %368
  store i32 65535, ptr %369, align 4, !tbaa !5
  br label %370

370:                                              ; preds = %366, %361
  %371 = extractelement <4 x i1> %364, i64 1
  br i1 %371, label %372, label %376

372:                                              ; preds = %370
  %373 = add i32 %310, -3
  %374 = zext nneg i32 %373 to i64
  %375 = getelementptr inbounds nuw i32, ptr %79, i64 %374
  store i32 65535, ptr %375, align 4, !tbaa !5
  br label %376

376:                                              ; preds = %372, %370
  %377 = extractelement <4 x i1> %364, i64 2
  br i1 %377, label %378, label %382

378:                                              ; preds = %376
  %379 = add i32 %310, -2
  %380 = zext nneg i32 %379 to i64
  %381 = getelementptr inbounds nuw i32, ptr %79, i64 %380
  store i32 65535, ptr %381, align 4, !tbaa !5
  br label %382

382:                                              ; preds = %378, %376
  %383 = extractelement <4 x i1> %364, i64 3
  br i1 %383, label %384, label %388

384:                                              ; preds = %382
  %385 = add nsw i32 %286, %309
  %386 = zext nneg i32 %385 to i64
  %387 = getelementptr inbounds nuw i32, ptr %79, i64 %386
  store i32 65535, ptr %387, align 4, !tbaa !5
  br label %388

388:                                              ; preds = %384, %382
  %389 = icmp samesign ult <4 x i32> %332, <i32 65, i32 64, i32 61, i32 56>
  %390 = and <4 x i1> %299, %389
  %391 = and <4 x i1> %334, %390
  %392 = extractelement <4 x i1> %391, i64 0
  br i1 %392, label %393, label %396

393:                                              ; preds = %388
  %394 = zext nneg i32 %310 to i64
  %395 = getelementptr inbounds nuw i32, ptr %79, i64 %394
  store i32 65535, ptr %395, align 4, !tbaa !5
  br label %396

396:                                              ; preds = %393, %388
  %397 = extractelement <4 x i1> %391, i64 1
  br i1 %397, label %398, label %402

398:                                              ; preds = %396
  %399 = add i32 %310, 1
  %400 = zext nneg i32 %399 to i64
  %401 = getelementptr inbounds nuw i32, ptr %79, i64 %400
  store i32 65535, ptr %401, align 4, !tbaa !5
  br label %402

402:                                              ; preds = %398, %396
  %403 = extractelement <4 x i1> %391, i64 2
  br i1 %403, label %404, label %408

404:                                              ; preds = %402
  %405 = add i32 %310, 2
  %406 = zext nneg i32 %405 to i64
  %407 = getelementptr inbounds nuw i32, ptr %79, i64 %406
  store i32 65535, ptr %407, align 4, !tbaa !5
  br label %408

408:                                              ; preds = %404, %402
  %409 = extractelement <4 x i1> %391, i64 3
  br i1 %409, label %410, label %414

410:                                              ; preds = %408
  %411 = add i32 %310, 3
  %412 = zext nneg i32 %411 to i64
  %413 = getelementptr inbounds nuw i32, ptr %79, i64 %412
  store i32 65535, ptr %413, align 4, !tbaa !5
  br label %414

414:                                              ; preds = %410, %408
  %415 = icmp samesign ult <4 x i32> %332, <i32 49, i32 40, i32 29, i32 16>
  %416 = and <4 x i1> %301, %415
  %417 = and <4 x i1> %334, %416
  %418 = extractelement <4 x i1> %417, i64 0
  br i1 %418, label %419, label %423

419:                                              ; preds = %414
  %420 = add i32 %310, 4
  %421 = zext nneg i32 %420 to i64
  %422 = getelementptr inbounds nuw i32, ptr %79, i64 %421
  store i32 65535, ptr %422, align 4, !tbaa !5
  br label %423

423:                                              ; preds = %419, %414
  %424 = extractelement <4 x i1> %417, i64 1
  br i1 %424, label %425, label %429

425:                                              ; preds = %423
  %426 = add i32 %310, 5
  %427 = zext nneg i32 %426 to i64
  %428 = getelementptr inbounds nuw i32, ptr %79, i64 %427
  store i32 65535, ptr %428, align 4, !tbaa !5
  br label %429

429:                                              ; preds = %425, %423
  %430 = extractelement <4 x i1> %417, i64 2
  br i1 %430, label %431, label %435

431:                                              ; preds = %429
  %432 = add i32 %310, 6
  %433 = zext nneg i32 %432 to i64
  %434 = getelementptr inbounds nuw i32, ptr %79, i64 %433
  store i32 65535, ptr %434, align 4, !tbaa !5
  br label %435

435:                                              ; preds = %431, %429
  %436 = extractelement <4 x i1> %417, i64 3
  br i1 %436, label %437, label %315

437:                                              ; preds = %435
  %438 = add i32 %310, 7
  %439 = zext nneg i32 %438 to i64
  %440 = getelementptr inbounds nuw i32, ptr %79, i64 %439
  store i32 65535, ptr %440, align 4, !tbaa !5
  br label %315

441:                                              ; preds = %471, %328
  %442 = add nsw i32 %304, 1
  %443 = icmp eq i32 %304, 8
  %444 = add i32 %303, 1
  br i1 %443, label %474, label %302, !llvm.loop !21

445:                                              ; preds = %328, %471
  %446 = phi i32 [ %472, %471 ], [ %329, %328 ]
  %447 = mul nsw i32 %446, %446
  %448 = add nuw nsw i32 %447, %306
  %449 = icmp samesign ult i32 %448, 65
  %450 = add nsw i32 %446, %286
  %451 = icmp ult i32 %450, 254
  %452 = and i1 %451, %449
  %453 = and i1 %308, %452
  br i1 %453, label %454, label %458

454:                                              ; preds = %445
  %455 = add i32 %310, %446
  %456 = zext nneg i32 %455 to i64
  %457 = getelementptr inbounds nuw i32, ptr %79, i64 %456
  store i32 65535, ptr %457, align 4, !tbaa !5
  br label %458

458:                                              ; preds = %454, %445
  %459 = add nsw i32 %446, 1
  %460 = mul nsw i32 %459, %459
  %461 = add nuw nsw i32 %460, %306
  %462 = icmp samesign ult i32 %461, 65
  %463 = add nsw i32 %459, %286
  %464 = icmp ult i32 %463, 254
  %465 = and i1 %464, %462
  %466 = and i1 %308, %465
  br i1 %466, label %467, label %471

467:                                              ; preds = %458
  %468 = add i32 %310, %459
  %469 = zext nneg i32 %468 to i64
  %470 = getelementptr inbounds nuw i32, ptr %79, i64 %469
  store i32 65535, ptr %470, align 4, !tbaa !5
  br label %471

471:                                              ; preds = %467, %458
  %472 = add nsw i32 %446, 2
  %473 = icmp eq i32 %459, 8
  br i1 %473, label %441, label %445, !llvm.loop !40

474:                                              ; preds = %441, %82
  %475 = load i32, ptr %4, align 16, !tbaa !5
  %476 = load i32, ptr %5, align 16, !tbaa !5
  %477 = ashr i32 %475, %476
  %478 = load i32, ptr %3, align 16, !tbaa !5
  %479 = sub nsw i32 %478, %477
  store i32 %479, ptr %3, align 16, !tbaa !5
  %480 = ashr i32 %479, %476
  %481 = add nsw i32 %480, %475
  store i32 %481, ptr %4, align 16, !tbaa !5
  %482 = load i32, ptr %42, align 4, !tbaa !5
  %483 = load i32, ptr %43, align 4, !tbaa !5
  %484 = ashr i32 %482, %483
  %485 = load i32, ptr %44, align 4, !tbaa !5
  %486 = sub nsw i32 %485, %484
  store i32 %486, ptr %44, align 4, !tbaa !5
  %487 = ashr i32 %486, %483
  %488 = add nsw i32 %487, %482
  store i32 %488, ptr %42, align 4, !tbaa !5
  %489 = load i32, ptr %45, align 8, !tbaa !5
  %490 = load i32, ptr %46, align 8, !tbaa !5
  %491 = ashr i32 %489, %490
  %492 = load i32, ptr %47, align 8, !tbaa !5
  %493 = sub nsw i32 %492, %491
  store i32 %493, ptr %47, align 8, !tbaa !5
  %494 = ashr i32 %493, %490
  %495 = add nsw i32 %494, %489
  store i32 %495, ptr %45, align 8, !tbaa !5
  %496 = load i32, ptr %48, align 4, !tbaa !5
  %497 = load i32, ptr %49, align 4, !tbaa !5
  %498 = ashr i32 %496, %497
  %499 = load i32, ptr %50, align 4, !tbaa !5
  %500 = sub nsw i32 %499, %498
  store i32 %500, ptr %50, align 4, !tbaa !5
  %501 = ashr i32 %500, %497
  %502 = add nsw i32 %501, %496
  store i32 %502, ptr %48, align 4, !tbaa !5
  %503 = load i32, ptr %51, align 16, !tbaa !5
  %504 = load i32, ptr %52, align 16, !tbaa !5
  %505 = ashr i32 %503, %504
  %506 = load i32, ptr %53, align 16, !tbaa !5
  %507 = sub nsw i32 %506, %505
  store i32 %507, ptr %53, align 16, !tbaa !5
  %508 = ashr i32 %507, %504
  %509 = add nsw i32 %508, %503
  store i32 %509, ptr %51, align 16, !tbaa !5
  %510 = load i32, ptr %54, align 4, !tbaa !5
  %511 = load i32, ptr %55, align 4, !tbaa !5
  %512 = ashr i32 %510, %511
  %513 = load i32, ptr %56, align 4, !tbaa !5
  %514 = sub nsw i32 %513, %512
  store i32 %514, ptr %56, align 4, !tbaa !5
  %515 = ashr i32 %514, %511
  %516 = add nsw i32 %515, %510
  store i32 %516, ptr %54, align 4, !tbaa !5
  %517 = load i32, ptr %7, align 16, !tbaa !5
  %518 = load i32, ptr %8, align 16, !tbaa !5
  %519 = ashr i32 %517, %518
  %520 = load i32, ptr %6, align 16, !tbaa !5
  %521 = sub nsw i32 %520, %519
  store i32 %521, ptr %6, align 16, !tbaa !5
  %522 = ashr i32 %521, %518
  %523 = add nsw i32 %522, %517
  store i32 %523, ptr %7, align 16, !tbaa !5
  %524 = load i32, ptr %57, align 4, !tbaa !5
  %525 = load i32, ptr %58, align 4, !tbaa !5
  %526 = ashr i32 %524, %525
  %527 = load i32, ptr %59, align 4, !tbaa !5
  %528 = sub nsw i32 %527, %526
  store i32 %528, ptr %59, align 4, !tbaa !5
  %529 = ashr i32 %528, %525
  %530 = add nsw i32 %529, %524
  store i32 %530, ptr %57, align 4, !tbaa !5
  %531 = load i32, ptr %60, align 8, !tbaa !5
  %532 = load i32, ptr %61, align 8, !tbaa !5
  %533 = ashr i32 %531, %532
  %534 = load i32, ptr %62, align 8, !tbaa !5
  %535 = sub nsw i32 %534, %533
  store i32 %535, ptr %62, align 8, !tbaa !5
  %536 = ashr i32 %535, %532
  %537 = add nsw i32 %536, %531
  store i32 %537, ptr %60, align 8, !tbaa !5
  %538 = load i32, ptr %63, align 4, !tbaa !5
  %539 = load i32, ptr %64, align 4, !tbaa !5
  %540 = ashr i32 %538, %539
  %541 = load i32, ptr %65, align 4, !tbaa !5
  %542 = sub nsw i32 %541, %540
  store i32 %542, ptr %65, align 4, !tbaa !5
  %543 = ashr i32 %542, %539
  %544 = add nsw i32 %543, %538
  store i32 %544, ptr %63, align 4, !tbaa !5
  %545 = load i32, ptr %66, align 16, !tbaa !5
  %546 = load i32, ptr %67, align 16, !tbaa !5
  %547 = ashr i32 %545, %546
  %548 = load i32, ptr %68, align 16, !tbaa !5
  %549 = sub nsw i32 %548, %547
  store i32 %549, ptr %68, align 16, !tbaa !5
  %550 = ashr i32 %549, %546
  %551 = add nsw i32 %550, %545
  store i32 %551, ptr %66, align 16, !tbaa !5
  %552 = load i32, ptr %69, align 4, !tbaa !5
  %553 = load i32, ptr %70, align 4, !tbaa !5
  %554 = ashr i32 %552, %553
  %555 = load i32, ptr %71, align 4, !tbaa !5
  %556 = sub nsw i32 %555, %554
  store i32 %556, ptr %71, align 4, !tbaa !5
  %557 = ashr i32 %556, %553
  %558 = add nsw i32 %557, %552
  store i32 %558, ptr %69, align 4, !tbaa !5
  call void @drawField(ptr noundef %79)
  tail call void (...) @simFlush() #9
  %559 = add nuw nsw i32 %74, 1
  br label %72
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #7

declare i32 @simRand(...) local_unnamed_addr #5

declare void @simFlush(...) local_unnamed_addr #5

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smin.i32(i32, i32) #8

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nofree norecurse nosync nounwind memory(argmem: write) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { noreturn nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #8 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #9 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"Ubuntu clang version 21.1.8 (6ubuntu1)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{!10}
!10 = distinct !{!10, !11}
!11 = distinct !{!11, !"LVerDomain"}
!12 = !{!13}
!13 = distinct !{!13, !11}
!14 = distinct !{!14, !15, !16, !17}
!15 = !{!"llvm.loop.mustprogress"}
!16 = !{!"llvm.loop.isvectorized", i32 1}
!17 = !{!"llvm.loop.unroll.runtime.disable"}
!18 = distinct !{!18, !15}
!19 = distinct !{!19, !15, !16}
!20 = distinct !{!20, !15, !16, !17}
!21 = distinct !{!21, !15}
!22 = distinct !{!22, !15, !16}
!23 = !{!24}
!24 = distinct !{!24, !25}
!25 = distinct !{!25, !"LVerDomain"}
!26 = !{!27}
!27 = distinct !{!27, !25}
!28 = !{!29}
!29 = distinct !{!29, !25}
!30 = !{!24, !27}
!31 = distinct !{!31, !15, !16, !17}
!32 = distinct !{!32, !15, !16}
!33 = distinct !{!33, !15}
!34 = distinct !{!34, !15}
!35 = distinct !{!35, !15}
!36 = distinct !{!36, !15, !16, !17}
!37 = distinct !{!37, !15, !16}
!38 = distinct !{!38, !15}
!39 = distinct !{!39, !15}
!40 = distinct !{!40, !15, !16}
