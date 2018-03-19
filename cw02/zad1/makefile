main:
	make clean
	gcc -Wall -c main.c
	gcc -Wall -o main main.o
	touch wyniki.txt
	rm -f *.o
	#gen 2mln 4, 1mln 512
	#copy 2mln 4, 1mln 512
	#sort 10000 4, 
	./main generate plik 2000000 4
	./main copy plik plik2 2000000 4 0 > wyniki.txt
	./main copy plik plik3 2000000 4 1 >> wyniki.txt
	./main sort plik 2000000 4 0 >>wyniki.txt
	./main sort plik 2000000 4 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 500000 4
	./main copy plik plik2 500000 4 0 >> wyniki.txt
	./main copy plik plik3 500000 4 1 >> wyniki.txt
	./main sort plik 500000 4 0 >>wyniki.txt
	./main sort plik 500000 4 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 500000 512
	./main copy plik plik2 500000 512 0 >> wyniki.txt
	./main copy plik plik3 500000 512 1 >> wyniki.txt
	./main sort plik 500000 512 0 >>wyniki.txt
	./main sort plik 500000 512 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 300000 512
	./main copy plik plik2 300000 512 0 >> wyniki.txt
	./main copy plik plik3 300000 512 1 >> wyniki.txt
	./main sort plik 300000 512 0 >>wyniki.txt
	./main sort plik 300000 512 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 100000 4096
	./main copy plik plik2 100000 4096 0 >> wyniki.txt
	./main copy plik plik3 100000 4096 1 >> wyniki.txt
	./main sort plik 100000 4096 0 >>wyniki.txt
	./main sort plik 100000 4096 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 50000 4096
	./main copy plik plik2 50000 4096 0 >> wyniki.txt
	./main copy plik plik3 50000 4096 1 >> wyniki.txt
	./main sort plik 50000 4096 0 >>wyniki.txt
	./main sort plik 50000 4096 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 20000 8192
	./main copy plik plik2 20000 8192 0 >> wyniki.txt
	./main copy plik plik3 20000 8192 1 >> wyniki.txt
	./main sort plik 20000 8192 0 >>wyniki.txt
	./main sort plik 20000 8192 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 10000 8192
	./main copy plik plik2 10000 8192 0 >> wyniki.txt
	./main copy plik plik3 10000 8192 1 >> wyniki.txt
	./main sort plik 10000 8192 0 >>wyniki.txt
	./main sort plik 10000 8192 1 >>wyniki.txt

clean:
	rm -f plik* *.o main