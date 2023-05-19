// write CSV file program
#include<stdio.h>
#include<string.h>


// function for creating and writing in file
void create_csv (char *filename) {
    // declaration of required variables
    FILE *pFile;
    int i, count, code, weight, height, age, gender;

    // creating the file and opening it in write mode
    sprintf(filename, "%s%s.csv", "../Data/", filename);
    pFile = fopen(filename,"w+");

    // adding the header to the file
    fprintf(pFile, "Code, Weight, Height, Age, Gender");

    // asking for the number of records to be saved
    printf("How many informations do you want to save ? ");
    scanf("%d", &count);

    printf("\n");

    // asking for the records to be saved in the file and writing them 
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

    // closing the file and printing a confirmation message
    fclose(pFile);
    printf("\n %s file created !\n",filename);
}

// main method
int main () {
    // declaration and initialization of the filename variable
    char str[50];
    printf("\n Enter the filename : ");
    gets(str);

    // calling the create_csv function
    create_csv(str);

    return 0;
}