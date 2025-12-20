import sys
import os
from typing import List

# Проверяем, что скрипт запускается из нужной директории
if not os.path.exists('config.yaml'):
    print("Ошибка: Файл 'config.yaml' не найден. Запускайте скрипт из корневой директории проекта.")
    sys.exit(1)

sys.path.insert(0, 'build')

try:
    from finite_ring_module import (
        FiniteRingRules, 
        SmallRingArithmetic, 
        BigRingArithmetic, 
        RingNumber, 
        DivisionResult
    )
except ImportError as e:
    print(f"Ошибка импорта модуля: {e}")
    print("Убедитесь, что C++ модуль скомпилирован и находится в директории 'build'.")
    sys.exit(1)


# --- Основная логика генерации таблиц ---

VARIANT_NAME = "D9" # Выбранный вариант D9
rules = FiniteRingRules("config.yaml", VARIANT_NAME)
small = SmallRingArithmetic(rules)

symbols = [rules.getValueChar(i) for i in range(rules.getSize())]
SIZE = rules.getSize()

# Создаем карту соответствия: символ -> индекс (0..N-1)
symbol_map = {sym: i for i, sym in enumerate(symbols)}

# Увеличенная ширина ячейки для идеального зазора
CELL_WIDTH = 3

def get_add_carry(a: str, b: str) -> str:
    """
    Вычисляет перенос при сложении.
    Carry = (Index(a) + Index(b)) // Size
    Возвращает символ кольца, соответствующий этому значению.
    """
    val_a = symbol_map[a]
    val_b = symbol_map[b]
    carry_idx = (val_a + val_b) // SIZE
    # В случае сложения перенос обычно 0 или 1, но код универсален
    return symbols[carry_idx]

def get_mult_carry(a: str, b: str) -> str:
    """
    Вычисляет перенос при умножении.
    Carry = (Index(a) * Index(b)) // Size
    Возвращает символ кольца, соответствующий этому значению.
    """
    val_a = symbol_map[a]
    val_b = symbol_map[b]
    carry_idx = (val_a * val_b) // SIZE
    return symbols[carry_idx]

def print_table(operation_name: str, symbols: List[str], operation_func):
    """Генерирует и выводит таблицу для заданной операции, обеспечивая идеальное выравнивание."""
    
    # 1. Заголовочная строка
    header_start = " " * CELL_WIDTH + "|"
    header_symbols = "".join([sym.ljust(CELL_WIDTH) for sym in symbols])
    
    print(f"\n{' ' * 8} Таблица переносов {operation_name}:\n")
    print(f"{header_start} {header_symbols}")
    
    # 2. Разделительная линия
    data_line_length = CELL_WIDTH * SIZE
    separator = "-" * CELL_WIDTH + "+" + "-" * (data_line_length + 1)
    print(separator)

    # 3. Строки данных
    for a in symbols:
        row_start = a.ljust(CELL_WIDTH) + "|"
        data_cells = []
        for b in symbols:
            result = operation_func(a, b)
            # result[-1] берет последний символ, работает и для char и для string
            result_char = result[-1] 
            data_cells.append(result_char.ljust(CELL_WIDTH))
        
        print(f"{row_start} {''.join(data_cells)}")

# --- Вывод ---

print("==========================================")
print(f"Генерация таблиц ПЕРЕНОСОВ для кольца Z{SIZE} (D9)")
print("==========================================")

# Таблица переносов сложения
print_table("сложения", symbols, get_add_carry)

# Таблица переносов умножения
print_table("умножения", symbols, get_mult_carry)

print("\n==========================================")