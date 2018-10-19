#ifndef __CELL_INCLUDED__
    #define __CELL_INCLUDED__

    #include <stdio.h>

    typedef struct cell CELL;
    extern CELL *newCELL(void);
    extern int setcellProps(CELL* c, int row, int col, int totRows, int totCols);
    extern int testBor(CELL* c);
    extern int getrWall(CELL* c);
    extern int getbWall(CELL* c);
    extern int getstepNum(CELL* c);
    extern void setSeed(int userSeed);
    extern char chooseBor(CELL* c);
    extern char getBor(CELL* c, int i);
    extern char getnumBors(CELL* c);
    extern void setcurrCell(CELL* curr, char direction);
    extern void setnextCell(CELL* next, char direction);
    extern void setopenBors(CELL* c, char direction);
    extern char getopenBor(CELL* c, int index);
    extern int getVisited(CELL* c);
    extern void removerWall(CELL* c);
    extern void removebWall(CELL* c);
    extern int getRow(CELL* c);
    extern int getCol(CELL* c);
    extern void setVisit(CELL* curr);
    extern void setrWall(CELL* c, int val);
    extern void setbWall(CELL* c, int val);
    extern void setStepNum(CELL* c, int val);
    extern void setNumBors(CELL* c, int val);
    extern void setVisited(CELL* c, int val);



#endif
