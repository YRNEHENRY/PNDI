// include libraires
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// define constants
#define MAX_SIZE 301
#define URL_SIZE 17
#define PATH_SIZE 38

// struct definition
typedef struct outlier OUTLIER;
struct outlier {
    int line;
    double vectorValue;
    struct outlier* next;
};

typedef struct LineVectors LINE_VECTORS;
struct LineVectors {
    int time;
    double vectorX;
    double vectorY;
    double vectorZ;
};

// function prototypes
void checkOutliers(OUTLIER **pStartOutliersX, OUTLIER **pStartOutliersY, OUTLIER **pStartOutliersZ, OUTLIER **pOutliersX, OUTLIER **pOutliersY, OUTLIER **pOutliersZ, int *nbOutliersX, int *nbOutliersY, int *nbOutliersZ, LINE_VECTORS lineVectors, double thresholds[3][2]);
void writeOutliers(FILE* file, char* url, int numLine, int isMissingTime, int nbOutliersX, int nbOutliersY, int nbOutliersZ, OUTLIER* pOutliersX, OUTLIER* pOutliersY, OUTLIER* pOutliersZ);
LINE_VECTORS getLineVectors(char* line);    

// main program
int main(){
    // 7 characters (the last character is reserved for the null character '\0' which marks the end of the string)
    char paths[15][8] = {
        "dws_1/","dws_2/","dws_11/",
        "jog_9/","jog_16/",
        "sit_5/","sit_13/",
        "std_6/","std_14/",
        "ups_3/","ups_4/","ups_12/",
        "wlk_7/","wlk_8/","wlk_15/"
    };
    // thresholds for the 3 vectors to detect outliers
    double thresholds[3][2] = {
        {0.00096087 - 3* 0.38875666, 0.00096087 + 3* 0.38875666},
        {0.05525659 - 3* 0.61937128, 0.05525659 + 3* 0.61937128},
        {0.0352192 - 3* 0.4300345,0.0352192 + 3* 0.4300345}
    };

    // opening the review file with the pFileOutliers pointer
    FILE *pFileOutliers = fopen("../Data/review_outliers.csv", "a");

    // checking if the file is opened
    if (pFileOutliers == NULL) {
        perror("Unable to open the file: review_outliers.csv");
        exit(1);
    }

    // We run through our 15 files containing the movements
    for (int iMovement = 0; iMovement < 15; iMovement++){
        // We run through our 24 movements (sub)
        for (int iSub = 1; iSub <= 24; iSub++){
            // create the url of the file
            char url[URL_SIZE];
            snprintf(url, URL_SIZE, "%s%s%d.csv", paths[iMovement], "sub", iSub);
            
            // create the path of the file combining the url and the path
            char filePath[PATH_SIZE] = "../Data/DeviceMotion/";
            strcat(filePath, url);

            FILE *pFileSub = fopen(filePath, "r"); // opening the sub file with the pFileSub pointer
            // checking if the file is opened
            if (pFileSub == NULL) {
                printf("Unable to open the file: %s", url);
                exit(1);
            }

            // initialization of the necessary variables
            int nbLine, isMissingTime = 0;
            int previousTime = -1;
            int nbOutliersX, nbOutliersY, nbOutliersZ = 0;
            // we create the first element of the linked list of outliers
            OUTLIER* pStartOutliersX = malloc(sizeof(OUTLIER));
            OUTLIER* pStartOutliersY = malloc(sizeof(OUTLIER));
            OUTLIER* pStartOutliersZ = malloc(sizeof(OUTLIER));
            // we initialize the pointers that will be used to add new elements to the linked list
            OUTLIER* pOutliersX = pStartOutliersX;
            OUTLIER* pOutliersY = pStartOutliersY;
            OUTLIER* pOutliersZ = pStartOutliersZ;

            char line[MAX_SIZE];
            fgets(line, MAX_SIZE, pFileSub); // we skip the first line (the header)

            // we read the file line by line, beginning with the second line, until the end of the file
            while (fgets(line, MAX_SIZE, pFileSub) != NULL) { 
                LINE_VECTORS lineVectors = getLineVectors(line);
                
                // we check if there is a missing time
                if (previousTime != -1 && isMissingTime != 1 && lineVectors.time != previousTime + 1) {
                    isMissingTime = 1;
                }
                previousTime = lineVectors.time;

                // we check if there are outliers
                checkOutliers(&pStartOutliersX, &pStartOutliersY, &pStartOutliersZ, &pOutliersX, &pOutliersY, &pOutliersZ, &nbOutliersX, &nbOutliersY, &nbOutliersZ, lineVectors, thresholds);

                nbLine++;
            }

            // we write the outliers in the review file
            writeOutliers(pFileOutliers, url, nbLine, isMissingTime, nbOutliersX, nbOutliersY, nbOutliersZ, pStartOutliersX, pStartOutliersY, pStartOutliersZ);
            fclose(pFileSub);
        }
    }
    fclose(pFileOutliers);
}


// function definitions
LINE_VECTORS getLineVectors(char* line) {
    LINE_VECTORS data;

    sscanf(line, "%d,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%lf,%lf,%lf",
           &data.time, &data.vectorX, &data.vectorY, &data.vectorZ);

    return data;
}

void writeOutliers(FILE* file, char* url, int numLine, int isMissingTime, int nbOutliersX, int nbOutliersY, int nbOutliersZ, OUTLIER* pOutliersX, OUTLIER* pOutliersY, OUTLIER* pOutliersZ) {
    // Write the URL in the file
    fprintf(file, "%s,", url);
    // Write numLine, isMissingTime, nbOutliersX in the file
    fprintf(file, "%d,%d,%d", numLine, isMissingTime, nbOutliersX);

    // Write the X outliers in the file
    for (OUTLIER* outlier = pOutliersX; outlier != NULL; outlier = outlier->next) {
        fprintf(file, ",%d, %f", outlier->line, outlier->vectorValue);
    }

    // Write the Y outliers in the file
    fprintf(file, ",%d",nbOutliersY);
    for (OUTLIER* outlier = pOutliersY; outlier != NULL; outlier = outlier->next) {
        fprintf(file, ",%d, %f", outlier->line, outlier->vectorValue);
    }

    // Write the Z outliers in the file
    fprintf(file, ",%d",nbOutliersZ);
    for (OUTLIER* outlier = pOutliersZ; outlier != NULL; outlier = outlier->next) {
        fprintf(file, ",%d, %f", outlier->line, outlier->vectorValue);
    }

    // Adds a new line at the end of the function
    fprintf(file, "\n");
}

void checkOutliers(OUTLIER **pStartOutliersX, OUTLIER **pStartOutliersY, OUTLIER **pStartOutliersZ, OUTLIER **pOutliersX, OUTLIER **pOutliersY, OUTLIER **pOutliersZ, int *nbOutliersX, int *nbOutliersY, int *nbOutliersZ, LINE_VECTORS lineVectors, double thresholds[3][2]){
    // check if the vector X is an outlier
    if (lineVectors.vectorX < thresholds[0][0] || lineVectors.vectorX > thresholds[0][1]) {
        // if it is the first outlier, we initialize the first element of the linked list
        if (*nbOutliersX == 0) {
            (*pStartOutliersX)->line = lineVectors.time;
            (*pStartOutliersX)->vectorValue = lineVectors.vectorX;
            (*pStartOutliersX)->next = NULL;
        } else {
            // if it is not the first outlier, we add a new element to the linked list
            (*pOutliersX)->next = malloc(sizeof(OUTLIER));
            (*pOutliersX) = (*pOutliersX)->next;
            (*pOutliersX)->line = lineVectors.time;
            (*pOutliersX)->vectorValue = lineVectors.vectorX;
            (*pOutliersX)->next = NULL;
        }
        (*nbOutliersX)++;
    }

    // check if the vector Y is an outlier
    if (lineVectors.vectorY < thresholds[1][0] || lineVectors.vectorY > thresholds[1][1]) {
        // if it is the first outlier, we initialize the first element of the linked list
        if (*nbOutliersY == 0) {
            (*pStartOutliersY)->line = lineVectors.time;
            (*pStartOutliersY)->vectorValue = lineVectors.vectorY;
            (*pStartOutliersY)->next = NULL;
        } else {
            // if it is not the first outlier, we add a new element to the linked list
            (*pOutliersY)->next = malloc(sizeof(OUTLIER));
            (*pOutliersY) = (*pOutliersY)->next;
            (*pOutliersY)->line = lineVectors.time;
            (*pOutliersY)->vectorValue = lineVectors.vectorY;
            (*pOutliersY)->next = NULL;
        }
        (*nbOutliersY)++;
    }

    // check if the vector Z is an outlier
    if (lineVectors.vectorZ < thresholds[2][0] || lineVectors.vectorZ > thresholds[2][1]) {
        // if it is the first outlier, we initialize the first element of the linked list
        if (*nbOutliersZ == 0) {
            (*pStartOutliersZ)->line = lineVectors.time;
            (*pStartOutliersZ)->vectorValue = lineVectors.vectorZ;
            (*pStartOutliersZ)->next = NULL;
        } else {
            // if it is not the first outlier, we add a new element to the linked list
            (*pOutliersZ)->next = malloc(sizeof(OUTLIER));
            (*pOutliersZ) = (*pOutliersZ)->next;
            (*pOutliersZ)->line = lineVectors.time;
            (*pOutliersZ)->vectorValue = lineVectors.vectorZ;
            (*pOutliersZ)->next = NULL;
        }
        (*nbOutliersZ)++;
        }
}