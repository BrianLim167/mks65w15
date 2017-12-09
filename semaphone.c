#include 'head.h'

int main(int argct, char** args){
  int sem;
  if(argct < 2){
    printf("Too few arguments.\n");
    exit(0);
  }
  else{
    //semaphore creation!
    //if more than 3 arguments "./run -c <num" and strcmp() == 0 
    if((argct >= 3) && (strcmp(args[1], "-c") == 0)){
      sem = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
      //if already exists, errno
      if (sem == -1){
	printf("The semaphore already exists.\n");
      }
      //else take inputted # and set val
      else{
	int input;
	sscanf(args[2], "%d", &input);
	int det = semctl(sem, 0, SETVAL, input);
	if (det == -1){printf("Error occured: %s\n", strerror(errno));}
	else{printf("Semaphore %d created with value %d.\n", sem, input);}
      }
    }
    //semaphore value procural!!
    //gets the semaphore val
    else if(strcmp(args[1], "-v") == 0){
      sem = semget(KEY, 0, 0);
      int val = semctl(sem, 0, GETVAL);
      if (val == -1){printf("Error occured: %s\n", strerror(errno));}
      else{printf("Value of semaphore: %d\n", val);}
    }
    //remove the semaphore :'(
    else if (strcmp(args[1], "-r") == 0){
      sem = semget(KEY, 0, 0);
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
