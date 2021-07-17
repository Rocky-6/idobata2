/*
idobata.c
*/

#include "mynet.h"
#include "idobata.h"
#include <stdlib.h>
#include <unistd.h>

#define SERVER_LEN 256
#define DEFAULT_PORT 50001

extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char *const *argv)
{
    int port_number = DEFAULT_PORT;
    char servername[SERVER_LEN] = "localhost";
    char username[USER_LEN];
    int c;

    /* オプション文字列の取得 */

    opterr = 0;
    while (1)
    {
        c = getopt(argc, argv, "u:s:p");
        if(c == -1) break;

        switch (c)
        {
        case 'u':
            snprintf(username, USER_LEN, "%s", optarg);
            break;

        case 's':
            snprintf(servername, SERVER_LEN, "%s", optarg);

        case 'p':
            port_number = atoi(optarg);

        default:
            break;
        }
    }

    idobata_client(servername, port_number, username);

    return 0;
}
