#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include "maze.h"
#include "cell.h"
#include "stack.h"
#include "queue.h"




struct maze
{
  int rows;
  int cols;
  int totDashes;
  int curRow;
  int curCol;
  int beenBuilt;
  CELL * (**cellsArr);
};

extern MAZE   *newMAZE(void)
{
  MAZE * mazeStruct = malloc(sizeof(MAZE));  //allocates struct
  mazeStruct->rows = 0;
  mazeStruct->cols = 0;
  mazeStruct->totDashes = 0;  //dashes that make up horizontal lines
  mazeStruct->curRow = 0;
  mazeStruct->curCol = 0;
  mazeStruct->beenBuilt = 0;
  mazeStruct->cellsArr = 0; //allocates array

  return mazeStruct;
}


//func for creatiing maze, setting cells
extern void createMaze(MAZE* m, int rows, int cols)
{
  //CREATE cellsArr 2D Array
  m->rows = rows;
  m->cols = cols;
  m->totDashes = (m->cols * 4) + 1;  //dashes in a row

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

   buildMaze(m);  //creates maze path
}

//determines path of maze
extern void buildMaze(MAZE* m)
{

  STACK * visited = newSTACK();
  CELL * currCell = m->cellsArr[0][0];
  char chosenBor;
  char tempBor;

  push(visited, currCell);  //first cell into stack

  if (m->rows == 1 && m->cols == 1)
  {
    return;
  }

  while (sizeSTACK(visited) != 0)
  {
      currCell = (CELL *)peekSTACK(visited);    //set curr to top stack
      setVisit(currCell); //set curr as been visited
      m->curRow = getRow(currCell); //update index location
      m->curCol = getCol(currCell);

      int count = 0;  //track index of nieghbors
      if (getVisited(currCell) == 1) //update adjacent cells
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
          //printf("ADJACENT %c -> [%d][%d]\n",tempBor, getRow(tempCell), getCol(tempCell));
          if (getVisited(tempCell) == 1) //if next cell have been visited
          {
            count = 0;
            setcurrCell(currCell, tempBor); //update currCell
          }
        }
      }

      chosenBor = chooseBor(currCell);
      //printf("CURROW: %d | ", m->curRow);
      //printf("CURCOL: %d | ", m->curCol);
      //printf("CHOSENBOR: %c", chosenBor);
      //printf(" | # BORS: %d\n",getnumBors(currCell) );

      CELL * nextCell = updateIndex(m, chosenBor);

      if (chosenBor == '!') //if no nieghbors
      {
        pop(visited);
        //testBor(currCell);
        //printf("POPPED!\n");
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
        //printf("PUSH @ [%d][%d]\n", m->curRow, m->curCol);
        push(visited, nextCell);
      }

  }
  freeSTACK(visited);
}

//put unsolved maze info into file
extern void mazetoFile(MAZE* m, char* mazeFile)
{
  FILE * outFileM;  //file containing maze info
  outFileM = fopen(mazeFile, "w");

  fprintf(outFileM, "%d %d %d\n", m->rows, m->cols, m->totDashes);  //maze info

  for (int i = 0; i < m->rows; i++)  //cell info
  {
      for (int j = 0; j < m->cols; j++)
      {
          CELL * c = m->cellsArr[i][j];
          // rWall, bWall, stepNumber, numBors, visited
          fprintf(outFileM, "%d %d %d %d %d\n", getrWall(c), getbWall(c), getstepNum(c), getnumBors(c), getVisited(c));
      }
  }

  fclose(outFileM);
}

extern MAZE * mazefromFile(char* mazeFile)
{
  FILE * inFile;
  inFile = fopen(mazeFile, "r");
  MAZE * m = newMAZE();

  int totRows = 0;
  int totCols = 0;
  int totDashes = 0;


  //else
  //{
      fscanf(inFile, "%d %d %d", &totRows, &totCols, &totDashes);
      m->totDashes = totDashes;
      createMaze(m, totRows, totCols);
      for (int i = 0; i < totRows; i++)
      {
          for (int j = 0; j < totCols; j++)
          {
              CELL * c = m->cellsArr[i][j];
              int rWall = 0;
              int bWall = 0;
              int stepNum = 0;
              int numBors = 0;
              int visited = 0;
              // reading numbers line by line
              // rightWall, bottomWall, value, neighbor count, visited
              fscanf(inFile, "%d %d %d %d %d", &rWall, &bWall, &stepNum, &numBors, &visited);
              // analyse res and process num
              setrWall(c, rWall);
              setbWall(c, bWall);
              setstepNum(c, stepNum);
              setNumBors(c, numBors);
              setVisited(c, visited);
          }
      //}
  }

  fclose(inFile);
  return m;
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

extern void solveMaze(MAZE* m)
{
  for (int i = 0; i < m->rows; ++i) //each row
  {
     for (int j = 0; j < m->cols; ++j)  //each col
     {
        CELL * c = m->cellsArr[i][j];
        setVisited(c, 0); //reset visited val to 0
        if (getrWall(c) == 0) //if no right wall
        {
          if(getRow(c) != m->rows -1 || getCol(c) != m->cols -1)  //if not exit
          {
            setopenBors(c, 'r');  //set possible neigbors for path
          }

          if(j != m->cols -1) //if not far right col
          {
            CELL * tempCell = m->cellsArr[i][j+1];
            setopenBors(tempCell, 'l');
          }
        }

        if (getbWall(c) == 0) //if no bottom wall
        {
          setopenBors(c, 'b');
          if (i != m->rows -1)  //if bottom row
          {
            CELL * tempCell = m->cellsArr[i+1][j];
            setopenBors(tempCell, 't');
          }
        }
     }
   }

  QUEUE * path = newQUEUE();
  CELL * currCell = m->cellsArr[0][0];

  int currStep = 0;
  int currRow = 0;
  int currCol = 0;

  setVisited(currCell, 1);
  setstepNum(currCell, currStep);

  enqueue(path, currCell);

  while(sizeQUEUE(path) != 0)
  {
    currCell = (CELL *)dequeue(path);
    currRow = getRow(currCell);
    currCol = getCol(currCell);

    if (currRow == m->rows - 1 && currCol == m->cols -1)  //if exit cell
    {
      currStep = getstepNum(currCell);
      setstepNum(currCell, currStep % 10);
      setVisited(currCell, 2);
      //int n = 0;
      while(sizeQUEUE(path) != 0)
      {
        CELL * leftover = (CELL *)dequeue(path); //cells leftover in QUEUE
        if (getstepNum(leftover) == getstepNum(currCell))
        {
            setVisited(leftover, 2);
        }

      }
      /*while(n < sizeQUEUE(path))  // TESTING
      {
        printf("CELL[%d][%d] - Visited = %d - Step# = %d\n", getRow(peekQUEUE(path)), getCol(peekQUEUE(path)), getVisited(peekQUEUE(path)), getstepNum(peekQUEUE(path)));
        n++;

      }*/
      break;
    }

    else
    {
      int tempRow = getRow(currCell);
      int tempCol = getCol(currCell);
      if(getopenBor(currCell, 0) == 't')  //if up FIXME: MIGHT BE AN ISSUE W THE ORDER
      {
        CELL * temp = m->cellsArr[tempRow -1][tempCol]; //set to next cell
        if (getVisited(temp) == 0)
        {
          setVisited(temp, 1);
          currStep = getstepNum(currCell) + 1;
          setstepNum(temp, currStep % 10);
          enqueue(path, temp);
        }
      }

      if(getopenBor(currCell, 3) == 'b')  //if down
      {
        CELL * temp = m->cellsArr[tempRow + 1][tempCol]; //set to next cell
        if (getVisited(temp) == 0)
        {
          setVisited(temp, 1);
          currStep = getstepNum(currCell) + 1;
          setstepNum(temp, currStep % 10);
          enqueue(path, temp);
        }
      }

      if(getopenBor(currCell, 1) == 'l')  //if left
      {
        CELL * temp = m->cellsArr[tempRow][tempCol - 1]; //set to next cell
        if (getVisited(temp) == 0)
        {
          setVisited(temp, 1);
          currStep = getstepNum(currCell) + 1;
          setstepNum(temp, currStep % 10);
          enqueue(path, temp);
        }
      }

      if(getopenBor(currCell, 2) == 'r')
      {
        CELL * temp = m->cellsArr[tempRow][tempCol + 1]; //set to next cell
        if (getVisited(temp) == 0)
        {
          setVisited(temp, 1);
          currStep = getstepNum(currCell) + 1;
          setstepNum(temp, currStep % 10);
          //printf("CELLT[%d][%d] - Step#: %d - Visited: %d\n", getRow(temp), getCol(temp), getstepNum(temp), getVisited(temp));
          enqueue(path, temp);
        }
      }

      setVisited(currCell, 2);

    }


  }
  freeQUEUE(path);

}

extern void drawMaze(MAZE* m)
{
  char midline[m->totDashes];
  int lineIndex; //index int to track midline
  //midline[lineIndex] = "-"; //arr holding midline info

  for (int z = 0; z < m->totDashes; z++)
  {
    printf("-");  //top wall
  }
  printf("\n");

  for (int i = 0; i < m->rows; i++) //for rows
  {
    for (int z = 0; z < m->totDashes; z++)
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
        if(getstepNum(currCell) >= 0 && getVisited(currCell) == 2)
        {
          printf(" %d |", getstepNum(currCell));
        }
        else
        {
          printf("   |");
        }
      }
      else if (getrWall(currCell) == 0) //no rWall
      {
        if(getstepNum(currCell) >= 0 && getVisited(currCell) == 2)
        {
            printf(" %d  ", getstepNum(currCell));
        }
        else
        {
          printf("    ");
        }

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
    for (int x = 0; x < m->totDashes; x++)
    {
      printf("%c", midline[x]);
    }
    printf("\n");

  }
}

extern void freeMaze(MAZE* m)
{
  for (int i = 0; i < m->rows; i++)
  {
    for (int j = 0; j < m->cols; j++)
    {
      free(m->cellsArr[i][j]);
    }
  }
  for (int i = 0; i < m->rows; i++)
    {
      free(m->cellsArr[i]);
    }
  free(m->cellsArr);
  free(m);
}
