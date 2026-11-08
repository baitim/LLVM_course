# For another LLVM version: make CC=clang-15 CXX=clang++-15 LLVM_CONFIG=llvm-config-15
CC = clang
CXX = clang++
LLVM_CONFIG = llvm-config
LLVM_FLAGS = $(shell $(LLVM_CONFIG) --cppflags --ldflags --libs)

# number of frames in tests
FRAMES = 150

all: asm2ir

sim.o: ../HW1/sim.c ../HW1/sim.h
	$(CC) -c $< -o $@

asm2ir: asm2ir.cpp sim.o
	$(CXX) $^ $(LLVM_FLAGS) -lSDL2 -o $@

# Run with a window:
#   ./asm2ir sim app.s | ./asm2ir ext app.s | ./asm2ir full app.s

# Generated IR of the modes ext and full
app_ext.ll app_full.ll: app_%.ll: asm2ir_check app.s
	FRAMES=1 ./asm2ir_check $* app.s $@ > /dev/null

#--- Tests: instead of the window the backend counts a hash of all pixels ---
check_backend.o: test/check_backend.c ../HW1/sim.h
	$(CC) -O2 -I../HW1 -c $< -o $@

asm2ir_check: asm2ir.cpp check_backend.o
	$(CXX) $^ $(LLVM_FLAGS) -o $@

# the same app in C (HW1)
app_ref: ../HW1/app.c ../HW1/start.c check_backend.o
	$(CC) -O2 -I../HW1 $^ -o $@

# hashes of the C app and of all three modes must be equal
check: app_ref asm2ir_check app.s
	@ref=$$(FRAMES=$(FRAMES) ./app_ref); echo "C app (HW1): $$ref"; \
	for m in sim ext full; do \
	  out=$$(FRAMES=$(FRAMES) ./asm2ir_check $$m app.s 2>/dev/null); \
	  echo "$$m: $$out"; \
	  [ "$$out" = "$$ref" ] || { echo "MISMATCH"; exit 1; }; \
	done; echo "all equal"

clean:
	rm -f sim.o check_backend.o asm2ir asm2ir_check app_ref app_ext.ll app_full.ll

.PHONY: all check clean
