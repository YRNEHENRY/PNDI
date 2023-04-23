#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int main() {
    // Ouverture des fichiers trainSet.csv et patterns.csv en mode lecture et écriture, respectivement
    FILE* trainSetFile = fopen("../Data/trainSet.csv", "r");
    FILE* patternsFile = fopen("../Data/patterns.csv", "w");

    if (trainSetFile == NULL || patternsFile == NULL) {
        printf("Erreur : impossible d'ouvrir un des fichiers.\n");
        exit(1);
    }

    // Variables pour stocker les moyennes des mouvements
    int currentMovement = -1;
    float currentSum = 0;
    int count = 0;

    // Boucle de lecture des lignes de trainSet.csv
    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, trainSetFile); // Lecture de la première ligne (entête)
    while (fgets(line, MAX_LINE_LENGTH, trainSetFile)) {
        // Extraction du numéro de mouvement et de la première cellule de la ligne
        char* token = strtok(line, ",");
        int movement = atoi(token);
        token = strtok(NULL, ",");
        float value = atof(token);

        // Si on commence un nouveau mouvement, on écrit la moyenne du précédent dans patterns.csv
        if (movement != currentMovement && count > 0) {
            fprintf(patternsFile, "%d,", currentMovement);
            for (int i = 0; i < 999; i++) {
                fprintf(patternsFile, "%f,", currentSum / count);
            }
            fprintf(patternsFile, "%f\n", currentSum / count);

            // Réinitialisation des variables pour le nouveau mouvement
            currentMovement = movement;
            currentSum = 0;
            count = 0;
        }

        // Ajout de la valeur à la somme en cours pour le mouvement actuel
        currentSum += value;
        count++;
    }

    // Écriture de la dernière moyenne dans patterns.csv
    fprintf(patternsFile, "%d,", currentMovement);
    for (int i = 0; i < 999; i++) {
        fprintf(patternsFile, "%f,", currentSum / count);
    }
    fprintf(patternsFile, "%f\n", currentSum / count);

    // Fermeture des fichiers
    fclose(trainSetFile);
    fclose(patternsFile);

    return 0;
}
