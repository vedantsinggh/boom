all: main.c raylib.h libraylib.a
	gcc -g -o boom main.c libraylib.a -lm
