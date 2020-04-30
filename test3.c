#include <stdio.h>
#include <omp.h>

#define LINES_SIZE_INC 1000 // the amount by which the size of the lines array is increased as needed
#define FILENAME "/homes/dan/625/wiki_dump.txt" // the path to the file to be read

int main() {
    FILE *fp; // pointer to a file stream object
	fpos_t pos; // stores a position within the file
	int *lines = malloc(LINES_SIZE_INC * sizeof(fpos_t)); // stores the locations line beginnings within the file
    int currLine = 1;  // the number of the line currently being read, indexed at 1
    char c;  // character read from file

    // Open the file, save beginning of line 1 to lines array
    fp = fopen(FILENAME, "r");
	fgetpos(fp, &pos);
	lines[currLine] = pos;

    // Check if file exists
    if (fp != NULL) {
		// read file and mark line beginnings
		for (c = getc(fp); c != EOF c = getc(fp)) {
			if (c == '\n'){ // advance line if '\n' is read
				currLine++;
				// if lines array is full, reallocate it with additional space.
				if (currLine >= (sizeof(lines) / sizeof(fpos_t))) {
					int *tmp = realloc(lines, (sizeof(lines) + (LINES_SIZE_INC * sizeof(fpos_t)));
					lines = tmp;
				}
				fgetpos(fp, &pos); // get the position immediately following the newline
				lines[currLine] = pos;
			}
		}
		fclose(fp); // close the file
    } else {
		// return failure
		printf("Could not open file");
		return -1;
	}
	
	// use lines array to calculate line sums and differences
	fp = fopen(FILENAME, "r");
	if (fp != NULL) {
		int firstLineSum = 0;
		int nextLineSum = 0;
		// run this section in parallel
		// iterate through each line beginning in lines array
		for (int i=1; i<=currLine; i++) {
			fsetpos(fp, lines[i]); // set file reader position to current line
			// read and sum characters of the line
			for (c = getc(fp); c != "\n"; c = getc(fp)) {
				firstLineSum += (int)c;
			}
			
			fsetpos(fp, lines[i+1]); // set file reader position to next line
			for (c = getc(fp); c != "\n"; c = getc(fp)) {
				nextLineSum += (int)c;
			}
			
			dif = firstLineSum - nextLineSum;
			printf("line %d-%d: %d\n",i-1,i,dif);
		}
	}
	return 0;
}
