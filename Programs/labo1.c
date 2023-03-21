// read CSV file program
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main () {
  // opening the file with the pFile pointer
  FILE *pFile = fopen("../Data/data_subjects_info.csv", "r");

  // checking if the file is opened
  if (pFile == NULL) {
    perror("Unable to open the file");
    exit(1);
  }

  char line[200];

  // reading the file line by line
  while (fgets(line, sizeof(line), pFile)) {
    char* token;
    token = strtok(line, ",");
    while (token != NULL) {
      printf("%-10s", token);
      token = strtok(NULL, ",");
    }
    printf("\n");
  }
}