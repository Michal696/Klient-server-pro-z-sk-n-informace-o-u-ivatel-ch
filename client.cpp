/*
  * IPK Projekt 1
  * xsajdi00
  * client
  */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;


#define MAX_BUF_RECV (10000)

int client_start(char* host, char* port, int flag, string login);

void help(){
  printf("--Try this: ./client -h host -p port [-n|-f|-l] login\n");
}

int main (int argc, char **argv){

  char* host;
  char* port;
  string login;
  int flag = 0;

//  printf("%d\n",argc);//TODO
  // potrebn0 argumenty
  if(argc > 7){
    fprintf(stderr,"--Too many arguments\n");
    help();
    exit(1);
  }
  if(argc > 5){
    if(strcmp(argv[1],"-h") == 0){
      host = argv[2];
  //    printf("%s\n",host);//TODO
    } else {
      fprintf(stderr,"--First argument is wrong\n");
      exit(1);
    }

    if(strcmp(argv[3],"-p") == 0){
      port = argv[4];
  //    printf("%s\n",port); //TODO
    } else {
      fprintf(stderr,"--Third argument is wrong\n");
      exit(1);
    }
  } else{
    fprintf(stderr,"--You need more arguments\n");
    help();
    exit(1);
  }
  // volnitelne argumenty
  if(argc > 5){
    if(strcmp(argv[5],"-n") == 0){
      if(argc != 7){
        fprintf(stderr,"--You need login too\n");
        help();
        exit(1);
      }
      flag = 1;
    }else
    if(strcmp(argv[5],"-f") == 0){
      if(argc != 7){
        fprintf(stderr,"--You need login too\n");
        help();
        exit(1);
      }
      flag = 2;
    }else
    if(strcmp(argv[5],"-l") == 0){
      flag = 3;
    }else{
      fprintf(stderr,"--Wrong 5. argument\n");
      help();
      exit(1);
    }
    if(argc == 7){
      login = argv[6];
      flag = flag + 10; // +10 indikace loginu pomoci flagu pre druhe prijate spravy
    }
//    cout << login.find("s")  << endl;
  }



  // client part

  return client_start(host,port,flag,login);

  //return 0;
}

int client_start(char* host, char* port, int flag, string login){
  int my_socket;

  if((my_socket = socket(AF_INET,SOCK_STREAM,0)) < 0){
    fprintf(stderr,"Error in opening socket\n");
    return(1);
  }
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(atoi(port));

  struct hostent *hptr;

  if((hptr = gethostbyname(host)) == NULL){
    fprintf(stderr,"Error did not get hostname\n");
    return(1);
  }

  memcpy(&sin.sin_addr, hptr->h_addr, hptr->h_length);
  if(connect(my_socket,(struct sockaddr *)&sin, sizeof(sin)) < 0){
    fprintf(stderr,"Error did not connect\n");
    return(1);
  }

  int *buf_send;
  buf_send = &flag;

  if(send(my_socket,buf_send,sizeof(buf_send),0) < 0){
    fprintf(stderr,"Error did not send a message\n");
    return(1);
  }

  if(flag / 10){
    if(send(my_socket,login.c_str(),login.length(),0) < 0){
      fprintf(stderr,"Error did not send a message\n");
      return(1);
    }
  }

  char buf_recv[MAX_BUF_RECV];
  int size_recv = 0;
  string message;
  message = "";


//  while
    if((size_recv = recv(my_socket,buf_recv,sizeof(buf_recv),0)) > 0){
    message.append(buf_recv,size_recv);
    if(message.compare("") == 0){
  //    printf("\n\nbreak\n\n"); //TODO del
    //  break;
    }
    printf("%s\n",message.c_str());
    message = "";
  }



  if(size_recv < 0){
    fprintf(stderr,"\nError did not receive correct data\n");
    return(1);
  }


  if(close(my_socket) < 0){
    fprintf(stderr,"Error did not close socket\n");
    return 1;
  }


return 0;
}
