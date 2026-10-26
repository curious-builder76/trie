
build:
	gcc -O2 -Wall -Wextra -fsanitize=undefined,address *.c 
	
