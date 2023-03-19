#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LG_CAR 6

typedef struct fic FIC;
struct fic{
    double rNbr;
    int intNbr;
    char car[LG_CAR];
};

int main(void){
    FILE* fichBin;
    int error;
    FIC data, dataRead;

    data.rNbr = -1.25;
    data.intNbr = -10;
    strcpy_s(data.car,LG_CAR, "abc");

    // FICHIER BINAIRE
    error = fopen_s(&fichBin, "data.dat", "wb");
    if (!error){
        fwrite(&data, sizeof(FIC), 1, fichBin);
        fclose(fichBin);
    }

    // FICHIER TEXTE
    FILE* fichText;
    int error_t;
    error_t = fopen_s(&fichText, "data.csv", "w");
    if(error_t){
        fprintf(fichText, "%f %d %s", data.rNbr, data.intNbr, data.car)
        fclose(fichText);
    }

    // LECTURE DU CSV
    error_t = fopen_s(&fichText, "data.csv", "r");
    if (!error_t){
        error_t = fscanf_s(fichText, "%lf %d %s", &dataRead.rNbr, &dataRead.intNbr, dataRead.car,LG_CAR);
        fclose(fichText);
        printf_s("Nombre réel : %f\nNombre entier : %d\nChaine de caractère : %s",dataRead.rNbr, dataRead.intNbr, dataRead.car);
    }
}