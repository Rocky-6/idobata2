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

int main(int argc, char *argv[])
{
    int port_number = DEFAULT_PORT;
    char servername[SERVER_LEN] = "localhost";
    char username[USER_LEN];
    int c;

    /* オプション文字列の取得 */

    opterr = 0;
    while (1)
    {
        c = getopt(argc, argv, "u:s:p:h");
        if(c == -1) break;

        switch (c)
        {
        case 'u': // usernameの指定
            snprintf(username, USER_LEN, "%s", optarg);
            break;

        case 's': // サーバアドレスの指定
            snprintf(servername, SERVER_LEN, "%s", optarg);
            break;

        case 'p': // ポート番号の指定
            port_number = atoi(optarg);
            break;

        case '?':
    	    fprintf(stderr,"Unknown option '%c'\n", optopt );

    	  case 'h':
    	    fprintf(stderr,"Usage: %s -u username -s server_name -p port_number\n", argv[0]);
    	    exit(EXIT_FAILURE);
    	    break;
        }
    }

    idobata_client(servername, port_number, username);

    return 0;
}
