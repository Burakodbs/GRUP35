all: compile run

compile:
	gcc -I ./include/ -o ./lib/background.o -c ./src/background.c
	gcc -I ./include/ -o ./lib/command_handler.o -c ./src/command_handler.c
	gcc -I ./include/ -o ./lib/pipe_handler.o -c ./src/pipe_handler.c
	gcc -I ./include/ -o ./lib/redirection.o -c ./src/redirection.c
	gcc -I ./include/ -o ./lib/utils.o -c ./src/utils.c


	gcc -I ./include/ -o ./bin/Test ./lib/background.o ./lib/command_handler.o ./lib/pipe_handler.o ./lib/redirection.o ./lib/utils.o ./src/main.c
run:
	./bin/Test