#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
   Name inputs (first, last and file) are max 100 characters,
   id inputs are max 100 characters but actual id:s are max 6 characters,
   commands are max 1000 characters.
*/

struct Student {
    char ID[7];
    char *firstName;
    char *lastName;
    int points[6];
    int totalPoints;
};

/*
   A function to add a student.
*/
struct Student *addStudent(struct Student *studentArray, unsigned int size, const char *id, const char *firstName, const char *lastName)
{
    struct Student *expandedArray = realloc(studentArray, sizeof(struct Student) * (size + 1));
    unsigned int i = size;

    strcpy(expandedArray[i].ID, id);

    expandedArray[i].firstName = malloc(strlen(firstName) + 1);
    if (expandedArray[i].firstName == NULL) {
        expandedArray = NULL;
        return NULL;
    }
    strcpy(expandedArray[i].firstName, firstName);

    expandedArray[i].lastName = malloc(strlen(lastName) + 1);
    if (expandedArray[i].lastName == NULL) {
        free(expandedArray[i].firstName);
        expandedArray = NULL;
        return NULL;
    }
    strcpy(expandedArray[i].lastName, lastName);

    for (unsigned int k = 0; k < 6; k++) {
        expandedArray[i].points[k] = 0;
    }
    expandedArray[i].totalPoints = 0;
    return expandedArray;
}

/*
   A function to update a student's points.
*/
int updatePoints(struct Student *studentArray, unsigned int size, const char *id, int round, int newPoints) {
    for (unsigned int i = 0; i < size; i++) {
        if (strcmp(studentArray[i].ID, id) == 0) {
            int oldPoints = studentArray[i].points[round - 1];
            studentArray[i].points[round - 1] = newPoints;
            studentArray[i].totalPoints -= oldPoints;
            studentArray[i].totalPoints += newPoints;
            return 1;
        }
    }
    return 0;
}

/*
   A function to print out the database's students and their points.
   The points are listed from round 1 to round 6 and the last points are the student's total points.
*/
void printStudents(struct Student *studentArray, unsigned int size) {
    struct Student *sortedStudents = studentArray;
    for (unsigned int i = 0; i < size; i++) {
        for (unsigned int k = i + 1; k < size; k++) {
           if (sortedStudents[k].totalPoints > sortedStudents[i].totalPoints) {
               struct Student lowerPointsStudent = sortedStudents[i];
               sortedStudents[i] = sortedStudents[k];
               sortedStudents[k] = lowerPointsStudent;
           }
        }
    }
    for (unsigned int i = 0; i < size; i++) {
        struct Student student = sortedStudents[i];
        printf("%s %s %s", student.ID, student.lastName, student.firstName);
        for (unsigned int k = 0; k < 6; k++) {
            printf(" %d", student.points[k]);
        }
        printf(" %d\n", student.totalPoints);
    }
}

/*
   A function to save the database's results into a text file.
   The format is the same as the printStudents function's.
*/
int saveResults(struct Student *studentArray, unsigned int size, const char *filename) {
    FILE *f = fopen(filename, "w");
	if (f == NULL) {
		return 0;
    }
    for (unsigned int i = 0; i < size; i++) {
		fputs(studentArray[i].ID, f);
		fprintf(f, " %s", studentArray[i].lastName);
        fprintf(f, " %s", studentArray[i].firstName);
        for (unsigned int k = 0; k < 6; k++) {
            fprintf(f, " %d", studentArray[i].points[k]);
        }
        fprintf(f, " %d", studentArray[i].totalPoints);
		fputs("\n", f);
	}
	fclose(f);
	return 1;
}

/*
   A function to free the database's memory.
*/
void freeMemory(struct Student *studentArray, unsigned int size) {
    for(unsigned int i = 0; i < size; i++) {
        free(studentArray[i].firstName);
        free(studentArray[i].lastName);
    }
    free(studentArray);
}

/*
   A function to download results to the database from a text file.
   The format is the same as the printStudents function's.
*/
struct Student *downloadResults(struct Student *currentStudentArray, unsigned int currentSize, const char *filename, unsigned int *arraySize) {
    FILE *f = fopen(filename, "r");
	struct Student *studentArray = NULL;
	if (f == NULL) {
		free(studentArray);
		return NULL;
	}
    freeMemory(currentStudentArray, currentSize);
	char id[7];
    char firstName[101];
    char lastName[101];
    unsigned int i = 0;
    int res = fscanf(f, "%s %s %s", id, lastName, firstName);
	do {
		struct Student *newStudentArray = addStudent(studentArray, i, id, firstName, lastName);
        studentArray = newStudentArray;
        for (unsigned int k = 0; k < 6; k++) {
            fscanf(f, "%d", &studentArray[i].points[k]);
        }
        fscanf(f, "%d", &studentArray[i].totalPoints);
        i++;
        res = fscanf(f, "%s %s %s", id, lastName, firstName);
	} while (res != EOF);
	fclose(f);
    *arraySize = i;
	return studentArray;
}

/*
   A function that checks whether a student ID is already in the database.
   Returns 1 if the ID is in the database and 0 if it isn't.
*/
int checkForID(struct Student *studentArray, unsigned int size, const char *id) {
    for (unsigned int i = 0; i < size; i++) {
        if (strcmp(studentArray[i].ID, id) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    struct Student *students = NULL;
    unsigned int arraySize = 0;

    char line[1001];
    fgets(line, 1001, stdin);
    while (*line != 'Q') {
        char commandCode = *line;
        switch (commandCode) {
        //Add a student; student number, lastname and firstname
        case 'A': {
            char id[101];
            char firstName[101];
            char lastName[101];
            int res = sscanf(line, "%c %s %s %s", &commandCode, id, lastName, firstName);
            if (res != 4 || res == EOF) {
                printf("A should be followed by exactly 3 arguments.\n");
            } else if (strlen(id) > 6) {
                printf("Student number %s cannot must be more than 6 digits.\n", id);
            } else if (checkForID(students, arraySize, id)) {
                printf("Student %s is already in the database.\n", id);
            } else {
                struct Student *newStudentArray = addStudent(students, arraySize, id, firstName, lastName);
                if (newStudentArray == NULL) {
                    printf("Student couldn't be added\n");
                } else {
                    arraySize++;
                    students = newStudentArray;
                    printf("SUCCESS\n");
                }
            }
            break;
        }
        //Print points; student number, lastname, firstname, round points divided by spaces, total points
        //Example:
        /*
          1234 Korhonen Kalle 0 1 0 0 0 2 3
          42355 Nieminen Aleksi 0 0 7 0 5 0 12
          112345 Virtanen Riku 3 0 0 0 0 0 3
        */
        case 'L':
            printStudents(students, arraySize);
            printf("SUCCESS\n");
            break;
        //Update a student's points; student number, number of the round and the number of new points
        case 'U': {
            char id[101];
            int round, newPoints;
            int res = sscanf(line, "%c %s %d %d", &commandCode, id, &round, &newPoints);
            if (res != 4 || res == EOF) {
                printf("U should be followed by exactly 3 arguments.\n");
            } else if (newPoints < 0) {
                printf("Student cannot have negative points.\n");
            } else if (round < 1 || round > 6) {
                printf("Round cannot be less than 1 or larger than 6\n");
            } else if (!checkForID(students, arraySize, id)) {
                printf("Student %s is not in the database.\n", id);
            } else {
                if (updatePoints(students, arraySize, id, round, newPoints)) {
                    printf("SUCCESS\n");
                } else {
                    printf("Points couldn't be updated\n");
                }
            }
            break;
        }
        //Save results. Writes results to the file with the given filename
        case 'W': {
            char filename[101];
            int res = sscanf(line, "%c %s", &commandCode, filename);
            if (res != 2 || res == EOF) {
                printf("W should be followed by exactly 1 argument.\n");
            } else if (saveResults(students, arraySize, filename)) {
                printf("SUCCESS\n");
            } else {
                printf("Results couldn't be saved\n");
            }
            break;
        }
        //Load results. Loads results from the file with the given filename and replaces the existing results
        case 'O': {
            char filename[101];
            int res = sscanf(line, "%c %s", &commandCode, filename);
            if (res != 2 || res == EOF) {
                printf("O should be followed by exactly 1 argument.\n");
            } else {
                unsigned int newArraySize;
                struct Student *newStudentArray = downloadResults(students, arraySize, filename, &newArraySize);
                if (newStudentArray == NULL) {
                    printf("Cannot open file %s for reading.\n", filename);
                } else {
                    arraySize = newArraySize;
                    students = newStudentArray;
                    printf("SUCCESS\n");
                }
            }
            break;
        }
        default:
            printf("Invalid command %c\n", commandCode);
            break;
        }
        fgets(line, 1001, stdin);
    }
    freeMemory(students, arraySize);
    printf("SUCCESS\n");
    return 0;
}
