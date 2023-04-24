#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_LINE_LENGTH 1024

// struct definition
typedef struct vAcc vAcc;
struct vAcc {
    int count;
    double summary;
};  

void main(void){

    //We open our files
    FILE* trainSetFile = fopen("../Data/trainSet.csv", "r");
    FILE* patternsFile = fopen("../Data/patterns.csv", "w");

    if (trainSetFile == NULL) {
        printf("Unable to open trainSetFile\n");
        exit(1);
    }
    if (patternsFile == NULL) {
        printf("Unable to open patternsFile\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    //We read the headLine
    fgets(line, MAX_LINE_LENGTH, trainSetFile);

    while(fgets(line, MAX_LINE_LENGTH, trainSetFile) != NULL){
        // we dont have trainset file
    }

}