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
#if 0
	argc = 2;
	argv[1] = "../examples/life.bf";
#endif
	char *code; //malloc later
	long code_size;
	{
		FILE *fp;
		if (argc != 2 || (fp = fopen(argv[1], "rb")) == NULL) {
			fprintf(stderr,
				"Invalid Arguments.\n"
				"Usage: %s [FILE]\n",
				argv[0]);
			exit(EXIT_FAILURE);
		}

		long fstart = ftell(fp);
		if (fseek(fp, 0, SEEK_END))
			goto err;
		long fend = ftell(fp);
		if (fseek(fp, 0, SEEK_SET))
			goto err;

		code_size = fend - fstart;

		if ((code = malloc(sizeof(char) * code_size + 1)) == NULL)
			goto err;

		fread(code, sizeof(char), code_size, fp);

		if (0) {
err:
			fprintf(stderr,
				"Error: Something went wrong. Try again\n");
			exit(EXIT_FAILURE);
		}
		fclose(fp);
	}

	Byte *buf = calloc(BUFFER_SIZE, sizeof(Byte));
	int buf_ind = 0;

	for (int i = 0; code[i] != '\0'; i++) {
		switch (code[i]) {
		case '>':
			if (buf_ind + 1 >= BUFFER_SIZE) {
				fprintf(stderr, "\nPointer Out-of-Bounds\n");
				exit(EXIT_FAILURE);
			}
			buf_ind++;
			break;
		case '<':
			if (buf_ind - 1 < 0) {
				fprintf(stderr, "\nPointer Out-of-Bounds\n");
				exit(EXIT_FAILURE);
			}
			buf_ind--;
			break;
		case '+':
			buf[buf_ind]++;
			break;
		case '-':
			buf[buf_ind]--;
			break;
		case '[':
			if (buf[buf_ind] == 0) {
				int brackets = 0;
				for (int ch, j = i + 1; j < code_size; j++) {
					ch = code[j];
					if (ch == ']' && brackets == 0) {
						i = j;
						break;
					} else if (ch == '[')
						brackets++;
					else if (ch == ']')
						brackets--;
				}
				if (brackets != 0) {
					fprintf(stderr,
						"Expected ']' somewhere\n");
					exit(EXIT_FAILURE);
				}
			}
			break;
		case ']':
			if (buf[buf_ind] != 0) { //jump to '['
				int brackets = 0;
				for (int ch, j = i - 1; j >= 0; j--) {
					ch = code[j];
					if (ch == '[' && brackets == 0) {
						i = j;
						break;
					} else if (ch == '[')
						brackets++;
					else if (ch == ']')
						brackets--;
				};
				if (brackets != 0) {
					fprintf(stderr,
						"Expected ']' somewhere\n");
					exit(EXIT_FAILURE);
				}
			}
			break;
		case ',':
			buf[buf_ind] = getchar();
			break;
		case '.':
			putchar(buf[buf_ind]);
			break;
		}
	}
	free(code);
	free(buf);

	return EXIT_SUCCESS;
}
