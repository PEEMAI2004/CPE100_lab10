#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GRADES 100
#define MAX_SUBJECTS 100

typedef struct {
  char code[50];
  char subjectName[50];
  float cr;
  float grade;
} Subject;

typedef struct {
  char grade[50];
  Subject subjects[MAX_SUBJECTS];
  int numSubjects;
  float avgGrade;
} GradeData;

typedef struct {
  char subjectGroup[3];
  float totalGrade;
  int count;
} SubjectGroup;

int main() {
  FILE *file, *outputFile;
  char line[100];
  GradeData grades[MAX_GRADES];
  int numGrades = 0;
  float totalGradeSum = 0;
  int totalNumSubjects = 0;
  SubjectGroup subjectGroups[MAX_SUBJECTS];
  int numSubjectGroups = 0;
  int isFirstLine = 1;

  file = fopen("grades.csv", "r");
  if (file == NULL) {
    perror("Error opening file");
    return -1;
  }

  outputFile = fopen("output.csv", "w");
  if (outputFile == NULL) {
    perror("Error creating output file");
    return -1;
  }

  while (fgets(line, sizeof(line), file)) {
    if (isFirstLine) {
      isFirstLine = 0;
      continue;
    }

    if (line[0] != ',') {
      sscanf(line, "%[^,],", grades[numGrades].grade);
      grades[numGrades].numSubjects = 0;
      numGrades++;
    } else {
      Subject newSubject;
      sscanf(line, ",%[^,],%[^,],%f,%f", newSubject.code,
             newSubject.subjectName, &newSubject.cr, &newSubject.grade);
      strcpy(grades[numGrades - 1]
                 .subjects[grades[numGrades - 1].numSubjects]
                 .code,
             newSubject.code);
      strcpy(grades[numGrades - 1]
                 .subjects[grades[numGrades - 1].numSubjects]
                 .subjectName,
             newSubject.subjectName);
      grades[numGrades - 1].subjects[grades[numGrades - 1].numSubjects].cr =
          newSubject.cr;
      grades[numGrades - 1].subjects[grades[numGrades - 1].numSubjects].grade =
          newSubject.grade;
      grades[numGrades - 1].numSubjects++;
      totalGradeSum += newSubject.grade;
      totalNumSubjects++;

      // Calculate subject group averages
      char subjectGroup[3];
      strncpy(subjectGroup, newSubject.code, 2);
      subjectGroup[2] = '\0';
      int found = 0;
      for (int i = 0; i < numSubjectGroups; i++) {
        if (strcmp(subjectGroup, subjectGroups[i].subjectGroup) == 0) {
          subjectGroups[i].totalGrade += newSubject.grade;
          subjectGroups[i].count++;
          found = 1;
          break;
        }
      }
      if (!found) {
        strcpy(subjectGroups[numSubjectGroups].subjectGroup, subjectGroup);
        subjectGroups[numSubjectGroups].totalGrade = newSubject.grade;
        subjectGroups[numSubjectGroups].count = 1;
        numSubjectGroups++;
      }
    }
  }

  for (int i = 0; i < numGrades; i++) {
    float sum = 0;
    for (int j = 0; j < grades[i].numSubjects; j++) {
      sum += grades[i].subjects[j].grade;
    }
    grades[i].avgGrade = sum / grades[i].numSubjects;
  }

  fprintf(outputFile, "Grade,Average Grade\n");
  for (int i = 0; i < numGrades; i++) {
    fprintf(outputFile, "%s,%.2f\n", grades[i].grade, grades[i].avgGrade);
  }

  fprintf(outputFile, "Overall Average Grade,%.2f\n",
          totalGradeSum / totalNumSubjects);

  fprintf(outputFile, "\nSubject Group,Average Grade\n");
  for (int i = 0; i < numSubjectGroups; i++) {
    fprintf(outputFile, "%s,%.2f\n", subjectGroups[i].subjectGroup,
            subjectGroups[i].totalGrade / subjectGroups[i].count);
  }

  fclose(file);
  fclose(outputFile);

  return 0;
}
