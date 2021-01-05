#include "BigInteger.h"

// default init by char
void initBigInteger(BigInteger* N, char value) {

	for (int i = 0; i < BIS + 1; i++) N->value[i] = 0;
	N->sign = 0;

	N->value[0] = value > 0 ? value : -value;
	N->sign = 0;

	if (value < 0) invertBigInteger(N);
}

void intInitBigInteger(BigInteger* N, int value) {
	int i = sizeof(int);
	char invert = 0;
	if (value < 0) {
		value = -value; 
		invert = 1;
	}

	for (int i = 0; i < BIS + 1; i++) N->value[i] = 0;
	N->sign = 0;

	while (i--) {
		N->value[sizeof(int) - i - 1] = value % 256;
		value /= 256;
	}

	if (invert) invertBigInteger(N);
}

void unsignedInitBigInteger(BigInteger* N, unsigned value) {
	int i = sizeof(int);

	for (int i = 0; i < BIS + 1; i++) N->value[i] = 0;
	N->sign = 0;

	while (i--) {
		N->value[sizeof(int) - i - 1] = value % 256;
		value /= 256;
	}
}

void invertBigInteger(BigInteger* N) {
	for (int i = 0; i < BIS + 1; i++) N->value[i] ^= 0xFF;
	N->sign = (N->sign + 1) % 2;
	incrementBigInteger(N);
}

void incrementBigInteger(BigInteger* N) {
	for (int i = 0; i < BIS + 1; i++) {
		N->value[i]++;
		if (N->value[i] != 0) break;
	}
}

void decrementBigInteger(BigInteger* N) {
	invertBigInteger(N);
	incrementBigInteger(N);
	invertBigInteger(N);
}

void addBigInteger(BigInteger* A, BigInteger* B) {
	int _255 = 0xFF;
	register byte temp;
	register byte next_tmp, next = 0;
	for (int i = 0; i < BIS; i++) {
		temp = A->value[i] + B->value[i] + next;
		next_tmp = 0;
		if (_255 - A->value[i] - B->value[i] - next < 0) next_tmp = 1;
		next = next_tmp;
		A->value[i] = temp;
	}
	if (A->value[BIS - 1] >> 7) A->sign = 1;
	else A->sign = 0;
}

void subBigInteger(BigInteger* A, BigInteger* B) {
	invertBigInteger(B);
	addBigInteger(A, B);
	invertBigInteger(B);
}

// unsigned only
void mulByteBigInteger(BigInteger* N, byte n) {
	byte tonext = 0;
	unsigned long temp;

	if (N->sign) invertBigInteger(N);

	for (int i = 0; i < BIS; i++) {
		temp = N->value[i] * n + tonext;
		tonext = (byte) (temp / 256);
		temp %= 256;
		N->value[i] = temp;
	}
}

void mulBigInteger(BigInteger* A, BigInteger* B) {

	char result_sign = 0;

	if (A->sign) {
		result_sign++;
		invertBigInteger(A);
	}
	if (B->sign) {
		result_sign++;
		invertBigInteger(B);
	}

	BigInteger sample = *A;
	BigInteger temp;

	initBigInteger(A, 0);

	for (int j = 0; j < BIS; j++) {
		temp = sample;
		mulByteBigInteger(&temp, B->value[j]);
		leftShiftBigInteger(&temp, 8*j);
		addBigInteger(A, &temp);
	}

	if (result_sign % 2) invertBigInteger(A);
}

// Unsigned byte only

void leftShiftBigInteger(BigInteger* N, int shift) {
	if (shift < 0) rightShiftBigInteger(N, -shift);
	else {
		int shift8 = shift / 8; // /8
		if (shift8) {
			for (int i = BIS; i >= shift8 - 1; i--) N->value[i] = N->value[i - shift8];
			for (int i = 0; i < shift8; i++) N->value[i] = 0;
		}

		shift %= 8;
		register byte temp, left = 0;

		for (int i = 0; i < BIS; i++) {
			temp = N->value[i] >> 8 - shift;
			N->value[i] <<= shift;
			N->value[i] += left;
			left = temp;
		}
 	}
}

void rightShiftBigInteger(BigInteger* N, int shift) {
	if (shift < 0) leftShiftBigInteger(N, -shift);
	else {
		int shift8 = shift / 8; // /8
		if (shift8) {
			for (int i = 0; i < BIS - shift8; i++) N->value[i] = N->value[i + shift8];
			for (int i = BIS; i > BIS - shift8; i--) N->value[i] = 0;
		}

		shift %= 8;
		register byte temp, right = 0;

		for (int i = BIS; i >= 0; i--) {
			temp = N->value[i] % pow(2, shift);
			N->value[i] >>= shift;
			N->value[i] += (right << 8 - shift);
			right = temp;
		}
	}
}

int strlen(char* str) {
	register const char* s = str; 
	while (*str++); 
	return str - s - 1;
}

unsigned pow(unsigned base, unsigned power) {
	unsigned result = 1;
	while (power--) result *= base;
	return result;
}

// strlen 1000 only
void fromStrBigInteger(char* str, BigInteger* N) {
	int counter = 0;
	unsigned partOfNum = 0;
	char sign = 0;

	initBigInteger(N, 0);
	BigInteger _10pow, temp;
	unsignedInitBigInteger(&_10pow, 1000000000);

	int len = strlen(str);
	int pointer = 0;

	if (str[0] == '-') {
		sign = 1;
		str++;
		len--;
	}


	while (pointer < len) {
		partOfNum *= 10;
		partOfNum += str[pointer] % 48;
		if (++counter == 9) {
			counter = 0;
			unsignedInitBigInteger(&temp, partOfNum);
			mulBigInteger(N, &_10pow);
			addBigInteger(N, &temp);
			partOfNum = 0;
		}
		pointer++;
	}

	if (counter > 0) {
		unsignedInitBigInteger(&_10pow, pow(10, counter));
		mulBigInteger(N, &_10pow);
		unsignedInitBigInteger(&temp, partOfNum);
		addBigInteger(N, &temp);
	}

	if (sign) invertBigInteger(N);
}

int cmpBigInteger(BigInteger* A, BigInteger* B) {
	if (A->sign > B->sign) return -1;
	if (B->sign > A->sign) return 1;

	int i = BIS;

	while (A->value[i] == B->value[i]) {
		i--;
		if (i < 0) return 0;
	}

	return A->value[i] - B->value[i];
}

// 
unsigned startBigInteger(BigInteger* A) {
	unsigned i = BIS;
	while (A->value[i] == 0) i--;
	return i;
}

// mod in A, return DIV
BigInteger divBigInteger(BigInteger* A, BigInteger* B) {
	BigInteger divisor, one;
	BigInteger DIV;
	//initBigInteger(&MOD, 0);
	initBigInteger(&DIV, 0);
	initBigInteger(&one, 1);

	unsigned char sign = 0;

	if (A->sign) {
		sign++;
		invertBigInteger(A);
	}

	if (B->sign) {
		sign++;
		invertBigInteger(B);
	}

	int total_shift = 0;
	int startA, startB;

	while (cmpBigInteger(A, B) >= 0) {
		divisor = *B;
		startA = startBigInteger(A);
		startB = startBigInteger(&divisor);

		// fast shift
		if (startA - startB) {
			total_shift += (startA - startB) * 8;
			leftShiftBigInteger(&divisor, total_shift);
		}

		//slow shift
		while (cmpBigInteger(A, &divisor) > 0) {
			total_shift += 1;
			leftShiftBigInteger(&divisor, 1);
		}
		while (cmpBigInteger(A, &divisor) < 0) {
			total_shift -= 1;
			rightShiftBigInteger(&divisor, 1);
		}

		leftShiftBigInteger(&one, total_shift);
		addBigInteger(&DIV, &one);
		initBigInteger(&one, 1);

		subBigInteger(A, &divisor);
		total_shift = 0;
	}

	if (sign % 2) invertBigInteger(&DIV);

	return DIV;
}

void toStrBigInteger(char* s, BigInteger N) {
	BigInteger ZERO, DIV, _100;
	char str_[1001];
	char* str = str_;
	char sign = 0;

	if (N.sign) {
		sign = 1;
		invertBigInteger(&N);
	}

	initBigInteger(&ZERO, 0);
	initBigInteger(&_100, 100);

	register byte mod = 0;

	while (cmpBigInteger(&N, &ZERO) > 0) {
		DIV = divBigInteger(&N, &_100);
		mod = N.value[0];
		*str = mod % 10 + 48;
		str++;
		*str = mod % 100 / 10 + 48;
		str++;
		N = DIV;
	}

	if (*(str - 1) == '0') str--;
	*str = 0;

	if (sign) {
		s[0] = '-';
		s++;
	}

	for (int i = str - str_ - 1; i >= 0; i--) {
		s[str - str_ - 1 - i] = str_[i];
	}
	s[str - str_] = 0;

	if (*s == 0) {
		*s = '0';
		*(s + 1) = 0;
	}
}

void powBigInteger(BigInteger* A, BigInteger* B) {
	BigInteger result, zero, two, A_copy, B_copy, B_div;
	initBigInteger(&result, 1);
	initBigInteger(&zero, 0);
	initBigInteger(&two, 2);

	while (cmpBigInteger(B, &zero) > 0) {
		B_copy = *B;
		B_div = divBigInteger(B, &two);
		if (cmpBigInteger(B, &zero) == 0) {
			*B = B_div;
			A_copy = *A;
			mulBigInteger(A, &A_copy);
		}
		else {
			*B = B_copy;
			decrementBigInteger(B);
			mulBigInteger(&result, A);
		}
	}

	*A = result;
}

void sumFromToBigInteger(BigInteger* A, BigInteger* B) {
	char cmp = cmpBigInteger(A, B);
	BigInteger sum = *A, n = *B;
	if (cmp == 0) return;
	else if (cmp > 0) {
		initBigInteger(A, 0);
		return;
	}
	else {
		addBigInteger(&sum, B);
		subBigInteger(&n, A);
		incrementBigInteger(&n);
		mulBigInteger(&sum, &n);
		initBigInteger(&n, 2);
		*A = divBigInteger(&sum, &n);
	}
}

BigInteger factorialBigInteger(BigInteger A) {
	BigInteger zero;
	BigInteger result;
	initBigInteger(&zero, 0);
	initBigInteger(&result, 1);

	while (cmpBigInteger(&A, &zero) > 0) {
		mulBigInteger(&result, &A);
		decrementBigInteger(&A);
	}
	return result;
}