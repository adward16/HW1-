#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> //used for reading options from command line

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
  //purpose of web_conn is to be sockfd for socket to web server; web_portno holds port on web server we want to connect to;
    int srv_sock, srv_portno, new_conn, web_conn, web_portno;
    socklen_t clilen;
    struct hostent *websrv;
    //websrv_addr holds address and port num of web server
    struct sockaddr_in serv_addr, cli_addr, websrv_addr;
    char buffer[1000000];
    FILE *fp;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    srv_portno = atoi(argv[1]); //original code
    srv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (srv_sock< 0)
        error("ERROR opening socket");

  /*
  websrv = gethostbyname(argv[1]); //original code
    if (websrv == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
  */

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(srv_portno);

    if (bind(srv_sock, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
             error("ERROR on binding");

    printf("Waiting for clients\n");
    listen(srv_sock,5);
    clilen = sizeof(cli_addr);
    new_conn = accept(srv_sock,
                (struct sockaddr *) &cli_addr,
                &clilen);
    if (new_conn < 0)
         error("ERROR on accept");
    printf("A client has connected\n");


//create new socket to connect to webserver
    web_conn = socket(AF_INET,SOCK_STREAM,0);
      if(web_conn < 0) error("ERROR, socket could not be created\n");
    web_portno = 80;
    bzero((char *) &websrv_addr, sizeof(websrv_addr));
    websrv_addr.sin_family = AF_INET;
    bcopy((char *)websrv->h_addr,
         (char *)&websrv_addr.sin_addr.s_addr,
         websrv->h_length);
    websrv_addr.sin_port = htons(web_portno);

// not sure if I really need this ^^^^^^^^^^^^^^^^^^^^^

    if (connect(web_conn,(struct sockaddr *) &websrv_addr,sizeof(websrv_addr)) < 0)
        error("ERROR connecting");


//what happens after a new connection is established?
    bzero(buffer,1000000);

    fp = fopen("testing.txt","w");

    if (fp!=NULL)
    {
          while((new_conn = read(srv_sock,buffer,1000000)) != 0){

          /*  if((fwrite(buffer,1,sizeof(buffer),fp))<0) printf("Error writing to file\n"); */
          fwrite(buffer,1,sizeof(buffer),fp);

          }

      fclose (fp);
    }

    new_conn = printf("testing.txt");

    /*
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer); */
    close(srv_sock);
    return 0;
}
