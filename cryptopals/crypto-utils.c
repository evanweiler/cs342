#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include "lang-utils.h"
#include "crypto-utils.h"

char hexCharToByte(char aHexChar) {
  if (aHexChar >= '0' && aHexChar <= '9') {
    return aHexChar - '0';
  }
  if (aHexChar >= 'a' && aHexChar <= 'f') {
    return (aHexChar - ('a' - 10));
  }
  printf("Failure! Invalid hex character.\n");
  exit(1);
}

char nibbleToHexChar(char aNibble) {
  if(0 <= aNibble && aNibble <= 9) {
    return '0' + aNibble;
  }
  if(10 <= aNibble && aNibble <= 15) {
    return 'a' + aNibble - 10;
  }
  printf("Failure! Nibble is out of hex character range!\n");
  exit(1);
}

// returns a malloc'd byte buffer, caller frees
char* hexStrToBytes(char* aHexStr, int* aOutLen) {
  // need valid input
  if (!aHexStr || !aOutLen) {
    return NULL;
  }
  // need an even number of hex digits to convert to bytes
  if (strlen(aHexStr) % 2 != 0) {
    return NULL;
  }

  // calculate the length of the output buffer
  *aOutLen = strlen(aHexStr) / 2;
  if (*aOutLen == 0) {
    return NULL;
  }

  // create output buffer
  char *outBuf = malloc(*aOutLen);
  if (!outBuf) {
    return NULL;
  }

  // fill the output buffer
  int inputLen = strlen(aHexStr);
  for (int i = 0; i < inputLen; i = i + 2) {
    char char1 = aHexStr[i];
    char char2 = aHexStr[i + 1];

    char byte = 0;

    byte = hexCharToByte(char1) << 4;
    byte = byte | hexCharToByte(char2);

    outBuf[i / 2] = byte;
  }

  return outBuf;
}

// returns a malloc'd byte buffer, caller frees
char* bytesToHexStr(char* aByteStr, int* aOutLen) {
  // need valid input
  if (!aByteStr || !aOutLen) {
    return NULL;
  }

  // calculate the length of the output buffer
  *aOutLen = strlen(aByteStr) * 2;
  if (*aOutLen == 0) {
    return NULL;
  }

  // create output buffer
  char *outBuf = malloc(*aOutLen + 1);
  if (!outBuf) {
    return NULL;
  }
  outBuf[*aOutLen] = '\0';
  // fill the output buffer
  int inputLen = strlen(aByteStr);
  for (int i = 0; i < inputLen; i++) {
    outBuf[i*2] = nibbleToHexChar((aByteStr[i] & 0xF0 ) >> 4);
    outBuf[i*2+1] = nibbleToHexChar(aByteStr[i] & 0x0F);
  }
  return outBuf;
}

int cmpByteArrays(char *array1, char* array2, int length){
  for(int i=0; i<length; i++) {
    if(array1[i] != array2[i]) {
      return 1;
    }
  }
  return 0;
}

// return's a malloc'd output string that the caller must free;
char *xORByChar(char *input, char key, int size) {
  char *output = (char *)malloc(sizeof(char)*(size + 1));
  if(!output) {
    printf("Failed to allocated for new xor string with key '%c'.", key);
    exit(1);
  }
  for(int i=0; i<size; i++) {
    output[i] = input[i] ^ key;
  }
  output[size] = '\0';
  return output;
}

int crackSingleCharXOR(char *input, int size, struct langSummary *referenceSummary, float threshold) {
  float bestAttemptScore = FLT_MAX;
  char bestAttempt = 0;
  struct langSummary attemptSummary;
  char * attempts[127];
  for(char c=0; c < 127; c++) {
    attempts[c] = xORByChar(input, c, size);
    trainSummary(&attemptSummary, attempts[c], size);
    float summaryDiff = compareSummaries(referenceSummary, &attemptSummary);
    if(summaryDiff < bestAttemptScore) {
      bestAttemptScore = summaryDiff;
      bestAttempt = c;
    }
  }
  if(bestAttemptScore < threshold) {
    printf("Character %d had score of %f:\n", bestAttempt, bestAttemptScore);
    printf("Decrypted string is:\n");
    printf("%s\n", attempts[bestAttempt]);
  }
  for(int i=0; i<127; i++) {
    free(attempts[i]);
  }
  return 0;
}
