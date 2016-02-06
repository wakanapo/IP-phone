//! gcc -o phone_serv3 phone_serv3.c -W -Wall
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#define N 8800
#define SEND_VOICE_PORT 10000
#define RECV_SOUND_PORT 20000
#define SEND_SOUND_PORT 30000

/*int playSound() {
  unsigned char data[N];

  struct sockaddr_in addr_child;
  addr_child.sin_family = AF_INET;
  addr_child.sin_addr.s_addr = INADDR_ANY;
  addr_child.sin_port = htons(PLAY_PORT);

  int s = socket(PF_INET, SOCK_STREAM, 0);
  if (s < 0) {
  perror("socket");
  exit(EXIT_FAILURE);
  }
  int ret = connect(s, (struct sockaddr*) &addr_child, sizeof(addr_child));
  if (ret < 0) {
  perror("playsound connect");
  exit(EXIT_FAILURE);
  }
  FILE *fp = popen("play -q -t raw -b 16 -c 1 -e s -r 44100 -", "w");
  int n;
  while(1){
  fprintf(stderr,"c recv\n");
  n = recv(s, data, N, 0);
  if (n == 0) {
  break;
  } else if (n < 0) {
  perror("c recv");
  return 1;
  }
  printf("n: %d\n", n);
  fwrite(data, 1, n, fp);
  }
  fprintf(stderr,"c close\n");
  close(s);
  pclose(fp);

  return 0;
  }*/

int file(FILE *fp, int s) {
  int n;
  unsigned char data[N];
  if (fp == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  n = fread(data, 1, N, fp);
  // printf("fread: %d\n", n);
  while (n > 0) {
    int remaining = n;
    while (remaining > 0)
      remaining -= send(s, data, remaining, 0);
    n = fread(data, 1, N, fp);
    // printf("fread: %d\n", n);
  }
  return 0;
}

int sendVoice() {
  // fprintf(stderr, "hoge1\n");
  // printf("ko\n");
  // non blocking
  struct termios save_settings;
  struct termios settings;

  tcgetattr(0,&save_settings);
  settings = save_settings;

  settings.c_lflag &= ~(ECHO|ICANON); /* 入力をエコーバックしない、バッファリングしない */
  settings.c_cc[VTIME] = 0;
  settings.c_cc[VMIN] = 1;
  tcsetattr(0,TCSANOW,&settings);
  fcntl(0,F_SETFL,O_NONBLOCK); /* 標準入力からの読み込むときブロックしないようにする */

  int ss = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (ss < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(SEND_VOICE_PORT);
  int rc = bind(ss, (struct sockaddr *)&addr, sizeof(addr));
  if (rc < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  rc = listen(ss, 10);
  if (rc < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  int s = accept(ss, (struct sockaddr *)&client_addr, &len);
  close(ss);
  if (s < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  /* main loop */
  // printf("kotunagatta\n");
  while(1){
	// printf("getchar\n");
    int c = getchar();
	// printf("%c",c);
    if(c > 0)
      // fprintf(stderr,"%c\n",c);
    switch (c) {
    case 'm':
      printf("m\n");
      FILE *fpm = fopen("morning.raw","rb");
      file(fpm, s);
      break;
       case 'n':
      printf("n\n");
      FILE *fpn = fopen("night.raw","rb");
      file(fpn, s);
      break;
       case 'u':
      printf("u\n");
      FILE *fpu = fopen("un.raw","rb");
      file(fpu, s);
      break;
       case 'h':
      printf("h\n");
      FILE *fph = fopen("heee.raw","rb");
      file(fph, s);
      break;
       case 's':
      printf("s\n");
      FILE *fps = fopen("sugoi.raw","rb");
      file(fps, s);
      break;
       case 'c':
      printf("c\n");
      FILE *fpc = fopen("cute.raw","rb");
      file(fpc, s);
      break;
       case 'l':
      printf("l\n");
       FILE *fpl = fopen("love.raw","rb");
      file(fpl, s);
      break;
       case 'k':
      printf("k\n");
      FILE *fpk = fopen("kiranaide.raw","rb");
      file(fpk, s);
      break;
       case 'w':
      printf("w\n");
      FILE *fpw = fopen("wait.raw","rb");
      file(fpw, s);
      break;
       case 'b':
      printf("b\n");
      FILE *fpb = fopen("byebye.raw","rb");
      file(fpb, s);
      break;
       case 'g':
      printf("g\n");
      FILE *fpg = fopen("gomen.raw","rb");
      file(fpg, s);
      break;
       case 'r':
      printf("r\n");
      FILE *fpr = fopen("really.raw","rb");
      file(fpr, s);
      break;
        case 'x':
      printf("x\n");
      FILE *fpx = fopen("noise.raw","rb");
      file(fpx, s);
      break;
    default:
      break;
    }
  }
  return 0;
}


int recvSound() {
  // fprintf(stderr, "hoge2\n");
  // printf("oya\n");
  // fflush(stdout);
  unsigned char data[N];
  int ss = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.as_addr = INADDR_ANY;
  addr.sin_port = htons(RECV_SOUND_PORT);
  bind(ss, (struct sockaddr *)&addr, sizeof(addr));
  listen(ss, 10);
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  // fprintf(stderr, "plen\n");
  int s = accept(ss, (struct sockaddr *)&client_addr, &len);
  // fprintf(stderr, "paccept\n");
  close(ss);
  int n;
  // fprintf(stderr,"a");
  // fprintf(stderr,"%d\n",s);
  if(s > 0){
    FILE *fp = popen("play -q -t raw -b 16 -c 1 -e s -r 44100 -", "w");
    // fprintf(stderr, "popen\n");
    while (1) {
      n=recv(s, data, N, 0);
      if (n == 0) {
        break;
      } else if (n < 0) {
        perror("recvSound");
      }
      /* printf("oyauketottayo: %d\n", n2); */
      fwrite(data, 1, n, fp);
      /* printf("oyasaiseisitayo: %d\n", n2); */
    }
    close(s);
    pclose(fp);
  }
  return 0;
}

int sendSound() {
  // fprintf(stderr, "hoge2\n");
  // printf("oya\n");
  // fflush(stdout);
  unsigned char data[N];
  int ss = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(SEND_SOUND_PORT);
  bind(ss, (struct sockaddr *)&addr, sizeof(addr));
  listen(ss, 10);
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  // fprintf(stderr, "plen\n");
  int s = accept(ss, (struct sockaddr *)&client_addr, &len);
  // fprintf(stderr, "paccept\n");
  close(ss);
  int n;
  // fprintf(stderr,"a");
  // fprintf(stderr,"%d\n",s);
  if(s > 0){
    FILE *fp = popen("rec -q -t raw -b 16 -c 1 -e s -r 44100 -", "r");
    // fprintf(stderr, "popen\n");
    while (1) {
      n=fread(data, 1, N, fp);
      /* printf("oyayondayo: %d\n", n); */
      send(s, data, n, 0);
      /* printf("oyaokuttayo: %d\n", n); */
    }
    close(s);
    pclose(fp);
  }
  return 0;
}


int main(){
//  pid_t pid = fork();
//  if (pid < 0) {
//    perror("fork");
//    exit(-1);
//  }
//  /* else if (pid == 0) {
//     pid_t pid = fork();
//     if (pid < 0) {
//     perror("fork");
//     exit(-1);
//     } */
//  else if (pid == 0) {//コプロせす鮮度
//    return subVoiceStream();
//  }
//  /* else { // 子プロセス再生
//     return playSound();
//     }
//     } */
//  else { // 親プロセス
//    return mainStream();
//  }
  pid_t pid;
  pid = fork();

  if ( pid < 0 ) {
    fprintf (stderr, "fork error\n");
    exit(1);
  } else if (pid == 0) {
    pid_t pid2;
    pid2 = fork();

    if (pid2 < 0) {
      fprintf (stderr, "fork error\n");
      exit(1);
    } else if (pid2 == 0) {
      return recvSound();
    } else {
      return sendSound();
    }
  } else {
    return sendVoice();
  }

}
found 0 associations
found 1 connections:
     1:	flags=82<CONNECTED,PREFERRED>
	outif en0
	src 192.168.11.2 port 52957
	dst 192.168.11.12 port 50000
	rank info not available
	TCP aux info available

Connection to 192.168.11.12 port 50000 [tcp/*] succeeded!
