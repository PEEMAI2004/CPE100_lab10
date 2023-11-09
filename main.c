#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SUBJECTS 100
#define MAX_TERMS 10
#define MAX_LINE_LENGTH 100

// Define structures for Subject and Term
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

// array of Term and variable to keep track of the number of terms
struct Term terms[MAX_TERMS];
int num_terms = 0;

// Function to read grades from a CSV file
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

// Function to divide numbers and round to 2 decimal places (round up, round down, or standard) base on flag
float divideAndRound(int flag,float dividend, float divisor)
{
  // flag = 0: round down, flag = 1: round up, flag = 2: standard
  float result = dividend / divisor;
  if (flag == 0)
  {
    result = floor(result * 100) / 100;
  }
  else if (flag == 1)
  {
    result = ceil(result * 100) / 100;
  }
  return result;
}


// Function to print all grades
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

// Function to print the average grade of each term
void printAvgGradeOfEachTerm()
{
  // Calculate the average grade for each term and print the result
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

// Function to print the overall average grade
void printOverallAvgGrade()
{
  // Calculate the overall average grade and print the result
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

// Function to print the average grade by subject groups
void printAvgGradeByGroup()
{
  // Calculate the average grade for each subject group and print the result
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

// Function to write output to a CSV file
void writeOutputToCSV(int flag)
{
  // Open the output file for writing
  FILE *file = fopen("output.csv", "w");
  if (file == NULL)
  {
    printf("Failed to create the output file.\n");
    exit(1);
  }

  // Write the average grade of each term to the file
  fprintf(file, "Average Grade of Each Term:\n");
  fprintf(file, "Term,Total Credits,avg grade\n"); // header
  for (int i = 0; i < num_terms; i++)
  {
    float total_grade = 0.0;
    for (int j = 0; j < terms[i].num_subjects; j++)
    {
      total_grade += terms[i].subjects[j].grade * terms[i].subjects[j].credit;
    }
    float avg_grade = divideAndRound(flag,total_grade, terms[i].total_credits);
    fprintf(file, "%s,%.1f,%.2f\n", terms[i].name, terms[i].total_credits, avg_grade);
  }

  // Calculate the overall average grade
  fprintf(file, "\nOverall Average Grade:");
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
  float overall_avg_grade = divideAndRound(flag,overall_total_grade, overall_total_credits);
  fprintf(file, ",%.2f\n", overall_avg_grade);
  // Write total credits
  fprintf(file, "Total Credits,%.1f\n", overall_total_credits);

  // Calculate the average grade by subject groups
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

  // Write the average grade by subject groups to the file
  fprintf(file, "\nAverage Grade by Subject Groups:\n");
  fprintf(file, "Subject Group,Total Credits,avg grade\n"); // header
  for (int i = 0; i < num_groups; i++)
  {
    float avg_grade = divideAndRound(flag,group_total_grade[i], group_total_credits[i]);
    fprintf(file, "%s,%.1f,%.2f\n", groups[i], group_total_credits[i], avg_grade);
  }

  // Close the output file
  fclose(file);
  printf("Done! Write Data to output.csv\n");
}

// Main function
int main()
{
  const char *filename = "grades.csv";
  readGradesFromFile(filename);

  // print data on terminal

  // printAllGrades();
  /* printAvgGradeOfEachTerm();
  printOverallAvgGrade();
  printAvgGradeByGroup();
  */
  // call function to write the file
  writeOutputToCSV(0);
  // flag = 0: round down, flag = 1: round up, flag = 2: standard
  return 0;
}
