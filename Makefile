CPP = gcc
FLAGS = -Wall -Wextra -O2
COMMAND = ${CPP} ${FLAGS}
# SRCPP = $(wildcard *.c)

main: main.c dispatcher.o command.o help.o exit.o ls.o cd.o process.o clear.o mkdir.o date.o time.o path.o process_manager.o list.o kill.o stop.o resume.o
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

cd.o: cd.c
	${COMMAND} -c $^

process.o: process.c
	${COMMAND} -c $^

clear.o: clear.c
	${COMMAND} -c $^

mkdir.o: mkdir.c
	${COMMAND} -c $^

date.o: date.c
	${COMMAND} -c $^

time.o: time.c
	${COMMAND} -c $^

path.o: path.c
	${COMMAND} -c $^

process_manager.o: process_manager.c
	${COMMAND} -c $^

list.o: list.c
	${COMMAND} -c $^

kill.o: kill.c
	${COMMAND} -c $^

stop.o: stop.c
	${COMMAND} -c $^

resume.o: resume.c
	${COMMAND} -c $^