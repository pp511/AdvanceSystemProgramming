CC      = gcc

default: hintsudokutest test

hintsudokutest: hintsudokutest.c
	$(CC) -o hintsudokutest hintsudokutest.c
	
test: test.c
	$(CC) -o test test.c
	
clean:
	rm -f core *.o hintsudokutest test
