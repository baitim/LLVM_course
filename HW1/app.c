#include "sim.h"

// 2D heat equation with moving heat sources and sinks.
// Arithmetic is 32-bit integer fixed point.

#define SCALE 4
#define W (SIM_X_SIZE / SCALE)
#define H (SIM_Y_SIZE / SCALE)

#define T_MAX 65535
#define STEPS_PER_FRAME 4

#define N_SRC 6
#define SRC_R 4
#define FIX 8
#define SPARK_PERIOD 16

#define PAL_N 7

// Maps tempreture [0, T_MAX] to an "inferno"-like palete
int heatColor(int t) {
  const int pal[PAL_N] = {0xFF000004, 0xFF280B54, 0xFF65156E, 0xFF9F2A63,
                          0xFFD44842, 0xFFF98C0A, 0xFFFCFFA4};
  int pos = t * (PAL_N - 1);
  int idx = pos >> 16;
  if (idx > PAL_N - 2)
    idx = PAL_N - 2;
  int frac = pos - (idx << 16);
  int c0 = pal[idx];
  int c1 = pal[idx + 1];
  int argb = 0xFF000000;
  for (int sh = 0; sh < 24; sh += 8) {
    int a = (c0 >> sh) & 0xFF;
    int b = (c1 >> sh) & 0xFF;
    int c = a + (((b - a) * frac) >> 16);
    argb |= c << sh;
  }
  return argb;
}

// One explicit step: T' = T + (N + S + E + W - 4T) / 5 = (T + N + S + E + W) / 5
// Border cells are never writen and stay cold (Dirichlet condition T = 0)
void diffuseStep(int *cur, int *next) {
  for (int y = 1; y < H - 1; y++)
    for (int x = 1; x < W - 1; x++) {
      int i = y * W + x;
      next[i] = (cur[i] + cur[i - 1] + cur[i + 1] + cur[i - W] + cur[i + W]) / 5;
    }
}

void applyDisc(int *field, int cx, int cy, int r, int val) {
  for (int dy = -r; dy <= r; dy++)
    for (int dx = -r; dx <= r; dx++) {
      int x = cx + dx;
      int y = cy + dy;
      if (dx * dx + dy * dy <= r * r && x > 0 && x < W - 1 && y > 0 &&
          y < H - 1)
        field[y * W + x] = val;
    }
}

// Minsky cirle algoritm: integer-only rotation of (p, q)
void rotate(int *p, int *q, int *shift, int n) {
  for (int i = 0; i < n; i++) {
    p[i] -= q[i] >> shift[i];
    q[i] += p[i] >> shift[i];
  }
}

void drawField(int *field) {
  for (int y = 0; y < H; y++)
    for (int x = 0; x < W; x++) {
      int argb = heatColor(field[y * W + x]);
      for (int dy = 0; dy < SCALE; dy++)
        for (int dx = 0; dx < SCALE; dx++)
          simPutPixel(x * SCALE + dx, y * SCALE + dy, argb);
    }
}

void app() {
  int field1[W * H] = {};
  int field2[W * H] = {};
  int *cur = field1;
  int *next = field2;

  // Each source moves along a Lissajous curve made of two oscillators
  int xp[N_SRC], xq[N_SRC], xs[N_SRC];
  int yp[N_SRC], yq[N_SRC], ys[N_SRC];
  int val[N_SRC];
  for (int i = 0; i < N_SRC; i++) {
    xp[i] = (W / 2 - SRC_R - 2 - simRand() % (W / 4)) << FIX;
    xq[i] = 0;
    xs[i] = 4 + simRand() % 3;
    yp[i] = 0;
    yq[i] = (H / 2 - SRC_R - 2 - simRand() % (H / 4)) << FIX;
    ys[i] = 4 + simRand() % 3;
    val[i] = (i % 3 == 2) ? 0 : T_MAX;
  }

  for (int frame = 0;; frame++) {
    for (int step = 0; step < STEPS_PER_FRAME; step++) {
      for (int i = 0; i < N_SRC; i++)
        applyDisc(cur, W / 2 + (xp[i] >> FIX), H / 2 + (yq[i] >> FIX), SRC_R,
                  val[i]);
      diffuseStep(cur, next);
      int *tmp = cur;
      cur = next;
      next = tmp;
    }
    if (frame % SPARK_PERIOD == 0)
      applyDisc(cur, 1 + simRand() % (W - 2), 1 + simRand() % (H - 2),
                2 * SRC_R, T_MAX);
    rotate(xp, xq, xs, N_SRC);
    rotate(yp, yq, ys, N_SRC);
    drawField(cur);
    simFlush();
  }
}
