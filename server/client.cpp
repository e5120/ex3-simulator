#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <thread>

#define PORT      30000  // 変更 0-65535
#define BUF_SIZE  1024   // バッファのサイズ

static int myconnect(char* host, int port);
void       send_msg(int& fd);  // スレッド(メッセージ送信)


int main(int argc, char *argv[]) {
    int       fd, n;
    char      buf[BUF_SIZE];

    // 引数チェック
    if (argc != 2) {
        fprintf(stderr, "useage: %s host\n", argv[0]);
        return 1;
    }

    // サーバと接続
    if ((fd = myconnect(argv[1], PORT)) < 0) {
        return 1;
    }

    // サーバとのデータのやり取り
    recv(fd, buf, sizeof(buf),0);
    buf[strlen(buf)-1] = '\0';
    printf("%s\n", buf);

    std::thread t1(send_msg, std::ref(fd));
    // サーバからのデータ受信
    buf[0] = '\0';  // 初期化
    while(1) {
        if((n = recv(fd, buf, sizeof(buf), 0)) < 0){
            printf("受信エラー\n");
            break;
        }
        else if(n == 0){
            printf("正常終了\n");
            break;
        }
        buf[n] = '\0';
        printf("%s\n", buf);
    }
    t1.join();
    // 終了
    close(fd);
    return 0;
}

 // スレッド & メッセージ送信
void send_msg(int& fd) {
    char    buf[BUF_SIZE];
    // 情報
    usleep(1000*30);
    printf( "!--INFORMATION\n"
            "  To logout the room, input'LOGOUT'\n");

    while(1) {
        buf[0] = '\0';
        usleep(500*100);
        // printf("Input message: ");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf)-1] = '\0';
        if (strcmp(buf, "LOGOUT") == 0) {  // ログアウト
            send(fd, buf, sizeof(buf),0);
            printf("logouted.\nPlease push 'Ctrl+C'.\n");  // ログアウト確認
            return;
        }
        else {
            send(fd, buf, strlen(buf),0);
        }
    }
}


// サーバに接続して記述子を返す
static int myconnect(char* host, int port) {
    int                fd;
    struct sockaddr_in addr;
    struct hostent     *hp;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    if ((hp = gethostbyname(host)) == NULL) {
        fprintf(stderr, "gethost error %s\n", host);
        close(fd);
        return -1;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return -1;
    }
    return fd;
}
