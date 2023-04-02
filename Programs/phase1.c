// include libraires
#include<stdio.h>
#include<string.h>

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
void checkOutliers(OUTLIER **pStartOutliersX, OUTLIER **pStartOutliersY, OUTLIER **pStartOutliersZ, OUTLIER **pOutliersX, OUTLIER **pOutliersY, OUTLIER **pOutliersZ, int *nbOutliersX, int *nbOutliersY, int *nbOutliersZ, LINE_VECTORS lineVectors);
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
                
                if (previousTime != -1 && isMissingTime != 1 && lineVectors.time != previousTime + 1) {
                    isMissingTime = 1;
                }
                previousTime = lineVectors.time;

                // checkOutliers()

                nbLine++;
            }

            writeOutliers(pFileOutliers, url, nbLine, isMissingTime, nbOutliersX, nbOutliersY, nbOutliersZ, pStartOutliersX, pStartOutliersY, pStartOutliersZ);
            fclose(pFileSub);
        }
    }
    fclose(pFileOutliers);
}


// function definitions
LINE_VECTORS getLineVectors(char* line){
   LINE_VECTORS data;

    char* token = strtok(line, ",");
    data.time = atoi(token);

    for (int i = 1; i <= 10; i++) {
        token = strtok(NULL, ",");
        if (i == 10) {
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            data.vectorX = atof(token);
        } else if (i == 11) {
            token = strtok(NULL, ",");
            data.vectorY = atof(token);
        } else if (i == 12) {
            token = strtok(NULL, ",");
            data.vectorZ = atof(token);
        }
    }

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

void checkOutliers(OUTLIER **pStartOutliersX, OUTLIER **pStartOutliersY, OUTLIER **pStartOutliersZ, OUTLIER **pOutliersX, OUTLIER **pOutliersY, OUTLIER **pOutliersZ, int *nbOutliersX, int *nbOutliersY, int *nbOutliersZ, LINE_VECTORS lineVectors){

}