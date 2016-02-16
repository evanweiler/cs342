#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tgmath.h>
#include <float.h>
#include <math.h>
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
    exit(1);
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
    if ('A' <= charCode && charCode <= 'Z') {
      charCode += 32; // to make the letter lowercase!
    }
    if(('0' <= charCode && charCode <= '9') ||
      ('a' <= charCode && charCode <= 'z')) {
        freqCount[charCode]++;
        currentWordLength++;
        isInsideWord = 1;
      } else {
        if (charCode == ' ' || charCode == '\n') {
          wordLenTotal += currentWordLength;
          numWords++;
        } else if (!charIsIn(charCode, "-()&.,!?$\"';:")) {
          forbiddenCharCount++;
        }
        isInsideWord = 0;
        currentWordLength = 0;
      }
  }

  int validCharacterCount = size - forbiddenCharCount;

  if (numWords != 0) {
    summary->avgWordLength = wordLenTotal / (float) numWords;
  } else {
    summary->avgWordLength = (float) size;
  }
  if (validCharacterCount != 0 || size == 0) {
    summary->forbiddenCharPercentage = forbiddenCharCount / (float) size;
    for(int i=0; i<128; i++) {
      // percentage of character appearance in all valid characters
      summary->freqTable[i] = freqCount[i] / (float) validCharacterCount;
    }
  } else {
    summary->forbiddenCharPercentage = 1.0;
    for(int i=0; i<128; i++) {
      // percentage of character appearance in all valid characters
      summary->freqTable[i] = 0.0;
    }
  }
  //printf("%f, %f\n", summary->avgWordLength, summary->forbiddenCharPercentage);
  return 0;
}

//returns an allocated language summary that the caller must free
struct langSummary *newTrainedSummaryFromFile(char *filePath) {
  FILE *file = fopen(filePath, "r");
  if(!file) {
        printf("Failed to open file %s.\n", filePath);
        exit(1);
  }
  size_t size = 0;
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  rewind(file);
  char *binary = (char *) malloc(size);
  if(!binary) {
    printf("Failure! Could not allocate memory for file text\n");
  }
  if(size != fread(binary, sizeof(char), size, file)) {
        free(binary);
        fclose(file);
        printf("Could not read file %s.\n", filePath);
        exit(1);
  }
  fclose(file);

  struct langSummary *newSummary = (struct langSummary*)malloc(sizeof(struct langSummary));
  if(!newSummary) {
    printf("Failure! Could not allocate memory for newSummary\n");
    exit(1);
  }
  trainSummary(newSummary, binary, size);

  free(binary);
  return newSummary;
}

float compareSummaries(struct langSummary *reference, struct langSummary *target) {
  float freqScore = 0.0;
  for(int i=0; i<128; i++) {
    freqScore += powf(reference->freqTable[i] - target->freqTable[i], 2.0) / (reference->freqTable[i] ? reference->freqTable[i] : FLT_MIN);
  }
  float wordLenDiff = fabsf(reference->avgWordLength - target->avgWordLength);

  return freqScore * 0.5 + wordLenDiff * 1 + (target->forbiddenCharPercentage ? FLT_MAX : 0.0);
}
