
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum number of schools, layers, and classes
#define MAX_LAYERS 12
#define MAX_CLASSES 10
#define PATH_OF_DATA "/Users/alex/Documents/ios_projects/check_point_project/check_point_project/students-with-class.txt"
#define PATH_OF_OUTPUT_DATA "/Users/alex/Documents/ios_projects/check_point_project/check_point_project/students_data.txt"
#define NAME_BAFER 50
#define PHONE_NAMBER_BAFER 20
#define NUM_OF_GRADES 10

enum menu_inputs {
    Insert = '0',
    Delete = '1',
    Edit = '2',
    Search = '3',
    Showall = '4',
    Top10 = '5',
    UnderperformedStudents = '6',
    Average = '7',
    Export = '8',
    Exit = '9'
};

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

struct TopTenStudents {
    struct Student* topTenStudentsOfLayers[12][10];
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
        classPtr->students = student;
        classPtr->students->next = current;
    }
    classPtr->studentCount++;
}


// Function to add a new student to the school
void insertStudent(struct School* school, struct Student* student) {
    // Add the student to the corresponding class in the layer of the school
    struct Class* currentClass = &school->layers[student->gradeNumber - 1].classes[student->classNumber - 1];
    addStudentToClass(currentClass, student);
}


struct Student* findStudentByName(const char* firstName, const char* lastName, struct School* school) {
    for (int layer = 0; layer < MAX_LAYERS; layer++) {
        for (int class = 0; class < MAX_CLASSES; class++) {
            struct Student* current = school->layers[layer].classes[class].students;
            while (current != NULL) {
                if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0)
                    return current;
                current = current->next;
            }
        }
    }
    return NULL;
}

void printStudent(struct Student* student) {
    printf("Layer: %d, Class: %d\n", student->gradeNumber, student->classNumber);
    printf("Student Name: %s %s\n", student->firstName, student->lastName);
    printf("Phone Number: %s\n", student->phoneNumber);
    printf("Grades: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", student->grades[i]);
    }
    printf("\n\n");
}

// Function to perform Bubble Sort on an array of Students
void bubbleSort(struct TopTenStudents* topTenStudent, int column, int index) {
    struct Student* temp;
    int swapped;
    int size = 10;

    for (int i = 0; i < size - 1; i++) {
        swapped = 0;

        // Compare adjacent elements and swap them if they are in the wrong order
        for (int j = 0; j < size - i - 1; j++) {
            if (topTenStudent->topTenStudentsOfLayers[column][j]->grades[index] > topTenStudent->topTenStudentsOfLayers[column][j + 1]->grades[index]) {
                // Swap elements
                temp = topTenStudent->topTenStudentsOfLayers[column][j];
                topTenStudent->topTenStudentsOfLayers[column][j] = topTenStudent->topTenStudentsOfLayers[column][j + 1];
                topTenStudent->topTenStudentsOfLayers[column][j + 1] = temp;
                swapped = 1;
            }
        }

        // If no two elements were swapped in the inner loop, the array is already sorted
        if (swapped == 0) {
            break;
        }
    }
}


void compareAndSwappStudentsbyGrade(struct TopTenStudents* topTenStudent, struct Student* student, int column, int index) {
    if (topTenStudent->topTenStudentsOfLayers[column][0]->grades[index] < student->grades[index]) {
        topTenStudent->topTenStudentsOfLayers[column][0] = student;
        bubbleSort(topTenStudent, column, index);
    }
}


// Function to find and sort the top ten students for each layer based on a specific grade
void tenBestStudentsOfLayers(struct School* school) {
    printf("\n\tPlease Enter a gradeIndex (0-9): ");
    int gradeIndex;
    if (scanf("%d", &gradeIndex) == 1) {
        struct TopTenStudents topTenStudent;
    
    
        // Step 1: Find the first ten students for each layer based on their grades
        for (int layer = 0; layer < MAX_LAYERS; layer++) {
            int count = 0;
            for (int class = 0; class < MAX_CLASSES; class++) {
                struct Student* current = school->layers[layer].classes[class].students;
                while (current != NULL && count < 10) {
                    topTenStudent.topTenStudentsOfLayers[layer][count] = current;
                    count++;
                    current = current->next;
                }
                if (count == 10) {
                    // Sort the top ten students for the current layer based on the grade
                    bubbleSort(&topTenStudent, layer, gradeIndex);
                    break;
                }
            }
        }
        
        // Step 2: Compare and swap students within the top ten students for each layer
        for (int layer = 0; layer < MAX_LAYERS; layer++) {
            for (int student = 0; student < 10; student++) {
                struct Student* current = topTenStudent.topTenStudentsOfLayers[layer][student];
                compareAndSwappStudentsbyGrade(&topTenStudent, current, layer, gradeIndex);
            }
        }
        
        // Step 3: Print the top ten students for each layer
        for (int layer = 0; layer < MAX_LAYERS; layer++) {
            for (int student = 0; student < 10; student++) {
                struct Student* current = topTenStudent.topTenStudentsOfLayers[layer][student];
                printStudent(current);
            }
        }
    }
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
                printStudent(current);
                current = current->next;
                count++;
            }
        }
    }
    printf("Count of students: %d\n", count);
}

// Function to check if a string contains any numeric digits
int containsNumbers(const char* str) {
    while (*str != '\0') {
        if ((*str < 'a' || *str > 'z') && (*str < 'A' || *str > 'Z')) {
            return 1; // Contains numbers
        }
        str++;
    }
    return 0; // Does not contain numbers
}

int isValidPhoneNumber(const char* phoneNum) {
    // Check if the string is NULL or empty
    if (phoneNum == NULL || phoneNum[0] == '\0') {
        return 0; // Not valid
    }

    // Check if the string has exactly 10 characters
    if (strlen(phoneNum) != 10) {
        return 0; // Not valid
    }

    // Check if the string starts with '0'
    if (phoneNum[0] != '0') {
        return 0; // Not valid
    }

    // Check if all characters are numeric
    for (int i = 1; i < 10; i++) {
        if (phoneNum[i] < '0' || phoneNum[i] > '9') {
            return 0; // Not valid
        }
    }

    // If all checks pass, the phone number is valid
    return 1;
}


/*
 * The function gets input from the user and ensures the input
 * doesn't exceed max_size. if yes - it will shrink it and will add \0.
 */
size_t get_input(char* dst, size_t max_size) {
    char* input = NULL;
    size_t len = 0;
    size_t len_size = 0;
    len_size = getline(&input, &len, stdin);
    if (len_size == -1)
        return -1;
    if (len_size < max_size) {
        input[len_size - 1] = '\0';
        strncpy(dst, input, len_size);
    }
    else {
        input[max_size - 1] = '\0';
        strncpy(dst, input, max_size);
        len_size = max_size;
    }
    free(input);
    return len_size;
}


void inputFirstName(char firstName[NAME_BAFER]) {

    printf("Enter the first name of the student: ");
    if (get_input(firstName, NAME_BAFER) != -1) {
        // Check if the first name contains numbers and prompt the user again if it does
        while (containsNumbers(firstName)) {
            printf("Invalid input. First name cannot contain numbers. Enter the first name again: ");
            if (get_input(firstName, NAME_BAFER) == -1)
                return;
        }
    }
}

void inputLastName(char lastName[50]) {

    printf("Enter the last name of the student: ");
    if (get_input(lastName, 50) != -1) {
        // Check if the first name contains numbers and prompt the user again if it does
        while (containsNumbers(lastName)) {
            printf("Invalid input. First name cannot contain numbers. Enter the first name again: ");
            if (get_input(lastName, 50) == -1)
                return;
        }
    }
}

void inputPhoneNamber(char phoneNum[50]) {

    printf("Enter the phone number of the student begin in 0 and length 10:\n");
    fflush(stdin);
    size_t size = get_input(phoneNum, 50);
    
    if (size != -1) {
        // Check if phone number is legal
        while (!isValidPhoneNumber(phoneNum)) {
            printf("Invalid input. Enter the phone number again: ");
            if (get_input(phoneNum, 50) == -1)
                return;
        }
    }
}

int inputLayer(){
    int layer;
    printf("\n\tPlease Enter a Layer (1-12): ");
    scanf("%d", &layer);
    
    // Check if layer is legal
    while (layer < 1 || layer > 12) {
        printf("Invalid input. Enter the layer again: ");
        scanf("%d", &layer);
    }

    return layer;
}

int inputClass(){
    int class;
    printf("\n\tPlease Enter a class (1-10): ");
    scanf("%d", &class);
    
    // Check if layer is legal
    while (class < 1 || class > 10) {
        printf("Invalid input. Enter the class again: ");
        scanf("%d", &class);
    }

    return class;
}


void inputGrades(int grades[10]) {
    int grade;

    for (int i = 0; i < 10; i++) {
        printf("Enter the %d grade (50 - 100) of the student: ", i);
        scanf("%d", &grade);
        
        // Check if grede is legal
        while (grade < 50 || grade > 100) {
            printf("Invalid input. Enter the grade again (50 - 10): ");
            scanf("%d", &grade);
        }
        grades[i] = grade;
    }
}

// Function to edit the grade of a specific course for a student
void editGrage(struct School* school) {
    int course;
    int grade;
    char firstName[50];
    char lastName[50];
    
    fflush(stdin);
    inputFirstName(firstName);
    fflush(stdin);
    inputLastName(lastName);
    
    // Find the student by their first name and last name
    struct Student* student = findStudentByName(firstName, lastName, school);
    
    if (student == NULL) {
        printf("The student not found");
        return;
    }
    
    printf("Enter the course (1 - 10)");
    if (scanf("%d", &course) == 1) {
        if (course < 1 || course >10) {
            printf("Not legal course");
            return;
        }
    }
    
    printf("Enter the grade (50 - 100)");
    if (scanf("%d", &grade) == 1) {
        if (grade < 50 || grade >100) {
            printf("Not legal grade");
            return;
        }
    }
    
    // Update the grade of the student for the specified course
    student->grades[course - 1] =grade;
}


int averageGradesOfStudent(struct Student* student) {
    int sum = 0;
    
    for (int i = 0; i < 10; i++) {
        sum += student->grades[i];
    }
    
    return sum / 10;
}


void averageGradesOfCourse(struct School* school) {
    int course;
    int layer = inputLayer();
    
    printf("\n\tPlease Enter a Course (1-10): ");
    if (scanf("%d", &course) == 1) {
        if (course < 1 || course > 10) {
            printf("\n\t A wrong course (1-10): ");
            return;
        }
    }
    
    int count = 0;
    long sum = 0;
    
    for (int class = 0; class < MAX_CLASSES; class++) {
        struct Student* current = school->layers[layer - 1].classes[class].students;
        while (current != NULL) {
            sum += current->grades[course];
            count++;
            current = current->next;
        }
    }
    long average = sum / count;
    
    printf("An average of layer %d of course %d is %ld: ", layer, course, average);
}


void underperformedSstudents(struct School* school) {
    for (int layer = 0; layer < MAX_LAYERS; layer++) {
        for (int class = 0; class < MAX_CLASSES; class++) {
            struct Student* current = school->layers[layer].classes[class].students;
            while (current != NULL) {
                if (averageGradesOfStudent(current) < 60) {
                    printStudent(current);
                }
                current = current->next;
            }
        }
    }
}


// Function to print a student's data to a file
void printStudentToFile(FILE* file, struct Student* student) {

    fprintf(file, "Layer: %d, Class: %d\n", student->gradeNumber, student->classNumber);
    fprintf(file, "Student Name: %s %s\n", student->firstName, student->lastName);
    fprintf(file, "Phone Number: %s\n", student->phoneNumber);
    fprintf(file, "Grades: ");
    for (int i = 0; i < 10; i++) {
        fprintf(file, "%d ", student->grades[i]);
    }
    fprintf(file, "\n\n");
}

// Function to write all students' data to a new file
void writeAllStudentsToFile(struct School* school, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file!");
        return;
    }

    int count = 0;
    for (int layer = 0; layer < MAX_LAYERS; layer++) {
        for (int class = 0; class < MAX_CLASSES; class++) {
            struct Student* current = school->layers[layer].classes[class].students;
            while (current != NULL) {
                printStudentToFile(file, current);
                current = current->next;
                count++;
            }
        }
    }

    fclose(file);
    printf("Data written to %s. Count of students: %d\n", filename, count);
}

void dataOfStudent(struct School* school) {
    // Prompt the user for first name and last name of the student to delete
    char firstName[NAME_BAFER];
    char lastName[NAME_BAFER];

    fflush(stdin);
    inputFirstName(firstName);
    fflush(stdin);
    inputLastName(lastName);
    
    struct Student* student = findStudentByName(firstName, lastName, school);
    if (student != NULL) {
        printStudent(student);
    }
    else {
        printf("A student not found");
    }
}

// Function to create a new student node
struct Student* createStudent() {
    char firstName[NAME_BAFER];
    char lastName[NAME_BAFER];
    char phoneNumber[NAME_BAFER];
    int grades[NUM_OF_GRADES];
    int layer;
    int class;
    
    fflush(stdin);
    inputFirstName(firstName);
    fflush(stdin);
    inputLastName(lastName);
    fflush(stdin);
    inputPhoneNamber(phoneNumber);
    layer = inputLayer();
    class = inputClass();
    inputGrades(grades);
    
    return createStudentNode(firstName, lastName, phoneNumber, layer, class, grades);
}

// Function to delete a student from the school's data structure by their first name and last name
void deleteStudentByName(struct School* school) {
    // Prompt the user for first name and last name of the student to delete
    char firstName[NAME_BAFER];
    char lastName[NAME_BAFER];
    
    fflush(stdin);
    inputFirstName(firstName);
    fflush(stdin);
    inputLastName(lastName);
    
    struct Student* temp; // Temporary pointer to hold the node to be deleted (if found)

    // Loop through all layers in the school
    for (int layer = 0; layer < MAX_LAYERS; layer++) {
        // Loop through all classes in the current layer
        for (int class = 0; class < MAX_CLASSES; class++) {
            struct Student* current = school->layers[layer].classes[class].students;

            // Check if the student to be deleted is the first student in the class
            if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0) {
                // Update the head of the student list for the class
                school->layers[layer].classes[class].students = current->next;
                // Free the memory of the student node
                free(current);
                return; // Exit the function after deletion
            }

            // If the student is not the first in the class, search through the class's students
            while (current->next != NULL) {
                // Check if the next student in the class matches the first and last name
                if (strcmp(current->next->firstName, firstName) == 0 && strcmp(current->next->lastName, lastName) == 0) {
                    // Store the pointer to the node to be deleted
                    temp = current->next;
                    // Update the link to skip the student node to be deleted
                    current->next = temp->next;
                    // Free the memory of the student node
                    free(temp);
                    return; // Exit the function after deletion
                }
                // Move to the next student node in the class
                current = current->next;
            }
        }
    }
}


void menu(struct School* school) {
    char input;
    // school.name = "schoolName";
    do {
        printf("\n|School Manager<::>Home|\n");
        printf("--------------------------------------------------------------------------------\n");
        //printf("Welcome to ( %s ) School!\nYou have inserted ( %zu ) students.\n\n", school.name, school.numOfStudents);
        printf("\t[0] |--> Insert\n");//
        printf("\t[1] |--> Delete\n");//
        printf("\t[2] |--> Edit\n");//
        printf("\t[3] |--> Search\n");//
        printf("\t[4] |--> Show All\n");//
        printf("\t[5] |--> Top 10 students per course\n");//
        printf("\t[6] |--> Underperformed students\n");//
        printf("\t[7] |--> Average per course\n");
        printf("\t[8] |--> Export\n");
        printf("\t[9] |--> Exit\n");
        printf("\n\tPlease Enter Your Choice (0-9): ");
        fflush(stdin);
        input = getc(stdin);
        //fflush(stdin);
        //getc(stdin);
        switch (input) {
            case Insert:{
                struct Student* newStudent = createStudent();
                insertStudent(school, newStudent);
                break;
            }
            case Delete:{
                deleteStudentByName(school);
                break;
            }
            case Edit:
                editGrage(school);
                break;
            case Search:{
                dataOfStudent(school);
                break;
            }
            case Showall:
                printAllStudents(school);
                break;
            case Top10:{
                tenBestStudentsOfLayers(school);
                break;
                }
            case UnderperformedStudents:
                underperformedSstudents(school);
                break;
            case Average:{
                averageGradesOfCourse(school);
                break;
            }
            case Export:{
                writeAllStudentsToFile(school, PATH_OF_OUTPUT_DATA);
                break;
            }
            case Exit:
                break;
            default:
                printf("\nThere is no item with symbol \"%c\".Please enter a number between 1-10!\nPress any key to continue...",
                       input);
                getc(stdin);
                getc(stdin);
                break;
        }
    } while (input != Exit);
}


int readDataFromFile(struct School* school, char* path) {
   
    // Open the file for reading
    FILE* file = fopen(PATH_OF_DATA, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read data from the file and populate the data structures
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char firstName[NAME_BAFER];
        char lastName[NAME_BAFER];
        char phoneNumber[PHONE_NAMBER_BAFER];
        int gradeNumber;
        int classNumber;
        int grades[NUM_OF_GRADES];

        sscanf(buffer, "%s %s %s %d %d", firstName, lastName, phoneNumber, &gradeNumber, &classNumber);
        
        char* token = strtok(buffer, " "); // Get the first token (grade)
        for (int i = 0; i < 5; i++) {
            token = strtok(NULL, " "); // Move to the next token
        }
        
        // Parse and fill the grades array using a loop
        for (int i = 0; i < NUM_OF_GRADES; i++) {
            int grade;
            int numItems = sscanf(token, "%d", &grade);

            if (numItems == 1) {
                grades[i] = grade;
            } else {
                // If sscanf couldn't read a grade, break out of the loop
                break;
            }

            token = strtok(NULL, " "); // Move to the next token (next grade)
        }

        // Create a new student node
        struct Student* newStudent = createStudentNode(firstName, lastName, phoneNumber, gradeNumber, classNumber, grades);

        // Add the student to the corresponding class in the layer of the school
        struct Class* currentClass = &school->layers[gradeNumber - 1].classes[classNumber - 1];
        addStudentToClass(currentClass, newStudent);
    }

    // Close the file
    fclose(file);
    return 0;
}

// Free memory occupied by student nodes
void freeStudentNodesMemory(struct School* school) {
    
    for (int i = 0; i < MAX_LAYERS; i++) {
        for (int j = 0; j < MAX_CLASSES; j++) {
            freeStudents(school->layers[i].classes[j].students);
        }
    }
}


int main() {
    // Initialize the data structures
    struct School school;
    memset(&school, 0, sizeof(struct School));

    readDataFromFile(&school, PATH_OF_DATA);
    
    menu(&school);

    freeStudentNodesMemory(&school);

    return 0;
}
