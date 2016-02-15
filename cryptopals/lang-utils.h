
struct langSummary {
  int freqTable[128];
  float avgWordLength;
  float forbiddenCharPercentage;
};

// returns an allocated int array of length 128, caller must free
struct langSummary *newTrainedSummaryFromFile(char *fileName);
