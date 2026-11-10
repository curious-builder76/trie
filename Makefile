

.PHONY:  bench build debug optimized_bench

build:
	gcc -Wall -Wextra -fsanitize=address,undefined   *.c 

bench:
	gcc -Wall -Wextra -O2 *.c && ./a.out

debug:
	gcc -Wall -Wextra -ggdb *.c

optimized_bench:
	gcc -Wall -Wextra -fomit-frame-pointer -march=native -O2 -static -flto  *.c && ./a.out
