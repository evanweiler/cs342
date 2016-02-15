
struct langSummary {
  float freqTable[128];
  float avgWordLength;
  float forbiddenCharPercentage;
};

// returns an allocated int array of length 128, caller must free
struct langSummary *newTrainedSummaryFromFile(char *fileName);

int trainSummary(struct langSummary *summary, char *text, int size);

float compareSummaries(struct langSummary *reference, struct langSummary *target);
