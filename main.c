#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FIELDS 100
#define MAX_STRING_LENGTH 1000

void trim(char *str) {
    int i = 0;
    int len = strlen(str);

    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        i++;
    }

    int j = 0;
    while (j + i < len) {
        str[j] = str[j + i];
        j++;
    }

    str[j] = '\0';

    j = strlen(str) - 1;
    while (j >= 0 && (str[j] == ' ' || str[j] == '\t' || str[j] == '\n')) {
        str[j] = '\0';
        j--;
    }
}


void jsonToCsv(const char *jsonFilename, const char *csvFilename) {
    FILE *jsonFile = fopen(jsonFilename, "r");
    FILE *csvFile = fopen(csvFilename, "w");

    if (jsonFile == NULL) {
        printf("Error opening JSON file\n");
        return;
    }

    if (csvFile == NULL) {
        printf("Error creating CSV file\n");
        fclose(jsonFile);
        return;
    }

    char line[MAX_STRING_LENGTH];
    char csvLine[MAX_STRING_LENGTH];

    fgets(line, sizeof(line), jsonFile);
    char *token = strtok(line, "{\":,}\n");
    while (token != NULL) {
        trim(token);
        strcat(csvLine, token);
        strcat(csvLine, ",");
        token = strtok(NULL, "{\":,}\n");
    }
    csvLine[strlen(csvLine) - 1] = '\n';
    fputs(csvLine, csvFile);

    // Write field values from JSON to CSV
    while (fgets(line, sizeof(line), jsonFile) != NULL) {
        strcpy(csvLine, "");
        token = strtok(line, "{\":,}\n");
        while (token != NULL) {
            trim(token);
            strcat(csvLine, token);
            strcat(csvLine, ",");
            token = strtok(NULL, "{\":,}\n");
        }
        csvLine[strlen(csvLine) - 1] = '\n';
        fputs(csvLine, csvFile);
    }

    fclose(jsonFile);
    fclose(csvFile);
}

void csvToJson(const char *csvFilename, const char *jsonFilename) {
    FILE *csvFile = fopen(csvFilename, "r");
    FILE *jsonFile = fopen(jsonFilename, "w");

    if (csvFile == NULL) {
        printf("Error opening CSV file\n");
        return;
    }

    if (jsonFile == NULL) {
        printf("Error creating JSON file\n");
        fclose(csvFile);
        return;
    }

    char line[MAX_STRING_LENGTH];
    char jsonLine[MAX_STRING_LENGTH];

    fgets(line, sizeof(line), csvFile);
    char *token = strtok(line, ",");
    int numFields = 0;
    char fieldNames[MAX_FIELDS][MAX_STRING_LENGTH];
    while (token != NULL && numFields < MAX_FIELDS) {
        trim(token);
        strcpy(fieldNames[numFields], token);
        numFields++;
        token = strtok(NULL, ",");
    }

    while (fgets(line, sizeof(line), csvFile) != NULL) {
        strcpy(jsonLine, "{");
        token = strtok(line, ",");
        int fieldIndex = 0;
        while (token != NULL && fieldIndex < numFields) {
            trim(token);
            strcat(jsonLine, "\"");
            strcat(jsonLine, fieldNames[fieldIndex]);
            strcat(jsonLine, "\":\"");
            strcat(jsonLine, token);
            strcat(jsonLine, "\"");
            if (fieldIndex != numFields - 1) {
                strcat(jsonLine, ",");
            }
            fieldIndex++;
            token = strtok(NULL, ",");
        }
        strcat(jsonLine, "}\n");
        fputs(jsonLine, jsonFile);
    }

    fclose(csvFile);
    fclose(jsonFile);
}

int main() {
    const char *inputFilename = "C:\\Users\\reeds\\CLionProjects\\RK2\\input.txt";
    const char *outputFilename = "C:\\Users\\reeds\\CLionProjects\\RK2\\output.txt";

    FILE *inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char line[MAX_STRING_LENGTH];
    fgets(line, sizeof(line), inputFile);
    trim(line);
    fclose(inputFile);

    if (strstr(line, "{") != NULL) {
        jsonToCsv(inputFilename, outputFilename);
        printf("Successfully converted JSON to CSV\n");
    } else if (strstr(line, ",") != NULL) {
        csvToJson(inputFilename, outputFilename);
        printf("Successfully converted CSV to JSON\n");
    } else {
        printf("Unsupported data format\n");
        return 1;
    }

    return 0;
}


