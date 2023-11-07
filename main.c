#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GRADES 100
#define MAX_SUBJECTS 100
// struct to store subject data
typedef struct
{
  char code[50];
  char subjectName[50];
  float cr;
  float grade;
} Subject;
// struct to store grade data
typedef struct
{
  char grade[50];
  Subject subjects[MAX_SUBJECTS];
  int numSubjects;
  float avgGrade;
} GradeData;
// struct to store subject group data
typedef struct
{
  char subjectGroup[3];
  float totalGrade;
  int count;
} SubjectGroup;

int main()
{
  // change file name here
  char filename[] = "grades.csv";
  FILE *file, *outputFile;
  char line[100];
  GradeData grades[MAX_GRADES];
  int numGrades = 0;
  float totalGradeSum = 0;
  int totalNumSubjects = 0;
  SubjectGroup subjectGroups[MAX_SUBJECTS];
  int numSubjectGroups = 0;
  int isFirstLine = 1;

  file = fopen(filename, "r");
  // in case file does not exist
  if (file == NULL)
  {
    perror("Error opening file");
    return -1;
  }
  // create output file
  outputFile = fopen("output.csv", "w");
  // in case file does not exist
  if (outputFile == NULL)
  {
    perror("Error creating output file");
    return -1;
  }
  // read file line by line
  while (fgets(line, sizeof(line), file))
  {
    // skip first line because it is the header
    if (isFirstLine)
    {
      isFirstLine = 0;
      continue;
    }
    // if line does not start with a comma, it is a new grade
    if (line[0] != ',')
    {
      sscanf(line, "%[^,],", grades[numGrades].grade); // read grade
      grades[numGrades].numSubjects = 0;               // reset number of subjects
      numGrades++;                                     // increment number of grades
    }
    else
    {
      // if line starts with a comma, it is a new subject
      Subject newSubject; // create new subject
      sscanf(line, ",%[^,],%[^,],%f,%f", newSubject.code,
             newSubject.subjectName, &newSubject.cr, &newSubject.grade); // read subject data
      strcpy(grades[numGrades - 1]
                 .subjects[grades[numGrades - 1].numSubjects]
                 .code,
             newSubject.code); // copy subject data to grade data
      strcpy(grades[numGrades - 1]
                 .subjects[grades[numGrades - 1].numSubjects]
                 .subjectName,
             newSubject.subjectName); // copy subject data to grade data
      grades[numGrades - 1].subjects[grades[numGrades - 1].numSubjects].cr =
          newSubject.cr; // copy subject data to grade data
      grades[numGrades - 1].subjects[grades[numGrades - 1].numSubjects].grade =
          newSubject.grade;                // copy subject data to grade data
      grades[numGrades - 1].numSubjects++; // increment number of subjects
      totalGradeSum += newSubject.grade;   // calculate total grade sum
      totalNumSubjects++;                  // calculate total number of subjects

      // Calculate subject group averages
      char subjectGroup[3];
      strncpy(subjectGroup, newSubject.code, 2); // get first 2 characters of subject code
      subjectGroup[2] = '\0';                    // add null terminator
      int found = 0;
      for (int i = 0; i < numSubjectGroups; i++) // check if subject group already exists
      {
        if (strcmp(subjectGroup, subjectGroups[i].subjectGroup) == 0) // if subject group exists
        {
          // add grade to total grade and increment count
          subjectGroups[i].totalGrade += newSubject.grade; // add grade to total grade
          subjectGroups[i].count++;                        // increment count
          found = 1;                                       // set found to true
          break;
        }
      }
      if (!found) // if subject group does not exist
      {
        strcpy(subjectGroups[numSubjectGroups].subjectGroup, subjectGroup); // copy subject group to subject group data
        subjectGroups[numSubjectGroups].totalGrade = newSubject.grade;      // set total grade to grade
        subjectGroups[numSubjectGroups].count = 1;                          // set count to 1
        numSubjectGroups++;                                                 // increment number of subject groups
      }
    }
  }
  // calculate average grade for each grade
  for (int i = 0; i < numGrades; i++) 
  {
    // calculate average grade
    float sum = 0;
    for (int j = 0; j < grades[i].numSubjects; j++)
    {
      sum += grades[i].subjects[j].grade; // add grade to sum
    }
    grades[i].avgGrade = sum / grades[i].numSubjects; // calculate average grade
  }

  // write to output file
  // write grade data
  fprintf(outputFile, "Grade,Average Grade\n"); // write header
  for (int i = 0; i < numGrades; i++) // write grade data
  {
    fprintf(outputFile, "%s,%.2f\n", grades[i].grade, grades[i].avgGrade);
  }
  // write overall average grade
  fprintf(outputFile, "Overall Average Grade,%.2f\n",
          totalGradeSum / totalNumSubjects);
  // write subject group averages
  fprintf(outputFile, "\nSubject Group,Average Grade\n");
  for (int i = 0; i < numSubjectGroups; i++) // write subject group data
  {
    fprintf(outputFile, "%s,%.2f\n", subjectGroups[i].subjectGroup,
            subjectGroups[i].totalGrade / subjectGroups[i].count);
  }
  
  fclose(file); // close input file
  fclose(outputFile); // close output file

  return 0;
}