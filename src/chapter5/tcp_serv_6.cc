
#include "./../lib/constant.h"
#include "./../lib/unp.h"

#include <signal.h>
#include <unistd.h>

typedef void SignalFunc(int);

SignalFunc* Signal(int signo, SignalFunc* func) {
    SignalFunc* sig_func;
    if ((sig_func = signal(signo, func)) == SIG_ERR) {
        LogErr("Signal error");
    }

    return (sig_func);
}

void SigChildWait(int signo) {
    pid_t pid;
    int stat;

    pid = wait(&stat);
    cout << "Child " << pid << " terminated" << endl;
    return;
}

// 字符串回射函数
void StrEcho(int sock_fd) {
    int n;
    char line[MAXLINE];

    for (;;) {
        if ((n = Readline(sock_fd, line, MAXLINE)) == 0) {
            return;
        }
        cout << "Received from client: " << line;
        Writen(sock_fd, line, n);
    }
}

int main(int argv, char* argc[]) {
    int conn_fd, listen_fd;
    struct sockaddr_in srv_addr;

    struct sockaddr_in cli_addr;
    pid_t child_pid;
    socklen_t cli_len;

    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&srv_addr, sizeof(srv_addr));

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(9876);

    Bind(listen_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    Listen(listen_fd, LISTENQ);

    Signal(SIGCHLD, SigChildWait);

    cout << "Server is running..." << endl;
    for (;;) {
        cli_len = sizeof(cli_addr);

        if ((conn_fd = accept(listen_fd, (struct sockaddr*)&cli_addr,
                              &cli_len)) < 0) {
            if (errno == EINTR)
                continue;
            else {
                LogErr("Accept error");
            }
        }

        if ((child_pid = Fork()) == 0) {
            Close(listen_fd);
            StrEcho(conn_fd);
            // Close(conn_fd);
            exit(0);
        }
        Close(conn_fd);
    }

    return 0;
}
