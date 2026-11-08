
build:
	gcc -Wall -Wextra -fsanitize=undefined,address *.c 

debug:
	gcc -Wall -Wextra -ggdb *.c

