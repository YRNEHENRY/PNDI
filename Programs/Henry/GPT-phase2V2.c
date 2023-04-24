#include <stdio.h>

#define MAX_VACCS 1000

struct VAcc {
    double summary;
    int count;
};

void getVAcc(struct VAcc vAccs[], FILE *pTrainSet) {
    int value;
    for(int i = 0; i < MAX_VACCS; i++) {
        fscanf(pTrainSet, "%d,", &value);
        if (value != 0) {
            vAccs[i].summary += value;
            vAccs[i].count++;
        }
    }
}

void calculMeansAndSaveFile(struct VAcc vAccs[], int movement, FILE *pPatterns) {
    fprintf(pPatterns, "%d", movement);
    for (int i = 0; i < MAX_VACCS; i++) {
        fprintf(pPatterns, ",%.lf", vAccs[i].summary / vAccs[i].count);
    }
    fprintf(pPatterns, "\n");
}

int main() {
    FILE *pTrainSet = fopen("trainSet.csv", "r");
    if (pTrainSet == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier trainSet.csv\n");
        return 1;
    }

    FILE *pPatterns = fopen("patterns.csv", "w");
    if (pPatterns == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier patterns.csv\n");
        fclose(pTrainSet);
        return 1;
    }

    fprintf(pPatterns, "movement");
    for (int i = 0; i < MAX_VACCS; i++) {
        fprintf(pPatterns, ",vAccs");
    }
    fprintf(pPatterns, "\n");

    int movement, actualMovement;
    fscanf(pTrainSet, "%d,", &movement);
    
    while (!feof(pTrainSet)) {
        actualMovement = movement;
        struct VAcc vAccs[MAX_VACCS] = {0};

        while (actualMovement == movement) {
            getVAcc(vAccs, pTrainSet);
            fscanf(pTrainSet, "%d", &movement);
        }

        calculMeansAndSaveFile(vAccs, movement, pPatterns);
    }

    fclose(pTrainSet);
    fclose(pPatterns);
    return 0;
}
