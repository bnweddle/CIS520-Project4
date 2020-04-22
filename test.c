// Should read the number lines of the wiki_dump.txt when you pass
// in the file path for where it is located

#include<stdio.h>
#include<stdlib.h>

int countlines(char *filename);

int main(int argc, char *argv[])
{

  if (argc != 2) {
       printf ("Missing argument for which file to read\n");
       return 1;
  }
  else
    printf("LINES: %d\n",countlines(argv[1]));

}


int countlines(char *filename)
{
  //count the number of lines in the file called filename                          
  FILE *fp = fopen(filename,"r");
  int ch=0;
  int lines=0;

  if (fp == NULL)
      return 0;
  lines++;

  while ((ch = fgetc(fp)) != EOF)
  {
     if (ch == '\n')
        lines++;
  }
  fclose(fp);
  return lines;
}
