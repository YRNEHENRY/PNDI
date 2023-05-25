// include libraries
#include <stdio.h>

// define constants
#define MAX_VACCS 1000
#define TRAIN_SET "../Data/Sets/trainSet.csv"
#define PATTERNS "../Data/Sets/patterns.csv"

// define structures
typedef struct VAcc VACC;
struct VAcc {
    double sum;
    int count;
};

// functions prototypes
void getVAcc(VACC vAccs[], FILE *pTrainSet);
void calculMeansAndSaveFile(VACC vAccs[], int movement, FILE *pPatterns);

// main program
int main() {
    // opening the trainSet.csv file (in read mode) with the pointer pTrainSet
    FILE *pTrainSet = fopen(TRAIN_SET, "r");
    if (pTrainSet == NULL) {
        printf("ERROR: Enable to open the `trainSet.csv` file\n");
        return 1;
    }

    // opening the patterns.csv file (in write mode) with the pointer pPatterns
    FILE *pPatterns = fopen(PATTERNS, "w");
    if (pPatterns == NULL) {
        printf("ERROR: Enable to open the `patterns.csv` file\n");
        fclose(pTrainSet);
        return 1;
    }

    // Skip first line of pTrainSet
    char buffer[1000];
    fgets(buffer, sizeof(buffer), pTrainSet);

    // writing the first line of the patterns.csv file (the header)
    fprintf(pPatterns, "movement, vAccs(1000)\n");

    // reading the trainSet.csv file and putting the current movement in the variable `movement`
    int movement, actualMovement;
    fscanf(pTrainSet, "%d,", &movement);
    
    // while the end of the file is not reached
    while (fscanf(pTrainSet, "%d,", &actualMovement) == 1) {
        actualMovement = movement;
        VACC vAccs[MAX_VACCS] = {0};

        // while the current movement is the same as the previous one: we add the values of the vAccs to the structure
        while (actualMovement == movement) {
            getVAcc(vAccs, pTrainSet);
            if (fscanf(pTrainSet, "%d,", &movement) != 1) {
                break;  // Arrêter la boucle si la lecture échoue
            }
            
        }

        calculMeansAndSaveFile(vAccs, actualMovement, pPatterns);
    }

    // closing the files
    fclose(pTrainSet);
    fclose(pPatterns);
    return 0;
}

// define functions

/*
    Add the values of the vAccs to the structure

    :param vAccs: the structure
    :param pTrainSet: the pointer to the trainSet.csv file
*/
void getVAcc(VACC vAccs[], FILE *pTrainSet) {
    double value;
    for(int i = 0; i < MAX_VACCS; i++) {
        fscanf(pTrainSet, "%lf,", &value);
        if (value != 0) {
            vAccs[i].sum += value;
            vAccs[i].count++;
        }
    }
}


/*
    Calculate the means of the vAccs and save them in the patterns.csv file
*/ 
void calculMeansAndSaveFile(VACC vAccs[], int movement, FILE *pPatterns) {
    fprintf(pPatterns, "%d", movement);
    for (int i = 0; i < MAX_VACCS; i++) {
        fprintf(pPatterns, ",%lf", vAccs[i].sum / vAccs[i].count);
    }
    fprintf(pPatterns, "\n");
}