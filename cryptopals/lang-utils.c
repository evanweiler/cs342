#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lang-utils.h"

static int charIsIn(char target, char *string) {
  if(!string) {
    return 0;
  }
  int i=0;
  while(string[i] != '\0') {
    if(target == string[i]) {
      return 1;
    }
    i++;
  }
  return 0;
}

int trainSummary(char *text, int size, struct langSummary *summary) {
  if(!text || !summary) {
    printf("Could not train summary due to null inputs\n");
    return 1;
  }

  int wordLenTotal = 0;
  int numWords = 0;
  int currentWordLength = 0;
  int isInsideWord = 0;
  int forbiddenCharCount = 0;
  
  for(int i=0; i<size; i++) {
    char charCode = text[i];
    if(' ' < charCode && charCode <= 'z' && !charIsIn(charCode, "#+[]{}\\|")) {
      summary->freqTable[charCode]++;
      isInsideWord = 1;
      currentWordLength++;
    } else {
      if (isInsideWord) {
        wordLenTotal += currentWordLength;
        currentWordLength = 0;
        numWords++;
        isInsideWord = 0;
      }
      if(charCode != ' ' && charCode != '\n') {
        forbiddenCharCount++;
      }
    }
  }
  summary->avgWordLength = wordLenTotal / (float) numWords;
  summary->forbiddenCharPercentage = forbiddenCharCount / (float) size;
  printf("Size %d\n", forbiddenCharCount);
  return 0;
}

//returns allocated int array of length 128, caller must free
struct langSummary *newTrainedSummaryFromFile(char *filePath) {
  FILE *file = fopen(filePath, "r");
  if(!file) {
        printf("Failed to open file %s.\n", filePath);
        return NULL;
  }
  size_t size = 0;
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  rewind(file);
  char *binary = (char *) malloc(size);
  if(size != fread(binary, sizeof(char), size, file)) {
        free(binary);
        fclose(file);
        printf("Could not read file %s.\n", filePath);
        return NULL;
  }
  fclose(file);

  struct langSummary *newSummary = (struct langSummary*)malloc(sizeof(struct langSummary));
  trainSummary(binary, size, newSummary);

  free(binary);
  return newSummary;
}
