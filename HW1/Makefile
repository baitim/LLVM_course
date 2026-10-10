CC = clang

all: app app.ll

app.ll: app.c sim.h
	$(CC) -emit-llvm -S -O2 app.c -o $@

app: app.ll start.c sim.c sim.h
	$(CC) -O2 app.ll start.c sim.c -lSDL2 -o $@

full.bc: app.ll start.c sim.c
	$(CC) -emit-llvm -S start.c -o start.ll
	$(CC) -emit-llvm -S sim.c -o sim.ll
	llvm-link start.ll sim.ll app.ll -o $@

clean:
	rm -f app start.ll sim.ll full.bc

.PHONY: all clean
