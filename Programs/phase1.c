#include<stdio.h>
#include<string.h>

#define MAX_SIZE 101
#define URL_SIZE 17
#define PATH_SIZE 38

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
    
    double vectorXOutliers[MAX_SIZE];
    double vectorYOutliers[MAX_SIZE];
    double vectorZOutliers[MAX_SIZE];
    int vectorXOutlierLines[MAX_SIZE];
    int vectorYOutlierLines[MAX_SIZE];
    int vectorZOutlierLines[MAX_SIZE];

    // opening the review file with the pFileOutliers pointer
    FILE *pFileOutliers = fopen("../Data/review_outliers.csv", "a");

    // checking if the file is opened
    if (pFileOutliers == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    // We run through our 15 files containing the movements
    for (int iMovement = 0; iMovement < 15; iMovement++){
        // We run through our 24 movements (sub)
        for (int iSub = 1; iSub <= 24; iSub++){
            char url[URL_SIZE];
            snprintf(url, URL_SIZE, "%s%s%d.csv", paths[iMovement], "sub", iSub);
            
            char filePath[PATH_SIZE] = "../Data/DeviceMotion/";
            strcat(filePath, url);

            // opening the sub file with the pFileSub pointer
            FILE *pFileSub = fopen(filePath, "r");

            // checking if the file is opened
            if (pFileSub == NULL) {
                perror("Unable to open the file");
                exit(1);
            }


        }
    }
}