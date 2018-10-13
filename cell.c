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
  int val;
  int rWall;
  int bWall;
  int bors[4]; //nieghbors of each cell [L/0, T/1, R/2, B/3]
  int numBors;
  char bors2[4];
  int visited;
};

extern CELL   *newCELL(void)
{
  CELL * cellStruct = malloc(sizeof(CELL));  //allocates struct
  cellStruct->row = 0;
  cellStruct->col = 0;
  cellStruct->val = 0;
  cellStruct->rWall = 1;
  cellStruct->bWall = 1;
  cellStruct->visited = 0;

  for (int i = 0; i < 4; i++)   //Default to no neighbors
  {
      cellStruct->bors[i] = 0;
      cellStruct->bors2[i] = ' ';
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
    c->bors2[0] = 'r'; //right
    c->bors2[1] = 'b'; //bottom
    c->visited = 1;
    c->numBors = 2;

  }
  else if(row == 0 && col == lastCol) //top right, 0, lastCol
  {
    c->bors2[0] = 'l'; //Left
    c->bors2[1] = 'b'; //bottom

    c->numBors = 2;
  }
  else if(row == lastRow && col == 0) //bottom left, 0, lastCol
  {
    c->bors2[0] = 't'; //top
    c->bors2[1] = 'r'; //right

    c->numBors = 2;
  }
  else if(row == lastRow && col == lastCol) //bottom right, 0, lastCol
  {
    c->bors2[0] = 't'; //top
    c->bors2[1] = 'l'; //left
    c->numBors = 2;
    c->rWall = 0; //Maze exit
  }
  else if (row == 0)  //top row, not corner
  {
    c->bors2[0] = 'l'; //left
    c->bors2[1] = 'r'; //right
    c->bors2[2] = 'b'; //bottom

    c->numBors = 3;
  }
  else if (row == lastRow)  //bottom row, not corner
  {
    c->bors2[0] = 't'; //top
    c->bors2[1] = 'l'; //left
    c->bors2[2] = 'r'; //right

    c->numBors = 3;
  }
  else if (col == 0)  //bottom row, not corner
  {
    c->bors2[0] = 't'; //top
    c->bors2[1] = 'r'; //bottom
    c->bors2[2] = 'b'; //right

    c->numBors = 3;
  }
  else if (col == lastCol)  //bottom row, not corner
  {
    c->bors2[0] = 't'; //top
    c->bors2[1] = 'l'; //left
    c->bors2[2] = 'b'; //bottom

    c->numBors = 3;
  }
  else
  {
    c->bors2[0] = 't'; //top
    c->bors2[1] = 'l'; //left
    c->bors2[2] = 'r'; //right
    c->bors2[3] = 'b'; //bottom

    c->numBors = 4;
  }

  return 0;
}

extern int testBor(CELL* c)
{
  printf("row: %d | col: %d\n",c->row, c->col );
  printf("curCell numBors: %d\n" , c->numBors);
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
    char chosen = c->bors2[index];
    //printf("Rand Index: %d = %c\n", index, chosen);
    return chosen;
  }

}

extern char getBor(CELL* c, int i)
{
  //printf("Rand Index: %d = %c\n", index, chosen);
  //printf("getBORS TEST[%d] = [%c,%c,%c,%c]\n",c->numBors,c->bors2[0],c->bors2[1],c->bors2[2],c->bors2[3]);
  return c->bors2[i];
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
  //printf("currBORS1[%d] = [%c,%c,%c,%c]\n",curr->numBors,curr->bors2[0],curr->bors2[1],curr->bors2[2],curr->bors2[3]);

  for (int i = 0; i < 4; i++)
  {
    if(curr->bors2[i] == direction)
    {
      curr->bors2[i] = ' ';
      for(int k = i; k <  3; k++) //shift
      {
        curr->bors2[k] = curr->bors2[k+1];
      }
      curr->bors2[3] = ' ';
    }
  }
  if (curr->numBors > 0)
  {
      curr->numBors--;
  }
  //printf("currBORS2[%d] = [%c,%c,%c,%c]\n",curr->numBors, curr->bors2[0],curr->bors2[1],curr->bors2[2],curr->bors2[3]);


}

extern void setnextCell(CELL* next, char direction)
{
  next->visited = 1;

  char opposite = ' ';
  if(direction == 't') {opposite = 'b';}
  else if(direction == 'l') {opposite = 'r';}
  else if(direction == 'r') {opposite = 'l';}
  else if(direction == 'b') {opposite = 't';}

  //printf("nextBORS1[%d] = [%c,%c,%c,%c]\n",next->numBors, next->bors2[0],next->bors2[1],next->bors2[2],next->bors2[3]);
  for (int i = 0; i < 4; i++)
  {
    if(next->bors2[i] == opposite)
    {
      next->bors2[i] = ' ';
      if (next->numBors > 0)
      {
          next->numBors--;
      }
      for(int k = i; k <  3; k++)
      {
        next->bors2[k] = next->bors2[k+1];
      }
      next->bors2[3] = ' ';
    }
  }


  //printf("nextBORS2[%d] = [%c,%c,%c,%c]\n",next->numBors,next->bors2[0],next->bors2[1],next->bors2[2],next->bors2[3]);
}

extern int beenVisited(CELL* c)
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
