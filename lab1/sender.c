#include "sender.h"
#include <errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct timespec start,end;
double time_taken=0;
int semid;

void send(message_t message, mailbox_t* mailbox_ptr){
  if(mailbox_ptr->flag==MSG_PASSING){

    clock_gettime(CLOCK_MONOTONIC, &start);//start timing
    int rc = msgsnd(mailbox_ptr->storage.msqid, &message, sizeof(message.msgText), 0);

    clock_gettime(CLOCK_MONOTONIC, &end);//end timing

    time_taken += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;

  }else if(mailbox_ptr->flag==SHARED_MEM){

    //semop(semid, &P(0), 1);//wait until can write

    strcpy(mailbox_ptr->storage.shm_addr, message.msgText);

    //semop(semid, &V(1), 1);//wait until can read

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
  semid = semget(sem_key, 1, IPC_CREAT | 0666);

  mailbox_t* mailbox = (mailbox_t*) malloc(sizeof(mailbox_t));
  if(option==1){
    mailbox -> storage.msqid = msgget(key, 0666 | IPC_CREAT);
  }else{
    mailbox -> storage.msqid = shmget(key, sizeof(message_t), 0666);
    mailbox->storage.shm_addr = shmat(mailbox->storage.msqid, NULL, 0); 
    //semid = semget(key, sizeof(message_t), 2, 0666 | IPC_CREAT);
  } 
  mailbox -> flag = option;

  //for(;;) if(semop(semid, &P, 1) == 0) break;
   //if(semop(semid, &p, 1) == -1) perror("semop P");
  //struct sembuf p_nowait = {0, -1, IPC_NOWAIT};

  //for (;;) {
   // if (semop(semid, &p, 1) == 0) break;     // 取得 → 開始計時
    //if(errno == EINTR) continue;
    //perror("semop P nowait"); /* 非預期錯誤 */ break;
  //}

  while(fgets(buf, 100, fptr)){

    strcpy(message.msgText, buf);
    printf("Sending message: %s", message.msgText);

    send(message, mailbox);

  } 
  strcpy(message.msgText, "EXIT"); send(message, mailbox);

  printf("\nEnd of input file! exit!");

  printf("\nTotal time taken in sending msg: %lf s\n", time_taken);

  fclose(fptr);
  return 0;    
}
