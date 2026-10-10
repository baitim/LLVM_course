# Logic module (app.c) and graphical interface are taken from HW1
APP_DIR ?= ../HW1
LLVM_INC := $(shell llvm-config --includedir)
PASS_FLAGS = -O2 -fpass-plugin=./libTracePass.so -I$(APP_DIR)
SIM_SRC = log.c $(APP_DIR)/sim.c $(APP_DIR)/start.c

all: stats.txt stats_no_phi_expand.txt

libTracePass.so: trace_pass.cpp
	clang++ $< -fPIC -shared -fno-rtti -I$(LLVM_INC) -o $@

# Instrumented IR of the logic module (for inspection)
app_trace.ll: $(APP_DIR)/app.c libTracePass.so
	clang $(PASS_FLAGS) -emit-llvm -S $< -o $@

app_trace.o: $(APP_DIR)/app.c libTracePass.so
	clang $(PASS_FLAGS) -c $< -o $@

trace-app: app_trace.o $(SIM_SRC)
	clang -O2 -I$(APP_DIR) $^ -lSDL2 -o $@

# Without the phi expansion: "user <- phi" records stay as is
app_nophi.o: $(APP_DIR)/app.c libTracePass.so
	TRACE_EXPAND_PHI=0 clang $(PASS_FLAGS) -c $< -o $@

trace-app-nophi: app_nophi.o $(SIM_SRC)
	clang -O2 -I$(APP_DIR) $^ -lSDL2 -o $@

# TRACE_FRAMES / TRACE_TOP / TRACE_OUT are described in log.c.
# SDL_VIDEODRIVER=dummy runs without a window.
stats.txt: trace-app
	TRACE_FRAMES=10 TRACE_OUT=$@ ./trace-app

stats_no_phi_expand.txt: trace-app-nophi
	TRACE_FRAMES=10 TRACE_OUT=$@ ./trace-app-nophi

clean:
	rm -f libTracePass.so app_trace.o app_trace.ll app_nophi.o trace-app trace-app-nophi

.PHONY: all clean
