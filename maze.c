#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include "maze.h"
#include "cell.h"
#include "stack.h"




struct maze
{
  int rows;
  int cols;
  CELL * (**cellsArr);
};

extern MAZE   *newMAZE(void)
{
  MAZE * mazeStruct = malloc(sizeof(MAZE));  //allocates struct
  mazeStruct->rows = 0;
  mazeStruct->cols = 0;
  mazeStruct->cellsArr = 0; //allocates array

  return mazeStruct;
}


//func for creatiing maze, setting cells
extern int createMaze(MAZE* m, int rows, int cols, char* mazeFile)
{
  //CREATE cellsArr 2D Array
  m->rows = rows;
  m->cols = cols;

  m->cellsArr = malloc(sizeof(CELL**) * m->rows);

  for (int i = 0; i < m->rows; ++i) //each row
  {
     //create a single row
     m->cellsArr[i] = malloc(sizeof(int *) * m->cols);
     for (int j = 0; j < m->cols; ++j)  //each col
     {
        m->cellsArr[i][j] = newCELL();
        setcellProps(m->cellsArr[i][j], i, j, rows, cols);
     }
   }
   buildMaze(m);
   testBor(m->cellsArr[0][0]);

  return 0;
}

//determines path of maze
extern int buildMaze(MAZE* m)
{
  printf("buildMaze\n");
  int curRow = 0;
  int curCol = 0;

  STACK * visited = newSTACK();
  CELL * currCell = m->cellsArr[curRow][curCol];
  char chosenBor;
  char tempBor;
  int tempRow = 0;
  int tempCol = 0;

  push(visited, currCell);  //first cell into stack

  //chosenBor = chooseBor(currCell);
  //printf("CHOSEN: %c\n", chosenBor);
  while (sizeSTACK(visited) != 0) //FIXME: A TIE?!?
  {

      currCell = (CELL *)peekSTACK(visited);    //set curr to top stack
      setVisit(currCell); //set curr as been visited
      curRow = getRow(currCell);
      curCol = getCol(currCell);

      int count = 0;  //track index of nieghbors
      if (beenVisited(currCell) == 1) //update adjacent cells
      {
        for(int i = 0; i < 4; i++)  //cycle through neighbors
        {
          tempBor = getBor(currCell, count); //get available bors
          count++;

          //printf("TEMPBOR: %c\n", tempBor);
          tempRow = curRow;
          tempCol = curCol;
          if (tempBor == ' ') //if empty, break loop
          {
            break;
          }
          else if(tempBor == 't') { tempRow = curRow-1; }
          else if(tempBor == 'l') { tempCol = curCol-1; }
          else if(tempBor == 'r') { tempCol = curCol+1; }
          else if(tempBor == 'b') { tempRow = curRow+1; }
          //else {break;}
          //printf("ADJACENT %c -> [%d][%d]\n",tempBor, tempRow, tempCol);
          CELL * tempCell = m->cellsArr[tempRow][tempCol];
          if (beenVisited(tempCell) == 1) //if next cell have been visited
          {
            count = 0;
            setcurrCell(currCell, tempBor); //update currCell
          }
        }
      }

      chosenBor = chooseBor(currCell);
      printf("CURROW: %d | ", curRow);
      printf("CURCOL: %d | ", curCol);
      printf("CHOSENBOR: %c", chosenBor);
      printf(" | # BORS: %d\n",getnumBors(currCell) );
      if(chosenBor == 't') { curRow--; }
      else if(chosenBor == 'l') { curCol--; }
      else if(chosenBor == 'r') { curCol++; }
      else if(chosenBor == 'b') { curRow++; }

      CELL * nextCell = m->cellsArr[curRow][curCol];

      if (chooseBor(currCell) == '!') //if no nieghbors
      {
        //printf("CELL VISITS: %d\n", beenVisited(nextCell));
        pop(visited);
        //testBor(currCell);
        printf("POPPED!\n");
      }
      else
      {
        if (chosenBor == 't')
        {
          printf("nextBWALL BEFORE[%d][%d]: %d\n", curRow, curCol, getbWall(nextCell));
          removebWall(nextCell);
          printf("nextBWALL AFTER[%d][%d]: %d\n", curRow, curCol, getbWall(nextCell));
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);
         }
        else if (chosenBor == 'b')
        {
          printf("currBWALL BEFORE[%d][%d]: %d\n", curRow, curCol, getbWall(currCell));
          removebWall(currCell);
          printf("currBWALL AFTER[%d][%d]: %d\n", curRow, curCol, getbWall(currCell));
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);

        }
        else if (chosenBor == 'l')
        {
          printf("nextRWALL BEFORE[%d][%d]: %d\n", curRow, curCol, getrWall(nextCell));
          removerWall(nextCell);
          printf("nextRWALL BEFORE[%d][%d]: %d\n", curRow, curCol, getrWall(nextCell));
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);
        }
        else if (chosenBor == 'r')
        {
          printf("nextRWALL BEFORE[%d][%d]: %d\n", curRow, curCol, getbWall(currCell));
          removerWall(currCell);
          printf("nextRWALL BEFORE[%d][%d]: %d\n", curRow, curCol, getbWall(currCell));
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);
        }
        printf("PUSH @ [%d][%d]\n", curRow, curCol);
        push(visited, nextCell);
      }

  }
  freeSTACK(visited);
  return 0;
}

extern int solveMaze(MAZE* m, char* mazeFile, char* solvedFile)
{
  printf("solveMaze\n");
  printf("mazeFile: %s\n", mazeFile);
  printf("solvedFile: %s\n", solvedFile);
  printf("rows: %d\n", m->rows);
  return 0;
}

extern int drawMaze(MAZE* m, char* drawFile)
{

  int numDash = (m->cols * 4) + 1;  //dashes in a row
  char midline[numDash];
  int lineIndex; //index int to track midline
  //midline[lineIndex] = "-"; //arr holding midline info

  for (int z = 0; z < numDash; z++)
  {
    printf("-");  //top wall
  }
  printf("\n");

  for (int i = 0; i < m->rows; i++) //for rows
  {
    for (int z = 0; z < numDash; z++)
    {
      midline[z] = '-';
    }

    if (i == 0) //if entrance to maze
    {
      printf(" "); //left wall
    }
    else
    {
      printf("|");  //left wall
    }
    lineIndex = 1;
    for(int j = 0; j < m->cols; j++) //for cols
    {
      if (getrWall(m->cellsArr[i][j]) == 1) //rWall exists
      {
        printf(" %d |", j);
      }
      else if (getrWall(m->cellsArr[i][j]) == 0) //no rWall
      {
        printf(" %d  ", j);
      }
      if (getbWall(m->cellsArr[i][j]) == 0) //remove bottom wall
      {
        midline[lineIndex] = ' ';
        midline[lineIndex + 1] = ' ';
        midline[lineIndex + 2] = ' ';
        lineIndex = lineIndex + 4;
      }
    }
    printf("\n");
    for (int x = 0; x < numDash; x++)
    {
      printf("%c", midline[x]);
    }
    printf("\n");

  }

  return 0;
}
