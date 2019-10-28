all: ejemplo_sin_sinc  sinc_con_sem  sinc_con_queue

ejemplo_sin_sinc: Ejemplo_sin_sinc.c
			gcc -o ejemplo_sin_sinc Ejemplo_sin_sinc.c

sinc_con_sem: Sinc_w_sem.c
			gcc -o sinc_con_sem Sinc_w_sem.c
sinc_con_queue: sinc_w_msg_queue.c
			gcc -o sinc_con_queue sinc_w_msg_queue.c 

clear:
			rm ejemplo_sin_sinc  sinc_con_sem sinc_con_queue