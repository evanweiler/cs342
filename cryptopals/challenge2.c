#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"
#include "base64.h"

// Compile with: gcc challenge2.c base64.c lang-utils.c crypto-utils.c -lm -g -o challenge2

static char* sInputStr = "1c0111001f010100061a024b53535009181c";

static char* sXORStr = "686974207468652062756c6c277320657965";

static char* sOutputStr = "746865206b696420646f6e277420706c6179";

int main(int argc, char *argv[]) {
  int len;
  char *xorBytes = hexStrToBytes(sXORStr, &len);
  if(!xorBytes) {
    printf("Failure! Could not convert sXORStr to bytes.\n");
    return 1;
  }
  char *inputBytes = hexStrToBytes(sInputStr, &len);
  if(!inputBytes) {
    printf("Failure! Could not convert sInputStr to bytes.\n");
    return 1;
  }
  char* resultBytes = (char*)malloc(len);
  if(!resultBytes) {
    printf("Failure! Couldn't alloc buffer for output string.\n");
    return 1;
  }

  for(int i=0; i<len; i++) {
    resultBytes[i] = inputBytes[i] ^ xorBytes[i];
  }

  char *outBytes = hexStrToBytes(sOutputStr, &len);
  if(!outBytes) {
    printf("Failure! Could not convert sOutputStr to bytes\n");
  }
  if (cmpByteArrays(resultBytes, outBytes, len) == 0) {
    printf("Success!\n");
  } else {
    printf("Failure!\n");
  }

  free(inputBytes);
  free(xorBytes);
  free(outBytes);
  free(resultBytes);
}
