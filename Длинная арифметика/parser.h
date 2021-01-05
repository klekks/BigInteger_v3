#pragma once
#include "BigInteger.h"

int _lenInputBigInteger(char*);
char getOperator(char*);
void clearSpaces(char*);
char makeOperation(char*, BigInteger* N);
void parserInit();