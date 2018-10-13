#ifndef __MAZE_INCLUDED__
    #define __MAZE_INCLUDED__

    #include <stdio.h>

    typedef struct maze MAZE;

    extern MAZE *newMAZE(void);

    extern int createMaze(MAZE* m, int rows, int cols, char* mazeFile);
    extern int buildMaze(MAZE* m);
    extern int solveMaze(MAZE* m, char* mazeFile, char* solvedFile);
    extern int drawMaze(MAZE* m, char* drawFile);


#endif
