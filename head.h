#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <fcntl.h>

#define SEM_KEY 9880
#define SHM_KEY 3880

union semun{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};
