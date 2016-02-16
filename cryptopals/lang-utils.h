#ifndef LANG_UITLS_H
#define LANG_UITLS_H

struct langSummary {
  float freqTable[128];
  float avgWordLength;
  float forbiddenCharPercentage;
};

// returns an allocated summary struct that the caller must free
struct langSummary *newTrainedSummaryFromFile(char *fileName);

int trainSummary(struct langSummary *summary, char *text, int size);

float compareSummaries(struct langSummary *reference, struct langSummary *target);

#endif
