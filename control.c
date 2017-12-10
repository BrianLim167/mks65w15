/*
 * Jen Yu and Brian Lim
 * Systems Pd 10
 * W#15: Semaphone!
 * 2017-12-09
 */
#include "head.h"

//prints out errors
void print_error(int val){
  if (val == -1){
    printf("Error occurred: %s\n", strerror(errno));
    exit(0);
  }
}

//retrieves story from story file
char * get_story(){
  int fd;
  struct stat sb;
  char * ret;
  fd = open("story", O_RDONLY);
  if (fd == -1){printf("Program has not been initialized.\n");exit(0);}
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
    //semaphore, shared memory, and story creation
    //if strcmp() == 0 
    if(strcmp(args[1], "-c") == 0){
      //create the semaphore
      sem = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
      if(sem == -1){printf("Program has already been initialized.\n");exit(0);}

      //use the union semun
      union semun su;
      su.val = 1;

      //set the value of the semaphore to 1
      int val = semctl(sem, 0, SETVAL, su);
      print_error(val);
      printf("Semaphore %d created with value %d.\n", sem, 1);

      //create a shared mem segment
      shm = shmget(SHM_KEY, 8, IPC_CREAT | 0644);
      if(shm == -1){printf("Shared memory segment already exists.\n");exit(0);}
      printf("Shared memory segment created.\n");

      //create a story file
      fd = open("story", O_CREAT | O_TRUNC, 0644);
      close(fd);
      printf("Story created.\n");
    }
    //Prints out entire story
    //Always free after mem alloc!!
    else if(strcmp(args[1], "-v") == 0){
      //retrieve the story
      char * me = get_story();
      printf("Story:\n%s\n", me);
      free(me);
    }
    //remove the semaphore, shared memory, and story file
    else if (strcmp(args[1], "-r") == 0){
      //code from client file, should only remove if semaphore is open
      //interact with the semaphore
      sem = semget(SEM_KEY, 1, 0);
      if (sem == -1){
	printf("Program has not been initialized.\n");
	exit(0);
      }
  
      //Attempts to take semaphore
      printf("Waiting for access...\n");
      //semop(sem, sembuf, size_t nspos)
      struct sembuf sbuf;
      sbuf.sem_num = 0;
      sbuf.sem_op = -1;
      //SEM_UNDO blocks when sem's value is 0 (in use)
      sbuf.sem_flg = SEM_UNDO;
      //printf("%d\n",semctl(sem, 0, GETVAL));
      //sleep(1);

      //takes the semaphore
      //only one user at a time
      semop(sem, &sbuf, 1);
      printf("Access granted.\n");
      
      //get the semaphore and remove it
      sem = semget(SEM_KEY, 0, 0);
      int val = semctl(sem, 0, IPC_RMID);
      print_error(val);
      printf("Removed semaphore: %d\n", val);

      //get the shared mem seg and remove it
      shm = shmget(SHM_KEY, 8, 0);
      val = shmctl(shm, IPC_RMID, 0);
      print_error(val);
      printf("Shared memory segment removed.\n");

      //print and remove story file
      char * me = get_story();
      printf("Story:\n%s\n", me);
      free(me);
      remove("story");
    }
    else{
      printf("Could not read input.\n");
    }
  }
  return 0;
}
