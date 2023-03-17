// read CSV file
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main () {
  FILE *pFile = fopen("data_subjects_info.csv", "r");

  if (pFile == NULL) {
    perror("Unable to open the file");
    exit(1);
  }

  char line[200];

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