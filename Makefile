
build:
	gcc -Wall -Wextra -march=native -O2  *.c 

debug:
	gcc -Wall -Wextra -ggdb *.c

