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
	@echo "  clean      - удалить все build директории"
	@echo "  help       - показать это сообщение"

.PHONY: all main $(LABS) run run-lab1 run-lab3 run-lab5 run-lab6 clean help
