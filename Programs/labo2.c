//write CSV file
#include<stdio.h>
#include<string.h>


// function for creating and writing in file
void create_csv (char *filename) {
    FILE *pFile;
    int i, count, code, weight, height, age, gender;

    filename = strcat(filename,".csv");

    pFile = fopen(filename,"w+");

    fprintf(pFile, "Code, Weight, Height, Age, Gender");
    printf("How many informations do you want to save ? ");
    scanf("%d", &count);

    printf("\n");

    for (i = 1; i <= count; i++) {
        printf("Enter code : ");
        scanf("%d", &code);

        printf("Enter weight : ");
        scanf("%d", &weight);

        printf("Enter height : ");
        scanf("%d", &height);

        printf("Enter age : ");
        scanf("%d", &age);

        printf("Enter gender : ");
        scanf("%d", &gender);

        fprintf(pFile,"\n%d,%d,%d,%d,%d",code,weight,height,age,gender);
        printf("\n");
    }

    fclose(pFile);
    printf("\n %s file created !\n",filename);
}

//main method
int main () {
    char str[50];
    printf("\n Enter the filename : ");

    gets(str);
    create_csv(str);

    return 0;
}