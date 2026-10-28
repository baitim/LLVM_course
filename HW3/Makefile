CC = clang-15
CXX = clang++-15
LLVM_CONFIG = llvm-config-15

all: app_ir_gen

sim.o: ../HW1/sim.c ../HW1/sim.h
	$(CC) -c $< -o $@

app_ir_gen: app_ir_gen.cpp sim.o
	$(CXX) $^ $(shell $(LLVM_CONFIG) --cppflags --ldflags --libs) -lSDL2 -o $@

clean:
	rm -f sim.o app_ir_gen

.PHONY: all clean
