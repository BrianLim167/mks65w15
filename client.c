/*
 * Jen Yu and Brian Lim
 * Systems Pd 10
 * W#15: Semaphone!
 * 2017-12-09
 */

#include "head.h"

int main()
{
  int sem, shm, fd;
  //interact with the semaphore
  sem = semget(SEM_KEY, 1, 0);
  if (sem == -1){
    printf("semaphore does not exist.\n");
    exit(0);
  }
  //Attempts to access story document
  printf("Waiting for opening...\n");
  //semop(sem, sembuf, size_t nspos)
  struct sembuf sbuf;
  sbuf.sem_num = 0;
  sbuf.sem_op = -1;
  //SEM_UNDO blocks when sem's value is 0 (in use)
  sbuf.sem_flg = SEM_UNDO;
  //printf("%d\n",semctl(sem, 0, GETVAL));
  //sleep(1);

  //perform semaphore operations
  semop(sem, &sbuf, 1);
  printf("Accessed story!\n");

  //open the story file for read/writing
  fd = open("story", O_RDWR | O_APPEND);
  shm = shmget(SHM_KEY, 8, 0);
  
  //get strlen of last addition
  int *len = shmat(shm, 0, 0);
  
  //seek -strlen bytes off the end of file
  lseek(fd, -1* *len, SEEK_END);
  
  //get the last addition to the story
  char *latest = (char *)calloc(1, *len + 1);
  read(fd, latest, *len);
  printf("The latest update to the story:\n %s\n", latest);
  free(latest);
  
  //ask for an addition
  printf("Add to the story: \n");
  char new[2048];
  fgets(new, 2048, stdin);
  
  //update the shm with the length of last addition
  //specified in assignment
  *len = strlen(new);
  
  //write the input to the end of the story
  lseek(fd, 0, SEEK_END);
  write(fd, new, strlen(new));
  
  //detach the shm
  shmdt(len);
  
  //release the semaphore
  sbuf.sem_op = 1;
  semop(sem, &sbuf, 1);
  return 0;
}
