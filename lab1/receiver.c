#include "receiver.h"
#include<stdlib.h>
#include<string.h>
union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
    struct seminfo  *__buf;
};


struct timespec start,end;
double time_taken=0;
int semid;

void receive(message_t* message_ptr, mailbox_t* mailbox_ptr){
    
  if(mailbox_ptr->flag==MSG_PASSING){

    clock_gettime(CLOCK_MONOTONIC, &start);//start timing
    int rc = msgrcv(mailbox_ptr->storage.msqid, message_ptr, sizeof(message_ptr->msgText), 0, 0);
    clock_gettime(CLOCK_MONOTONIC, &end);//end timing

    time_taken += (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) * 1e-9;

    
  }else if(mailbox_ptr->flag==SHARED_MEM){

    //semop(semid, &P(1), 1);//wait til can read
    
    strcpy(message_ptr->msgText, mailbox_ptr->storage.shm_addr );

//    semop(semid, &V(0), 1);//signal can write

  }else{
    printf("Flag out of range\n");
  } 
    
}

int main(int argc, char* argv[] ){
    int option = atoi(argv[1]  );


    key_t sem_key = ftok("semfile", 75);
    semid = semget(sem_key, 1, IPC_CREAT | 0666);
    union semun { int val; struct semid_ds *buf; unsigned short *array; };
    union semun arg; arg.val = 0;
    semctl(semid, 0, SETVAL, arg);


    key_t key = ftok("mailbox", 65);
    mailbox_t* mailbox = (mailbox_t*) malloc(sizeof(mailbox_t));


    if(option & 1){

      mailbox -> storage.msqid = msgget(key, 0666 | IPC_CREAT);

    }else{

      mailbox -> storage.msqid = shmget(key, sizeof(message_t), 0666);
      mailbox->storage.shm_addr = shmat(mailbox->storage.msqid, NULL, 0); 

    } 

    mailbox -> flag = option;
    message_t msg;

    //for(;;) if(semop(semid, &P, 1) == 0) break;
    //if(semop(semid, &P, 1) == -1) perror("semop P");
    //semop(semid, &p, 1);
    while(1){

      receive(&msg, mailbox);

      if(strcmp(msg.msgText, "EXIT")==0){
        printf("\nSender exit!\n");
        break;
      } 
      printf("Receiving message: %s", msg.msgText);

    } 

    msgctl(mailbox->storage.msqid, IPC_RMID, NULL);

    printf("\nTotal time taken in receiving msg: %lf s\n", time_taken);
}
