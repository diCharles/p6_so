#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


/*************  message queue implementation ***********/
#include <sys/msg.h>

typedef struct {
	long mesg_type ;
	char mesg_text[3];
}g_message_t ; 

int g_msgid;/** will contain msg queue id*/ 
g_message_t msg_queue ;//

void create_msg_queue(void );
void send_msg_queue(void );
void wait_msg_queue(void );
void destroy_msg_queue( void );
/*************  message queue implementation ***********/

#define CICLOS 10

char *pais[3]={"Peru","Bolivia","Colombia"};
int *g;

void proceso(int i)
{
	int k;
	int l;
	for(k=0;k<CICLOS;k++)
	{
		// Entrada a la sección crítica
		wait_msg_queue();// entrada si aparece el msg
		
		printf("Entra %s",pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("- %s Sale\n",pais[i]);
		
		send_msg_queue();//manda msg para que otro proceso entre
		// Salida de la sección crítica
		
		
		// Espera aleatoria fuera de la sección crítica
		sleep(rand()%3);
	}
	exit(0);
	// Termina el proceso
}

int main()
{
	int pid;
	int status;
	int args[3];
	int i;
	srand(getpid());
	
	/** message queue for protect the  critical section creation*/
	create_msg_queue();//also a msg is put into queue
	
	for(i=0;i<3;i++)
	{
		pid=fork();
		if(pid==0)
			proceso(i);
	}
	for(i=0;i<3;i++)
		pid = wait(&status);
	
	destroy_msg_queue();
}

void create_msg_queue(void )
{
	key_t key ;
	
	/** obtain a key for mssg queue creation*/
	key= ftok("progfile", 65);
	
	/** create new message queue for both producer and consumer */
	
	g_msgid = msgget(key, 0666 | IPC_CREAT);
	msg_queue.mesg_type = 1; 
	
	/** put one message into queue
	the first process to dequeue the message will enter the cs*/
	send_msg_queue();
	
}
void send_msg_queue(void )
{
	/** send a message to queue*/
	msg_queue.mesg_text[0] = 't' ;
	msgsnd(g_msgid, &(msg_queue), sizeof(msg_queue), 0 ) ;
}

void wait_msg_queue(void )
{
	/** dequeue message from queue*/
	msgrcv(g_msgid,  &(msg_queue), sizeof(msg_queue), 1 , 0 );
	//printf("%c received message  \n",msg_queue.mesg_text[0] );
	
}
void destroy_msg_queue( void )
{
	msgctl(g_msgid, IPC_RMID, NULL);

}