// Reads the characters in a text file sequentially and sums up their values 
// on a per-line basis. Requires a "test.txt" file be present in the same directory.

#include <stdio.h>

int main() {
  char c;
  int sum = 0;
  FILE *input_file = fopen("test.txt", "r");
  FILE *output_file = fopen("results.txt","w");

  if (input_file) {
    while ((c = getc(input_file)) != EOF) {
      if(c == '\n') {
        fprintf(output_file, "%d\n", sum);
        sum = 0;
      } else {
        sum = sum + (int)c;
      }
    }
    fclose(output_file);
    fclose(input_file);
  }
  return 0;
}
