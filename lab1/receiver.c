#include "receiver.h"
#include<stdlib.h>
#include<string.h>

#define SEM_MUTEX 0  /* 臨界區鎖，初值 1 */
#define SEM_EMPTY 1  /* 空位數，初值 1 (單槽) */
#define SEM_FULL  2  /* 可取數，初值 0 */

struct timespec start,end;
double time_taken=0;
int semid;

void P(int semid, unsigned short num) {
  struct sembuf op = { .sem_num = num, .sem_op = -1, .sem_flg = 0 };
  semop(semid, &op, 1) ;
}

void V(int semid, unsigned short num) {
  struct sembuf op = { .sem_num = num, .sem_op = +1, .sem_flg = 0 };
  semop(semid, &op, 1) ;
}

void receive(message_t* message_ptr, mailbox_t* mailbox_ptr){

  if(mailbox_ptr->flag==MSG_PASSING){

    clock_gettime(CLOCK_MONOTONIC, &start);//start timing
    msgrcv(mailbox_ptr->storage.msqid, message_ptr, sizeof(message_ptr->msgText), 0, 0);
    clock_gettime(CLOCK_MONOTONIC, &end);//end timing

    time_taken += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
  }else if(mailbox_ptr->flag==SHARED_MEM){

    clock_gettime(CLOCK_MONOTONIC, &start);//start timing
    strcpy(message_ptr->msgText, mailbox_ptr->storage.shm_addr );

    clock_gettime(CLOCK_MONOTONIC, &end);//end timing

    time_taken += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
  }else{
    printf("Flag out of range\n");
  } 
};

int main(int argc, char* argv[] ){
  int option = atoi(argv[1]  );

  key_t sem_key = ftok("semfile", 75);
  semid = semget(sem_key, 3, 0666);  // try to get existing set
  key_t key = ftok("mailbox", 65);
  mailbox_t* mailbox = (mailbox_t*) malloc(sizeof(mailbox_t));

  if (semid == -1) {
    printf("semid == -1\n");
    semid = semget(sem_key, 3, IPC_CREAT | 0666);  // create and init once
    unsigned short vals[3] = {1, 1, 0}; // mutex, empty, full
    union semun arg;
    arg.array = vals;
    semctl(semid, 0, SETALL, arg);
  }

  if(option==1){

    mailbox -> storage.msqid = msgget(key, 0666 | IPC_CREAT);

  }else{

    mailbox -> storage.msqid = shmget(key, sizeof(message_t), 0666 | IPC_CREAT);
    mailbox->storage.shm_addr = shmat(mailbox->storage.msqid, NULL, 0); 

  } 

  mailbox -> flag = option;
  message_t msg;

  while(1){
    P(semid, SEM_FULL);   // Wait for message available
    P(semid, SEM_MUTEX);  // Acquire mutex

    receive(&msg, mailbox);

    V(semid, SEM_MUTEX);  // Release mutex
    V(semid, SEM_EMPTY);  // Signal empty slot available

    if(strcmp(msg.msgText, "EXIT")==0){
      printf("\nSender exit!\n");
      break;
    } 
    printf("Receiving message: %s", msg.msgText);

  } 

  msgctl(mailbox->storage.msqid, IPC_RMID, NULL);
  semctl(semid, 0, IPC_RMID, 0);  // cleanup semaphore

  printf("\nTotal time taken in receiving msg: %lf s\n", time_taken);
}
