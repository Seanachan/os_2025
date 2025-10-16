#include "sender.h"
#include <errno.h>
#include<stdio.h>
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

void send(message_t message, mailbox_t* mailbox_ptr){
  if(mailbox_ptr->flag==MSG_PASSING){

    clock_gettime(CLOCK_MONOTONIC, &start);//start timing
                                           
    msgsnd(mailbox_ptr->storage.msqid, &message, sizeof(message.msgText), 0);

    clock_gettime(CLOCK_MONOTONIC, &end);//end timing
    time_taken += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;
  }else if(mailbox_ptr->flag==SHARED_MEM){

    clock_gettime(CLOCK_MONOTONIC, &start);//start timing

    strcpy(mailbox_ptr->storage.shm_addr, message.msgText);

    clock_gettime(CLOCK_MONOTONIC, &end);//end timing
    time_taken += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;

  }else{
    printf("Flag out of range\n");
  } 
  return;
}

int main(int argc, char *argv[] ){
  int option = atoi(argv[1]  );
  char* FILE_PTH = argv[2] ; 
  char buf[100] ;
  FILE *fptr = fopen(FILE_PTH, "r");

  printf((option==1)? "Message Passing\n":"Shared Memory\n");

  //establish new msgs
  message_t message;
  message.mType = 1;

  //establish mailbox
  key_t key = ftok("mailbox", 65);
  key_t sem_key = ftok("semfile", 75);

  // Wait for receiver to create and initialize the semaphore set
  while ((semid = semget(sem_key, 0, 0)) == -1) {
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 100 * 1000000 }; // 100ms
    nanosleep(&ts, NULL);
  }

  mailbox_t* mailbox = (mailbox_t*) malloc(sizeof(mailbox_t));
  if(option==1){
    // Wait for receiver to create the message queue
    while ((mailbox -> storage.msqid = msgget(key, 0666)) == -1) {
      printf("keep waiting\n");
      struct timespec ts = { .tv_sec = 0, .tv_nsec = 100 * 1000000 }; // 100ms
      nanosleep(&ts, NULL);
    }
    printf("mailbox done\n");

  }else{

    mailbox->storage.shm_addr = shmat(mailbox->storage.msqid, NULL, 0); 

  } 
  mailbox -> flag = option;

  while(fgets(buf, 100, fptr)){

    strcpy(message.msgText, buf);
    printf("Sending message: %s", message.msgText);

    P(semid, SEM_EMPTY);   // Wait for empty slot
    P(semid, SEM_MUTEX);  // Acquire mutex

    send(message, mailbox);

    V(semid, SEM_MUTEX);   // Release mutex
    V(semid, SEM_FULL);  // Signal message available
  } 
  
  // Send EXIT message with semaphore protection
  strcpy(message.msgText, "EXIT");
  
  P(semid, SEM_EMPTY);   // Wait for empty slot
  P(semid, SEM_MUTEX);  // Acquire mutex
  
  send(message, mailbox);
  
  V(semid, SEM_MUTEX);   // Release mutex
  V(semid, SEM_FULL);  // Signal message available
  
  // Wait for receiver to consume EXIT message
  // Block until FULL (sem 2) becomes 0, meaning receiver has consumed it
  struct sembuf wait_op = {.sem_num = 2, .sem_op = 0, .sem_flg = 0};
  semop(semid, &wait_op, 1);

  printf("\nEnd of input file! exit!");

  printf("\nTotal time taken in sending msg: %lf s\n", time_taken);

  fclose(fptr);
  return 0;    
}
