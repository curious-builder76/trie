

.PHONY:  bench build debug

build:
	gcc -Wall -Wextra -fsanitize=address,undefined   *.c 

bench:
	gcc -Wall -Wextra -O2 *.c && ./a.out

debug:
	gcc -Wall -Wextra -ggdb *.c

