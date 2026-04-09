CPP = gcc
FLAGS = -Wall -Wextra -O2
# SRCPP = $(wildcard *.c)

main: main.c directory.o prompt.o
	${CPP} ${FLAGS} -o main $^

directory.o: directory.c
	${CPP} ${FLAGS} -c $^

prompt.o: prompt.c
	${CPP} ${FLAGS} -c $^

