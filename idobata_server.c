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

  imember p = client->top;
  int Max_sd = sock;

  FD_ZERO(&mask);
  FD_SET(0, &mask);
  FD_SET(sock, &mask);
  while (p != NULL) {
    if(strcmp(p->username, "xxx") == 0) {
      p = p->next;
      continue;
    }
    FD_SET(p->sock, &mask);
    if(p->sock > Max_sd) Max_sd = p->sock;
    p = p->next;
  }

  for (;;) {
    from_len = sizeof(from_adrs);
    readfds = mask;
    //fprintf(stderr, "x\n");
    select(Max_sd+1, &readfds, NULL, NULL, NULL);
    //fprintf(stderr, "x\n");
    if(FD_ISSET(sock, &readfds)) {
      strsize = Recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr*)&from_adrs, &from_len);
    }
    else if(FD_ISSET(0, &readfds)) {
      char k_buf[BUFSIZE-6];
      fgets(k_buf, BUFSIZE-6, stdin);
      snprintf(buf, BUFSIZE, "POST %s", k_buf);
      strsize = strlen(buf);
      fprintf(stderr, "[%s]%s", username, buf+5);
      fflush(stdout);
      char s_buf[BUFSIZE];
      snprintf(s_buf, BUFSIZE, "MESG [%s]%s", username, buf+5);
      strsize = strlen(s_buf);
      p = client->top;
      while (p != NULL) {
        if(strcmp(p->username, "xxx") == 0) {
          p = p->next;
          continue;
        }
        Send(p->sock, s_buf, strsize, 0);
        p = p->next;
      }
      continue;
    }
    else {
      p = client->top;
      while (p != NULL) {
        if(FD_ISSET(p->sock, &readfds)) {
          break;
        }
        p = p->next;
      }
      if(p == NULL) continue;
      strsize = Recv(p->sock, buf, BUFSIZE, 0);
      buf[strsize] = '\0';
    }

    packet = (struct idobata*)buf;

    switch (analyze_header(packet->header)) {
      case HELLO:
        Sendto(sock, "HERE", 4, 0, (struct sockaddr*)&from_adrs, sizeof(from_adrs));
        tcpsock_accepted = accept(tcpsock_listen, NULL, NULL);
        strsize = Recv(tcpsock_accepted, buf, BUFSIZE, 0);
        packet = (struct idobata*)buf;
        if(analyze_header(packet->header) == JOIN) {
          push(client, chop_nl(packet->data), tcpsock_accepted);
          p = client->top;
          FD_ZERO(&mask);
          FD_SET(0, &mask);
          FD_SET(sock, &mask);
          while (p != NULL) {
            if(strcmp(p->username, "xxx") == 0) {
              p = p->next;
              continue;
            }
            FD_SET(p->sock, &mask);
            if(p->sock > Max_sd) Max_sd = p->sock;
            p = p->next;
          }
        }
        break;

      case POST:
        fprintf(stderr, "[%s]%s", p->username, buf+5);
        fflush(stdout);
        char s_buf[BUFSIZE];
        snprintf(s_buf, BUFSIZE, "MESG [%s]%s", p->username, buf+5);
        strsize = strlen(s_buf);
        p = client->top;
        while (p != NULL) {
          if(strcmp(p->username, "xxx") == 0) {
            p = p->next;
            continue;
          }
          Send(p->sock, s_buf, strsize, 0);
          p = p->next;
        }
        break;

      case QUIT:
        fprintf(stderr, "%s\n", p->username);
        close(p->sock);
        imember prev = client->top;
        while (prev->next != p) {
          prev = prev->next;
        }
        prev->next = p->next;
        free(p);
        p = client->top;
        FD_ZERO(&mask);
        FD_SET(0, &mask);
        FD_SET(sock, &mask);
        while (p != NULL) {
          if(strcmp(p->username, "xxx") == 0) {
            p = p->next;
            continue;
          }
          FD_SET(p->sock, &mask);
          if(p->sock > Max_sd) Max_sd = p->sock;
          p = p->next;
        }
        break;

      default:
        break;
    }

  }

}
