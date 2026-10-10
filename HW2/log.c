// Runtime for the trace pass. Counts windows of 1..5 consecutive records
// "user <- operand" and prints the most frequent ones.
//
// Environment:
//   TRACE_FRAMES - how many frames to trace (default 3, 0 - until exit)
//   TRACE_TOP    - how many patterns to print for each length (default 20)
//   TRACE_OUT    - output file (default stats.txt)
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WIN 5
#define TABLE_SIZE (1 << 20)

// hash table: key is a window (len records), value is its count
int TabLen[TABLE_SIZE]; // 0 - empty cell
int TabRec[TABLE_SIZE][MAX_WIN];
uint64_t TabCount[TABLE_SIZE];

const char *OpNames[128];
int Last[MAX_WIN]; // last records, Last[0] is the newest
uint64_t Records;
uint64_t Windows[MAX_WIN + 1];
int Frames;
int Dumped;

int getEnv(const char *name, int def) {
  char *s = getenv(name);
  if (s)
    return atoi(s);
  return def;
}

// record = user opcode and operand opcode (both < 128)
void addWindow(int len) {
  uint64_t h = len;
  for (int i = 0; i < len; i++)
    h = h * 1000003 + Last[i];
  h ^= h >> 20;
  int idx = h % TABLE_SIZE;

  while (TabLen[idx] != 0) {
    if (TabLen[idx] == len && memcmp(TabRec[idx], Last, len * sizeof(int)) == 0)
      break;
    idx = (idx + 1) % TABLE_SIZE;
  }
  if (TabLen[idx] == 0) {
    TabLen[idx] = len;
    memcpy(TabRec[idx], Last, len * sizeof(int));
  }
  TabCount[idx]++;
}

void registerOp(int id, const char *name) { OpNames[id] = name; }

void logUse(int user, int operand) {
  for (int i = MAX_WIN - 1; i > 0; i--)
    Last[i] = Last[i - 1];
  Last[0] = user * 128 + operand;
  Records++;

  for (int len = 1; len <= MAX_WIN; len++) {
    if (len > Records)
      break;
    addWindow(len);
    Windows[len]++;
  }
}

int cmp(const void *a, const void *b) {
  uint64_t ca = TabCount[*(int *)a];
  uint64_t cb = TabCount[*(int *)b];
  if (ca < cb)
    return 1;
  if (ca > cb)
    return -1;
  // equal counts: order by records, so the output is always the same
  int *ra = TabRec[*(int *)a];
  int *rb = TabRec[*(int *)b];
  return memcmp(ra, rb, MAX_WIN * sizeof(int));
}

void dumpStats() {
  if (Dumped)
    return;
  Dumped = 1;

  char *path = getenv("TRACE_OUT");
  FILE *f = fopen(path ? path : "stats.txt", "w");
  if (!f)
    return;
  int top = getEnv("TRACE_TOP", 20);
  int *cells = malloc(TABLE_SIZE * sizeof(int));

  fprintf(f, "Frames traced: %d\n", Frames);
  fprintf(f, "Records in trace (user <- operand): %llu\n",
          (unsigned long long)Records);

  for (int len = 1; len <= MAX_WIN; len++) {
    int n = 0;
    for (int i = 0; i < TABLE_SIZE; i++)
      if (TabLen[i] == len)
        cells[n++] = i;
    qsort(cells, n, sizeof(int), cmp);

    fprintf(f, "\n=== Window length %d: %llu windows, %d distinct ===\n", len,
            (unsigned long long)Windows[len], n);
    for (int i = 0; i < n && i < top; i++) {
      int c = cells[i];
      fprintf(f, "%12llu %6.2f%%  ", (unsigned long long)TabCount[c],
              100.0 * TabCount[c] / Windows[len]);
      // the newest record is stored first, so print from the end
      for (int k = len - 1; k >= 0; k--) {
        int rec = TabRec[c][k];
        fprintf(f, "%s <- %s", OpNames[rec / 128], OpNames[rec % 128]);
        fputs(k ? " ; " : "\n", f);
      }
    }
  }
  free(cells);
  fclose(f);
}

// the window is closed: sim.c calls assert and we get SIGABRT
void onAbort(int sig) {
  dumpStats();
  _Exit(0);
}

__attribute__((constructor)) void init() {
  atexit(dumpStats);
  signal(SIGABRT, onAbort);
}

// called before every simFlush(), i.e. once per frame
void logFrame() {
  Frames++;
  int max = getEnv("TRACE_FRAMES", 3);
  if (max != 0 && Frames >= max)
    exit(0);
}
