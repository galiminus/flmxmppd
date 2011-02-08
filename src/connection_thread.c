#include <stdlib.h>

#include <flm/flm.h>

#include "connection.h"

struct connection_thread_State {
	flm_Monitor *		monitor;
	flm_ThreadPool *	thread_pool;
};

struct connection_thread_AcceptParams {
	int			fd;
};

void
connection_thread_AcceptHandler (void * _state,
				 void * _params);

int
connection_thread_Accept (flm_ThreadPool *	thread_pool,
			  int			fd)
{
	struct connection_thread_AcceptParams * params;

	params = malloc (sizeof (struct connection_thread_AcceptParams));
	if (params == NULL) {
		goto error;
	}

	params->fd = fd;

	if (flm_ThreadPoolCall (thread_pool,
				connection_thread_AcceptHandler,
				params) == -1) {
		goto free_params;
	}

	return (0);

free_params:
	free (params);
error:
	return (-1);
}

void
connection_thread_AcceptHandler (void *	_state,
				 void *	_params)
{
	struct connection_thread_State *	state;
	struct connection_thread_AcceptParams *	params;

	state = _state;
	params = _params;

	if (connection_Start (state->monitor, params->fd) == -1) {
		/* error */
		return ;
	}
}

flm_Thread *
connection_thread_Start (flm_ThreadPool * thread_pool)
{
	struct connection_thread_State *	state;
	flm_Thread *				thread;
	flm_Monitor *				monitor;

	if ((monitor = flm_MonitorNew ()) == NULL) {
		goto error;
	}
	
	state = malloc (sizeof (struct connection_thread_State));
	if (state == NULL) {
		goto release_monitor;
	}
	state->monitor = monitor;
	state->thread_pool = thread_pool;

	thread = flm_ThreadNew (monitor, state);
	if (thread == NULL) {
            goto free_state;
	}

	return (thread);

free_state:
	free (state);
release_monitor:
	flm_Release (FLM_OBJ (monitor));
error:
	return (NULL);
}
				 
