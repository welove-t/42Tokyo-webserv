#include "server.h"
#include "cout_color.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

int create_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cerr << RED << "ソケットの作成に失敗しました。" << RESET << std::endl;
        return -1;
    }
    return sockfd;
}

/*
TCPサーバーを再起動したときに前回の実行で使用されたポートがすぐに再利用できるようにするための設定
これにより、サーバーを停止してすぐに再起動する際に、ポートが利用可能であることが保証される
*/
bool set_socket_options(int sockfd)
{
    int opt = 1; // オプション有効の意
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << RED <<  "ソケットオプションの設定に失敗しました。" << RESET << std::endl;
        return false;
    }
    return true;
}

bool set_non_blocking(int sockfd)
{
    // ソケットをノンブロッキングモードに設定
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << RED << "ノンブロッキング設定に失敗しました。" << RESET << std::endl;
        return false;
    }
    return true;
}

bool bind_and_listen(int sockfd)
{
    struct sockaddr_in addr; // サーバーのアドレス情報を保持するための構造体
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY; // サーバーが任意のインターフェースで接続を受け付ける(すべての利用可能なネットワークインターフェース)

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << RED << "バインドに失敗しました。" << RESET << std::endl;
        return false;
    }

    if (listen(sockfd, SOMAXCONN) < 0)
    {
        std::cerr << RED << "リスニングに失敗しました。" << RESET << std::endl;
        return false;
    }

    return true;
}
