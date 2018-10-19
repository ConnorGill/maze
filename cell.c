#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>

#include "cell.h"

extern void srandom(unsigned int);
extern long int random(void);

struct cell //typedef outside of func?
{
  int row;
  int col;
  int stepNum;
  int rWall;
  int bWall;
  int numBors;
  char bors[4];
  char openBors[4];
  int visited;
};

extern CELL   *newCELL(void)
{
  CELL * cellStruct = malloc(sizeof(CELL));  //allocates struct
  cellStruct->row = 0;
  cellStruct->col = 0;
  cellStruct->stepNum = 0;
  cellStruct->rWall = 1;
  cellStruct->bWall = 1;
  cellStruct->visited = 0;

  for (int i = 0; i < 4; i++)   //Default to no neighbors
  {
      cellStruct->bors[i] = ' ';
      cellStruct->openBors[i] = ' ';
  }
  return cellStruct;
}

extern int setcellProps(CELL* c, int row, int col, int totRows, int totCols)
{
  int lastRow = totRows - 1;  //last row index val
  int lastCol = totCols - 1;  //last col index val


  c->row = row;    //Initializing cells
  c->col = col;

  //Determining which nieghbors exists
  if (row == 0 && col == 0)   //top left  0,0
  {
    c->bors[0] = 'r'; //right
    c->bors[1] = 'b'; //bottom
    c->visited = 1;
    c->numBors = 2;

  }
  else if(row == 0 && col == lastCol) //top right, 0, lastCol
  {
    c->bors[0] = 'l'; //Left
    c->bors[1] = 'b'; //bottom

    c->numBors = 2;
  }
  else if(row == lastRow && col == 0) //bottom left, 0, lastCol
  {
    c->bors[0] = 't'; //top
    c->bors[1] = 'r'; //right

    c->numBors = 2;
  }
  else if(row == lastRow && col == lastCol) //bottom right, 0, lastCol
  {
    c->bors[0] = 't'; //top
    c->bors[1] = 'l'; //left
    c->numBors = 2;
    c->rWall = 0; //Maze exit
  }
  else if (row == 0)  //top row, not corner
  {
    c->bors[0] = 'l'; //left
    c->bors[1] = 'r'; //right
    c->bors[2] = 'b'; //bottom

    c->numBors = 3;
  }
  else if (row == lastRow)  //bottom row, not corner
  {
    c->bors[0] = 't'; //top
    c->bors[1] = 'l'; //left
    c->bors[2] = 'r'; //right

    c->numBors = 3;
  }
  else if (col == 0)  //bottom row, not corner
  {
    c->bors[0] = 't'; //top
    c->bors[1] = 'r'; //bottom
    c->bors[2] = 'b'; //right

    c->numBors = 3;
  }
  else if (col == lastCol)  //bottom row, not corner
  {
    c->bors[0] = 't'; //top
    c->bors[1] = 'l'; //left
    c->bors[2] = 'b'; //bottom

    c->numBors = 3;
  }
  else
  {
    c->bors[0] = 't'; //top
    c->bors[1] = 'l'; //left
    c->bors[2] = 'r'; //right
    c->bors[3] = 'b'; //bottom

    c->numBors = 4;
  }

  return 0;
}

extern int testBor(CELL* c)
{
  printf("openBORS2[%d][%d] = [%c,%c,%c,%c]\n",c->row, c->col, c->openBors[0],c->openBors[1],c->openBors[2],c->openBors[3]);
  return 0;
}

extern int getrWall(CELL* c)
{
  return c->rWall;
}

extern void removerWall(CELL* c)
{
  c->rWall = 0;
}

extern int getbWall(CELL* c)
{
  return c->bWall;
}

extern void removebWall(CELL* c)
{
  c->bWall = 0;
}

extern int getstepNum(CELL* c)
{
  return c->stepNum;
}

extern void setSeed(int userSeed)
{
  srandom(userSeed);
}

extern char chooseBor(CELL* c)
{
  if (c->numBors == 0) {return '!';}
  else
  {
    int index = random() % c->numBors;
    //printf("RAND INDEX: %d | BORS#: %d\n", index, c->numBors);
    char chosen = c->bors[index];
    //printf("Rand Index: %d = %c\n", index, chosen);
    return chosen;
  }

}

extern char getBor(CELL* c, int i)
{
  //printf("Rand Index: %d = %c\n", index, chosen);
  //printf("getBORS TEST[%d] = [%c,%c,%c,%c]\n",c->numBors,c->bors[0],c->bors[1],c->bors[2],c->bors[3]);
  return c->bors[i];
}

extern char getnumBors(CELL* c)
{
  return c->numBors;
}

extern void setVisit(CELL* curr)
{
  curr->visited = 1;
}

extern void setcurrCell(CELL* curr, char direction)
{
  curr->visited = 1;
  //printf("curr Bors = %d\n", curr->numBors);
  //printf("currBORS1[%d] = [%c,%c,%c,%c]\n",curr->numBors,curr->bors[0],curr->bors[1],curr->bors[2],curr->bors[3]);

  for (int i = 0; i < 4; i++) //update bors array
  {
    if(curr->bors[i] == direction)
    {
      curr->bors[i] = ' ';
      for(int k = i; k <  3; k++) //shift
      {
        curr->bors[k] = curr->bors[k+1];
      }
      curr->bors[3] = ' ';
    }
  }
  if (curr->numBors > 0)
  {
      curr->numBors--;
  }
  //printf("currBORS2[%d] = [%c,%c,%c,%c]\n",curr->numBors, curr->bors[0],curr->bors[1],curr->bors[2],curr->bors[3]);
}

extern void setnextCell(CELL* next, char direction)
{
  next->visited = 1;

  char opposite = ' ';
  if(direction == 't') {opposite = 'b';}
  else if(direction == 'l') {opposite = 'r';}
  else if(direction == 'r') {opposite = 'l';}
  else if(direction == 'b') {opposite = 't';}

  //printf("nextBORS1[%d] = [%c,%c,%c,%c]\n",next->numBors, next->bors[0],next->bors[1],next->bors[2],next->bors[3]);
  for (int i = 0; i < 4; i++)
  {
    if(next->bors[i] == opposite)
    {
      next->bors[i] = ' ';
      if (next->numBors > 0)
      {
          next->numBors--;
      }
      for(int k = i; k <  3; k++)
      {
        next->bors[k] = next->bors[k+1];
      }
      next->bors[3] = ' ';
    }
  }
  //printf("nextBORS2[%d] = [%c,%c,%c,%c]\n",next->numBors,next->bors[0],next->bors[1],next->bors[2],next->bors[3]);
}

extern void setopenBors(CELL* c, char direction)
{
  //printf("openBORS1[%d][%d] = [%c,%c,%c,%c]\n", c->row, c->col, c->openBors[0],c->openBors[1],c->openBors[2],c->openBors[3]);
  for (int i = 0; i < 4; i++)
  {
    if(direction == 't')
    {
      c->openBors[0] = 't';
    }
    else if(direction == 'l')
    {
      c->openBors[1] = 'l';
    }
    else if(direction == 'r')
    {
      c->openBors[2] = 'r';
    }
    else if(direction == 'b')
    {
      c->openBors[3] = 'b';
    }
  }
  //printf("openBORS2[%d][%d] = [%c,%c,%c,%c]\n",c->row, c->col, c->openBors[0],c->openBors[1],c->openBors[2],c->openBors[3]);
}

extern char getopenBor(CELL* c, int index)
{
  return c->openBors[index];
}



extern int getVisited(CELL* c)
{
  return c->visited;
}

extern int getRow(CELL* c)
{
  return c->row;
}

extern int getCol(CELL* c)
{
  return c->col;
}

extern void setrWall(CELL* c, int val)
{
  c->rWall = val;
}

extern void setbWall(CELL* c, int val)
{
  c->bWall = val;
}

extern void setStepNum(CELL* c, int val)
{
  c->stepNum = val;
}

extern void setNumBors(CELL* c, int val)
{
  c->numBors = val;
}

extern void setVisited(CELL* c, int val)
{
  c->visited = val;
}
