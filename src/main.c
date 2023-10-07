#define BUFFER_SIZE 30000
#define STACK_SIZE 512

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

#if defined(__unix__) || defined(__posix__) || \
	(defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#elif (defined(_WIN64) || (defined(_WIN32)))
#include "windows.h"
#endif

int main(int argc, char *argv[])
{
	Byte *buf = calloc(BUFFER_SIZE, sizeof(Byte));
	int buf_index = 0;

	FILE *fp_code;
	if (argc != 2 || (fp_code = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr,
			"Invalid Arguments.\n"
			"Usage: %s [FILE]\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	for (int ch; (ch = fgetc(fp_code)) != EOF;) {
		switch (ch) {
		case '>':
			if (buf_index + 1 >= BUFFER_SIZE) {
				fprintf(stderr, "\nPointer Out-of-Bounds\n");
				exit(EXIT_FAILURE);
			}
			buf_index++;
			break;
		case '<':
			if (buf_index - 1 < 0) {
				fprintf(stderr, "\nPointer Out-of-Bounds\n");
				exit(EXIT_FAILURE);
			}
			buf_index--;
			break;
		case '+':
			buf[buf_index]++;
			break;
		case '-':
			buf[buf_index]--;
			break;
		case '[':
			if (buf[buf_index] == 0) {
				int brackets = 0;
				for (;;) {
					ch = fgetc(fp_code);
					if ((ch == ']' && brackets == 0) ||
					    ch == EOF)
						break;
					else if (ch == '[')
						brackets++;
					else if (ch == ']')
						brackets--;
				}
			}
			break;
		case ']':
			if (buf[buf_index] != 0) { //jump to '['
				int brackets = 0;
				for (;;) {
					fseek(fp_code, -2, SEEK_CUR);
					ch = fgetc(fp_code);
					if ((ch == '[' && brackets == 0) ||
					    ch == EOF)
						break;
					else if (ch == '[')
						brackets++;
					else if (ch == ']')
						brackets--;
				};
			}
			break;
		case ',':
			buf[buf_index] = getchar();
			break;
		case '.':
			putchar(buf[buf_index]);
			break;
		}
	}
	free(buf);
	fclose(fp_code);

	return EXIT_SUCCESS;
}
