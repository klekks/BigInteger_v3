#include "parser.h"
#include "BigInteger.h"
#include <stdio.h>
#include <locale.h>

char all_symbols[] = "0123456789+-*/%^!~";
char all_operators[] = "+-*/%^!~";
char all_digits[] = "0123456789";

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
	while (!in(*str, all_digits) && *str) if (in(*str, all_operators)) return *str;
	return 0;
}

void clearSpaces(char* str) {
	char* pointer = str;
	while (*str) {
		if (in(*str, all_symbols)) {
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
	char operator_;
	int length1, length2;
	length1 = _lenInputBigInteger(input);
	getNumber(input, length1, &A);
	input += length1;
	operator_ = getOperator(input);
	if (!operator_) {
		printf("\t InputError\n \t Operator not found\n");
		return 0;
	}

	//�������� ��������, ������ ����� ����������
	if (!(operator_ == '!')) {
		input++;
		length2 = _lenInputBigInteger(input);
		getNumber(input, length2, &B);
	}

	BigInteger ZERO;
	initBigInteger(&ZERO, 0);

	switch (operator_) {
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

void parserInit() {
	char* locale = setlocale(LC_ALL, "");
	printf("\tWelcom to BigInteger Calculator Lite v3.1\n");
	printf("\t\t for Lab �2 ������ ����������������, SPBSTU 2021\n");
	printf("\t\t by ������ �. � �������� �.\n\n");
	printf("����������� ������������ ����� ��������� ���:\n");
	printf("\t+  �������� �������� (��� ���� �����) ��������\n");
	printf("\t-  �������� �������� ���������\n");
	printf("\t*  �������� �������� ���������\n");
	printf("\t/  �������� �������� ����� ����� �� �������\n");
	printf("\t%  �������� �������� ������� �� �������\n");
	printf("\t^  �������� �������� ���������� � �������\n");
	printf("\t~  �������� �������� ����� ����� �� ������ �������� �� ������� ������������\n");
	printf("\t!  ������� �������� (��� ������ �����) ������ ����������\n\n");
	printf("��� ������� ������������� ����� ������ ����������� ������ '-' ����� ������\n\n");
	printf("������ ����� ��� ���������� ����� -3 � ������� 4: \n");
	printf("\t -3^4 \n");
	printf("\t\t (����������� ������������� �������� � ����� ����������)\n\n");
	printf("������ ����� ��� ������ ���������� ����� 256: \n");
	printf("\t 256! \n");
	printf("\t\t (����������� ������������� �������� � ����� ����������) \n\n");
	printf("��������� �������������!\n");
}