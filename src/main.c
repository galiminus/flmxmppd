#include <unistd.h>
#include <stdlib.h>

#include <flm/flm.h>

#include "conf.h"
#include "listener_thread.h"

int
main (int argc, char ** argv)
{
	flm_Thread * thread;

        struct conf conf;

        conf.host = "localhost";
        conf.port = 5222;
        conf.nb_threads = 4;

        if (conf_Parse (argc, argv, &conf) == -1) {
            return (EXIT_FAILURE);
        }

	thread = listener_thread_Start (&conf);
	if (thread == NULL) {
		return (1);
	}
	flm_ThreadJoin (thread);
	return (EXIT_SUCCESS);
}
