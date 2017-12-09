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
  sem = semget(SEM_KEY, 1, 0);
  if (sem == -1){
    printf("semaphore does not exist.\n");
    exit(0);
  }
  printf("waiting for opening...\n");
  //semop(sem, sembuf, size_t nspos)
  struct sembuf sbuf;
  sbuf.sem_num = 0;
  sbuf.sem_op = -1;
  printf("%d\n",semctl(sem, 0, GETVAL));
  sleep(1);
  sbuf.sem_flg = IPC_NOWAIT;
  semop(sem, &sbuf, 1);
  printf("%d\n",semctl(sem, 0, GETVAL));
  printf("done waiting!\n");
  fd = open("story", O_RDWR | O_APPEND);
  
  return 0;
}
