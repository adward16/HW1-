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
    int mysrv_sock, my_portno, n, written, host_sock, client, h, local_port, i, numWrite, p;
    struct sockaddr_in websrv_addr, serv_addr, cli_addr;
    struct hostent *websrv;
    socklen_t clilen;
    char buffer[1000000]; char url_req[1000], h_path[500], host[500], h_buffer[1000], response[100];
    FILE *fp;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    local_port = atoi(argv[1]); //original code
    host_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (host_sock < 0)
        error("ERROR opening socket");


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(local_port);


    if (bind(host_sock, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR on binding");

    printf("Waiting for clients\n");

    while(1){
      listen(host_sock,5);
      clilen = sizeof(cli_addr);

      client = accept(host_sock,(struct sockaddr *) &cli_addr,&clilen);
      if (client < 0)
           error("ERROR on accept");

      printf("A client has connected\n");

      if((h = read(client,h_buffer,sizeof(h_buffer))) < 0){
        printf("ERROR occured in reading from buffer \n");
        close(client);
        continue;
      }
      // {
      //
      //   memcpy(url_req,h_buffer,sizeof(h_buffer));
      //
      // }

      printf("Checkpoint 1 Passed %d\n", h);

      printf("something\n");
  /****** configutation for socket connection to web server ******/

      my_portno = 80; //original code
      mysrv_sock = socket(AF_INET, SOCK_STREAM, 0);
      if (mysrv_sock< 0)
          error("ERROR opening socket");

    // websrv = gethostbyname(argv[1]); //original code
    //   if (websrv == NULL) {
    //       fprintf(stderr,"ERROR, no such host\n");
    //       exit(0);
    //   }

      for(i = 0; i < strlen(h_buffer); i++){
        if(h_buffer[i] == '/'){
          strncpy(host,h_buffer,i);
          host[i] = '\0';
          break;
        }
      }

      printf("The host is %s \n", host);

      for(i; i<strlen(h_buffer); i++){
        strcat(h_path,&h_buffer[i]);
        break;
      }

      printf("The path is %s\n",h_path);

      printf("Checkpoint 2 Passed\n");

      websrv = gethostbyname(host); //-------> getting stuck here, websrv is still NULL
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

      //n = write(mysrv_sock,"GET / HTTP/1.1 \r\n",strlen("GET / HTTP/1.1 \r\n"));

      //may want to us snprintf per Milad
      sprintf(url_req,"GET %s HTTP/1.0\r\nHost:%s\r\n\r\n",h_path,host);

      n = write(mysrv_sock,url_req,strlen(url_req));

      if(n < 0)
           error("ERROR writing to socket"); //change this to format of error checking on lines 57-60
      bzero(buffer,sizeof(buffer));

      printf("Checkpoint 3 Passed\n");

      fp = fopen("testing.txt","wb+");

      if (fp!= NULL)
      {

        printf("Checkpoint 4 Passed\n");

        while((n = read(mysrv_sock,buffer,sizeof(buffer))) != 0){
          // printf("Number of bytes read: %d\n",n);


          //check bytes in buffer for "\r\n\r\n" sequence
          numWrite = fwrite(buffer,1,sizeof(buffer),fp);
          //may need to change buffer to exact memory location of where data starts
          //maybe need to change size of buffer to subtract size of first couple bytes?

        }


        fclose(fp);

        sprintf(response,"Successful write to file of %d bytes\n",numWrite);

        write(client,response,strlen(response));

        close(client);

      }
      close(mysrv_sock);
    }
    return 0;
}
