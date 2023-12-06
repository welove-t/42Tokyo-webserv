#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

const int PORT = 8080;
const int MAX_CONNECTIONS = 5;

int create_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cerr << "ソケットの作成に失敗しました。" << std::endl;
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
        std::cerr << "ソケットオプションの設定に失敗しました。" << std::endl;
        return false;
    }
    return true;
}

bool set_non_blocking(int sockfd)
{
    // ソケットをノンブロッキングモードに設定
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "ノンブロッキング設定に失敗しました。" << std::endl;
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
        std::cerr << "バインドに失敗しました。" << std::endl;
        return false;
    }

    if (listen(sockfd, SOMAXCONN) < 0)
    {
        std::cerr << "リスニングに失敗しました。" << std::endl;
        return false;
    }

    return true;
}

int main()
{
    int sockfd = create_socket();
    if (sockfd < 0)
        return -1;

    if (!set_socket_options(sockfd) || !set_non_blocking(sockfd))
    {
        close(sockfd);
        return -1;
    }

    if (!bind_and_listen(sockfd))
    {
        close(sockfd);
        return -1;
    }

    std::cout << "サーバーが起動し、ポート" << PORT << "で接続を待っています。" << std::endl;

    while (true)
    {
        int client_sockfd = accept(sockfd, NULL, NULL);
        if (client_sockfd < 0)
        {
            // ノンブロッキングモードで接続要求がない場合は、単にcontinueする
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else
            {
                std::cerr << "クライアントからの接続受付に失敗しました。" << std::endl;
                continue;
            }
        }

        // HTTPレスポンスを送信
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        send(client_sockfd, response.c_str(), response.length(), 0);

        // クライアントとの通信処理
        char buffer[1024] = {0};
        ssize_t bytes_received = recv(client_sockfd, buffer, sizeof(buffer), 0);
        if (bytes_received < 0) {
            // エラーハンドリング
        } else if (bytes_received == 0) {
            // クライアントが接続を閉じた
        } else {
            // リクエストデータの処理
            std::cout << "受信したリクエスト: " << buffer << std::endl;
            // 以下、レスポンス処理（省略）
        }
        // クライアントソケットを閉じる
        close(client_sockfd);
    }

    close(sockfd);
    return 0;
}
