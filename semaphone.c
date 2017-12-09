#include "head.h"

void print_error(int val){
  if (val == -1){
    printf("Error occurred: %s\n", strerror(errno));
    exit(0);
  }
}

int main(int argct, char** args){
  int sem, shm;
  int fd;
  if(argct < 2){
    printf("Too few arguments.\n");
    exit(0);
  }
  else{
    //semaphore creation!
    //if strcmp() == 0 
    if(strcmp(args[1], "-c") == 0){
      sem = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
      print_error(sem);
      union semun su;
      su.val = 8;
      int val = semctl(sem, 0, SETVAL, su);
      print_error(val);
      printf("Semaphore %d created with value %d.\n", sem, 8);
      shm = shmget(SHM_KEY, 256, IPC_CREAT | 0644);
      print_error(shm);
      printf("Shared memory segment created.\n");
      fd = open("story", O_CREAT | O_APPEND, 0644);
      close(fd);
      print_error(fd);
      printf("Story created.\n");
    }
    //semaphore value procural!!
    //gets the semaphore val
    else if(strcmp(args[1], "-v") == 0){
      sem = semget(SEM_KEY, 0, 0);
      int val = semctl(sem, 0, GETVAL);
      if (val == -1){printf("Error occured: %s\n", strerror(errno));}
      else{printf("Value of semaphore: %d\n", val);}
    }
    //remove the semaphore :'(
    else if (strcmp(args[1], "-r") == 0){
      sem = semget(SEM_KEY, 0, 0);
      int val = semctl(sem, 0, IPC_RMID);
      if (val == -1){printf("Error occured: %s\n", strerror(errno));}
      else{printf("Removed semaphore: %d\n", val);}
    }
    else{
      printf("Could not read input.\n");
    }
  }
  return 0;
}
