#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT 0
#define NUM_OF_OPS 1

int create_sem(int key, int init_val)
{
	int id_ret = 0;
	int init_state = 0;
	union semun
	{
		int val;
		struct semid_ds *buf;
		ushort  * array;
	} init_values;
	init_values.val = init_val;
	
	id_ret = semget(key, 1, 0666 | IPC_CREAT);
	if(id_ret < 0)
		printf("FATAL_ERROR: Could not generate semaphore\n");
	
	init_state = semctl(id_ret, 0, SETVAL, init_values);
	if(init_state < 0)
	{
		printf("FATAL_ERROR: Could not initialize value\n");
		id_ret = init_state;
	}
	
	return id_ret;
}

//V
int sem_signal(int sem_key)
{
	int ret_state, sem_id;
	struct sembuf semops;
	
	sem_id = semget(sem_key, 1, 0666);
	if (sem_id < 0)
	{
		printf("ERROR: Semaphore doesn't exist\n");
		return -1;
	}
	
	semops.sem_num = 0;
	semops.sem_op = 1;
	semops.sem_flg = DEFAULT;
	
	ret_state = semop(sem_id, &semops, NUM_OF_OPS);
	
	if(ret_state < 0)
		printf("FATAL_ERROR: Could not modify the semaphore\n");
	
	return ret_state;
}

//P
int sem_wait(int sem_key)
{
	int ret_state, sem_id;
	struct sembuf semops;
	
	sem_id = semget(sem_key, 1, 0666);
	if(sem_id < 0)
	{
		printf("ERROR: Semaphore doesn't exist\n");
		return -1;
	}
	
	semops.sem_num = 0;
	semops.sem_op = -1;
	semops.sem_flg = DEFAULT;
	
	ret_state = semop(sem_id, &semops, NUM_OF_OPS);
	
	if(ret_state < 0)
		printf("FATAL_ERROR: Could not modify semaphore\n");
	
	return ret_state;
}