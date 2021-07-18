/*
idobata_util.c
*/
#include "mynet.h"
#include "idobata.h"

typedef struct _imember {
    char username[USER_LEN];
    int sock;
    struct _imember* next;
} *imember;

char* create_packet(char* buffer, u_int32_t type, char* message) {

    switch (type)
    {
    case HELLO:
        snprintf(buffer, MSGBUF_SIZE, "HELO");
        break;

    case HERE:
        snprintf(buffer, MSGBUF_SIZE, "HERE");
        break;

    case JOIN:
        snprintf(buffer, MSGBUF_SIZE, "JOIN %s", message);
        break;

    case POST:
        snprintf(buffer, MSGBUF_SIZE, "POST %s", message);
        break;

    case MESSAGE:
        snprintf(buffer, MSGBUF_SIZE, "MESG %s", message);
        break;

    case QUIT:
        snprintf(buffer, MSGBUF_SIZE, "QUIT");
        break;

    default:
        return(NULL);
        break;
    }

    return buffer;
}

u_int32_t analyze_header(char* header) {
    if(strncmp(header, "HELO", 4) == 0) return(HELLO);
    if(strncmp(header, "HERE", 4) == 0) return(HERE);
    if(strncmp(header, "JOIN", 4) == 0) return(JOIN);
    if(strncmp(header, "POST", 4) == 0) return(POST);
    if(strncmp(header, "MESG", 4) == 0) return(MESSAGE);
    if(strncmp(header, "QUIT", 4) == 0) return(QUIT);
    return 0;
}

int Accept(int s, struct sockaddr* addr, socklen_t* addrlen) {
    int r;
    if ((r = accept(s, addr, addrlen)) == -1) {
        exit_errmesg("accept()");
    }
    return (r);
}

int Send(int s, void* buf, size_t len, int flags) {
    int r;
    if ((r = send(s, buf, len, flags)) == -1) {
        exit_errmesg("send()");
    }
    return (r);
}

int Recv(int s, void* buf, size_t len, int flags) {
    int r;
    if ((r = recv(s, buf, len, flags)) == -1) {
        exit_errmesg("recv()");
    }
    return (r);
}

char *chop_nl(char *s)
{
  int len;
  len = strlen(s);
  if( s[len-1] == '\n' ){
    s[len-1] = '\0';
  }
  return(s);
}
