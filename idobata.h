#ifndef IDOBATA_H_
#define IDOBATA_H_
/*
idobata.h
*/
#include "mynet.h"
#include <stdlib.h>
#include <sys/select.h>

#define MSGBUF_SIZE 512

#define HELLO 1
#define HERE 2
#define JOIN 3
#define POST 4
#define MESSAGE 5
#define QUIT 6

#define USER_LEN 15

struct idobata {
    char header[4];
    char sep;
    char data[];
};

typedef struct _imember *imember;

/* サーバ */
void idobata_server(int port_number, char* username);

/* クライアント */
void idobata_client(char* servername, int port_number, char* username);

/* パケット解析 */
u_int32_t analyze_header(char* header);

int Accept(int s, struct sockaddr* addr, socklen_t* addrlen);

int Send(int s, void* buf, size_t len, int flags);

int Recv(int s, void* buf, size_t len, int flags);

char *chop_nl(char *s);

#endif /* IDOOBATA_H_ */
