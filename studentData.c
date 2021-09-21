#include <stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct student

{
   int id;
   char *lname;
   float *scores;
   float std_avg;

} student;

typedef struct course

{
   char *course_name;
   int num_sections;
   student **sections;
   int *num_students;
    int *num_scores;

} course;



// Methods
course *read_courses(FILE *fp, int *num_courses);
student **read_sections(FILE *fp, int num_students[], int num_scores[], int num_sections);
void process_courses(course *courses, int num_courses);
void display_sections(student **sections, int num_students[], int num_scores[], int num_sections);
void release_sections(student **sections, int num_sections, int*num_students, int *num_scores);
void release_courses( course *courses, int num_courses);




//MaIn
int main(void)

{

//open file
FILE *fp = fopen("assignment1input.txt", "r");

course *courses;

int test_case;
fscanf(fp, "%d", &test_case);

//Loop through the test case

for(int i=0; i<test_case; i++)

{
    int num_courses;

    printf("Test case %d\n", i+1);

   courses = read_courses(fp, &num_courses);
    process_courses(courses, num_courses);
    release_courses(courses, num_courses);

   printf("\n");

}

// close file
fclose(fp);

return 0;

}


// Function to setup and load courses array

course *read_courses(FILE *fp, int *num_courses)

{

//read number of courses from file
fscanf(fp, "%d", num_courses);

//allocating memory to store courses
course *courses = (course*) calloc(*num_courses,sizeof(course));


//loop through the course
for(int i = 0; i< *num_courses; i++)

{
    //allocate memory for course name
    courses[i].course_name = (char*)malloc(sizeof(char)*50);

    //read course name and num_sections from file
    fscanf(fp, "%s",courses[i].course_name );
    fscanf(fp, "%d", &courses[i].num_sections);


    //allocating memory for num_students and num_scores array. Will be used to store number of students for each section of courses[c]
    courses[i].num_students = (int*) calloc(courses[i].num_sections,sizeof(int));
    courses[i].num_scores = (int*) calloc(courses[i].num_sections,sizeof(int));

   // allocating mem for **sections. Address will be returned by read_sections function
    courses[i].sections = read_sections(fp, courses[i].num_students, courses[i].num_scores, courses[i].num_sections);

}

return courses;

}

// Setting up Sections and loading student struct

student **read_sections(FILE *fp, int num_students[], int num_scores[], int num_sections)

{

//Alocating memory for an array of student*
student **sections = (student**) calloc(num_sections,sizeof(student *));



// loop through each section
for(int i = 0; i<num_sections; i++)

{
    //read in  num_student and num_cores from the file
    fscanf(fp, "%d %d", &num_students[i], &num_scores[i]);

    //allocate memory for an array of student structs
    sections[i] = (student*) calloc(num_students[i],sizeof(student));

    //loop through each student
    for(int j = 0; j<num_students[i]; j++)

    {
        //allocate memory fill in lname
      sections[i][j].lname = (char*)calloc(50,sizeof(char));
      fscanf(fp, "%d %s", &sections[i][j].id, sections[i][j].lname);


      //allocate memory for score

      sections[i][j].scores = (float *)calloc ( num_scores[i],sizeof(float));

   //Filling  in scores and getting the sum
      float sum = 0;

      for(int sc = 0; sc<num_scores[i]; sc++)

      {
        fscanf(fp, "%f", &sections[i][j].scores[sc]);
        sum += sections[i][j].scores[sc];

      }

    //Calculate and fill in the average
      sections[i][j].std_avg = sum/num_scores[i];

}

}



return sections;

}



void process_courses(course *courses, int num_courses)

{



for (int i = 0; i<num_courses; i++)

{

    printf("%s ", courses[i].course_name);

    display_sections(courses[i].sections, courses[i].num_students, courses[i].num_scores, courses[i].num_sections);

}

}

//print details for the sections of a course

void display_sections(student **sections, int num_students[], int num_scores[], int num_sections)

{

//Allocating memory to store average of each section
float *sect_avg = (float *) calloc(num_sections,sizeof(float));


int num_passed = 0;
float max_avg = 0;
int section_index = 0;
int student_index = 0;



for(int i = 0; i<num_sections; i++)

{

    float sum_section = 0;


    for(int j= 0; j<num_students[i]; j++)

    {

      //calculating num_passed

      if (sections[i][j].std_avg >=70 )
      {
        num_passed++;

      }

    //sum of all the averages
      sum_section += sections[i][j].std_avg;






      //tracking student with highest average
      if(sections[i][j].std_avg>max_avg)

      {

        max_avg = sections[i][j].std_avg;

        section_index = i;
        student_index = j;

      }

    }

    sect_avg[i] = sum_section/num_students[i];

}

printf("%d ", num_passed);

for(int i = 0; i<num_sections; i++)
{
 printf("%.2f ", sect_avg[i]);

}

printf("%d %s %.2f\n",sections[section_index][student_index].id,sections[section_index][student_index].lname,sections[section_index][student_index].std_avg);


free(sect_avg);

}



void release_courses( course *courses, int num_courses)

{

//loop though each course and free memory
for (int i = 0; i<num_courses; i++)

{
    free(courses[i].course_name);

//to free memory for all the sections
    release_sections(courses[i].sections, courses[i].num_sections, courses[i].num_students, courses[i].num_scores);

    free(courses[i].num_students);
    free(courses[i].num_scores);

}

free(courses);

}



void release_sections(student **sections, int num_sections, int *num_students, int *num_scores)

{

//loop through each section and free memory

for(int i = 0; i<num_sections; i++)

{

    //loop through each student and clear memory
    for(int j = 0; j<num_students[i]; j++)

    {
      free(sections[i][j].lname);
      free(sections[i][j].scores);
    }


    //free each section
    free(sections[i]);

}



free(sections);

}
