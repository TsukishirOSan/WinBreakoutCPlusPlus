#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
/** write a function let us say, `randVlo(char s)` that takes an x or y as argument and returns the required numbers.
*** {-6, -5, -4,-3, 3, 4, 5, 6}
***where x would be in the list above and y the positives from the list only
**/
 
/*based on reading the char x or y other excluded , read the char, if x use the if x part else the y part*/
int randVlo(char v)
{
    const int range_arr[8] = {-6, -5, -4,-3, 3, 4, 5, 6};  
    srand(time(NULL));
     
    int index;
    char c = '\0';
     
    if(c != 'x' && c!= 'y')
        exit(0);
    else
    {
        // handles vy aka no negatives   
        if(c == 'y')
        {    
            do
            {
                index = rand() % 7;
            }
            while(range_arr[index] < 0);
        }
        //  handles vx all possile values in the array
        else
        {
            index = rand() % 7;
        }
        //printf("{%i}[%d]", index, range_arr[index]);==> check values of index and array value
    }
    //printf("\n"); end check
    return(index);     
}
