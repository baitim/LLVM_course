# Heat equation: HW1/app.c rewritten on our ISA (see ISA.md)
#
# Memory (words):
#   0      .. 32767   field A (256 x 128, index = y * 256 + x)
#   32768  .. 65535   field B
#   65536  .. 65542   palette, 7 colors
#   65552  .. 65593   sources: xp xq xs yp yq ys val, 6 words each
#                     (element i of array k is at 65552 + 6 * k + i)
# Registers:
#   x0 - current field, x1 - next field, x2 - frame counter,
#   x3 .. x21 - temporaries of the sections below

entry
    MOVi x0 0
    MOVi x1 32768
    MOVi x2 0

    # palette (inferno-like)
    MOVi x3 65536
    MOVi x4 0xFF000004
    ST x4 x3 0
    MOVi x4 0xFF280B54
    ST x4 x3 1
    MOVi x4 0xFF65156E
    ST x4 x3 2
    MOVi x4 0xFF9F2A63
    ST x4 x3 3
    MOVi x4 0xFFD44842
    ST x4 x3 4
    MOVi x4 0xFFF98C0A
    ST x4 x3 5
    MOVi x4 0xFFFCFFA4
    ST x4 x3 6

    # sources: random start of every oscillator
    MOVi x3 0                 # i
    MOVi x7 0
init_src
    ADDi x6 x3 65552          # address of element i
    RAND x4
    REMi x4 x4 64
    MOVi x5 122
    SUB x5 x5 x4
    SHLi x5 x5 8
    ST x5 x6 0                # xp[i] = (122 - rand % 64) << 8
    ST x7 x6 6                # xq[i] = 0
    RAND x4
    REMi x4 x4 3
    ADDi x4 x4 4
    ST x4 x6 12               # xs[i] = 4 + rand % 3
    ST x7 x6 18               # yp[i] = 0
    RAND x4
    REMi x4 x4 32
    MOVi x5 58
    SUB x5 x5 x4
    SHLi x5 x5 8
    ST x5 x6 24               # yq[i] = (58 - rand % 32) << 8
    RAND x4
    REMi x4 x4 3
    ADDi x4 x4 4
    ST x4 x6 30               # ys[i] = 4 + rand % 3
    MOVi x5 65535             # val[i] = (i % 3 == 2) ? 0 : 65535
    REMi x4 x3 3
    BNEi x4 2 store_val
    MOVi x5 0
store_val
    ST x5 x6 36
    ADDi x3 x3 1
    BLTi x3 6 init_src

frame_loop
    MOVi x3 0                 # step
step_loop
    # heat sources: disc of radius 4 around every source
    MOVi x4 0                 # i
src_loop
    ADDi x5 x4 65552
    LD x6 x5 0
    SHRi x6 x6 8
    ADDi x6 x6 128            # cx = 128 + (xp[i] >> 8)
    LD x7 x5 24
    SHRi x7 x7 8
    ADDi x7 x7 64             # cy = 64 + (yq[i] >> 8)
    MOVi x8 4                 # r
    LD x9 x5 36               # val
    MOVi x10 0                # return to after_src
    JMP disc
after_src
    ADDi x4 x4 1
    BLTi x4 6 src_loop

    # one step of diffusion, borders stay cold
    ADDi x4 x0 257            # &cur[1][1]
    ADDi x5 x1 257            # &next[1][1]
    MOVi x6 1                 # y
diff_y
    MOVi x7 1                 # x
diff_x
    LD x8 x4 0
    LDADD x8 x4 -1
    LDADD x8 x4 1
    LDADD x8 x4 -256
    LDADD x8 x4 256
    DIVi x8 x8 5
    ST x8 x5 0
    ADDi x4 x4 1
    ADDi x5 x5 1
    ADDi x7 x7 1
    BLTi x7 255 diff_x
    ADDi x4 x4 2              # skip the last cell of the row and the first of the next
    ADDi x5 x5 2
    ADDi x6 x6 1
    BLTi x6 127 diff_y

    MOV x4 x0                 # cur <-> next
    MOV x0 x1
    MOV x1 x4
    ADDi x3 x3 1
    BLTi x3 4 step_loop

    # a random spark every 16 frames
    REMi x4 x2 16
    BNEi x4 0 rotate
    RAND x6
    REMi x6 x6 254
    ADDi x6 x6 1              # cx = 1 + rand % 254
    RAND x7
    REMi x7 x7 126
    ADDi x7 x7 1              # cy = 1 + rand % 126
    MOVi x8 8
    MOVi x9 65535
    MOVi x10 1                # return to after_spark
    JMP disc
after_spark
rotate
    # sources move: Minsky circle algorithm
    MOVi x3 0
rot_loop
    ADDi x5 x3 65552
    LD x6 x5 0                # xp
    LD x7 x5 6                # xq
    LD x8 x5 12               # xs
    SHR x9 x7 x8
    SUB x6 x6 x9
    SHR x9 x6 x8
    ADD x7 x7 x9
    ST x6 x5 0
    ST x7 x5 6
    LD x6 x5 18               # yp
    LD x7 x5 24               # yq
    LD x8 x5 30               # ys
    SHR x9 x7 x8
    SUB x6 x6 x9
    SHR x9 x6 x8
    ADD x7 x7 x9
    ST x6 x5 18
    ST x7 x5 24
    ADDi x3 x3 1
    BLTi x3 6 rot_loop

    # draw the field: every cell is a 4 x 4 block of one color
    MOVi x3 0                 # y
    MOV x4 x0                 # pointer to the cell
draw_y
    SHLi x20 x3 2             # y * 4
    MOVi x5 0                 # x
draw_x
    LD x6 x4 0                # temperature
    MULi x6 x6 6
    SHRi x7 x6 16             # palette index
    BLTi x7 6 idx_ok
    MOVi x7 5
idx_ok
    SHLi x8 x7 16
    SUB x8 x6 x8              # fraction between two colors
    ADDi x9 x7 65536
    LD x10 x9 0
    LD x11 x9 1
    MOVi x12 0xFF000000       # argb
    MOVi x13 0                # shift of the channel
channel
    EXTB x14 x10 x13
    EXTB x15 x11 x13
    LERP x14 x14 x15 x8
    ORSHL x12 x14 x13
    ADDi x13 x13 8
    BLTi x13 24 channel
    SHLi x16 x5 2             # x * 4
    MOVi x17 0                # dy
block_y
    ADD x18 x20 x17
    MOVi x19 0                # dx
block_x
    ADD x21 x16 x19
    PUT_PIXEL x21 x18 x12
    ADDi x19 x19 1
    BLTi x19 4 block_x
    ADDi x17 x17 1
    BLTi x17 4 block_y
    ADDi x4 x4 1
    ADDi x5 x5 1
    BLTi x5 256 draw_x
    ADDi x3 x3 1
    BLTi x3 128 draw_y
    FLUSH
    ADDi x2 x2 1
    JMP frame_loop

# disc around (x6, x7) with radius x8, value x9, x10 selects where to return
disc
    MUL x16 x8 x8             # r * r
    MOVi x11 0
    SUB x11 x11 x8            # dy = -r
disc_y
    MOVi x12 0
    SUB x12 x12 x8            # dx = -r
disc_x
    ADD x13 x6 x12            # x = cx + dx
    ADD x14 x7 x11            # y = cy + dy
    MUL x15 x12 x12
    MUL x17 x11 x11
    ADD x15 x15 x17
    BLT x16 x15 disc_skip     # dx * dx + dy * dy > r * r
    BLTi x13 1 disc_skip
    BGEi x13 255 disc_skip
    BLTi x14 1 disc_skip
    BGEi x14 127 disc_skip
    SHLi x15 x14 8
    ADD x15 x15 x13
    ADD x15 x15 x0
    ST x9 x15 0
disc_skip
    ADDi x12 x12 1
    BGE x8 x12 disc_x         # dx <= r
    ADDi x11 x11 1
    BGE x8 x11 disc_y         # dy <= r
    BEQi x10 0 after_src
    JMP after_spark
