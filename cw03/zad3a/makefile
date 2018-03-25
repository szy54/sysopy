main:
	gcc -Wall -c main.c
	gcc -Wall -o main main.o
	gcc -Wall -c endless_loop.c
	gcc -Wall -o eloop endless_loop.o
	rm -R *.o
	./main commands 1 1 
	
clean:
	rm -R main *.o eloop commands_cp