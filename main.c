
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum number of schools, layers, and classes
//#define MAX_SCHOOLS 100
#define MAX_LAYERS 12
#define MAX_CLASSES 10

// Structure to store student details
struct Student {
    char firstName[50];
    char lastName[50];
    char phoneNumber[20];
    int gradeNumber;
    int classNumber;
    int grades[10];
    struct Student* next;
};

// Structure to store class details
struct Class {
    struct Student* students;
    int studentCount;
};

// Structure to store layer details
struct Layer {
    struct Class classes[MAX_CLASSES];
};

// Structure to store school details
struct School {
    struct Layer layers[MAX_LAYERS];
};

// Function to create a new student node
struct Student* createStudentNode(const char* firstName, const char* lastName, const char* phoneNumber, int gradeNumber, int classNumber, int* grades) {
    struct Student* newStudent = (struct Student*)malloc(sizeof(struct Student));
    strcpy(newStudent->firstName, firstName);
    strcpy(newStudent->lastName, lastName);
    strcpy(newStudent->phoneNumber, phoneNumber);
    newStudent->gradeNumber = gradeNumber;
    newStudent->classNumber = classNumber;
    memcpy(newStudent->grades, grades, sizeof(int) * 10);
    newStudent->next = NULL;
    return newStudent;
}

// Function to add a student to the class using a linked list
void addStudentToClass(struct Class* classPtr, struct Student* student) {
    if (classPtr->students == NULL) {
        classPtr->students = student;
    } else {
        struct Student* current = classPtr->students;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = student;
    }
    classPtr->studentCount++;
}

// Function to free memory occupied by student nodes
void freeStudents(struct Student* student) {
    while (student != NULL) {
        struct Student* temp = student;
        student = student->next;
        free(temp);
    }
}


// Function to print all students in the school
void printAllStudents(struct School* school) {
    int count = 0;
    for (int layer = 0; layer < MAX_LAYERS; layer++) {
        for (int class = 0; class < MAX_CLASSES; class++) {
            struct Student* current = school->layers[layer].classes[class].students;
            while (current != NULL) {
                printf("Layer: %d, Class: %d\n", layer + 1, class + 1);
                printf("Student Name: %s %s\n", current->firstName, current->lastName);
                printf("Phone Number: %s\n", current->phoneNumber);
                printf("Grades: ");
                for (int i = 0; i < 10; i++) {
                    printf("%d ", current->grades[i]);
                }
                printf("\n\n");
                current = current->next;
                count++;
            }
        }
    }
    printf("Count of students: %d\n", count);
}


int main() {
    // Initialize the data structures
    struct School school;
    memset(&school, 0, sizeof(struct School));

    // Open the file for reading
    FILE* file = fopen("/Users/alex/Documents/ios_projects/check_point_project/check_point_project/students-with-class.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read data from the file and populate the data structures
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char firstName[50];
        char lastName[50];
        char phoneNumber[20];
        int gradeNumber;
        int classNumber;
        int grades[10];

        sscanf(buffer, "%s %s %s %d %d %d %d %d %d %d %d %d %d %d %d",
               firstName, lastName, phoneNumber,
               &gradeNumber, &classNumber,
               &grades[0], &grades[1], &grades[2], &grades[3], &grades[4],
               &grades[5], &grades[6], &grades[7], &grades[8], &grades[9]);

        // Create a new student node
        struct Student* newStudent = createStudentNode(firstName, lastName, phoneNumber, gradeNumber, classNumber, grades);

        // Add the student to the corresponding class in the layer of the school
        struct Class* currentClass = &school.layers[gradeNumber - 1].classes[classNumber - 1];
        addStudentToClass(currentClass, newStudent);
    }

    // Close the file
    fclose(file);

    // Example: Accessing a student's details
   /* int layer = 11; // Example layer
    int class = 8; // Example class
    int studentNum = 0; // Index of the student within the class

    if (layer <= MAX_LAYERS && class <= MAX_CLASSES) {
        struct Student* current = school.layers[layer - 1].classes[class - 1].students;
        int studentIndex = 0;
        while (current != NULL && studentIndex < studentNum) {
            current = current->next;
            studentIndex++;
        }

        if (current != NULL) {
            printf("Student Name: %s %s\n", current->firstName, current->lastName);
            printf("Phone Number: %s\n", current->phoneNumber);
            printf("Grades: ");
            for (int i = 0; i < 10; i++) {
                printf("%d ", current->grades[i]);
            }
            printf("\n");
        } else {
            printf("Student not found.\n");
        }
    } else {
        printf("Invalid grade, layer, or class number.\n");
    }*/
    
    // Print all students in the school
    printAllStudents(&school);

    // Free memory occupied by student nodes
    for (int i = 0; i < MAX_LAYERS; i++) {
        for (int j = 0; j < MAX_CLASSES; j++) {
            freeStudents(school.layers[i].classes[j].students);
        }
    }

    return 0;
}
