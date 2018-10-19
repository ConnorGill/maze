#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include "maze.h"
#include "cell.h"
#include "amaze.h"

int main(int argc,char **argv)
{
  MAZE *m = newMAZE();

  char *mazeFile;
  char *solvedFile;
  char *drawFile;
  int rows = 0;
  int cols = 0;
  int userSeed = 0;
  int name = 0;
  int create = 0;
  int solve = 0;
  int draw = 0;

  if (argc == 1) Fatal("%d arguments!\n",argc-1);

  for (int i = 1; i < argc; ++i)
  {
    if (strcmp(argv[i], "-r") == 0)
    {
      userSeed = atoi(argv[i + 1]);
      setSeed(userSeed);
    }

    if (strcmp(argv[i], "-v") == 0) { name = i; }
    if (strcmp(argv[i], "-c") == 0) { create = i; }
    if (strcmp(argv[i], "-s") == 0) { solve = i; }
    if (strcmp(argv[i], "-d") == 0) { draw = i; }
  }

  if (name)
  {
    printf("Connor Gill\n");
  }
  if (create)
  {
    rows = atoi(argv[create + 1]);
    cols = atoi(argv[create + 2]);
    mazeFile = argv[create + 3];
    createMaze(m, rows, cols);
    mazetoFile(m, mazeFile); //store maze info in file
  }
  if (solve)
  {
    mazeFile = argv[solve + 1];
    solvedFile = argv[solve + 2];
    MAZE * m1 = mazefromFile(mazeFile);
    solveMaze(m1);
    mazetoFile(m1, solvedFile);
  }
  if (draw)
  {
    drawFile = argv[draw + 1];
    MAZE * m2 = mazefromFile(drawFile);
    drawMaze(m2);
  }

  //ProcessOptions(m,argc,argv);

  return 0;
}


extern void Fatal(char *fmt, ...)
{
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}

extern int ProcessOptions(MAZE* m, int argc, char **argv)
{
    char *arg;
    char *mazeFile;
    char *solvedFile;
    char *drawFile;
    int start,argIndex;
    int argsUsed;
    int rows = 0;
    int cols = 0;
    int userSeed = 0;

    argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-')
        {
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        argsUsed = 0;
        start = argIndex;


        /* advance argIndex to point to the first argument to the option */
        if (argv[start][2] == '\0')
            {
            arg = argv[start+1];
            ++argIndex;
            }
        else
            {
            /* first arg is connected to option, so don't advance */
            arg = argv[start]+2;
            }

        switch (argv[start][1])
            {

            case 'v':    //give author's name
                printf("Connor Gill\n");
                argsUsed = 0;
                return 0;
                break;
            case 'c':    //create maze
                rows = atoi(arg);
                cols = atoi(argv[argIndex+1]);
                mazeFile = argv[argIndex+2];
                argsUsed = 3;
                createMaze(m, rows, cols);
                mazetoFile(m, mazeFile); //store maze info in file
                break;
            case 's':    //solve maze
                mazeFile = arg;
                solvedFile = argv[argIndex+1];
                argsUsed = 2;
                MAZE * m1 = mazefromFile(mazeFile);
                //solveMaze(m1);
                mazetoFile(m1, solvedFile);
                break;
            case 'd':   //draw maze
                drawFile = arg;
                argsUsed = 1;
                MAZE * m2 = mazefromFile(drawFile);
                drawMaze(m2);
                break;
            case 'r':    //seed random number
                userSeed = atoi(arg);
                setSeed(userSeed);
                argsUsed = 1;
                break;
            default:
                fprintf(stderr,"option %s not understood\n",argv[start]);
                exit(-1);
            }

        argIndex += argsUsed;
        }



    return argIndex;
}
