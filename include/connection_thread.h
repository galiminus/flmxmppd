#include <flm/flm.h>

int
connection_thread_Accept (flm_ThreadPool * thread_pool,
			  int fd);

flm_Thread *
connection_thread_Start (flm_ThreadPool * thread_pool);
