#ifndef __MAZE_INCLUDED__
    #define __MAZE_INCLUDED__

    #include <stdio.h>
    #include "cell.h"

    typedef struct maze MAZE;

    extern MAZE *newMAZE(void);

    extern void createMaze(MAZE* m, int rows, int cols);
    extern void buildMaze(MAZE* m);
    extern void mazetoFile(MAZE* m, char* mazeFile);
    extern MAZE * mazefromFile(char* mazeFile);
    extern void solveMaze(MAZE* m);
    extern void drawMaze(MAZE* m);
    extern CELL* updateIndex(MAZE* m, char direction);
    extern void freeMaze(MAZE* m);


#endif
