#include <stdio.h>
#include "utils.h"

int readln(char s[], int strlen) {
	int i = 0;
	int run = 1;
	char c;
	while (run) {
		c = getchar();
		if (c == '\n' || c == EOF) {
			run = 0;
		} else if (i < strlen-1) {
			s[i] = c;
			i++;
		}
	}
	s[i] = '\0';
	return i;
}