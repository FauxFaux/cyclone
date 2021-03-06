/**********************/
/* fib.c, 02 Mar 1997 */
/* Al Aburto Jr.      */
/**********************/

/* timing stuff ripped out */

#include <stdio.h>
#include <stdlib.h>

unsigned long fib(unsigned long x)
{
 if (x > 2)
  return(fib(x-1)+fib(x-2));
 else
  return(1);
}


int main(int argc, char* argv[])
{
 unsigned long IMax,value;

 IMax = 40;

 if (argc > 1)
  IMax = atol(argv[1]);

 value = fib(IMax);
  
 printf("The %02ld'th Fibonacci Number is: %ld\n",IMax,value);
 return(0);
}

/* ---------- End of fib.c, Say goodnight Samantha! ---------- */
