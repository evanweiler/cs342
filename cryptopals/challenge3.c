#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"
#include "lang-utils.h"
#include "base64.h"

// Compile with: gcc challenge3.c base64.c lang-utils.c crypto-utils.c -g -o challenge2

static char* sInputStr = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

int main(int argc, char *argv[]) {
  int inputBytesLen;
  char *inputBytes = hexStrToBytes(sInputStr, &inputBytesLen);
  if (!inputBytes) {
    printf("Failure! Couldn't convert hex to bytes.\n");
    return 1;
  }
  struct langSummary *englishSummary = newTrainedSummaryFromFile("resources/alice.txt");
  for(int i=0; i<128; i++) {
    printf("%d\n", englishSummary->freqTable[i]);
  }

  printf("Average word length is %f.\n", englishSummary->avgWordLength);
  printf("forbiddenCharCount is %f.\n", englishSummary->forbiddenCharPercentage);

  free(englishSummary);
  free(inputBytes);
}
