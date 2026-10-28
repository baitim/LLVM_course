# IR генератор логического модуля

`app_ir_gen.cpp` с помощью `IRBuilder` генерирует LLVM IR логики из HW1 (`../HW1/app.c`): функции `heatColor`, `diffuseStep`, `applyDisc`, `rotate`, `drawField` и `app`. Сгенерированный модуль печатается, проверяется `verifyModule` и запускается в `ExecutionEngine`. Графические функции `simPutPixel`, `simRand`, `simFlush` пробрасываются в движок из `../HW1/sim.c` через `InstallLazyFunctionCreator`.

За основу взят IR, который выдаёт `clang -O2 -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -fno-inline-functions`: без инлайна каждая функция остаётся отдельной и вызывается из `app`.

## Сборка и запуск
```
sudo apt install clang-15 llvm-15 llvm-15-dev libsdl2-dev
make
./app_ir_gen
```
