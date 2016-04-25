#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int isValSudoku(char board[][9]);

int main()
{
	char inp[200];//Input boardray.
    char *token;
	char board[9][9]={0};//Data structure to store the current state of board.
	int i=0,j=0,k=0,l=0;
	int hint[9][9][9]={0};//Output.
/*The current sudoku state is stored in sudokuboard.txt in the form of a string.
* This is fetched and stored in board.
*/
	FILE *fp = fopen("sudokuboard.txt","r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	//printf("Input String\n");
	fscanf(fp,"%[^\n]",inp);
	//printf("\n%s",inp);
	token = strtok(inp, " ");
	while(token != NULL){
		board[i][j]=token[0];
		token = strtok(NULL, " ");
		j = (j+1)%9;
		if(j == 0) i++;
	}
	fclose(fp);
	//printf("\nScanning Complete. Printing the Current sudoku state \n");
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
		//	printf("%c ",board[i][j]);
		}
	//	printf("\n");
	}
/*Checkin if the given board has any errors*/
	if(!isValSudoku(board)){
		printf("Error");
		return(0);
	}
	//printf("This is a valid Sudoku. Let's give you hints...\n\n");
	int temp[9]={0};
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			int row=0,col=0;
/*Temp array is used to keep a track of hints for every cell.
 *After execution of "if" condition,for every value of temp[i] if(temp[i]) = 0 =>
 *i+1 is a valid hint because temp[i] is not seen either in row,coluom or the sub grid.*/
			if(board[i][j]=='.'){
				for(row=0;row<9;row++){
					if(board[row][j]!='.')
						temp[board[row][j] - '1']++;//All elements in jth row are updated in the temp.
				}
				for(col=0;col<9;col++){
					if(board[i][col]!='.')
					temp[board[i][col] - '1']++;//All elements in jth row are updated in the temp.
				}
				int m=0,n=0;
				int blockrow=i/3;
				int blockcol=j/3;
				for(m=0;m<3;m++){//Elements in the block to which this cell (i,j) belongs are updated in temp.
						for(n=0;n<3;n++){
							if(board[blockrow*3 + m][blockcol*3 + n]!='.'){
								temp[board[blockrow*3 + m][blockcol*3 + n]-'1']++;
							}
						}
				}
				l=0;
				for(k=0;k<9;k++){
					if(!temp[k]){
						hint[i][j][l++]=k+1;//Outout data structure hint is updated based on the value of temp.
					}
					else temp[k]=0;
				}
			}
		}
	}
/*Printing the Output*/
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			if(board[i][j]!='.')
				//printf("Position (%d,%d). Possible Choices-> None",i+1,j+1);
				printf("0");
			else{
				//printf("Position (%d,%d). Possible Choices->",i+1,j+1);
				for(k=0;k<9 && hint[i][j][k]!=0;k++){
					if(hint[i][j][k])
						printf("%d",hint[i][j][k]);
					}
			}
		printf("\n");

		}
	}
}

/*Function to determine if the given board  is valid.Different data structures are
* used to track if multiple elements appear in a row, column or a sub-grid.
* Hints are given only if this function succeeds. Else Error
* message is displayed
*/
int isValSudoku(char board[][9]) {
	int r,c;
    int row[9][9]={0};
    int col[9][9]={0};
    int block[3][3][9]={0};
    for(r=0;r<9;r++)
        for(c=0;c<9;c++)
            if(board[r][c]!='.'){
                int number=board[r][c]-'1';
                if(row[r][number]++) return 0;
                if(col[c][number]++) return 0;
                if(block[r/3][c/3][number]++) return 0;
            }
    return 1;
}


