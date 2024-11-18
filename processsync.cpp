// -------------------------------
// Assignment: Sleeping Teaching Assignment
// Due date: 11/17/24
// Purpose: Use knowledge of pthread and process/thread synchronization in order to implement an application for a sleeping teaching assistant helping students.
// -------------------------------

#include <pthread.h>   //Create POSIX threads.
#include <time.h>      //Wait for a random time.
#include <unistd.h>    //Thread calls sleep for specified number of seconds.
#include <semaphore.h> //To create semaphores
#include <stdlib.h>
#include <stdio.h> //Input Output

pthread_t *Students; // N threads running as Students.
pthread_t TA;        // Separate Thread for TA.

int ChairsCount = 0;
int CurrentIndex = 0;
// Declaration of Semaphores and Mutex Lock. Semaphores used:
// A semaphore to signal and wait TA's sleep.
sem_t ta_sleep_semaphore;

// An array of 3 semaphores to signal and wait chair to wait for the TA.
sem_t chair_semaphores[3];

// A semaphore to signal and wait for TA's next student.
sem_t ta_ready_semaphore;

// Mutex Lock used:
// To lock and unlock variable chairs_count_mutex to increment and decrement its value. Hint: Use sem_t and pthread_mutex_t
pthread_mutex_t chairs_count_mutex;

// Declared Functions
void *TA_Activity(void *arg); // Updated the signature here to take 'void *arg'
void *Student_Activity(void *threadID);
void try_chairsit(int student_id);

void initialize_resources()
{
  if (pthread_mutex_init(&chairs_count_mutex, NULL) != 0)
  {
    perror("Initialization for chairs_count_mutex failed");
    exit(1);
  }

  if (sem_init(&ta_sleep_semaphore, 0, 0) != 0)
  {
    perror("Semaphore initialization for ta_sleep_semaphore failed");
    exit(1);
  }

  if (sem_init(&ta_ready_semaphore, 0, 0) != 0)
  {
    perror("Semaphore initialization for ta_ready_semaphore failed");
    exit(1);
  }

  for (int i = 0; i < 3; i++)
  {
    if (sem_init(&chair_semaphores[i], 0, 1) != 0)
    {
      perror("Semaphore initialization for chair_semaphores failed");
      exit(1);
    }
  }
}

int main(int argc, char *argv[])
{
  int number_of_students; // a variable taken from the user to create student threads. Default is 5 student threads.
  int id;
  srand(time(NULL));

  /*TODO*/
  // Initializing Mutex Lock and Semaphores Hint: use sem_init() and pthread_mutex_init().
  initialize_resources();

  if (argc < 2)
  {
    printf("Number of Students not specified. Using default (5) students.\n");
    number_of_students = 5;
  }
  else
  {
    printf("Number of Students specified. Creating %d threads.\n", number_of_students);
    number_of_students = atoi(argv[1]);
  }

  // Allocate memory for Students
  Students = (pthread_t *)malloc(sizeof(pthread_t) * number_of_students);

  // Allocate memory for Student IDs
  int *student_ids = (int *)malloc(sizeof(int) * number_of_students);

  // Creating one TA thread and N Student threads. Hint: Use pthread_create.
  // Error handling + creation for TA thread
  if (pthread_create(&TA, NULL, TA_Activity, NULL) != 0)
  {
    perror("TA thread creation failed");
    exit(1);
  }

  // Error handling + creation for Student threads
  for (id = 0; id < number_of_students; id++)
  {
    // Student ID initialization
    student_ids[id] = id;

    if (pthread_create(&Students[id], NULL, Student_Activity, (void *)&student_ids[id]) != 0)
    {
      perror("STUDENT thread creation failed");
      exit(1);
    }
  }

  // Waiting for TA thread and N Student threads. Hint: Use pthread_join.
  for (int id = 0; id < number_of_students; id++)
  {
    pthread_join(Students[id], NULL);
  }

  pthread_join(TA, NULL);

  // Free allocated memory
  free(Students);
  free(student_ids);
  return 0;
}

void *TA_Activity(void *arg)
{

  // Lock
  pthread_mutex_lock(&chairs_count_mutex);

  // TA is sleeping
  printf("TA is sleeping");

  // If chairs are empty, break the loop
  if (ChairsCount == 0)
  {
    printf("Chairs are empty. Breaking loop.\n");
    pthread_mutex_unlock(&chairs_count_mutex);
    return NULL;
  }
  // Unlock
  pthread_mutex_unlock(&chairs_count_mutex);

  while (1)
  {
    sem_wait(&ta_sleep_semaphore);
    printf("TA is sleeping. \n");

    // Lock
    pthread_mutex_lock(&chairs_count_mutex);

    if (ChairsCount > 0)
    {
      sem_wait(&chair_semaphores[CurrentIndex]);
      CurrentIndex = (CurrentIndex + 1) % 3;
      sem_post(&ta_ready_semaphore);
      ChairsCount--;
    }

    // Unlock
    pthread_mutex_unlock(&chairs_count_mutex);

    // TA is currently helping a student.
    printf("TA is helping a student");
  }

  return NULL;
}

void *Student_Activity(void *threadID)
{
  int student_id = *((int *)threadID);

  // Student needs help from the TA
  printf("Student %d needs help from the TA.\n", student_id);

  // Student tries to sit on a chair
  try_chairsit(student_id);

  // Wake up the TA
  sem_post(&ta_sleep_semaphore);

  // Lock
  pthread_mutex_lock(&chairs_count_mutex);

  CurrentIndex = (CurrentIndex + 1) % 3;

  // Unlock
  pthread_mutex_unlock(&chairs_count_mutex);

  // Student is getting help from the TA
  printf("Student %d is getting help from the TA. \n", student_id);

  sem_wait(&ta_ready_semaphore);

  // Student leaves his/her chair
  printf("Student %d is leaving their chair as they are done getting help from the TA. \n", student_id);

  // Student left the TA room
  sem_post(&chair_semaphores[CurrentIndex]);

  printf("Student %d left the TA room. \n", student_id);

  // If student didn't find any chair to sit on student will return at a later time. This is done through the function: try_chairsit();

  return NULL;
}

void try_chairsit(int student_id)
{
  if (sem_trywait(&chair_semaphores[CurrentIndex]) != 0)
  {
    // If no chair is available, student will wait for a random amount of time
    printf("Student %d will return later as no chairs are available.\n", student_id);
    sleep(rand() % 3 + 1);

    sem_wait(&chair_semaphores[CurrentIndex]);

    // Student successfully sits and gets help from the TA
    printf("Student %d is getting help from the TA.\n", student_id);

    // Wake up TA if the student is seated
    sem_post(&ta_sleep_semaphore);
  }
  else
  {
    printf("Student %d is not getting help from the TA yet, but is sitting on a chair. \n", student_id);

    sem_post(&ta_sleep_semaphore);
  }
}