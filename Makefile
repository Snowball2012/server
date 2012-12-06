all:
	gcc -Wall -g src/main.c -o bin/server
clean:
	rm -rf src/*~
