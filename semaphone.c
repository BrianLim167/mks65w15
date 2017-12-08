#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define KEY 9880
#define KEY2 3880

union semun{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};


int main(int argc, char **argv){
  if ( argc > 1 ) {

    if ( !strcmp(argv[1], "-c") ) {
      if (argc != 3) {
        printf("invalid args\n");
        return 1;
      }
      int semaphore = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
      int shmd = shmget(KEY2, 256, IPC_CREAT | IPC_EXCL | 0644);
      char * mem;
      if (shmd == -1){printf("Shared mem segment already exists.\n");}
      else{mem = shmat(shmd, 0, 0);}
      if ( semaphore == -1 ) {
        printf("semaphore already exists\n");
      } else {
        int semval;
        sscanf(argv[2], "%d", &semval);
	union semun su;
	su.val = semval;
        semctl(semaphore, 0, SETVAL, su);
        printf("semaphore %d created with value %d\n", semaphore, semval);
	printf("Shared memory segment created %s\n", mem);
      }

      return 0;
    }

    if ( !strcmp(argv[1], "-v") ) {
      if (argc != 2) {
        printf("invalid args\n");
        return 1;
      }
      int semaphore = semget(KEY, 0, 0);
      if ( semaphore == -1 ) {
        printf("semaphore cannot be found\n");
      } else {
        int semval = semctl(semaphore, 0, GETVAL);
        printf("semaphore %d has value %d\n", semaphore, semval);
      }

      return 0;
    }

    if ( !strcmp(argv[1], "-r") ) {
      if (argc != 2) {
        printf("invalid args\n");
        return 1;
      }
      int semaphore = semget(KEY, 0, 0);
      if ( semaphore == -1 ) {
        printf("semaphore cannot be found\n");
      } else {
        int semval = semctl(semaphore, 0, IPC_RMID);
        printf("semaphore %d removed\n", semaphore);
      }

      return 0;
    }

  }
  printf("usage: controller [flag] [n]\n");
  printf("\t-c N\n\t\tCreate a semaphore and set its value to N\n\t-v\n\t\tView the current value of the semaphore\n\t-r\n\t\tremove the semaphore\n");
  return 0;
}
