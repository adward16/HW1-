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
  //m_string will be used to print formatted message to buffer shared with server socket
    int cli_sock, mysrv_portno, n, k;
    struct sockaddr_in mysrv_addr;
    struct hostent *mysrv; //for web server address and local server address
    char buffer[200], host_req[199];
    const char *element;
    FILE *fp;
    int i;

    if(argc < 6) printf("usage %s port hostname url",argv[0]);

    //go through args and assign appropriate vals
    for(i = 1; i < 6; ++i){
      if (strcmp(argv[i], "-p")==0)
      {
          mysrv_portno = atoi(argv[i+1]); //original code
          //printf("%d\n",mysrv_portno);
      }
      else if(strcmp(argv[i], "-h")==0)
      {
        mysrv = gethostbyname(argv[i+1]); //original code
        //printf("%s\n",mysrv->h_name);
      }
      else if(strcmp(argv[i],"-u")==0)
      {
        element = argv[i+1]; //-------->PICK UP FROM HERE, assignment not working
        //printf("%s\n",host_req->h_name);
        memcpy(host_req,element,100);
      }
  }

    if(mysrv_portno == 0){
      fprintf(stderr, "ERROR, no port number specified\n");
    }
    if (mysrv == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
      }
    // if(host_req == NULL){
    //   fprintf(stderr,"ERROR, requested URL does not exist\n");
    //   exit(0);
    //   }


    cli_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_sock< 0)
        error("ERROR opening socket");

    bzero((char *) &mysrv_addr, sizeof(mysrv_addr));
    mysrv_addr.sin_family = AF_INET;
    bcopy((char *)mysrv->h_addr,
         (char *)&mysrv_addr.sin_addr.s_addr,
         mysrv->h_length);
    mysrv_addr.sin_port = htons(mysrv_portno);
    if (connect(cli_sock,(struct sockaddr *) &mysrv_addr,sizeof(mysrv_addr)) < 0)
        error("ERROR connecting");

    printf("Connected to the server at port %d host %s\n",mysrv_portno,mysrv->h_name);

  //  n = write(cli_sock,"GET /",strlen("GET / \n"));
    bzero(buffer,sizeof(buffer));

    //printf("Checkpoint 1 Passed\n");

    //Request to local server should be in following format:
    //"GET / <specific page/index> HTTP/1.0 \r\n Host: <hostname> \r\n\r\n"

    memcpy(buffer,host_req,sizeof(host_req));

    //printf("Checkpoint 2 Passed\n");

    n = send(cli_sock,buffer,sizeof(buffer),0);
    if(n < 0)
      error("ERROR writing to socket");

    //printf("Checkpoint 3 Passed\n");

    bzero(buffer,sizeof(buffer));

    while((n = recv(cli_sock,buffer,sizeof(buffer),0)) != 0)
    if (n < 0)
         error("ERROR reading from socket");

    //printf("%s\n",buffer);
    close(cli_sock);
    return 0;
}
