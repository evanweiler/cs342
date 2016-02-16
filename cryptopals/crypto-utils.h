#ifndef CRYTPO_UITLS_H
#define CRYTPO_UITLS_H

#include "lang-utils.h"

char hexCharToByte(char aHexChar);

char* hexStrToBytes(char* aHexStr, int* aOutLen);

char* bytesToHexStr(char* aByteStr, int* aOutLen);

int cmpByteArrays(char *array1, char* array2, int length);

char *xORByChar(char *input, char key, int size);

int crackSingleCharXOR(char *input, int size, struct langSummary *referenceSummary, float threshold);

#endif
