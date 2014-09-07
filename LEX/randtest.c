/* srand example */
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

int main ()
{
  srand (time(NULL));
  for(int i=0;i<10;i++){
	  printf ("Random number: %d\n", rand()%100);
	}
  printf ("First number: %d\n", rand()%100);
  //srand (1);
  printf ("Again the first number: %d\n", rand()%100);

  return 0;
}