LABS = lab1 lab3 lab5 lab6
JOBS ?= 4

all: main $(LABS)

main:
	@echo "[main] Configuring and building..."
	@if [ ! -d build ]; then mkdir build; fi
	cd build && cmake -DCMAKE_CXX_COMPILER=clang++ .. && make -j$(JOBS)

$(LABS):
	@echo "[$@] Configuring and building..."
	@if [ ! -d $@/build ]; then mkdir -p $@/build; fi
	cd $@/build && cmake -DCMAKE_CXX_COMPILER=clang++ .. && make -j$(JOBS)

run: main
	@echo "Запуск главного меню..."
	./build/graph_main

run-lab1: lab1
	@echo "Запуск Lab 1..."
	./lab1/build/graph_lab1

run-lab3: lab3
	@echo "Запуск Lab 3..."
	./lab3/build/graph_lab3

run-lab5: lab5
	@echo "Запуск Lab 5..."
	./lab5/build/graph_lab5

run-lab6: lab6
	@echo "Запуск Lab 6..."
	./lab6/build/graph_lab6

clean:
	@echo "Cleaning all build directories..."
	rm -rf build
	for d in $(LABS); do rm -rf $$d/build; done

check: main
	@echo "Running clang-tidy analysis (read-only)..."
	@if [ ! -f build/compile_commands.json ]; then \
		echo "Error: compile_commands.json not found. Run 'make' first."; \
		exit 1; \
	fi
	@find . \( -name "*.cc" -o -name "*.h" -o -name "*.tpp" \) -not -path "*/build/*" -not -path "*/venv/*" -not -path "*/lab6/*" | xargs clang-tidy -p build --system-headers --header-filter='^(?!.*(/opt/homebrew|/usr|/System|/Library|/lab6)).*\.(cc|h|tpp)$$' 2>&1 | grep -v "^/opt/homebrew" | grep -v "^/usr" | grep -v "^/System" | grep -v "^/Library" | grep -v "^/Users/.*/venv/" || true

format:
	@echo "Applying clang-format..."
	find . \( -name "*.cc" -o -name "*.h" -o -name "*.tpp" \) -not -path "*/build/*" | xargs clang-format -i

help:
	@echo "Доступные цели:"
	@echo "  all        - собрать main и все лабораторные через CMake"
	@echo "  main       - собрать только main (build/)"
	@echo "  lab1       - собрать lab1 (lab1/build/)"
	@echo "  lab3       - собрать lab3 (lab3/build/)"
	@echo "  lab5       - собрать lab5 (lab5/build/)"
	@echo "  lab6       - собрать lab6 (lab6/build/)"
	@echo "  run        - запустить главное меню (./build/graph_main)"
	@echo "  run-lab1   - запустить lab1 (./lab1/build/graph_lab1)"
	@echo "  run-lab3   - запустить lab3 (./lab3/build/graph_lab3)"
	@echo "  run-lab5   - запустить lab5 (./lab5/build/graph_lab5)"
	@echo "  run-lab6   - запустить lab6 (./lab6/build/graph_lab6)"
	@echo "  check      - запустить clang-tidy анализ (только чтение)"
	@echo "  format     - применить clang-format ко всем файлам"
	@echo "  clean      - удалить все build директории"
	@echo "  help       - показать это сообщение"

.PHONY: all main $(LABS) run run-lab1 run-lab3 run-lab5 run-lab6 clean check format help
