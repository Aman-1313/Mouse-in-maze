#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 100

// constant definitions for the different cell states
const char WALL    = '1';
const char SPACE   = '0';
const char VISITED = '.';
const char MOUSE   = 'm';
const char EXIT    = 'e';

typedef enum BOOL { false, true } Boolean;

struct CELL
{
  int row;
  int column;
};
typedef struct CELL Cell;

typedef struct CELL_NODE CellNode;
struct CELL_NODE
{
  Cell     cell;
  CellNode *next;
};

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

CellNode *top = NULL;

// a 2D array used to store the maze
char maze[MAX_DIMENSION][MAX_DIMENSION];
int mazeRows;
int mazeCols;

// holds the location of the mouse and escape hatch
Cell mouse;
Cell escape;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

// basic cell manipulation

// returns true if the cells are at the same position in our maze
Boolean equalCells(const Cell cell1, const Cell cell2);
// returns a new cell object
Cell makeCell(const int row, const int col);
// returns true if the cell is within our maze
Boolean validCell(const Cell theCell);

// routines for managing our backtracking

// returns true if there are no more cells to try
Boolean noMoreCells();
// returns the next cell to try for a path out of the maze
Cell nextCell();
// introduces a new cell to try
void addCell(const Cell cell);
void deleteTop();
void printMaze();
void loadMaze();

// returns true if there's a solution to the maze
Boolean solveMaze();

// our invariant checker
void checkState();

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
    loadMaze();//read from the file into the 2d array

    if ( solveMaze() )
      printf( "The mouse is free!!!!\n" );
    else
      printf( "The mouse is trapped!!!!\n" );

    printf( "\nEnd of processing\n" );

  return EXIT_SUCCESS;
}


//////////////////////////////////////////////
// Cell routines
//////////////////////////////////////////////
Boolean equalCells(const Cell cell1, const Cell cell2){//checks if 2 cells are equal
  if ((cell1.row==cell2.row)&&(cell1.column==cell2.column)) {
    return true;
  }
  return false;
}

Cell makeCell(const int row, const int col){//make a new cell 
  Cell refer;
  refer.row = row;
  refer.column = col;
  return refer;
}

Boolean validCell(const Cell theCell){//check if the cell is b/w the range of maze
  if(theCell.row<0||theCell.column<0||
    theCell.row>mazeRows||theCell.column>mazeCols){
    return false;
  }
  return true;
}
//............BACKTRACKING ALGORITHM..................................\\
Boolean solveMaze(){//
  printMaze();
    Cell goalCell = escape;
    Cell startCell = mouse;
    Cell currentCell = startCell;
    while(!equalCells(currentCell,goalCell)){
        int row = currentCell.row;
      int col = currentCell.column;
      maze[row][col] = VISITED;
      printMaze();
      if(!(maze[row+1][col]==WALL||maze[row+1][col]==VISITED)){
        addCell(makeCell(row+1,col));
        //printf("asvjvc");
      }
      if(!(maze[row-1][col]==WALL||maze[row-1][col]==VISITED)){
        addCell(makeCell(row-1,col));
        // printf("asvjvc");
      }
      if(!(maze[row][col+1]==WALL||maze[row][col+1]==VISITED)){
        addCell(makeCell(row,col+1));
        // printf("asvjvc");
      }
      if(!(maze[row][col-1]==WALL||maze[row][col-1]==VISITED)){
        addCell(makeCell(row,col-1));
        // printf("asvjvc");
      }
      if(noMoreCells()){
        return false;
      }
      else{
         Cell c = nextCell();
         currentCell.row = c.row;
         currentCell.column=c.column;
         printf("%d%d\n",currentCell.row,currentCell.column );
         deleteTop();
           printf("%d%d\n",currentCell.row,currentCell.column );

    }
  }return true;

}
//////////////////////////////////////////////
// List routines
//////////////////////////////////////////////
void deleteTop()//delete the cell from the list
{   if(top->next == NULL){
       Cell c =top->cell;
       printf("deleting %d %d \n",c.row,c.column);
       top =NULL;
    }else{
      Cell c =top->cell;
      printf("deleting %d %d \n",c.row,c.column);
      CellNode *temp = top->next;
      top->cell = top->next->cell;
      top->next = temp->next;
      free(temp);
      c =top->cell;
      printf("new top %d %d \n",c.row,c.column);
    }
}
void addCell(const Cell cell){//add the cell to the linked list

  CellNode *node=malloc(sizeof(CellNode));
  node->cell=cell;
  node->next=top;
  top = node;
  Cell c =top->cell;
printf("adding %d %d \n",c.row,c.column);

}
Boolean noMoreCells(){//return true if the list is empty
  if(top==NULL){
    return true;
  }
  return false;
}

Cell nextCell(){//return the next cell from the list
  Cell c =top->cell;
  Cell a;
  a.row = c.row;
  a.column = c.column;
  printf("going to %d %d \n",c.row,c.column);
  return a;
}


//////////////////////////////////////////////
// Maze routines
//////////////////////////////////////////////
void loadMaze(){//reads from file to the array
    char line[MAX_DIMENSION];
    int i = 0;
    FILE *f =fopen("maze3.txt","r");
    if (fgets(line, MAX_DIMENSION , f ) != NULL)
      {
         mazeRows = atoi(&line[0]);
         mazeCols = atoi(&line[2]);
         printf("%d %d\n",mazeRows,mazeCols);
         while (fgets(line, MAX_DIMENSION, f ) != NULL)
        {   int k = 0;
            for (int j = 0; j < strlen(line); j++ ){
                if(!isspace(line[j])){// dont read the spaces into the array
                  maze[i][k]=line[j];
                  char c = line[j];
                  if(c=='r'){
                    mouse=makeCell(i,k);
                  }
                  if(c=='e'){
                    escape=makeCell(i,k);
                  }
                  k++;
                }
            }i++;
        }
    }
}

void printMaze(){//print the array
  for(int i =0;i<mazeRows;i++){
    for (int j = 0; j < mazeCols; j++) {
      printf("%c",maze[i][j]);
    }
    printf("\n");
  }printf("\n");
}
