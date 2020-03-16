myprog : main.o backup.o server.o monitor.o update.o lock_unlock.o
	gcc -o myprog main.o backup.o server.o monitor.o update.o lock_unlock.o -lrt

main.o : main.c backup.h server.h monitor.h update.h lock_unlock.h
	gcc -c main.c

backup.o : backup.c
	gcc -c backup.c

monitor.o : monitor.c
	gcc -c monitor.c

server.o : server.c backup.h
	gcc -c server.c -lrt

update.o : update.c 
	gcc -c update.c 

lock_unlock.o : lock_unlock.c 
	gcc -c lock_unlock.c 

clean : rm myprog backup.o main.o
