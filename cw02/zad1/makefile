main:
	make clean
	gcc -Wall -c main.c
	gcc -Wall -o main main.o
	touch wyniki.txt
	rm -f *.o
	#gen 2mln 4, 1mln 512
	#copy 2mln 4, 1mln 512
	#sort 10000 4, 
	./main generate plik 5000 4
	./main copy plik plik2 5000 4 0 > wyniki.txt
	rm -f plik2
	./main copy plik plik3 5000 4 1 >> wyniki.txt
	chmod +rw plik3
	./main sort plik 5000 4 0 >>wyniki.txt
	./main sort plik3 5000 4 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 4000 4
	./main copy plik plik2 4000 4 0 >> wyniki.txt
	rm -f plik2
	./main copy plik plik3 4000 4 1 >> wyniki.txt
	chmod +rw plik3
	./main sort plik 4000 4 0 >>wyniki.txt
	./main sort plik3 4000 4 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 3000 512
	./main copy plik plik2 3000 512 0 >> wyniki.txt
	rm -f plik2
	./main copy plik plik3 3000 512 1 >> wyniki.txt
	chmod +rw plik3
	./main sort plik 3000 512 0 >>wyniki.txt
	./main sort plik3 3000 512 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 2000 512
	./main copy plik plik2 2000 512 0 >> wyniki.txt
	rm -f plik2
	./main copy plik plik3 2000 512 1 >> wyniki.txt
	chmod +rw plik3
	./main sort plik 2000 512 0 >>wyniki.txt
	./main sort plik3 2000 512 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 2500 4096
	./main copy plik plik2 2500 4096 0 >> wyniki.txt
	rm -f plik2
	./main copy plik plik3 2500 4096 1 >> wyniki.txt
	chmod +rw plik3
	./main sort plik 2500 4096 0 >>wyniki.txt
	./main sort plik3 2500 4096 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 2000 4096
	./main copy plik plik2 2000 4096 0 >> wyniki.txt
	rm -f plik2
	./main copy plik plik3 2000 4096 1 >> wyniki.txt
	chmod +rw plik3
	./main sort plik 2000 4096 0 >>wyniki.txt
	./main sort plik3 2000 4096 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 2000 8192
	./main copy plik plik2 2000 8192 0 >> wyniki.txt
	rm -f plik2
	./main copy plik plik3 2000 8192 1 >> wyniki.txt
	chmod +rw plik3
	./main sort plik 2000 8192 0 >>wyniki.txt
	./main sort plik3 2000 8192 1 >>wyniki.txt
	rm -f plik*
	./main generate plik 2500 8192
	./main copy plik plik2 2500 8192 0 >> wyniki.txt
	rm -f plik2
	./main copy plik plik3 2500 8192 1 >> wyniki.txt
	chmod +rw plik3
	./main sort plik 2500 8192 0 >>wyniki.txt
	./main sort plik3 2500 8192 1 >>wyniki.txt

clean:
	rm -f plik* *.o main