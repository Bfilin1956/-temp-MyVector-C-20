# **<template>MyVector C++20**
Шаблонный аналог std::vector, поддерживает любые типы, включая пользовательские.

## Реализация вектора через шаблон:
 - Методы: emplace_back, operator[], begin, end, size, capacity, clear, destroy, reserve
 - Аллокация через ::operator new
 - Конструирование на месте contsruct_at
 - Использование RAII

