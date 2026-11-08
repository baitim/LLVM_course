// Replacement of sim.c for tests: instead of a window it counts a hash of
// all drawn pixels and stops after FRAMES frames (default 5)
#include "sim.h"
#include <stdio.h>
#include <stdlib.h>

static unsigned long long Hash = 14695981039346656037ull;
static int Frames;
static int MaxFrames = 5;

static void mix(unsigned v) { Hash = (Hash ^ v) * 1099511628211ull; }

void simInit() {
  char *s = getenv("FRAMES");
  if (s)
    MaxFrames = atoi(s);
}

void simExit() { printf("frames %d hash %016llx\n", Frames, Hash); }

void simFlush() {
  Frames++;
  mix(0xF1);
  if (Frames >= MaxFrames) {
    simExit();
    exit(0);
  }
}

void simPutPixel(int x, int y, int argb) {
  if (x < 0 || x >= SIM_X_SIZE || y < 0 || y >= SIM_Y_SIZE) {
    fprintf(stderr, "pixel out of range: %d %d\n", x, y);
    exit(1);
  }
  mix(x);
  mix(y);
  mix(argb);
}

int simRand() { return rand(); }

int simHasClick() { return 0; }

int simGetClick() { return 0; }
