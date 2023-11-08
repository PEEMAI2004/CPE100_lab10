#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBJECTS 100
#define MAX_TERMS 10
#define MAX_LINE_LENGTH 100

struct Subject
{
  char code[20];
  float credit;
  float grade;
};

struct Term
{
  char name[50];
  struct Subject subjects[MAX_SUBJECTS];
  int num_subjects;
  float total_credits;
};

struct Term terms[MAX_TERMS];
int num_terms = 0;

void readGradesFromFile(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    printf("Failed to open the file.\n");
    exit(1);
  }

  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), file))
  {
    if (line[0] == '\n' || line[0] == '\0')
    {
      continue; // Skip empty lines
    }

    if (line[0] != ',')
    {
      // New term
      if (num_terms < MAX_TERMS)
      {
        sscanf(line, "%[^,],", terms[num_terms].name);
        terms[num_terms].num_subjects = 0;
        terms[num_terms].total_credits = 0.0;
        num_terms++;
      }
      else
      {
        printf("Too many terms, ignoring the rest.\n");
        break;
      }
    }
    else
    {
      // Subject data
      if (num_terms > 0)
      {
        if (terms[num_terms - 1].num_subjects < MAX_SUBJECTS)
        {
          sscanf(line, ",%[^,],%*[^,],%f,%f", terms[num_terms - 1].subjects[terms[num_terms - 1].num_subjects].code,
                 &terms[num_terms - 1].subjects[terms[num_terms - 1].num_subjects].credit,
                 &terms[num_terms - 1].subjects[terms[num_terms - 1].num_subjects].grade);
          terms[num_terms - 1].num_subjects++;
          terms[num_terms - 1].total_credits += terms[num_terms - 1].subjects[terms[num_terms - 1].num_subjects - 1].credit;
        }
        else
        {
          printf("Too many subjects in the current term, ignoring the rest.\n");
        }
      }
    }
  }

  fclose(file);
}

void printAllGrades()
{
  for (int i = 0; i < num_terms; i++)
  {
    printf("%s\n", terms[i].name);
    for (int j = 0; j < terms[i].num_subjects; j++)
    {
      printf("Subject Code: %s, Credit: %.1f, Grade: %.1f\n",
             terms[i].subjects[j].code, terms[i].subjects[j].credit, terms[i].subjects[j].grade);
    }
  }
}

void printAvgGradeOfEachTerm()
{
  for (int i = 0; i < num_terms; i++)
  {
    float total_grade = 0.0;
    for (int j = 0; j < terms[i].num_subjects; j++)
    {
      total_grade += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
    }
    float avg_grade = total_grade / terms[i].total_credits;
    printf("Term: %s, Average Grade: %.2f\n", terms[i].name, avg_grade);
  }
}

void printOverallAvgGrade()
{
  float overall_total_grade = 0.0;
  float overall_total_credits = 0.0;

  for (int i = 0; i < num_terms; i++)
  {
    for (int j = 0; j < terms[i].num_subjects; j++)
    {
      overall_total_grade += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
      overall_total_credits += terms[i].subjects[j].credit;
    }
  }

  float overall_avg_grade = overall_total_grade / overall_total_credits;
  printf("Overall Average Grade: %.2f\n", overall_avg_grade);
}
void printAvgGradeByGroup()
{
  char groups[MAX_SUBJECTS][3] = {0};
  float group_total_grade[MAX_SUBJECTS] = {0};
  float group_total_credits[MAX_SUBJECTS] = {0};
  int num_groups = 0;

  for (int i = 0; i < num_terms; i++)
  {
    for (int j = 0; j < terms[i].num_subjects; j++)
    {
      char group[3] = {terms[i].subjects[j].code[0], terms[i].subjects[j].code[1], '\0'};
      int found = 0;
      for (int k = 0; k < num_groups; k++)
      {
        if (strcmp(groups[k], group) == 0)
        {
          found = 1;
          group_total_grade[k] += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
          group_total_credits[k] += terms[i].subjects[j].credit;
          break;
        }
      }
      if (!found)
      {
        strcpy(groups[num_groups], group);
        group_total_grade[num_groups] = terms[i].subjects[j].grade * terms[i].subjects[j].credit;
        group_total_credits[num_groups] = terms[i].subjects[j].credit;
        num_groups++;
      }
    }
  }

  printf("\nAverage Grade by Subject Groups:\n");
  for (int i = 0; i < num_groups; i++)
  {
    float avg_grade = group_total_grade[i] / group_total_credits[i];
    printf("%s,%.2f\n", groups[i], avg_grade);
  }
}

void calculateAvgGradeByGroup()
{
  char groups[MAX_SUBJECTS][3] = {0};
  float group_total_grade[MAX_SUBJECTS] = {0};
  float group_total_credits[MAX_SUBJECTS] = {0};
  int num_groups = 0;

  for (int i = 0; i < num_terms; i++)
  {
    for (int j = 0; j < terms[i].num_subjects; j++)
    {
      char group[3] = {terms[i].subjects[j].code[0], terms[i].subjects[j].code[1], '\0'};
      int found = 0;
      for (int k = 0; k < num_groups; k++)
      {
        if (strcmp(groups[k], group) == 0)
        {
          found = 1;
          group_total_grade[k] += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
          group_total_credits[k] += terms[i].subjects[j].credit;
          break;
        }
      }
      if (!found)
      {
        strcpy(groups[num_groups], group);
        group_total_grade[num_groups] = terms[i].subjects[j].grade * terms[i].subjects[j].credit;
        group_total_credits[num_groups] = terms[i].subjects[j].credit;
        num_groups++;
      }
    }
  }

  FILE *file = fopen("output.csv", "a");
  if (file == NULL)
  {
    printf("Failed to open the output file for writing.\n");
    exit(1);
  }

  fprintf(file, "\nAverage Grade by Subject Groups:\n");
  for (int i = 0; i < num_groups; i++)
  {
    float avg_grade = group_total_grade[i] / group_total_credits[i];
    fprintf(file, "%s,%.2f\n", groups[i], avg_grade);
  }

  fclose(file);
}

void writeOutputToCSV() {
    // Open the output file for writing
    FILE *file = fopen("output.csv", "w");
    if (file == NULL) {
        printf("Failed to create the output file.\n");
        exit(1);
    }

    // Write the average grade of each term to the file
    fprintf(file, "Average Grade of Each Term:\n");
    for (int i = 0; i < num_terms; i++) {
        float total_grade = 0.0;
        for (int j = 0; j < terms[i].num_subjects; j++) {
            total_grade += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
        }
        float avg_grade = total_grade / terms[i].total_credits;
        fprintf(file, "%s,%.2f\n", terms[i].name, avg_grade);
    }

    // Calculate the overall average grade
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

    // Calculate the average grade by subject groups
    char groups[MAX_SUBJECTS][3] = {0};
    float group_total_grade[MAX_SUBJECTS] = {0};
    float group_total_credits[MAX_SUBJECTS] = {0};
    int num_groups = 0;

    for (int i = 0; i < num_terms; i++) {
        for (int j = 0; j < terms[i].num_subjects; j++) {
            char group[3] = {terms[i].subjects[j].code[0], terms[i].subjects[j].code[1], '\0'};
            int found = 0;
            for (int k = 0; k < num_groups; k++) {
                if (strcmp(groups[k], group) == 0) {
                    found = 1;
                    group_total_grade[k] += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
                    group_total_credits[k] += terms[i].subjects[j].credit;
                    break;
                }
            }
            if (!found) {
                strcpy(groups[num_groups], group);
                group_total_grade[num_groups] = terms[i].subjects[j].grade * terms[i].subjects[j].credit;
                group_total_credits[num_groups] = terms[i].subjects[j].credit;
                num_groups++;
            }
        }
    }

    // Write the average grade by subject groups to the file
    fprintf(file, "\nAverage Grade by Subject Groups:\n");
    for (int i = 0; i < num_groups; i++) {
        float avg_grade = group_total_grade[i] / group_total_credits[i];
        fprintf(file, "%s,%.2f\n", groups[i], avg_grade);
    }

    // Close the output file
    fclose(file);
}


int main()
{
  const char *filename = "grades.csv";
  readGradesFromFile(filename);

  printAvgGradeOfEachTerm();
  printOverallAvgGrade();
  printAvgGradeByGroup();

  writeOutputToCSV();
  calculateAvgGradeByGroup();
  return 0;
}