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
    int mysrv_sock, my_portno, n, h_only;
    struct sockaddr_in websrv_addr;
    struct hostent *websrv;
    char buffer[1000000]; char head_req[1024];
    FILE *fp;

    my_portno = 80; //original code
    mysrv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (mysrv_sock< 0)
        error("ERROR opening socket");
  websrv = gethostbyname(argv[1]); //original code
    if (websrv == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &websrv_addr, sizeof(websrv_addr));
    websrv_addr.sin_family = AF_INET;
    bcopy((char *)websrv->h_addr,
         (char *)&websrv_addr.sin_addr.s_addr,
         websrv->h_length);
    websrv_addr.sin_port = htons(my_portno);
    if(connect(mysrv_sock,(struct sockaddr *) &websrv_addr,sizeof(websrv_addr)) < 0)
        error("ERROR connecting");

    n = write(mysrv_sock,"GET / \n",strlen("GET / \n"));

    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,sizeof(buffer));

    fp = fopen("testing.txt","w");

    if (fp!= NULL)
    {
          while((n = read(mysrv_sock,buffer,sizeof(buffer))) != 0){

            fwrite(buffer,1,sizeof(buffer),fp);

          }

      fclose (fp);
    }

    close(mysrv_sock);
    return 0;
}
