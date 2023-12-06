#include "server.h"
#include "request_handler.h"
#include "cout_color.h"
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

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
                std::cerr << RED << "クライアントからの接続受付に失敗しました。" << RESET << std::endl;
                continue;
            }
        }

        // HTTPレスポンスを送信
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        send(client_sockfd, response.c_str(), response.length(), 0);

        // クライアントとの通信処理
        char buffer[1024] = {0};
        ssize_t bytes_received = recv(client_sockfd, buffer, sizeof(buffer), 0);
        std::string request(buffer);
        parse_request(request);
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
