static:
	#clean up firs
	make clean
	#prepare .o out of .c
	gcc -Wall -c alloc.c
	#then make the lib
	ar crs liballoc.a alloc.o
	gcc -Wall main.c -o main -L. -lalloc
	#clean before finish:
	rm -f *.o
shared:
	make clean
	gcc -Wall -fPIC -c alloc.c
	gcc -Wall -shared -o liballoc.so alloc.o
	gcc -Wall main.c -o main -L. -lalloc -Wl,-rpath=`pwd`
	rm -f *.0

clean:
	rm -f *.o *.a *.so main