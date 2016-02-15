#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tgmath.h>
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

int trainSummary(struct langSummary *summary, char *text, int size) {
  if(!text || !summary) {
    printf("Could not train summary due to null inputs\n");
    return 1;
  }

  int wordLenTotal = 0;
  int numWords = 0;
  int currentWordLength = 0;
  int isInsideWord = 0;
  int forbiddenCharCount = 0;
  int freqCount[128];
  for(int i=0; i<128; i++) {
    freqCount[i] = 0;
  }

  for(int i=0; i<size; i++) {
    char charCode = text[i];
    if(' ' < charCode && charCode <= 'z' && !charIsIn(charCode, "`*#+[]{}\\|")) {
      freqCount[charCode]++;
      isInsideWord = 1;
      currentWordLength++;
    } else {
      if (isInsideWord) {
        if (charCode == ' ' || charCode == '\n') {
          wordLenTotal += currentWordLength;
          numWords++;
        }
        currentWordLength = 0;
        isInsideWord = 0;
      }
      if(charCode != ' ' && charCode != '\n') {
        forbiddenCharCount++;
      }
    }
  }

  for(int i=0; i<128; i++) {
    // percentage of character appearance in all valid characters
    summary->freqTable[i] = freqCount[i] / (float) (size - forbiddenCharCount);
  }
  summary->avgWordLength = wordLenTotal / (float) numWords;
  summary->forbiddenCharPercentage = forbiddenCharCount / (float) size;
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
  trainSummary(newSummary, binary, size);

  free(binary);
  return newSummary;
}

float compareSummaries(struct langSummary *reference, struct langSummary *target) {
  float freqDiffSum = 0.0;
  for(int i=0; i<128; i++) {
    freqDiffSum += fabsf(reference->freqTable[i] - target->freqTable[i]);
  }
  float wordLenDiff = fabsf(reference->avgWordLength - target->avgWordLength);


  return freqDiffSum * 1 + (wordLenDiff < 1.0 ? wordLenDiff : 1.0) * 2 + target->forbiddenCharPercentage * 1;
}
