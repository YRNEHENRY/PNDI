void checkOutliers(OUTLIER **pStartOutliersX, OUTLIER **pStartOutliersY, OUTLIER **pStartOutliersZ, OUTLIER **pOutliersX, OUTLIER **pOutliersY, OUTLIER **pOutliersZ, int *nbOutliersX, int *nbOutliersY, int *nbOutliersZ, LINE_VECTORS lineVectors, double thresholds[3][2]) {
    // Check for outliers in X
    if (lineVectors.vectorX < thresholds[0][0] || lineVectors.vectorX > thresholds[0][1]) {
        // Create a new outlier node
        OUTLIER *newOutlierX = (OUTLIER *)malloc(sizeof(OUTLIER));
        newOutlierX->line = lineVectors.time;
        newOutlierX->vectorValue = lineVectors.vectorX;
        newOutlierX->next = NULL;

        // Check if pStartOutliersX is NULL, indicating an empty list
        if (*pStartOutliersX == NULL) {
            *pStartOutliersX = newOutlierX; // Set the start outlier as the new node
            *pOutliersX = newOutlierX; // Set the current outlier as the new node
        } else {
            (*pOutliersX)->next = newOutlierX; // Set the next of the current outlier as the new node
            *pOutliersX = newOutlierX; // Set the current outlier as the new node
        }
        (*nbOutliersX)++; // Increment the count of outliers in X
    }

    // Check for outliers in Y
    if (lineVectors.vectorY < thresholds[1][0] || lineVectors.vectorY > thresholds[1][1]) {
        // Create a new outlier node
        OUTLIER *newOutlierY = (OUTLIER *)malloc(sizeof(OUTLIER));
        newOutlierY->line = lineVectors.time;
        newOutlierY->vectorValue = lineVectors.vectorY;
        newOutlierY->next = NULL;

        // Check if pStartOutliersY is NULL, indicating an empty list
        if (*pStartOutliersY == NULL) {
            *pStartOutliersY = newOutlierY; // Set the start outlier as the new node
            *pOutliersY = newOutlierY; // Set the current outlier as the new node
        } else {
            (*pOutliersY)->next = newOutlierY; // Set the next of the current outlier as the new node
            *pOutliersY = newOutlierY; // Set the current outlier as the new node
        }
        (*nbOutliersY)++; // Increment the count of outliers in Y
    }

    // Check for outliers in Z
    if (lineVectors.vectorZ < thresholds[2][0] || lineVectors.vectorZ > thresholds[2][1]) {
        // Create a new outlier node
        OUTLIER *newOutlierZ = (OUTLIER *)malloc(sizeof(OUTLIER));
        newOutlierZ->line = lineVectors.time;
        newOutlierZ->vectorValue = lineVectors.vectorZ;
        newOutlierZ->next = NULL;

        // Check if pStartOutliersZ is NULL, indicating an empty list
        if (*pStartOutliersZ == NULL) {
            *pStartOutliersZ = newOutlierZ; // Set the start outlier as the new node
            *pOutliersZ = newOutlierZ; // Set the current outlier as the new node
        } else {
            (*pOutliersZ)->next = newOutlierZ; // Set the next of the current outlier as the new node
            *pOutliersZ = newOutlierZ; // Set the current outlier as the new node
        }
        (*nbOutliersZ)++; // Increment the count of outliers in Z
        }
}


void writeOutliers(FILE* file, char* url, int numLine, int isMissingTime, int nbOutliersX, int nbOutliersY, int nbOutliersZ, OUTLIER* pStartOutlierX, OUTLIER* pStartOutlierY, OUTLIER* pStartOutlierZ) {
    // Write the URL in the file
    fprintf(file, "%s,", url);
    // Write numLine, isMissingTime, nbOutliersX in the file
    fprintf(file, "%d,%d,%d", numLine, isMissingTime, nbOutliersX);

    // Write the X outliers in the file
    OUTLIER* outlierX = pStartOutlierX;
    while (outlierX != NULL) {
        fprintf(file, ",%d,%f", outlierX->line, outlierX->vectorValue);
        OUTLIER* nextX = outlierX->next;
        free(outlierX); // free the current outlierX
        outlierX = nextX; // move to the next outlierX
    }

    // Write the Y outliers in the file
    fprintf(file, ",%d", nbOutliersY);
    OUTLIER* outlierY = pStartOutlierY;
    while (outlierY != NULL) {
        fprintf(file, ",%d,%f", outlierY->line, outlierY->vectorValue);
        OUTLIER* nextY = outlierY->next;
        free(outlierY); // free the current outlierY
        outlierY = nextY; // move to the next outlierY
    }

    // Write the Z outliers in the file
    fprintf(file, ",%d", nbOutliersZ);
    OUTLIER* outlierZ = pStartOutlierZ;
    while (outlierZ != NULL) {
        fprintf(file, ",%d,%f", outlierZ->line, outlierZ->vectorValue);
        OUTLIER* nextZ = outlierZ->next;
        free(outlierZ); // free the current outlierZ
        outlierZ = nextZ; // move to the next outlierZ
    }

    // Adds a new line at the end of the function
    fprintf(file, "\n");
}