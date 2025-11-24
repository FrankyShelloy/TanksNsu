# Tanks

 игра «ТанкиNSU», написанная с использованием Qt5

## Структура проекта
- `include/` — все заголовочные файлы (.h)
- `src/` — реализации (.cpp)
- `CMakeLists.txt` — конфигурация сборки CMake


## Краткая инструкция по сборке
Требования:
- C++ компилятор с поддержкой C++20
- CMake >= 3.16
- Qt5 

Сборка из корня проекта :

```bash
mkdir -p cmake-build-debug
cd cmake-build-debug
cmake ..
cmake --build . --config Debug -- -j 4
```

