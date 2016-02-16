#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"

static char *sKey = "ICE";

static char *sInputStr = "Burning \'em, if you ain't quick and nimble I go crazy when I hear a cymbal";

int main(int argc, char *argv[]) {
  int keyLen = strlen(sKey);
  int inputLen = strlen(sInputStr);

  char *outputBytes = (char *)malloc(inputLen + 1);
  outputBytes[inputLen] = '\0';

  int k = 0;
  for(int i=0; i<inputLen; i++) {
    outputBytes[i] = sInputStr[i] ^ sKey[i % keyLen];
  }

  int hexLen;
  char *hexOut = bytesToHexStr(outputBytes, &hexLen);

  printf("%s\n", hexOut);

  free(hexOut);
  free(outputBytes);
}
