CC = gcc
CFLAG = -Wall -Wextra -Iinclude -O2

OBJS = build/main.o build/eval_rpn.o build/logic.o build/forms.o build/basis.o
HEADERS = $(wildcard include/*.h)

main: $(OBJS)
	@mkdir -p build
	${CC} ${CFLAG} $(OBJS) -o build/main

build/%.o: src/%.c $(HEADERS)
	@mkdir -p build
	${CC} ${CFLAG} -c $< -o $@

clean:
	rm -rf build

run: main
	./build/main formula.txt

.PHONY: main clean run