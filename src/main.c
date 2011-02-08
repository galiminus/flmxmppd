#include <unistd.h>

#include <flm/flm.h>

#include "config.h"
#include "listener_thread.h"

int
main (int argc, char ** argv)
{
	flm_Thread * thread;

        struct config * config;

        config = config_Parse (argc, argv);
        if (config == NULL) {
            return (EXIT_FAILURE);
        }

	thread = listener_thread_Start ("localhost", 3000);
	if (thread == NULL) {
		return (1);
	}
	flm_ThreadJoin (thread);
	return (EXIT_SUCCESS);
}
