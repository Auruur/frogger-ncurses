all: compile linking
	rm *.o
	chmod +x froggerT

compile: global main control frog bullet vehicle log enemy timer music menu printer
	echo "compiling"

global:
	gcc -c src/global.c -o global.o

main:
	gcc -c src/main.c -o main.o

control:
	gcc -c src/control.c -o control.o

frog:
	gcc -c src/frog.c -o frog.o

bullet:
	gcc -c src/bullet.c -o bullet.o

vehicle:
	gcc -c src/vehicle.c -o vehicle.o

log:
	gcc -c src/log.c -o log.o

enemy:
	gcc -c src/enemy.c -o enemy.o

timer:
	gcc -c src/timer.c -o timer.o

music:
	gcc -c src/music.c -o music.o

menu:
	gcc -c src/menu.c -o menu.o

printer:
	gcc -c src/printer.c -o printer.o

linking:
	echo "linking"
	gcc *.o -lncurses -lpthread -o froggerT

clean:
	rm *.o
	rm froggerT