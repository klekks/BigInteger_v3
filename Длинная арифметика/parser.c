#include "parser.h"

void getNumber(char* str, int len, BigInteger* N) {
	char buffer[1001];
	for (int i = 0; i < len; i++) buffer[i] = *str++;
	buffer[len] = 0;
	fromStrBigInteger(buffer, N);
}

int _lenInputBigInteger(char* str) {
	char* s = str;
	if (*str == '-') str++;
	while (*str % 48 < 10 && *str / 48 == 1) str++;
	return str - s;
}

char in(char s, char* str) {
	while (*str) if (*str++ == s) return 1;
	return 0;
}

char getOperator(char* str) {
	while (!in(*str, "0123456789") && *str) if (in(*str, "+-*/%!^~")) return *str;
	return 0;
}

void clearSpaces(char* str) {
	char* pointer = str;
	while (*str) {
		if (in(*str, "0123456789+-*/^!%~")) {
			*pointer = *str;
			pointer++;
			str++;
		}
		else str++;
	}
	*pointer = 0;
}

char makeOperation(char* input, BigInteger* result) {
	clearSpaces(input);
	BigInteger A, B;
	char operator;
	int length1, length2;
	length1 = _lenInputBigInteger(input);
	getNumber(input, length1, &A);
	input += length1;
	operator = getOperator(input);
	if (!operator) {
		printf("\t InputError\n \t Operator not found\n");
		return 0;
	}

	//бинарный оператор, второе число необходимо
	if (!(operator == '!')) {
		input++;
		length2 = _lenInputBigInteger(input);
		getNumber(input, length2, &B);
	}

	BigInteger ZERO;
	initBigInteger(&ZERO, 0);

	switch (operator) {
	case '+':
		addBigInteger(&A, &B);
		break;
	case '-':
		subBigInteger(&A, &B);
		break;
	case '*':
		mulBigInteger(&A, &B);
		break;
	case '/':
		if (cmpBigInteger(&B, &ZERO)) {
			A = divBigInteger(&A, &B);
		}
		else {
			printf("\tZeroDivisionError\n");
			return 0;
		}
		break;
	case '%':
		if (cmpBigInteger(&B, &ZERO)) {
			divBigInteger(&A, &B);
		}
		else {
			printf("\tZeroDivisionError\n");
			return 0;
		}
		break;
	case '^':
		if (cmpBigInteger(&B, &ZERO) < 0) {
			printf("\tNegative power is not supported\n");
			return 0;
		}
		powBigInteger(&A, &B);
		break;
	case '~':
		sumFromToBigInteger(&A, &B);
		break;
	case '!':
		if (cmpBigInteger(&A, &ZERO) < 0) {
			printf("\tNegative factorial is not exist\n");
			return 0;
		}
		A = factorialBigInteger(A);
	}
	*result = A;
	return 1;
}

#include <stdio.h>
#include <locale.h>



void parserInit() {
	char* locale = setlocale(LC_ALL, "");
	printf("\tWelcom to BigInteger Calculator Lite v3.1\n");
	printf("\t\t for Lab є2 ћетоды программировани€, SPBSTU 2021\n");
	printf("\t\t by ѕетров ». и ¬асильев ѕ.\n\n");
	printf(" алькул€тор поддерживает такие операторы как:\n");
	printf("\t+  бинарный оператор (дл€ двух чисел) сложени€\n");
	printf("\t-  бинарный оператор вычитани€\n");
	printf("\t*  бинарный оператор умножени€\n");
	printf("\t/  бинарный оператор целой части от делени€\n");
	printf("\t%  бинарный оператор остатка от делени€\n");
	printf("\t^  бинарный оператор возведени€ в степень\n");
	printf("\t~  бинарный оператор суммы чисел от левого операнда до правого включительно\n");
	printf("\t!  унарный оператор (дл€ одного числа) вз€ти€ факториала\n\n");
	printf("ƒл€ ведени€ отрицательных чисел просто используйте символ '-' перед числом\n\n");
	printf("ѕример ввода дл€ возведени€ числа -3 в степень 4: \n");
	printf("\t -3^4 \n");
	printf("\t\t (допускаетс€ использование пробелов в любом количестве)\n\n");
	printf("ѕример ввода дл€ вз€ти€ факториала числа 256: \n");
	printf("\t 256! \n");
	printf("\t\t (допускаетс€ использование пробелов в любом количестве) \n\n");
	printf("ѕри€тного использовани€!\n");
}