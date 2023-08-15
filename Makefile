.PHONY = clean run valgrind debug
SOURCES = main.cpp
main: main.cpp .FORCE
	g++ -std=c++20 -Werror -Wextra -Wall -pedantic -g $(SOURCES) -o $@

clean:
	rm -f main
	rm -f asan

run:
	./main

valgrind: main
	valgrind ./main

asan: main.cpp .FORCE
	g++ -std=c++20 -Werror -Wextra -Wall -pedantic -g -fsanitize=address $(SOURCES) -o $@
	./asan

debug: main
	gdb ./main core

.FORCE:
