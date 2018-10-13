#ifndef __CELL_INCLUDED__
    #define __CELL_INCLUDED__

    #include <stdio.h>

    typedef struct cell CELL;
    extern CELL *newCELL(void);
    extern int setcellProps(CELL* c, int row, int col, int totRows, int totCols);
    extern int testBor(CELL* c);
    extern int getrWall(CELL* c);
    extern int getbWall(CELL* c);
    extern void setSeed(int userSeed);
    extern char chooseBor(CELL* c);
    extern char getBor(CELL* c, int i);
    extern char getnumBors(CELL* c);
    extern void setcurrCell(CELL* curr, char direction);
    extern void setnextCell(CELL* next, char direction);
    extern int beenVisited(CELL* c);
    extern void removerWall(CELL* c);
    extern void removebWall(CELL* c);
    extern int getRow(CELL* c);
    extern int getCol(CELL* c);
    extern void setVisit(CELL* curr);


#endif
