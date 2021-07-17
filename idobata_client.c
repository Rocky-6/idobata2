/*
idobata_client.c
*/

#include "mynet.h"
#include "idobata.h"
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define S_BUFSIZE 512
#define R_BUFSIZE 512
#define K_BUFSIZE 256
#define TIMEOUT_SEC 2

void idobata_client(char* servername, int port_number, char* username)
{
    struct sockaddr_in broadcast_adrs;
    struct sockaddr_in from_adrs;
    socklen_t from_len;

    int sock, tcpsock;
    int broadcast_sw = 1;
    fd_set mask, readfds;
    struct timeval timeout;

    char s_buf[S_BUFSIZE], r_buf[R_BUFSIZE], k_buf[K_BUFSIZE];
    int strsize;

    set_sockaddr_in_broadcast(&broadcast_adrs, port_number);

    sock = init_udpclient();
    if ((tcpsock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
      exit_errmesg("socket()");
    }


    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&broadcast_sw, sizeof(broadcast_sw)) == -1)
    {
        exit_errmesg("setsockopt()");
    }


    FD_ZERO(&mask);
    FD_SET(sock, &mask);


    int i;
    for (i = 0; i < 3; ++i)
    {
      snprintf(s_buf, S_BUFSIZE, "HELO");
      strsize = strlen(s_buf);


      Sendto(sock, s_buf, strsize, 0, (struct sockaddr*)&broadcast_adrs, sizeof(broadcast_adrs));

      readfds = mask;
      timeout.tv_sec = TIMEOUT_SEC;
      timeout.tv_usec = 0;

      if(select(sock+1, &readfds, NULL, NULL, &timeout) == 0)
      {
          printf("Timeout\n");
          continue;
      }


      from_len = sizeof(from_adrs);
      strsize = Recvfrom(sock, r_buf, R_BUFSIZE-1, 0, (struct sockaddr*)&from_adrs, &from_len);

      r_buf[strsize] = '\0';

      if(strncmp(r_buf, "HERE", 4) == 0)
      {
        if (connect(tcpsock, (struct sockaddr*)&from_adrs, from_len) == -1)
        {
          exit_errmesg("connect()");
        }
        snprintf(s_buf, S_BUFSIZE, "JOIN %s", username);
        strsize = strlen(s_buf);
        Send(tcpsock, s_buf, strsize, 0);
        fprintf(stderr, "Connected to Server.\n");
        break;
      }
    }
    if (i == 3)
    {
      printf("Server not found.\n");
      exit(EXIT_FAILURE);
    }

    for(;;){

    /* 受信データの有無をチェック */
    FD_ZERO(&mask);
    FD_SET(0, &mask);
    FD_SET(tcpsock, &mask);

    readfds = mask;
    select( tcpsock+1, &readfds, NULL, NULL, NULL );

    if( FD_ISSET(0, &readfds) ){
      /* キーボードから文字列を入力する */
      fgets(k_buf, K_BUFSIZE, stdin);
      if ( strncmp(k_buf, "QUIT", 4) == 0)
      {
        snprintf(s_buf, S_BUFSIZE, "%s", k_buf);
        strsize = strlen(s_buf);
        Send(tcpsock, s_buf, strsize, 0);
        exit(EXIT_SUCCESS);
      }
      snprintf(s_buf, S_BUFSIZE, "POST %s", k_buf);
      strsize = strlen(s_buf);
      Send(tcpsock, s_buf, strsize, 0);
    }

    if( FD_ISSET(tcpsock, &readfds) ){
      /* サーバから文字列を受信する */
      strsize = Recv(tcpsock, r_buf, R_BUFSIZE, 0);
      r_buf[strsize] = '\0';
      fprintf(stderr, "%s",r_buf + 5);
      fflush(stdout);
    }
  }

    close(sock);

    exit(EXIT_SUCCESS);
}
