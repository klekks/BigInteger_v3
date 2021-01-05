#pragma once

#define byte unsigned char
#define BIL 1000

#define BIS (int)(3.33*BIL) / 8 + 1

typedef struct {
	byte value[BIS + 1]; // указатель на массив данных
	byte sign; // знак числа (вместо вычисления знакового бита)
} BigInteger;

void initBigInteger(BigInteger*, char);
void unsignedInitBigInteger(BigInteger*, unsigned);
void intInitBigInteger(BigInteger*, int);
void invertBigInteger(BigInteger*);
void incrementBigInteger(BigInteger*);
void decrementBigInteger(BigInteger*);
void addBigInteger(BigInteger*, BigInteger*);
void subBigInteger(BigInteger*, BigInteger*);
void mulByteBigInteger(BigInteger*, byte);
void mulBigInteger(BigInteger*, BigInteger*);
void leftShiftBigInteger(BigInteger*, int);
void rightShiftBigInteger(BigInteger*, int);
int cmpBigInteger(BigInteger*, BigInteger*);
void fromStrBigInteger(char*, BigInteger*);
unsigned startBigInteger(BigInteger*);
BigInteger divBigInteger(BigInteger*, BigInteger*);
void toStrBigInteger(char*, BigInteger);
void powBigInteger(BigInteger*, BigInteger*);
void sumFromToBigInteger(BigInteger*, BigInteger*);
BigInteger factorialBigInteger(BigInteger);
int strlen(char*);
unsigned pow(unsigned base, unsigned power);