#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	int status;
	int pfd[2];//File descriptors for pipe
	char hint[9][9][9]={0};
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int i=0,j=0,k=0;
	int l=0;
	char board[9][9];//Board stores the current state of the sudoku.
	char* token;
	char inp[200];
	char element;
	int count=0;
	int interation = 0;
	FILE *fp = fopen("sudokuboard.txt","r");//Reading from the file
	if (fp == NULL)
		exit(EXIT_FAILURE);
	fscanf(fp,"%[^\n]",inp);
		//printf("\n%s",inp);
	token = strtok(inp, " ");
	while(token != NULL){
		board[i][j]=token[0];
		token = strtok(NULL, " ");
		j = (j+1)%9;
		if(j == 0) i++;
	}
	for(i=0;i<9;i++){//Displaying the Sudoku
		for(j=0;j<9;j++){
			printf("%c ",board[i][j]);
		}
		printf("\n");
	}
	pipe(pfd);//Creating pipe with pfd[0],pfd[1] as two ends


	if (pipe(pfd) < 0){
	 perror("pipe");
	 return 1;
	}
	pid_t pid = fork();//Creating child process.
	switch(pid){
	case 0:
	  dup2(pfd[1], 1);//Connecting the Standard output of the child process with the pipe.
	  close(pfd[0]);//Not needed by child.
	  execvp("./hintsudokutest",NULL);//Starting the hintsudokutest application.
	  break;
	case -1:
	  perror("Fork");
	  break;
	default:
	  dup2(pfd[0], 0);//Connecting the Standard input of the parent process with the pipe.
	  close(pfd[1]);//Not needed by parent.
	  break;
	}
	char* p[81]={NULL};//Array of string to which the output comming from hintsudokutest app is copied.
	for(i=0;i<81;i++){
			 if((read = getline(&line, &len, stdin))!=-1){
				 if(!count){
					 count=1;
					 //Error meesage is sent to the std output by hintsudokutest app if board is found faulty.
					 if(strcmp(line,"Error")==0){
						 printf("Algorithm detects faulty Sudoku Configuration. Check the board\n");
						 while ((pid = wait(&status)) != -1)
						//Waiting for child process to exit before termination so that the child process may not become orphan.
						 	//printf("process %d exits\n", pid);
						 goto failure;
					 }
				 }
				 /*If error is not received by parent in it's std input, it means that board was found OK.
				  *Now the hintsudokutest is trying to generate hints which are stored in p*/
				  p[l]=(char *)malloc(sizeof(char)*len);
				  strcpy(p[l],line);
				  l++;
			 }
	}
	while ((pid = wait(&status)) != -1)
		//printf("process %d exits\n", pid);

	k=0;
	l=0;
	//Copying the data tp the hint data structue - a 3d array which copies hints for every cell(i,j).
	for(i=0;i<81;i++){
		for(j=0;p[i][j]!='\0';j++){
			//printf("%c",p[i][j]);
			hint[k][l][j]=p[i][j];
		}
		l = (l+1)% 9 ;
		if(l==0) k++;
	}
/*Check for every hint of (i,j) cell if the number appears in either the same row,colum or sub-grid*/
	l=0;
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			for(k=0;k < 9 && hint[i][j][k]!='0' ;k++){
				element = hint[i][j][k];
			}
			int row=0,col=0;
			for(row=0;row<9;row++){
				if(board[row][j]==element){
					printf("Test Failed");
					goto failure;
				}
			}
			for(col=0;col<9;col++){
				if(board[i][col]==element){
					printf("Test Failed");
					goto failure;
				}
			}
			int m=0,n=0;
			int blockrow=i/3;
			int blockcol=j/3;
			for(m=0;m<3;m++){
					for(n=0;n<3;n++){
						if(board[blockrow*3 + m][blockcol*3 + n]==element){
							printf("Test Failed");
							goto failure;
						}
					}
				}
			}
		}
	printf("Test Passed.\n");
/*	for(i=0;i<9;i++){
			for(j=0;j<9;j++){
					for(k=0;k<9;k++){
						printf("%c",hint[i][j][k]);
						}
				}
	}*/
	failure:
	fclose(fp);
	close(pfd[0]);
	return 0;
}
