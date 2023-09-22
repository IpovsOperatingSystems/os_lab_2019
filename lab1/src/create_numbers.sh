#!/bin/bash

# Генерируем 150 случайных чисел и записываем их в файл numbers.txt
head -c 600 /dev/urandom | od -An -i > numbers.txt

echo "Создан файл numbers.txt с 150 случайными числами."
