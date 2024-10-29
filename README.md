# JIT and AOT Compilers in VM (MIPT Course)

## Сборка проекта
Для сборки проекта из корневой директории следует выполнить команды
```
cmake -S . -B build
cd build && make
```

## Запуск тестов
Для запуска теста из директории build следует выполнить команду
```
./tests/run_test_NameOfTest
```

## Доступные тесты

test_graph        (1 задание)

test_RPO          (2 задание)

test_domTree      (2 задание)

test_loopAnalyzer (3 задание)
