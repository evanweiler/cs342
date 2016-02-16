#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include "crypto-utils.h"
#include "lang-utils.h"
#include "base64.h"

// Compile with: gcc challenge4.c crypto-utils.c lang-utils.c -lm -g -o challenge4

int main(int argc, char *argv[]) {
  struct langSummary *englishSummary = newTrainedSummaryFromFile("resources/alice.txt");

  FILE *file = fopen("resources/c4.txt", "r");
  if(!file) {
    printf("Could not open file.\n");
    return 1;
  }
  size_t lineLen = 0;
  ssize_t read;
  char * line = NULL;

  int lineNum = 1;
  while((read = getline(&line, &lineLen, file)) != -1) {
    read--;
    if(line[read] == '\n') {
      line[read] = '\0';
    }
    int lineBytesLen = 0;
    char* lineBytes = hexStrToBytes(line, &lineBytesLen);
    if(!lineBytes) {
      printf("Failed to convert hex to bytes\n");
    }
    if(crackSingleCharXOR(lineBytes, lineBytesLen, englishSummary, 3.0)) {
      printf("Cracked line number %d:\n", lineNum);
      printf("%s\n", line);
      fclose(file);
      free(lineBytes);
      free(englishSummary);
      free(line);
      return 0;
    }
    free(lineBytes);
    lineNum++;
  }
  free(line);
  fclose(file);
  free(englishSummary);
}
