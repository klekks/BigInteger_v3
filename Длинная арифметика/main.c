#include <stdio.h>
#include <conio.h>
#include "BigInteger.h"
#include "parser.h"

int main() {
	BigInteger A, B, C;
	char a[1000];
	char b[1000];
	parserInit();
	while (1) {
		printf(">>> ");
		gets_s(b, 1000);
		if (makeOperation(b, &A)) {
			toStrBigInteger(a, A);
			printf("    = %s\n", a);
		}

	}
}