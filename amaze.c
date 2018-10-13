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

  if (argc == 1) Fatal("%d arguments!\n",argc-1);

  ProcessOptions(m,argc,argv);

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
            /*
             * when option has an argument, do this
             *
             *     examples are -m4096 or -m 4096
             *
             *     case 'm':
             *         MemorySize = atol(arg);
             *         argsUsed = 1;
             *         break;
             *
             * when option has multiple arguments, do this
             *
             *     examples are -r4096 1280 or -r 4096 1280
             *
             *     case 'r':
             *         Rows = atoi(arg);
             *         Cols = atoi(argv[argIndex+1]);
             *         argsUsed = 2;
             *         break;
             *
             * when option does not have an argument, do this
             *
             *     example is -a
             *
             *     case 'a':
             *         PrintActions = 1;
             *         break;
             */

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
                createMaze(m, rows, cols, mazeFile);
                break;
            case 's':    //solve maze
                mazeFile = arg;
                solvedFile = argv[argIndex+1];
                argsUsed = 2;
                solveMaze(m, mazeFile, solvedFile);
                break;
            case 'd':   //draw maze
                drawFile = arg;
                argsUsed = 1;
                drawMaze(m, drawFile);
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
