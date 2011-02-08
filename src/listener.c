#include <stdlib.h>

#include <flm/flm.h>

#include "connection_thread.h"

struct listener_State {
	flm_ThreadPool *	thread_pool;
};

void
listener_ErrorHandler (void *, int);

void
listener_AcceptHandler (void *, int);

flm_ThreadPool *
listener_Start (const char *	host,
		unsigned short	port)
{
	flm_ThreadPool *	thread_pool;
	flm_Thread *		thread;

	flm_Monitor *		monitor;
	flm_TCPServer *		listener;

	struct listener_State *	state;

	int			i;

	thread_pool = flm_ThreadPoolNew ();
	if (thread_pool == NULL) {
		goto error;
	}
	
	for (i = 0; i < NB_THREADS; i++) {
		thread = connection_thread_Start (thread_pool);
		if (thread == NULL) {
			/* should be an error */
			break ;
		}
		if (flm_ThreadPoolAdd (thread_pool, thread) == -1) {
			/* should be an error */
			break ;
		}
	}

	if ((monitor = flm_MonitorNew ()) == NULL) {
		goto release_thread_pool;
	}

	state = malloc (sizeof (struct listener_State));
	if (state == NULL) {
		goto release_monitor;
	}
	state->thread_pool = thread_pool;

	listener = flm_TCPServerNew (monitor, host, port, state);
	if (listener == NULL ) {
		goto free_state;
	}

	flm_TCPServerOnAccept (listener, listener_AcceptHandler);
	flm_IOOnError (FLM_IO(listener), listener_ErrorHandler);

	flm_Release (FLM_OBJ (listener));

	flm_MonitorWait (monitor);

	flm_Release (FLM_OBJ (monitor));

	return (thread_pool);

free_state:
	free (state);
release_monitor:
	flm_Release (FLM_OBJ (monitor));
release_thread_pool:
	flm_Release (FLM_OBJ (thread_pool));
error:
	return (NULL);
}

void
listener_AcceptHandler (void *	_state,
			int		fd)
{
	struct listener_State * state;

	state = _state;
	connection_thread_Accept (state->thread_pool, fd);
	return ;
}

void
listener_ErrorHandler (void *	_state,
		       int		error)
{
	(void) _state;
	(void) error;
	return ;
}
