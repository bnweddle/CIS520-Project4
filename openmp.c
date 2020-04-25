#include <stdio.h>
#define MAX_FILE_NAME 100
#define FILENAME "/homes/dan/625/wiki_dump.txt"

// Does the same thing as the test.c should.

int main()
{
    FILE *fp;
    int count = 0;  // Line counter (result)
    char c;  // To store a character read from file
    int lineA = 0;
    int lineB = 0;
    int dif = 0;
    int flag = 0;

    // Open the file
    fp = fopen(FILENAME, "r");

    // Check if file exists
    if (fp == NULL)
    {
       printf("Could not open file"); // breaks here
       return 0;
    }

    // Extract characters from file and store in character c
    for (c = getc(fp); count != 100; c = getc(fp)) {
        if(flag==1) {
           lineA = lineA + (int)c;
        } else {
           lineB = lineB + (int)c;
        }
        if (c == '\n'){ // Increment count if this character is newline
           count++;
           if(flag==0) {
             lineB -= 10;
             flag = 1;
             dif = lineA-lineB;
             lineA = 0;
           } else {
             lineA -= 10;
             flag = 0;
             dif=lineB-lineA;
             lineB=0;
           }
             printf("line %d-%d: %d\n",count-1,count,dif);
        }
    }
	                                                         
    // Close the file
    fclose(fp);
    return 0;
}
