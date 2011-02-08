#include <flm/flm.h>

#include "conf.h"

int
connection_thread_Accept (flm_ThreadPool *      thread_pool,
                          struct conf *         conf,
			  int                   fd);

flm_Thread *
connection_thread_Start (flm_ThreadPool * thread_pool);
