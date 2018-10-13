#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"
#include "da.h"

struct da
{
  void * (*array);  //struct will contain Dyn array of type void
  int size;
  int capacity;
  int debugVal;
  void (*freeP)(void * ptr); //pointer for use in freeDA function
  void (*displayP)(void * ptr, FILE *fp); //pointer for use in displayDA function
};

struct stack
{
  DA * stackArr;
  int debugVal;
  void (*freeP)(void * ptr); //pointer for use in freeDA function
  void (*displayP)(void * ptr, FILE *fp); //pointer for use in displayDA function
};

extern STACK *newSTACK(void)
{
  STACK * stackStruc = malloc(sizeof(STACK));
  assert(stackStruc != 0);
  stackStruc->stackArr = newDA();  //sets stackArr to dynArr
  assert(stackStruc->stackArr != 0);
  stackStruc->debugVal = 0;
  stackStruc->freeP = 0;
  stackStruc->displayP = 0;

  return stackStruc;
}

extern void  setSTACKdisplay(STACK *usrStack, void (*displayPtr)(void * ptr, FILE *fp))
{
  usrStack->displayP = displayPtr;
}

extern void  setSTACKfree(STACK *usrStack, void (*freePtr)(void *ptr))
{
  usrStack->freeP = freePtr;
}

/*The push method runs in constant or amortized constant time. The value to be
pushed is stored in the underlying data structure. */
extern void  push(STACK *usrStack,void *val)
{
  insertDA(usrStack->stackArr, sizeDA(usrStack->stackArr), val);
}

/*The pop method runs in constant or amortized constant time. The value to be
popped is removed in the underlying data structure. */
extern void *pop(STACK *usrStack)
{
  void * popVal = removeDA(usrStack->stackArr, sizeDA(usrStack->stackArr) - 1);
  return popVal;
}

/*The peek method returns the value ready to come off the stack, but leaves
the stack unchanged. It runs in constant time. */
extern void *peekSTACK(STACK *usrStack)
{
  void * peekVal = getDA(usrStack->stackArr, sizeDA(usrStack->stackArr) - 1);
  return peekVal;
}

/*The display method prints the items stored in the stack. If the integers
5, 6, 2, 9, and 1 are pushed in the order given, the method would generate
this output: |1,9,2,6,5| with no preceding or following whitespace. An empty
stack displays as ||. However, the display can be modified by the debug routine. */
extern void  displaySTACK(STACK *usrStack, FILE *fp)
{
  if (usrStack->debugVal == 0)
  {
    if (usrStack->displayP == 0)
    {
      fprintf(fp, "|");
      for (int i = sizeSTACK(usrStack) - 1; i >= 0; i--)
      {
        usrStack->displayP(&usrStack->stackArr->array[i], fp);
        if (i != 0) { fprintf(fp, ","); }
      }
      fprintf(fp, "|");
    }
    else
    {
      fprintf(fp, "|");
      for (int i = sizeSTACK(usrStack) - 1; i >= 0; i--) {
        usrStack->displayP(getDA(usrStack->stackArr, i), fp);
        if (i != 0) { fprintf(fp, ","); }
      }
      fprintf(fp, "|");
    }
  }

  else if (usrStack->debugVal == 1)
  {
    debugDA(usrStack->stackArr, 0);
    setDAdisplay(usrStack->stackArr, usrStack->displayP);
    displayDA(usrStack->stackArr, fp);
  }
  else
  {
    debugDA(usrStack->stackArr, 1);
    setDAdisplay(usrStack->stackArr, usrStack->displayP);
    displayDA(usrStack->stackArr, fp);
  }
}

/*This if the debug level is set to one, the display method uses the underlying
data structure's display method. If the debug method is set to two, the display
method uses the underlying data structure's debugged display method. */
extern int   debugSTACK(STACK *usrStack, int newBug)
{
  int prevBug = usrStack->debugVal;
  usrStack->debugVal = newBug;

  return prevBug;
}
extern void  freeSTACK(STACK *usrStack)
{
  setDAfree(usrStack->stackArr, usrStack->freeP);
  freeDA(usrStack->stackArr);
  free(usrStack);
}

/*The size method returns the number of items stored in the stack.
It runs in amortized constant time.  */
extern int   sizeSTACK(STACK *usrStack)
{
  return sizeDA(usrStack->stackArr);
}
