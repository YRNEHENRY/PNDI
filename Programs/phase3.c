// include libraires
#include <stdio.h>
#include <math.h>

// define constants
#define TAB_SIZE 1000
#define NB_MOVEMENTS 32

// function prototypes
void loadModels(float (*models)[6][TAB_SIZE]);
void loadTest(FILE *pTestSet, float *vAccs[TAB_SIZE], int *realClasses[NB_MOVEMENTS], int nbMovements);
void findModel(float models[][TAB_SIZE], float vAccs[TAB_SIZE], int *realClasses[NB_MOVEMENTS], int *estimatedClasses[NB_MOVEMENTS], int nbMovements);
float euclidianDistance(float models[][TAB_SIZE], float vAccs[TAB_SIZE], int movement);

// main program
int main(){
    // opening the test set file with the pTestSet pointer
    FILE *pTestSet = fopen("../Data/Sets/testSet.csv", "r");

    // checking if the file is opened
    if (pTestSet == NULL) {
        perror("Unable to open the file: testSet.csv");
        exit(1);
    }

    // initialization of the necessary variables
    float models[6][TAB_SIZE] = {};
    float vAccs[TAB_SIZE] = {};
    int realClasses[NB_MOVEMENTS] = {};
    int estimatedClasses[NB_MOVEMENTS] = {};
    int nbMovements = 0;

    // loading the models
    loadModels(models);
    
    // while the end of the file is not reached
    while(nbMovements < NB_MOVEMENTS){
        // loading the test set line
        loadTest(pTestSet, vAccs, realClasses, nbMovements);
        // finding the right model for the vAccs
        findModel(models, vAccs, realClasses, estimatedClasses, nbMovements);
        nbMovements++;
    }

}

// function definitions
/*
    This function loads the models of the movements from the models.csv file
    @param models: the models of the movements
    @return: the vAccs of the models for each movement
*/
void loadModels(float (*models)[6][TAB_SIZE]){
    FILE *pModels = fopen("../Data/Sets/models.csv", "r");
    if (pModels == NULL) {
        perror("Unable to open the file: models.csv");
        exit(1);
    }

    // initialization of the necessary variables
    int movement;
    float vAcc;

    fscanf(pModels, "%*[^\n]\n"); // skipping the first line of the file

    // for each movement / read the file line by line
    while (fscanf(pModels, "%d,", &movement) == 1) {
        movement--; // adujsting the movement number
        for (int i = 0; i < TAB_SIZE; i++) {
            // getting the vAccs
            fscanf(pModels, "%f,", &vAcc);
            (*models)[movement][i] = vAcc;
        }
    }
    
    fclose(pModels);
}

/*
    This function loads the test set from the testSet.csv file
    @param pTestSet: the pointer to the testSet.csv file
    @param vAccs: the vAccs of the testSet line
    @param realClasses: the real classes of the testSet line
    @param nbMovements: the number of the movement in the testSet
    @return: the vAccs and the real classes of the testSet line
*/
void loadTest(FILE *pTestSet, float *vAccs[TAB_SIZE], int *realClasses[NB_MOVEMENTS], int nbMovements){
    // getting the movement number
    int movement;
    fscanf(pTestSet, "%d,", &movement);
    realClasses[nbMovements] = movement;

    // getting the vAccs
    for (int i = 0; i < TAB_SIZE; i++){
        fscanf(pTestSet, "%f", &vAccs[i]);
    }
}

/*
    This function finds the right model for the vAccs
    @param models: the models of the movements
    @param vAccs: the vAccs of the testSet line
    @param realClasses: the real classes of the testSet line
    @param estimatedClasses: the estimated classes of the testSet line
    @param nbMovements: the number of the movement in the testSet
    @return: the estimated classes of the testSet line
*/
void findModel(float models[][TAB_SIZE], float vAccs[TAB_SIZE], int *realClasses[NB_MOVEMENTS], int *estimatedClasses[NB_MOVEMENTS], int nbMovements){
    int estimation = 0;
    float bestDistance = MAXFLOAT;

    // for each movement of the models
    for (int i = 0; i < 6; i++){
        // calculating the euclidian distance between the movement model and the vAccs from the testSet line
        float distance = euclidianDistance(models, vAccs, i);
        // if the distance is smaller than the best distance, the estimation is the movement
        if (distance < bestDistance){
            bestDistance = distance;
            estimation = i+1;
        }
    }
    estimatedClasses[nbMovements] = estimation;
}

/*
    This function calculates the euclidian distance between the movement model and the vAccs
    @param models: the models of the movements
    @param vAccs: the vAccs of the testSet
    @param movement: the movement of the model
    @return the euclidian distance between the movement model and the vAccs
*/
float euclidianDistance(float models[][TAB_SIZE], float vAccs[TAB_SIZE], int movement){
    float distance = 0;
    for (int i = 0; i < TAB_SIZE; i++){
        distance += pow(models[movement][i] - vAccs[i], 2);
    }
    return distance;
}