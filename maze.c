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
  int curRow;
  int curCol;
  CELL * (**cellsArr);
};

extern MAZE   *newMAZE(void)
{
  MAZE * mazeStruct = malloc(sizeof(MAZE));  //allocates struct
  mazeStruct->rows = 0;
  mazeStruct->cols = 0;
  mazeStruct->curRow = 0;
  mazeStruct->curCol = 0;
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
   //testBor(m->cellsArr[0][0]);

  return 0;
}

//determines path of maze
extern int buildMaze(MAZE* m)
{
  printf("buildMaze\n");
  //int curRow = 0;
  //int curCol = 0;

  STACK * visited = newSTACK();
  CELL * currCell = m->cellsArr[0][0];
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
      m->curRow = getRow(currCell);
      m->curCol = getCol(currCell);

      int count = 0;  //track index of nieghbors
      if (beenVisited(currCell) == 1) //update adjacent cells
      {
        for(int i = 0; i < 4; i++)  //cycle through neighbors
        {
          tempBor = getBor(currCell, count); //get available bors
          count++;

          //printf("TEMPBOR: %c\n", tempBor);
          tempRow = m->curRow;
          tempCol = m->curCol;
          if (tempBor == ' ') //if empty, break loop
          {
            break;
          }
          //else if(tempBor == 't') { tempRow = tempRow-1; }
          //else if(tempBor == 'l') { tempCol = tempCol-1; }
          //else if(tempBor == 'r') { tempCol = tempCol+1; }
          //else if(tempBor == 'b') { tempRow = tempRow+1; }
          //else {break;}
          //printf("ADJACENT %c -> [%d][%d]\n",tempBor, tempRow, tempCol);
          //CELL * tempCell = m->cellsArr[tempRow][tempCol];
          CELL * tempCell = updateIndex(m, tempBor);
          if (beenVisited(tempCell) == 1) //if next cell have been visited
          {
            count = 0;
            setcurrCell(currCell, tempBor); //update currCell
          }
        }
      }

      chosenBor = chooseBor(currCell);
      printf("CURROW: %d | ", m->curRow);
      printf("CURCOL: %d | ", m->curCol);
      printf("CHOSENBOR: %c", chosenBor);
      printf(" | # BORS: %d\n",getnumBors(currCell) );
      //if(chosenBor == 't') { m->curRow--; }
      //else if(chosenBor == 'l') { m->curCol--; }
      //else if(chosenBor == 'r') { m->curCol++; }
      //else if(chosenBor == 'b') { m->curRow++; }

      //CELL * nextCell = m->cellsArr[m->curRow][m->curCol];
      CELL * nextCell = updateIndex(m, chosenBor);

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
          printf("nextBWALL BEFORE[%d][%d]: %d\n", m->curRow, m->curCol, getbWall(nextCell));
          removebWall(nextCell);
          printf("nextBWALL AFTER[%d][%d]: %d\n", m->curRow, m->curCol, getbWall(nextCell));
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);
         }
        else if (chosenBor == 'b')
        {
          printf("currBWALL BEFORE[%d][%d]: %d\n", m->curRow-1, m->curCol, getbWall(currCell));
          removebWall(currCell);
          printf("currBWALL AFTER[%d][%d]: %d\n", m->curRow-1, m->curCol, getbWall(currCell));
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);

        }
        else if (chosenBor == 'l')
        {
          printf("nextRWALL BEFORE[%d][%d]: %d\n", m->curRow, m->curCol, getrWall(nextCell));
          removerWall(nextCell);
          printf("nextRWALL AFTER[%d][%d]: %d\n", m->curRow, m->curCol, getrWall(nextCell));
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);
        }
        else if (chosenBor == 'r')
        {
          printf("currRWALL BEFORE[%d][%d]: %d\n", m->curRow, m->curCol-1, getrWall(currCell));
          removerWall(currCell);
          printf("currRWALL AFTER[%d][%d]: %d\n", m->curRow, m->curCol-1, getrWall(currCell));
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);
        }
        printf("PUSH @ [%d][%d]\n", m->curRow, m->curCol);
        push(visited, nextCell);
      }

  }
  freeSTACK(visited);
  return 0;
}

extern CELL* updateIndex(MAZE* m, char direction)
{
  int tempRow = m->curRow;
  int tempCol = m->curCol;
  if(direction == 't') { tempRow--; }
  else if(direction == 'l') { tempCol--; }
  else if(direction == 'r') { tempCol++; }
  else if(direction == 'b') { tempRow++; }
  CELL * nextCell = m->cellsArr[tempRow][tempCol];
  return nextCell;
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
      midline[z] = '-'; //reset midline to full
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
      CELL * currCell = m->cellsArr[i][j];
      //printf("BWALL AT[%d][%d] = %d\n", i, j, getbWall(currCell));
      if (getrWall(currCell) == 1) //rWall exists
      {
        printf(" %d |", j);
      }
      else if (getrWall(currCell) == 0) //no rWall
      {
        printf(" %d  ", j);
      }
      if (getbWall(currCell) == 0) //remove bottom wall
      {
        //printf("COL: %d | TRIGGERED lineIndex: %d\n",j, lineIndex);
        midline[lineIndex] = ' ';
        midline[lineIndex + 1] = ' ';
        midline[lineIndex + 2] = ' ';
        lineIndex = lineIndex + 4;
      }
      else if(getbWall(currCell) == 1)  //update line index
      {
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
