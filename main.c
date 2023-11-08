#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBJECTS 100
#define MAX_TERMS 10
#define MAX_LINE_LENGTH 100

struct Subject {
    char code[20];
    float credit;
    float grade;
};

struct Term {
    char name[50];
    struct Subject subjects[MAX_SUBJECTS];
    int num_subjects;
    float total_credits;
};

struct Term terms[MAX_TERMS];
int num_terms = 0;

void readGradesFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0') {
            continue;  // Skip empty lines
        }

        if (line[0] != ',') {
            // New term
            if (num_terms < MAX_TERMS) {
                sscanf(line, "%[^,],", terms[num_terms].name);
                terms[num_terms].num_subjects = 0;
                terms[num_terms].total_credits = 0.0;
                num_terms++;
            } else {
                printf("Too many terms, ignoring the rest.\n");
                break;
            }
        } else {
            // Subject data
            if (num_terms > 0) {
                if (terms[num_terms - 1].num_subjects < MAX_SUBJECTS) {
                    sscanf(line, ",%[^,],%*[^,],%f,%f", terms[num_terms - 1].subjects[terms[num_terms - 1].num_subjects].code,
                           &terms[num_terms - 1].subjects[terms[num_terms - 1].num_subjects].credit,
                           &terms[num_terms - 1].subjects[terms[num_terms - 1].num_subjects].grade);
                    terms[num_terms - 1].num_subjects++;
                    terms[num_terms - 1].total_credits += terms[num_terms - 1].subjects[terms[num_terms - 1].num_subjects - 1].credit;
                } else {
                    printf("Too many subjects in the current term, ignoring the rest.\n");
                }
            }
        }
    }

    fclose(file);
}

void printAllGrades() {
    for (int i = 0; i < num_terms; i++) {
        printf("%s\n", terms[i].name);
        for (int j = 0; j < terms[i].num_subjects; j++) {
            printf("Subject Code: %s, Credit: %.1f, Grade: %.1f\n",
                   terms[i].subjects[j].code, terms[i].subjects[j].credit, terms[i].subjects[j].grade);
        }
    }
}

void printAvgGradeOfEachTerm() {
    for (int i = 0; i < num_terms; i++) {
        float total_grade = 0.0;
        for (int j = 0; j < terms[i].num_subjects; j++) {
            total_grade += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
        }
        float avg_grade = total_grade / terms[i].total_credits;
        printf("Term: %s, Average Grade: %.2f\n", terms[i].name, avg_grade);
    }
}

void printOverallAvgGrade() {
    float overall_total_grade = 0.0;
    float overall_total_credits = 0.0;

    for (int i = 0; i < num_terms; i++) {
        for (int j = 0; j < terms[i].num_subjects; j++) {
            overall_total_grade += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
            overall_total_credits += terms[i].subjects[j].credit;
        }
    }

    float overall_avg_grade = overall_total_grade / overall_total_credits;
    printf("Overall Average Grade: %.2f\n", overall_avg_grade);
}
void writeOutputToCSV() {
    FILE *file = fopen("output.csv", "w");
    if (file == NULL) {
        printf("Failed to create the output file.\n");
        exit(1);
    }

    fprintf(file, "Average Grade of Each Term:\n");
    for (int i = 0; i < num_terms; i++) {
        float total_grade = 0.0;
        for (int j = 0; j < terms[i].num_subjects; j++) {
            total_grade += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
        }
        float avg_grade = total_grade / terms[i].total_credits;
        fprintf(file, "%s,%.2f\n", terms[i].name, avg_grade);
    }

    fprintf(file, "\nOverall Average Grade:\n");
    float overall_total_grade = 0.0;
    float overall_total_credits = 0.0;
    for (int i = 0; i < num_terms; i++) {
        for (int j = 0; j < terms[i].num_subjects; j++) {
            overall_total_grade += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
            overall_total_credits += terms[i].subjects[j].credit;
        }
    }
    float overall_avg_grade = overall_total_grade / overall_total_credits;
    fprintf(file, "%.2f\n", overall_avg_grade);

    fclose(file);
}


int main() {
    const char *filename = "grades.csv";
    readGradesFromFile(filename);

    writeOutputToCSV();

    return 0;
}