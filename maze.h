#ifndef __MAZE_INCLUDED__
    #define __MAZE_INCLUDED__

    #include <stdio.h>
    #include "cell.h"

    typedef struct maze MAZE;

    extern MAZE *newMAZE(void);

    extern void createMaze(MAZE* m, int rows, int cols, char* mazeFile);
    extern void buildMaze(MAZE* m);
    extern int solveMaze(MAZE* m, char* mazeFile, char* solvedFile);
    extern void drawMaze(MAZE* m, char* drawFile);
    extern CELL* updateIndex(MAZE* m, char direction);


#endif
