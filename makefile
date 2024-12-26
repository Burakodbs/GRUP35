#/
# @file           makefile
# @description    İşletim Sistemleri Dersi Proje Ödevi
# @course         1A ve 2A grubu
# @assignment     Projeodevi
# @date           26.12.2024
# @author         Elif Günaydın elif.gunaydin2@ogr.sakarya.edu.tr
# @author         Mert Bayır mert.bayir1@ogr.sakarya.edu.tr
# @author         Buse Nur Öğünşen buse.ogunsen@ogr.sakarya.edu.tr
# @author         Berkay Göçer berkay.gocer@ogr.sakarya.edu.tr
# @author         Burak Odabaş burak.odabas2@ogr.sakarya.edu.tr
#/
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