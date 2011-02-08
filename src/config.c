#include "config.h"

struct config *
config_Parse (int argc, char ** argv)
{
    struct config * config;

    while ((opt = getopt(argc, argv, "ptc")) != -1) {
        switch (opt) {
        case 'p':
            config->port = atoi(optarg);
            break;
        case 't':
            config->nb_threads = atoi(optarg);
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-p port] [-t threads]\n", argv[0]);
            return (NULL);
        }
    }
    return (config);
}
