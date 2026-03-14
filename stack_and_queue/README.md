# stack_queue

Иллюстрация реализации стеков и очередей на C.

Поддерживаются две реализации:

- через статический массив (array)
- через связный список (list)
- общий интерфейс вынесен в `include/`.
- тесты в каталоге tests
- документация общая (одинаковый интерфейс)

## Сборка и запуск (GNU/Linux)

### Сборка

```
make
```

### Сборка и запуск тестов

```
make test
```

### Тестирование конкретной реализации

Реализация через массив:

```
make test-array
```

Реализация через список:

```
make test-list
```

### Для очистки

```
make clean
```

### Valgrind

Пример проверки:

```
valgrind ./queue_test_array
valgrind ./stack_test_array
```

и

```
valgrind ./queue_test_list
valgrind ./stack_test_list
```

### Документация

```
make docs
```
