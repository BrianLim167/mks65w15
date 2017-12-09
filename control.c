#include "head.h"

void print_error(int val){
  if (val == -1){
    printf("Error occurred: %s\n", strerror(errno));
    exit(0);
  }
}

char * get_story(){
  int fd;
  struct stat sb;
  char * ret;
  fd = open("story", O_RDONLY);
  print_error(fd);
  stat("story", &sb);
  read(fd, ret = malloc(sb.st_size), sb.st_size);
  return ret;
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
    //prints out entire story
    else if(strcmp(args[1], "-v") == 0){
      printf("Story:%s\n", get_story());
    }
    //remove the semaphore, shared memory, and story file
    else if (strcmp(args[1], "-r") == 0){
      sem = semget(SEM_KEY, 0, 0);
      int val = semctl(sem, 0, IPC_RMID);
      print_error(val);
      printf("Removed semaphore: %d\n", val);
      shm = shmget(SHM_KEY, 256, 0);
      val = shmctl(shm, IPC_RMID, 0);
      print_error(val);
      printf("Shared memory segment removed.\n");
      printf("Story:%s\n", get_story());
      remove("story");
    }
    else{
      printf("Could not read input.\n");
    }
  }
  return 0;
}
