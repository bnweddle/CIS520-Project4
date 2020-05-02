#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define LINES_SIZE_INC 125 // the amount by which the size of the lines array is increased as needed
#define MAX_THREADS 4
//#define FILENAME "/homes/dan/625/wiki_dump.txt" // the path to the file to be read
#define FILENAME "a.txt"
int main() {
    FILE *fp; // pointer to a file stream object
    fpos_t pos; // stores a position within the file
    int i;
    fpos_t* lines;
    lines = malloc(LINES_SIZE_INC * sizeof(fpos_t)); // stores the locations line beginnings within the file
    int currLine = 1;  // the number of the line currently being read.
    char c;  // character read from file

    omp_set_num_threads(MAX_THREADS);

    // Open the file, save beginning of line 1 to lines array
    fp = fopen(FILENAME, "r");
	fgetpos(fp, &pos);
	lines[currLine] = pos;

    // Check if file exists
    if (fp != NULL) {
		// read file and mark line beginnings
		for (c = getc(fp); currLine <= 5; c = getc(fp)) {
			if (c == '\n'){ // advance line if '\n' is read
				currLine++;
				// if lines array is full, reallocate it with additional space.
				//if () {
				//	fpos_t *tmp = realloc(lines, (sizeof(lines) + (LINES_SIZE_INC * sizeof(fpos_t))));
				//	lines = tmp;
				//}
				fgetpos(fp, &pos); // get the position immediately following the newline
				lines[currLine] = pos;
			}
		}
    } else {
		// return failure
		printf("Could not open file");
		return -1;
	}
	
	// use lines array to calculate line sums and differences
	int firstLineSum = 0;
	int nextLineSum = 0;
	int dif = 0;
	FILE *private_fp;
	// run this section in parallel
	#pragma omp parallel
    	{
    		#pragma omp shared(lines,currLine,i) private(private_fp,firstLineSum,nextLineSum,dif) for schedule(dynamic)
		// iterate through each line beginning in lines array
		for ( i=1; i<=currLine; i++) {
			private_fp = fopen(FILENAME, "r");
			fsetpos(private_fp, &lines[i]); // set file reader position to current line
			// read and sum characters of the line
			for (c = getc(private_fp); c != '\n'; c = getc(private_fp)) {
				firstLineSum += (int)c;
			}
			fsetpos(private_fp, &lines[i+1]); // set file reader position to next line
			for (c = getc(private_fp); c != '\n'; c = getc(private_fp)) {
				nextLineSum += (int)c;
			}
			dif =  firstLineSum - nextLineSum;
			printf("(%d) line %d-%d: %d\n",omp_get_thread_num(),i-1,i,dif);
			fclose(private_fp);
		}
	}
	return 0;
}
