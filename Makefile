
build:
	gcc -O2 -Wall -Wextra -march=native *.c 

debug:
	gcc -Wall -Wextra -ggdb *.c

