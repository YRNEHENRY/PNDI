#include <stdio.h>

void getVAcc(double vAccs[2][1000], FILE* pTrainSet) {
    for (int i = 0; i < 1000; i++) {
        double value;
        fscanf(pTrainSet, "%lf,", &value);
        if (value != 0) {
            vAccs[0][i] += value;
            vAccs[1][i] += 1;
        }
    }
}

void calculMeansAndSaveInFile(double vAccs[2][1000], int movement, FILE* pPatterns) {
    fprintf(pPatterns, "%d,", movement);
    for (int i = 0; i < 1000; i++) {
        fprintf(pPatterns, "%lf,", vAccs[0][i] / vAccs[1][i]);
    }
}

int main() {
    FILE* pTrainSet = fopen("../Data/Sets/trainSet.csv", "r");
    if (pTrainSet == NULL) {
        printf("ERROR: Unable to open trainSet.csv file!\n");
        return 1;
    }

    FILE* pPatterns = fopen("../Data/Sets/patterns.csv", "w");
    if (pPatterns == NULL) {
        printf("ERROR: Unable to open patterns.csv file!\n");
        fclose(pTrainSet);
        return 1;
    }

    int movement;
    fscanf(pTrainSet, "%d,", &movement);

    while (!feof(pTrainSet)) {
        int actualMovement = movement;
        double vAccs[2][1000] = {0};

        while (actualMovement == movement) {
            getVAcc(vAccs, pTrainSet);
            fscanf(pTrainSet, "%d,", &movement);
        }

        calculMeansAndSaveInFile(vAccs, actualMovement, pPatterns);
    }

    fclose(pTrainSet);
    fclose(pPatterns);

    return 0;
}