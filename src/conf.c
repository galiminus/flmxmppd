#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "conf.h"

int
conf_Parse (int                 argc,
            char **             argv,
            struct conf *       conf)
{
    int                 opt;
    
    while ((opt = getopt (argc, argv, "p:t:h:")) != -1) {
        switch (opt) {
        case 'p':
            printf("%x\n", optarg);
            conf->port = atoi (optarg);
            break;
        case 't':
            conf->nb_threads = atoi (optarg);
            break;
        case 'h':
            conf->host = optarg;
            break;
        default:
            fprintf (stderr, "Usage: %s [-p port] [-t threads]\n", argv[0]);
            return (-1);
        }
    }
    return (0);
}
