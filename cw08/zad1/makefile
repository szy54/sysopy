all:
	gcc -Wall -c filter_gen.c
	gcc -Wall -o filter_gen filter_gen.o
	./filter_gen f1 3
	./filter_gen f2 10
	./filter_gen f3 30
	./filter_gen f4 65
	gcc -Wall -c main.c
	gcc -Wall -o main main.o -lpthread -lm
	./main 1 '1.pgm' 'f1' 'o1_1.pgm'
	./main 2 '1.pgm' 'f1' 'o1_2.pgm'
	./main 4 '1.pgm' 'f1' 'o1_3.pgm'
	./main 8 '1.pgm' 'f1' 'o1_4.pgm'
	./main 16 '1.pgm' 'f1' 'o1_5.pgm'
	./main 1 '2.pgm' 'f2' 'o2_1.pgm'
	./main 2 '2.pgm' 'f2' 'o2_2.pgm'
	./main 4 '2.pgm' 'f2' 'o2_3.pgm'
	./main 8 '2.pgm' 'f2' 'o2_4.pgm'
	./main 16 '2.pgm' 'f2' 'o2_5.pgm'
	./main 1 '1.pgm' 'f3' 'o3_1.pgm'
	./main 2 '1.pgm' 'f3' 'o3_2.pgm'
	./main 4 '1.pgm' 'f3' 'o3_3.pgm'
	./main 8 '1.pgm' 'f3' 'o3_4.pgm'
	./main 16 '1.pgm' 'f3' 'o3_5.pgm'
	./main 1 '2.pgm' 'f4' 'o4_1.pgm'
	./main 2 '2.pgm' 'f4' 'o4_2.pgm'
	./main 4 '2.pgm' 'f4' 'o4_3.pgm'
	./main 8 '2.pgm' 'f4' 'o4_4.pgm'
	./main 16 '2.pgm' 'f4' 'o4_5.pgm'
	rm -f *.o
clean:
	rm -f *.o f1 f2 f3 f4 main Times.txt o*
