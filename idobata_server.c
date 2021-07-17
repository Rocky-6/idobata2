/*
idobata_server.c
*/

#include "mynet.h"
#include "idobata.h"
#include "list.h"
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define BUFSIZE 512
#define TIMEOUT_SEC 2

void idobata_server(int port_number)
{
  struct sockaddr_in from_adrs;
  int sock, tcpsock;
  socklen_t from_len;

  char buf[BUFSIZE];
  struct idobata *packet;
  int strsize;

  sock = init_udpserver((in_port_t)port_number);
  if(tcpsock = socket(PF_INET, SOCK_STREAM, 0) == -1) {
    exit_errmesg("socket()");
  }


  for (;;) {
    from_len = sizeof(from_adrs);
    strsize = Recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr*)&from_adrs, &from_len);

    packet = (struct idobata*)buf;

    switch (analyze_header(packet->header)) {
      case HELLO:
        Sendto(sock, "HERE", 4, 0, (struct sockaddr*)&from_adrs, sizeof(from_adrs));

        break;

      case JOIN:

    }

  }

}
