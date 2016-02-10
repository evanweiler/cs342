#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"
#include "base64.h"

// Compile with: gcc challenge2.c base64.c crypto-utils.c -g -o challenge2

static char* sInputStr = "1c0111001f010100061a024b53535009181c";

static char* sXORStr = "686974207468652062756c6c277320657965";

static char* sOutputStr = "746865206b696420646f6e277420706c6179";

int cmpByteArrays(char *array1, char* array2, int length){
  for(int i=0; i<length; i++) {
    if(array1[i] != array2[i]) {
      return 1;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int len = strlen(sInputStr) * 4;
  char *xorBytes = hexStrToBytes(sXORStr, &len);
  char *inputBytes = hexStrToBytes(sInputStr, &len);
  char* resultBytes = (char*)malloc(len);

  for(int i=0; i<len; i++) {
    resultBytes[i] = inputBytes[i] ^ xorBytes[i];
  }

  char *outBytes = hexStrToBytes(sOutputStr, &len);
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
