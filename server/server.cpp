#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <thread>
#include <vector>

#define PORT        30000  // 変更 0-65535
#define MAX_CLIENT  2     // 接続要求を受け付ける数
#define BUF_SIZE    1024   // バッファのサイズ

// メンバーの情報を格納
typedef struct {
    int  fd;
    int  no;
} Mem;

Mem member[MAX_CLIENT];

static int open_server(int port);
void       thr(Mem& mem);
void       send_msg(char* buf);
void       send_msg(char* buf,int fd);


int main() {
    int fd;
    std::vector<std::thread> t1(MAX_CLIENT);
    for (int i = 0; i < MAX_CLIENT; ++i) {
        member[i].fd = 0;
    }
    printf("Press 'Ctrl+C' to end.\n");
    // サーバ準備
    if ((fd = open_server(PORT)) < 0) {
        return 1;
    }
    // クライアントの接続を待つ
    for (int i = 0; i < MAX_CLIENT; ++i) {
        printf("waiting for connection of client No.%d\n", i);
        if ((member[i].fd = accept(fd, NULL, NULL)) < 0) {
            perror("accept");
            close(fd);
            return 1;
        }
        printf("client_fd: %d come\n", member[i].fd);
        member[i].no = i;
        std::thread t(thr,std::ref(member[i]));
        t1[i] = std::move(t);
    }
    for(int i = 0; i < MAX_CLIENT; ++i){
        t1[i].join();
    }
    close(fd);
    return 0;
}

// スレッド
void thr(Mem& mem)
{
    char buf[BUF_SIZE];
    int  n;

    //sprintf(buf, "id:%dが参加しました.\n", mem.fd);
    //send_msg(buf);

    //sprintf(buf, "If you want to exit,please input 'LOGOUT'.\n");
    //send(mem.fd, buf, strlen(buf),0);

    while(1) {
        while ((n = recv(mem.fd, buf, sizeof(buf), 0)) < 0) {
            printf("受信エラー\n");
        }
        buf[n-1] = '\0';
/*        if (strcmp(buf, "LOGOUT") == 0) {
            sprintf(buf, "id:%d が退室しました.\n", mem.fd);
            printf("%s",buf);
            send_msg(buf);
            return;
        }*/
        printf("id:%d>%s\n", mem.fd, buf);
        send_msg(buf, mem.fd);
        //send_msg(buf);
    }
}


// メッセージ送信部(全体)
void send_msg(char *buf) {
    for (int i = 0; i < MAX_CLIENT; ++i) {
        if (member[i].fd != 0) {
            send(member[i].fd, buf, strlen(buf),0);
        }
    }
}

// メッセージ送信部(自分以外)
void send_msg(char *buf, int fd) {
    for (int i = 0; i < MAX_CLIENT; ++i) {
        if (member[i].fd != 0 && member[i].fd != fd) {
            send(member[i].fd, buf, strlen(buf),0);
        }
    }
}

// サーバソケットをオープンして記述子を返す
static int open_server(int port) {
    int                fd;
    int                opt;
    struct sockaddr_in addr;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(port);

    opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }
    if (listen(fd, MAX_CLIENT) < 0) {
        perror("listen");
        close(fd);
        return -1;
    }
    return fd;
}
