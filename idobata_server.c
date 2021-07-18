/*
idobata_server.c
*/

#include "mynet.h"
#include "idobata.h"
#include "idobata_list.h"
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define BUFSIZE 512
#define TIMEOUT_SEC 2

void idobata_server(int port_number, char* username)
{
  fprintf(stderr, "I am Server.\n");
  struct sockaddr_in from_adrs;
  int sock, tcpsock_listen, tcpsock_accepted;
  socklen_t from_len;

  list *client = make_list();

  fd_set mask, readfds;

  char buf[BUFSIZE];
  struct idobata *packet;
  int strsize;

  sock = init_udpserver((in_port_t)port_number);
  tcpsock_listen = init_tcpserver(port_number, 5);

  for (;;) {
    FD_ZERO(&mask);
    imember p = client->top;
    int Max_sd = 0;
    while (p != NULL) {
      FD_SET(p->sock, &mask);
      Max_sd++;
      p = p->next;
    }

    readfds = mask;
    select(Max_sd, &readfds, NULL, NULL, NULL);

    from_len = sizeof(from_adrs);

    strsize = Recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr*)&from_adrs, &from_len);

    p = client->top;
    while(p != NULL) {
      if(FD_ISSET(p->sock, &readfds)) {
        strsize = Recv(p->sock, buf, BUFSIZE-1, 0);
        buf[strsize] = '\0';
        break;
      }
      p = p->next;
    }

    packet = (struct idobata*)buf;

    switch (analyze_header(packet->header)) {
      case HELLO:
        Sendto(sock, "HERE", 4, 0, (struct sockaddr*)&from_adrs, sizeof(from_adrs));
        tcpsock_accepted = accept(tcpsock_listen, NULL, NULL);
        break;

      case JOIN:
        push(client, chop_nl(packet->data), tcpsock_accepted);
        break;

      case POST:
        snprintf(buf, BUFSIZE, "[%s]%s", p->username, packet->data);
        fprintf(stderr, "%s", buf);
        fflush(stdout);
        snprintf(buf, BUFSIZE, "MESG [%s]%s", p->username, packet->data);
        strsize = strlen(buf);
        p = client->top;
        while (p != NULL) {
          Send(p->sock, buf, strsize, 0);
          p = p->next;
        }
        break;

      default:
        break;
    }

  }

}
