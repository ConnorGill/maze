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
extern void createMaze(MAZE* m, int rows, int cols, char* mazeFile)
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

   FILE * outFileM;
   outFileM = fopen(mazeFile, "w");
   fprintf(outFileM, "TEMP\n");
   //testBor(m->cellsArr[0][0]);
   fclose(outFileM);
}

//determines path of maze
extern void buildMaze(MAZE* m)
{
  printf("buildMaze\n");

  STACK * visited = newSTACK();
  CELL * currCell = m->cellsArr[0][0];
  char chosenBor;
  char tempBor;

  push(visited, currCell);  //first cell into stack

  while (sizeSTACK(visited) != 0) //FIXME: A TIE?!?
  {

      currCell = (CELL *)peekSTACK(visited);    //set curr to top stack
      setVisit(currCell); //set curr as been visited
      m->curRow = getRow(currCell); //update index location
      m->curCol = getCol(currCell);

      int count = 0;  //track index of nieghbors
      if (beenVisited(currCell) == 1) //update adjacent cells
      {
        for(int i = 0; i < 4; i++)  //cycle through neighbors
        {
          tempBor = getBor(currCell, count); //get available bors
          count++;

          //printf("TEMPBOR: %c\n", tempBor);
          if (tempBor == ' ') //if empty, break loop
          {
            break;
          }

          CELL * tempCell = updateIndex(m, tempBor);
          printf("ADJACENT %c -> [%d][%d]\n",tempBor, getRow(tempCell), getCol(tempCell));
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

      CELL * nextCell = updateIndex(m, chosenBor);

      if (chosenBor == '!') //if no nieghbors
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
          removebWall(nextCell);
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);
         }
        else if (chosenBor == 'b')
        {
          removebWall(currCell);
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);

        }
        else if (chosenBor == 'l')
        {
          removerWall(nextCell);
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);
        }
        else if (chosenBor == 'r')
        {
          removerWall(currCell);
          setcurrCell(currCell, chosenBor);
          setnextCell(nextCell, chosenBor);
        }
        printf("PUSH @ [%d][%d]\n", m->curRow, m->curCol);
        push(visited, nextCell);
      }

  }
  freeSTACK(visited);
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

  return 0;
}

extern void drawMaze(MAZE* m, char* drawFile)
{
  FILE * outFile;
  outFile = fopen(drawFile, "w");

  int numDash = (m->cols * 4) + 1;  //dashes in a row
  char midline[numDash];
  int lineIndex; //index int to track midline
  //midline[lineIndex] = "-"; //arr holding midline info

  for (int z = 0; z < numDash; z++)
  {
    fprintf(outFile, "-");  //top wall
  }
  fprintf(outFile,"\n");

  for (int i = 0; i < m->rows; i++) //for rows
  {
    for (int z = 0; z < numDash; z++)
    {
      midline[z] = '-'; //reset midline to full
    }

    if (i == 0) //if entrance to maze
    {
      fprintf(outFile, " "); //left wall
    }
    else
    {
      fprintf(outFile, "|");  //left wall
    }
    lineIndex = 1;
    for(int j = 0; j < m->cols; j++) //for cols
    {
      CELL * currCell = m->cellsArr[i][j];
      //printf("BWALL AT[%d][%d] = %d\n", i, j, getbWall(currCell));
      if (getrWall(currCell) == 1) //rWall exists
      {
        fprintf(outFile, "   |");
      }
      else if (getrWall(currCell) == 0) //no rWall
      {
        fprintf(outFile, "    ");
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
    fprintf(outFile, "\n");
    for (int x = 0; x < numDash; x++)
    {
      fprintf(outFile, "%c", midline[x]);
    }
    fprintf(outFile, "\n");

  }
  fclose(outFile);
}
