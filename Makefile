CPP = gcc
FLAGS = -Wall -Wextra -O2
COMMAND = ${CPP} ${FLAGS}
# SRCPP = $(wildcard *.c)

main: main.c dispatcher.o command.o help.o exit.o ls.o process.o
	${COMMAND} -o main $^

clean:
	del /Q /F *.o
	del /Q /F *.exe

dispatcher.o: dispatcher.c
	${COMMAND} -c $^

command.o: command.c
	${COMMAND} -c $^

help.o: help.c
	${COMMAND} -c $^

exit.o: exit.c
	${COMMAND} -c $^

ls.o: ls.c
	${COMMAND} -c $^

process.o: process.c
	${COMMAND} -c $^

