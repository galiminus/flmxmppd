#include <stdlib.h>

#include <flm/flm.h>

#include "listener.h"

struct listener_thread_State {
    struct conf *       conf;
};

void
listener_thread_StartHandler (void *, void *);

flm_Thread *
listener_thread_Start (struct conf * conf)
{
	struct listener_thread_State *		state;
	flm_Monitor *				monitor;
	flm_Thread *				thread;
	
	state = malloc (sizeof (struct listener_thread_State));
	if (state == NULL) {
		goto error;
	}
	state->conf = conf;

	monitor = flm_MonitorNew ();
	if (monitor == NULL) {
            goto free_state;
	}
	
	thread = flm_ThreadNew (monitor, state);
	if (thread == NULL) {
		goto release_monitor;
	}

	if (flm_ThreadCall (thread, listener_thread_StartHandler, NULL) == -1) {
		goto release_thread_pool;
	}
	return (thread);

release_thread_pool:
	flm_Release (FLM_OBJ (thread));
release_monitor:
	flm_Release (FLM_OBJ (monitor));
free_state:
	free (state);
error:
	return (NULL);
}

void
listener_thread_StartHandler (void *	_state,
			      void *	_params)
{
	struct listener_thread_State *	state;
	flm_ThreadPool *		thread_pool;

	(void) _params;

	state = _state;

	thread_pool = listener_Start (state->conf);
	if (thread_pool == NULL) {
		return ;
	}

	flm_ThreadPoolJoin (thread_pool);

	return ;
}
