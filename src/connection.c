#include <stdlib.h>
#include <stdio.h>

#include <flm/flm.h>

struct connection_State {
	flm_Timer *	timer;
	flm_Stream *	connection;
	int		count;
};

void
connection_ErrorHandler (void *, int);

void
connection_ReadHandler (void *, flm_Buffer *);

void
connection_TickHandler (void *);

int
connection_Start (flm_Monitor *	monitor,
		  int		fd)
{
	struct connection_State *	state;

	state = malloc (sizeof (struct connection_State));
	if (state == NULL) {
		goto error;
	}

	state->count = 0;

	state->connection = flm_StreamNew (monitor, fd, state);
	if (state->connection == NULL) {
            goto free_state;
	}

	flm_StreamOnRead (state->connection, connection_ReadHandler);
	flm_IOOnError (FLM_IO (state->connection), connection_ErrorHandler);

	state->timer = flm_TimerNew (monitor,			\
				     connection_TickHandler,	\
                                     state,                     \
				     1);
	if (state->timer == NULL) {
		goto close_connection;
	}

	return (0);

close_connection:
	flm_IOClose (FLM_IO (state->connection));
free_state:
	free (state);
error:
	return (-1);
}

void
connection_ErrorHandler (void *                 _state,
			 int			error)
{
	struct connection_State * state;

	state = _state;

	printf ("GOT ERROR %d\n", error);
	return ;
}

void
connection_ReadHandler (void *		_state,
			flm_Buffer *	buffer)
{
	struct connection_State * state;

	state = _state;

	(void) buffer;
	return ;
}

void
connection_TickHandler (void *	_state)
{
	struct connection_State * state;

	state = _state;
	
	flm_StreamPrintf (state->connection, "Tick! -> %d\n", state->count++);
	flm_TimerReset (state->timer, 1);
	return ;
}
