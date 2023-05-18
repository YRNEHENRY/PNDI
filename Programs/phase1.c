// include libraires
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// define constants
#define MAX_SIZE 301
#define URL_SIZE 18
#define PATH_SIZE 38

#define THRESHOLDX_LEFT 0.00096087 - 3* 0.38875666
#define THRESHOLDX_RIGHT 0.00096087 + 3* 0.38875666

#define THRESHOLDY_LEFT 0.05525659 - 3* 0.61937128
#define THRESHOLDY_RIGHT 0.05525659 + 3* 0.61937128

#define THRESHOLDZ_LEFT 0.0352192 - 3* 0.4300345
#define THRESHOLDZ_RIGHT 0.0352192 + 3* 0.4300345

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
void checkOutliers(OUTLIER **pStartOutliersX, OUTLIER **pStartOutliersY, OUTLIER **pStartOutliersZ, OUTLIER **pOutliersX, OUTLIER **pOutliersY, OUTLIER **pOutliersZ, int *nbOutliersX, int *nbOutliersY, int *nbOutliersZ, LINE_VECTORS lineVectors);
void writeOutliers(FILE* file, char* url, int numLine, int isMissingTime, int nbOutliersX, int nbOutliersY, int nbOutliersZ, OUTLIER* pStartOutlierX, OUTLIER* pStartOutlierY, OUTLIER* pStartOutlierZ);
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
            snprintf(url, URL_SIZE, "%s%s%d.csv", paths[iMovement], "sub_", iSub);
            
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
            int nbLine = 0;
            int isMissingTime = 0;
            int previousTime = -1;
            int nbOutliersX = 0;
            int nbOutliersY = 0;
            int nbOutliersZ = 0;
            // we create the first element of the linked list of outliers
            OUTLIER* pStartOutliersX = NULL;
            OUTLIER* pStartOutliersY = NULL;
            OUTLIER* pStartOutliersZ = NULL;
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
                checkOutliers(&pStartOutliersX, &pStartOutliersY, &pStartOutliersZ, &pOutliersX, &pOutliersY, &pOutliersZ, &nbOutliersX, &nbOutliersY, &nbOutliersZ, lineVectors);
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
/*
    This function get the time, vectorX, vectorY and vectorZ values of a line in the csv file

    @param line: the line of the csv file
    @return data: the time, vectorX, vectorY and vectorZ values of the line
*/
LINE_VECTORS getLineVectors(char* line) {
    LINE_VECTORS data;

    sscanf(line, "%d,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%*f,%lf,%lf,%lf",
           &data.time, &data.vectorX, &data.vectorY, &data.vectorZ);

    return data;
}

/*
    This function takes all the informations of the csvs and write in a review file all the outliers

    @param file: the review file
    @param url: the url of the csv file
    @param numLine: the number of lines of the csv file
    @param isMissingTime: 1 if there is a missing time, 0 otherwise
    @param nbOutliersX: the number of outliers in the X vector
    @param nbOutliersY: the number of outliers in the Y vector
    @param nbOutliersZ: the number of outliers in the Z vector
    @param pStartOutlierX: the pointer to the first outlier of the X vector to write in the review file
    @param pStartOutlierY: the pointer to the first outlier of the Y vector to write in the review file
    @param pStartOutlierZ: the pointer to the first outlier of the Z vector to write in the review file
*/
void writeOutliers(FILE* file, char* url, int numLine, int isMissingTime, int nbOutliersX, int nbOutliersY, int nbOutliersZ, OUTLIER* pStartOutlierX, OUTLIER* pStartOutlierY, OUTLIER* pStartOutlierZ) {
    // Write the URL in the file
    fprintf(file, "%s,", url);
    // Write numLine, isMissingTime, nbOutliersX in the file
    fprintf(file, "%d,%d,%d", numLine, isMissingTime, nbOutliersX);

    // Write the X outliers in the file
    OUTLIER* outlierX = pStartOutlierX;
    while (outlierX != NULL) {
        fprintf(file, ",%d,%f", outlierX->line, outlierX->vectorValue);
        OUTLIER* nextX = outlierX->next;
        free(outlierX); // free the current outlierX
        outlierX = nextX; // move to the next outlierX
    }

    // Write the Y outliers in the file
    fprintf(file, ",%d", nbOutliersY);
    OUTLIER* outlierY = pStartOutlierY;
    while (outlierY != NULL) {
        fprintf(file, ",%d,%f", outlierY->line, outlierY->vectorValue);
        OUTLIER* nextY = outlierY->next;
        free(outlierY); // free the current outlierY
        outlierY = nextY; // move to the next outlierY
    }

    // Write the Z outliers in the file
    fprintf(file, ",%d", nbOutliersZ);
    OUTLIER* outlierZ = pStartOutlierZ;
    while (outlierZ != NULL) {
        fprintf(file, ",%d,%f", outlierZ->line, outlierZ->vectorValue);
        OUTLIER* nextZ = outlierZ->next;
        free(outlierZ); // free the current outlierZ
        outlierZ = nextZ; // move to the next outlierZ
    }

    // Adds a new line at the end of the function
    fprintf(file, "\n");
}

/*
    Check if there are outliers in the line and add them to the linked list of outliers

    @param pStartOutliersX: pointer to the first element of the linked list of outliers in X
    @param pStartOutliersY: pointer to the first element of the linked list of outliers in Y
    @param pStartOutliersZ: pointer to the first element of the linked list of outliers in Z
    @param pOutliersX: pointer to the last element of the linked list of outliers in X
    @param pOutliersY: pointer to the last element of the linked list of outliers in Y
    @param pOutliersZ: pointer to the last element of the linked list of outliers in Z
    @param nbOutliersX: number of outliers in X
    @param nbOutliersY: number of outliers in Y
    @param nbOutliersZ: number of outliers in Z
    @param lineVectors: structure containing the values of the line
*/
void checkOutliers(OUTLIER **pStartOutliersX, OUTLIER **pStartOutliersY, OUTLIER **pStartOutliersZ, OUTLIER **pOutliersX, OUTLIER **pOutliersY, OUTLIER **pOutliersZ, int *nbOutliersX, int *nbOutliersY, int *nbOutliersZ, LINE_VECTORS lineVectors) {
    // Check for outliers in X
    if (lineVectors.vectorX < THRESHOLDX_LEFT || lineVectors.vectorX > THRESHOLDX_RIGHT) {
        // Create a new outlier node
        OUTLIER *newOutlierX = (OUTLIER *)malloc(sizeof(OUTLIER));
        newOutlierX->line = lineVectors.time;
        newOutlierX->vectorValue = lineVectors.vectorX;
        newOutlierX->next = NULL;

        // Check if pStartOutliersX is NULL, indicating an empty list
        if (*pStartOutliersX == NULL) {
            *pStartOutliersX = newOutlierX; // Set the start outlier as the new node
            *pOutliersX = newOutlierX; // Set the current outlier as the new node
        } else {
            (*pOutliersX)->next = newOutlierX; // Set the next of the current outlier as the new node
            *pOutliersX = newOutlierX; // Set the current outlier as the new node
        }
        (*nbOutliersX)++; // Increment the count of outliers in X
    }

    // Check for outliers in Y
    if (lineVectors.vectorY < THRESHOLDY_LEFT || lineVectors.vectorY > THRESHOLDY_RIGHT) {
        // Create a new outlier node
        OUTLIER *newOutlierY = (OUTLIER *)malloc(sizeof(OUTLIER));
        newOutlierY->line = lineVectors.time;
        newOutlierY->vectorValue = lineVectors.vectorY;
        newOutlierY->next = NULL;

        // Check if pStartOutliersY is NULL, indicating an empty list
        if (*pStartOutliersY == NULL) {
            *pStartOutliersY = newOutlierY; // Set the start outlier as the new node
            *pOutliersY = newOutlierY; // Set the current outlier as the new node
        } else {
            (*pOutliersY)->next = newOutlierY; // Set the next of the current outlier as the new node
            *pOutliersY = newOutlierY; // Set the current outlier as the new node
        }
        (*nbOutliersY)++; // Increment the count of outliers in Y
    }

    // Check for outliers in Z
    if (lineVectors.vectorZ < THRESHOLDZ_LEFT || lineVectors.vectorZ > THRESHOLDZ_RIGHT) {
        // Create a new outlier node
        OUTLIER *newOutlierZ = (OUTLIER *)malloc(sizeof(OUTLIER));
        newOutlierZ->line = lineVectors.time;
        newOutlierZ->vectorValue = lineVectors.vectorZ;
        newOutlierZ->next = NULL;

        // Check if pStartOutliersZ is NULL, indicating an empty list
        if (*pStartOutliersZ == NULL) {
            *pStartOutliersZ = newOutlierZ; // Set the start outlier as the new node
            *pOutliersZ = newOutlierZ; // Set the current outlier as the new node
        } else {
            (*pOutliersZ)->next = newOutlierZ; // Set the next of the current outlier as the new node
            *pOutliersZ = newOutlierZ; // Set the current outlier as the new node
        }
        (*nbOutliersZ)++; // Increment the count of outliers in Z
        }
}