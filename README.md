# Уравнение теплопроводности (SDL)

Визуализация двумерного уравнения теплопроводности в реальном времени.
Явная разностная схема `T' = (T + T_N + T_S + T_E + T_W) / 5` на сетке 256x128,
границы поддерживаются холодными, T = 0. По кривым Лиссажу движутся источники
тепла и холодильники, периодически появляются случайные «вспышки».
Вся арифметика целочисленная `32-bit fixed point`, траектории считаются
алгоритмом окружности Минского без тригонометрии.

## Файлы
- `app.c` — логика
- `app.ll` — оптимизированный LLVM IR
- `sim.h`, `sim.c`, `start.c` — графический интерфейс

## Сборка и запуск
```
sudo apt install clang llvm libsdl2-dev
make
./app
```
Через интерпретатор:
```
make full.bc
lli --load=/usr/lib/x86_64-linux-gnu/libSDL2.so full.bc
```

<p align="center"><img src="https://github.com/baitim/LLVM_course/blob/main/images/cat.gif" width="50%"></p>

## Support
**This project is created by [baitim](https://t.me/bai_tim)**