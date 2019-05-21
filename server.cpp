/*
  * IPK Projekt 1
  * xsajdi00
  * server
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
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex.h>
#include <locale.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CONNECTIONS (42)
#define MAX_BUF_RECV (10000)

using namespace std;

string get_passwd(string login, int flag);
int server_start(char* port);

void help(){
  printf("--Try this: ./server -p [port]\n");
}

void ctrl_C(int signal){
  fprintf(stderr,"Shutting down server - SIGNINT\n");
  exit(0);
}

int main (int argc, char **argv){
  string name;
  signal(SIGINT, ctrl_C);
    if (argc != 3){
      fprintf(stderr,"--Wrong number of arguments\n");
      help();
      return 1;
    }

    char *port;
    if (strcmp(argv[1],"-p") == 0){
      port = argv[2];
    }else{
      fprintf(stderr,"--Wrong format of arguments\n");
      help();
      return 1;
    }


    return server_start(port);

}

int server_start(char* port){
  int my_socket;
  if((my_socket = socket(AF_INET, SOCK_STREAM,0)) < 0){
    fprintf(stderr,"Error in opening socket\n");
    return 1;
  }

  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(atoi(port));
  sin.sin_addr.s_addr  = INADDR_ANY; // all IP addresses
  if(bind(my_socket,(struct sockaddr *)&sin, sizeof(sin)) < 0){
    fprintf(stderr,"Error in binding socket\n");
    return 1;
  }

  if(listen(my_socket,MAX_CONNECTIONS) < 0){
    fprintf(stderr,"Error in listening - probably too many connections\n");
    return 1;
  }

  // Infinit loop until SIGINT
  int pom_socket;
  int sin_size = sizeof(sin);
  struct hostent *he;
  int size_recv;
  int buf_recv[2];
  string message;
  int flag;
  char recv_login[1000];
  message = "";
// TODO

  while(1){
    // waiting for request
    string pom_string;
    if((pom_socket = accept(my_socket, (struct sockaddr *)&sin, (socklen_t*)&sin_size)) < 0){
      fprintf(stderr,"Error did not accept connection\n");
      return 1;
    }

    pid_t process = fork();

    if(process == 0){
      he = (struct hostent *)gethostbyaddr((char *)&sin.sin_addr,sizeof((char *)&sin.sin_addr),AF_INET);//TODO
      // get flag
      if((size_recv = recv(pom_socket,buf_recv, sizeof(buf_recv),0)) > 0){
        flag = *buf_recv;
      }
      if(size_recv < 0){
        fprintf(stderr,"Error did not receive correct data\n");
        return(1);
      }


      if(flag / 10 ){
        if((size_recv = recv(pom_socket,recv_login, sizeof(recv_login),0)) > 0){
          message.append(recv_login,size_recv);

        }
        if(size_recv < 0){
          fprintf(stderr,"Error did not receive correct data\n");
          return(1);
        }
      }else {
        message = "";
      }


      while((pom_string = get_passwd(message, flag)) != ""){

        if (send(pom_socket, pom_string.c_str(), strlen(pom_string.c_str()), 0) < 0)
        {
          fprintf(stderr,"Error during sending results to client\n");
          return(1);
        }

        pom_string = "";
        break;

      }

      if(close(pom_socket) < 0){
        fprintf(stderr,"Error did not close socket\n");
        return 1;
      }

/*      if(pom_string == ""){
        if(send(pom_socket, pom_string.c_str(),strlen(pom_string.c_str()),0) < 0){
          fprintf(stderr,"Error during sending ending of results to client\n");
          return(1);
        }


      }*/ // end of sending data

    }else{
      if(process < 0){
        fprintf(stderr,"Error unable to create process\n");
        close(pom_socket);
        return 1;
      }
    }

  } // while(1)

  if(close(my_socket) < 0){
    fprintf(stderr,"Error did not close socket\n");
    return 1;
  }

return 0;
}

string get_passwd(string login , int flag) {
  string pom_login;
  string line;
  string result_to_return;
  string pokus = "swarog";
  char *pom;
  line = "";
  int i = 0;



//  transform(login.begin(), login.end(), login.begin(), ::tolower);
  struct passwd *pswd;
  if(flag == 12){
  //  cout << login.compare("swarog");
    setpwent();
    while((pswd = getpwent()) != NULL) {
      line = "";
      pom_login = pswd->pw_name;
      line.append(pom_login);
      if(line.compare(login) == 0 ) {
  //      printf("asdsdasds\n\n");
        result_to_return.append(pswd->pw_dir);
        break;
      }
    }

  //cout << result_to_return << endl;
  return result_to_return;
  }else
  if(flag == 11){
    setpwent();
    while((pswd = getpwent()) != NULL) {
      line = "";
      pom_login = pswd->pw_name;
      line.append(pom_login);
      if(line.compare(login) == 0 ) {
  //      printf("asdsdasds\n\n");
        result_to_return.append(pswd->pw_gecos);
        break;
      }
    }

  return result_to_return;
  }else
  if(flag == 13){
    setpwent();
    while((pswd = getpwent()) != NULL) {
      pom_login = pswd->pw_name;
      if(pom_login.find(login) == 0)
        result_to_return.append(pom_login + "\n");
    }

  return result_to_return;
  }else
  if(flag == 3){
    while((pswd = getpwent()) != NULL) {
      pom_login = pswd->pw_name;
      result_to_return.append(pom_login + "\n");
    }
  return result_to_return;
  }


  return "";
}
