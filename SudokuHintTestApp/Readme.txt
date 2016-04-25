Main test file is “test.c”. 
Steps to execute: Go to the directory “TestCode”.
Compile : make all
Execute : ./test
Clean : make clean
By default, the Sudoku configuration from sudokuboard.txt is loaded for the test.
•	When test is executed, it creates a child process and calls exec on it to load the hintsudokutest application.
•	Standard output of hintsudokutest is connected to the write end of a pipe using dup2 in order to establish inter-process communication.
•	Standard input of the parent process(test) is modified to read from the read end of the pipe.
•	hintsudokutest prints an error message if it finds that the board is invalid. Parent reads this message from the pipe and displays “Algorithm detects faulty Sudoku Configuration. Check the board”.
•	If the board is not faulty then hints for every cell is send to the standard output in sequential order.
•	This input is read by the test application in array of string.
•	This array of strings is parsed and the hints are stored in a 3d array ‘hint’.
•	Till this time, test application has not yet verified anything, it has just gathered the output of the hintsudoku application.
•	test application then checks the validity of every hint ‘k’ corresponding to a cell i,j. It scans through the row ‘j’ , columne ‘i’ and the sub-grid in which (i,j) belongs. If the hint matches with any of these values then it is the incorrect hint.

Limitations:
•	isValSudoku is just checking if the current state of Sudoku is valid or not. A valid Sudoku doesn’t mean that it is solvable.
•	I have found hintsudokutest application to be working in the test cases I could imagine. Even when the test application covering all the negative test cases, in positive test cases it is just able 		to verify if a given hint is correct or not for a given cell. It is not checking if the list of hints are all exhaustive. In order to check if the hints are there are two ways possible:
	Either we have a different working tested app with which we match out output and confirm.
	We come up with a list of all possible hints and store them in a data structure. Then we could match it without output.

