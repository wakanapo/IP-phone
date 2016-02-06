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
#include <arpa/inet.h>

#define N 8800
#define PLAY_VOICE_PORT 10000
#define RECV_SOUND_PORT 30000 // serv's SEND_SOUND_PORT
#define SEND_SOUND_PORT 20000 // sev's RECV_SOUND_PORT

const char *ip = "192.168.11.12";
//const char *ip = "127.0.0.1";

int playVoice() {
  unsigned char data[N];

  struct sockaddr_in addr_child;
  addr_child.sin_family = AF_INET;
  addr_child.sin_addr.s_addr = inet_addr(ip);
  addr_child.sin_port = htons(PLAY_VOICE_PORT);

  int s = socket(PF_INET, SOCK_STREAM, 0);
  int ret = connect(s, (struct sockaddr*) &addr_child, sizeof(addr_child));
  if (ret < 0) {
    exit(EXIT_FAILURE);
  }
  FILE *fp = popen("play -q -t raw -b 16 -c 1 -e s -r 44100 -", "w");
  int n;
  while(1){
    // fprintf(stderr,"c recv\n");
    n = recv(s, data, N, 0);
    if (n == 0) {
      break;
    } else if (n < 0) {
      perror("c recv");
    }
    // printf("n: %d\n", n);
    fwrite(data, 1, n, fp);
  }
  fprintf(stderr,"c close\n");
  close(s);
  pclose(fp);

  return 0;
}

// int sendSound() {
//   fprintf(stderr, "hoge1\n");
//   printf("ko\n");
//   // non blocking
//   struct termios save_settings;
//   struct termios settings;

//   tcgetattr(0,&save_settings);
//   settings = save_settings;

//   settings.c_lflag &= ~(ECHO|ICANON); /* 入力をエコーバックしない、バッファリングしない */
//   settings.c_cc[VTIME] = 0;
//   settings.c_cc[VMIN] = 1;
//   tcsetattr(0,TCSANOW,&settings);
//   fcntl(0,F_SETFL,O_NONBLOCK); /* 標準入力からの読み込むときブロックしないようにする */
//   int n;
//   unsigned char data[N];
//   int ss = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

//   struct sockaddr_in addr;
//   addr.sin_family = AF_INET;
//   addr.sin_addr.s_addr = INADDR_ANY;
//   addr.sin_port = htons(SEND_SOUND_PORT);
//   bind(ss, (struct sockaddr *)&addr, sizeof(addr));
//   listen(ss, 10);

//   struct sockaddr_in client_addr;
//   socklen_t len = sizeof(struct sockaddr_in);
//   int s = accept(ss, (struct sockaddr *)&client_addr, &len);
//   close(ss);

//   /* main loop */
//   if(s > 0){
//     printf("kotunagatta\n");
//     while(1){
//       // printf("kopuro\n");
//       /* FILE *fp = popen("rec -q -t raw -b 16 -c 1 -e s -r 44100 -", "r"); */
//       /* FILE *fp2 = popen("play -q -t raw -b 16 -c 1 -e s -r 44100 -", "w"); */
//       /* FILE *fp3 = fopen("wakanapo.raw","rb"); */
//       /* printf("kofopen\n"); */
//       int c=getchar();
//       if(c > 0)
//         fprintf(stderr,"%d\n",c);
//       switch (c) {
//       case 'a':
//         printf("a\n");
//         FILE *fp3 = fopen("wakanapo.raw","rb");
//         printf("hogeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n");
//         if (fp3 == NULL) {
//           perror("fopen");
//           exit(EXIT_FAILURE);
//         }
//         while ((n = fread(data, 1, N, fp3)) > 0) {
//           printf("fread: %d\n", n);
//           int remaining = n;
//           while (remaining > 0)
//             remaining -= send(s, data, remaining, 0);
//         }
//         fclose(fp3);
//         break;
//       default:
//         break;
//       }
//     }
//   }
//   return 0;
// }

// int talk() {
//   unsigned char data[N], data2[N];

//   struct sockaddr_in addr_parent;
//   addr_parent.sin_family = AF_INET;
//   addr_parent.sin_addr.s_addr = inet_addr(ip);
//   addr_parent.sin_port = htons(TALK_PORT);

//   // static int counter = 0;
//   // printf("p socket: %d\n", counter++);
//   printf("接続中..\n");
//   int s = socket(PF_INET, SOCK_STREAM, 0);
//   if (s < 0) {
//     perror("socket");
//     exit(EXIT_FAILURE);
//   }
//   // printf("p connect\n");
//   int ret = connect(s, (struct sockaddr*) &addr_parent, sizeof(addr_parent));
//   if (ret < 0) {
//     exit(EXIT_FAILURE);
//   }
//   // printf("p popen\n");
//   FILE *fp = popen("rec -q -t raw -b 16 -c 1 -e s -r 44100 -", "r");
//   FILE *fp2 = popen("play -q -t raw -b 16 -c 1 -e s -r 44100 -", "w");
//   int n,n2, cnt;
//   printf("\r通話が開始しました\n");
//   while(1){
//     // fprintf(stderr,"p recv\n");
//     n2 = recv(s, data2, N, 0);
//     // fprintf(stderr,"p write\n");
//     if (n2 == 0) {
//       break;
//     } else if (n < 0) {
//       perror("c recv");
//     }
//     fwrite(data2, 1, n2, fp2);
//     // write(fileno(fp2), data2, n2);
//     // n = read(fileno(fp), data, N);
//     // fprintf(stderr,"p fread\n");
//     n=fread(data, 1, N, fp);
//     // fprintf(stderr,"p send\n");
//     send(s, data, n, 0);
//     cnt++;
//   }
//   fprintf(stderr,"p close");
//   close(s);
//   pclose(fp);
//   pclose(fp2);

//   return 0;
// }

int sendSound() {
  unsigned char data[N];

  struct sockaddr_in addr_parent;
  addr_parent.sin_family = AF_INET;
  addr_parent.sin_addr.s_addr = inet_addr(ip);
  addr_parent.sin_port = htons(SEND_SOUND_PORT);

  // static int counter = 0;
  // printf("p socket: %d\n", counter++);
  int s = socket(PF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  // printf("p connect\n");
  int ret = connect(s, (struct sockaddr*) &addr_parent, sizeof(addr_parent));
  if (ret < 0) {
    exit(EXIT_FAILURE);
  }
  // printf("p popen\n");
  FILE *fp = popen("rec -q -t raw -b 16 -c 1 -e s -r 44100 -", "r");
  int n;
  while(1){
    // fprintf(stderr,"p recv\n");
    // fprintf(stderr,"p write\n");
    // write(fileno(fp2), data2, n2);
    // n = read(fileno(fp), data, N);
    n = fread(data, 1, N, fp);
    send(s, data, n, 0);
  }
  fprintf(stderr,"p close");
  close(s);
  pclose(fp);

  return 0;
}

int recvSound() {
  unsigned char data[N];

  struct sockaddr_in addr_parent;
  addr_parent.sin_family = AF_INET;
  addr_parent.sin_addr.s_addr = inet_addr(ip);
  addr_parent.sin_port = htons(RECV_SOUND_PORT);

  // static int counter = 0;
  // printf("p socket: %d\n", counter++);
  // printf("接続中..\n");
  int s = socket(PF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  // printf("p connect\n");
  int ret = connect(s, (struct sockaddr*) &addr_parent, sizeof(addr_parent));
  if (ret < 0) {
    exit(EXIT_FAILURE);
  }
  // printf("p popen\n");
  FILE *fp = popen("play -q -t raw -b 16 -c 1 -e s -r 44100 -", "w");
  int n;
  // printf("\r通話が開始しました\n");
  while(1){
    // fprintf(stderr,"p recv\n");
    n = recv(s, data, N, 0);
    if (n == 0) {
      break;
    } else if (n < 0) {
      perror("c recv");
    }
    // fprintf(stderr,"p write\n");
    fwrite(data, 1, n, fp);
  }
  fprintf(stderr,"p close");
  close(s);
  pclose(fp);

  return 0;
}



int calculate() {
  char str[16];
  int num, i, prime_flag;
  while (1) {
    fgets(str, 16, stdin);
    num = atoi(str);

    prime_flag = 1;
    for (i = 2; i < num; i++) {
      if (num % i == 0) {
        prime_flag = 0;
        break;
      }
    }
    if (num > 1 && prime_flag == 1) {
      printf("%dは素数です！\n", num);
    } else {
      printf("%dは%dで割り切れます・・\n", num, i);
    }
  }
  return 0;
}

int timestamp() {
  int i, cnt = 1;
  int prime_flag;
  while (1) {
    sleep(2);
    printf("%d秒経過 ", cnt);
    prime_flag = 1;
    for (i = 3; i < cnt; i += 2) {
      if (cnt % i == 0) {
        prime_flag = 0;
      }
    }
    if (cnt != 1 && prime_flag == 1) {
      printf("素数です！\n");
    } else {
      printf("\n");
    }
    cnt += 2;
  }
}

int main(int argc, char** argv) {  // non blocking
  // struct termios save_settings;
  // struct termios settings;

  // tcgetattr(0,&save_settings);
  // settings = save_settings;

  // settings.c_lflag &= ~(ECHO|ICANON);  /* 入力をエコーバックしない、バッファリングしない */
  // settings.c_cc[VTIME] = 0;
  // settings.c_cc[VMIN] = 1;
  // tcsetattr(0,TCSANOW,&settings);
  // fcntl(0,F_SETFL,O_NONBLOCK);        /* 標準入力からの読み込むときブロックしないようにする */

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
    pid_t pid3;
    pid3 = fork();

    if (pid3 < 0) {
      fprintf (stderr, "fork error\n");
      exit(1);
    } else if (pid3 == 0) {
      return playVoice();
    } else {
      pid_t pid4;
      pid4 = fork();
      if (pid4 < 0) {
        exit(1);
      } else if (pid4 == 0) {
        return timestamp();
      } else {
        return calculate();
      }
    }
  }
}
