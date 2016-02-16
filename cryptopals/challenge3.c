#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"
#include "lang-utils.h"
#include "base64.h"

// Compile with: gcc challenge3.c base64.c lang-utils.c crypto-utils.c -lm -g -o challenge2

static char* sInputStr = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

int main(int argc, char *argv[]) {
  int inputBytesLen;
  char *inputBytes = hexStrToBytes(sInputStr, &inputBytesLen);
  if (!inputBytes) {
    printf("Failure! Couldn't convert hex to bytes.\n");
    return 1;
  }
  struct langSummary *englishSummary = newTrainedSummaryFromFile("resources/alice.txt");

  if(!crackSingleCharXOR(inputBytes, inputBytesLen, englishSummary, 2)) {
    printf("Failure! Could not decrypt input.");
    return 1;
  }

  free(englishSummary);
  free(inputBytes);
}
