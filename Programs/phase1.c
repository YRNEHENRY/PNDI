#include<stdio.h>
#include<string.h>

#define MAX_SIZE 201
#define URL_SIZE 17
#define PATH_SIZE 38

typedef struct outlier OUTLIER;
struct outlier {
    int line;
    double vectorValue;
    struct outlier* next;
};

void checkOutliers();
void writeOutliers();
char** getLineVectors(char* line);
void freeLineVectors(char** lineVectors, int numValues);

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
            int nbLine, time, missingTime = 0;
            int nbOutliersX, nbOutliersY, nbOutliersZ = 0;
            OUTLIER* pStartOutliersX = malloc(sizeof(OUTLIER));
            OUTLIER* pStartOutliersY = malloc(sizeof(OUTLIER));
            OUTLIER* pStartOutliersZ = malloc(sizeof(OUTLIER));

            char line[MAX_SIZE];
            fgets(line, MAX_SIZE, pFileSub); // we skip the first line (the header)

            // we read the file line by line, beginning with the second line, until the end of the file
            while (fgets(line, MAX_SIZE, pFileSub) != NULL) { 
                char** lineVectors = getLineVectors(line);
            }

            fclose(pFileSub);
        }
    }
    fclose(pFileOutliers);
}


char** getLineVectors(char* line){

}

void freeLineVectors(char** lineVectors, int numValues) {
    for (int i = 0; i < numValues; i++) {
        free(lineVectors[i]);
    }
    free(lineVectors);
}