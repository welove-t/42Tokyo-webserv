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
        if (client_sockfd < 0){
            // ノンブロッキングモードで接続要求がない場合は、単にcontinueする
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else{
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
        if (bytes_received < 0) {
            std::cerr << RED << "recv failed with errno: " << errno << RESET << std::endl;
        }
        if (bytes_received > 0) {
            std::string request(buffer, bytes_received);
            std::string method, uri, body;
            std::map<std::string, std::string> headers;
            // リクエストラインの解析
            parse_request_line(request, method, uri, headers);

            // ヘッダーとボディの解析
            parse_headers_and_body(request, headers, body);

            // ヘッダーとボディの内容を表示（デバッグ用）
            for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
                std::cout << GREEN << it->first << ": " << it->second << RESET << std::endl;
            }
            std::cout << GREEN << "Body: " << body << RESET << std::endl;
            // handle_request関数の呼び出し
            handle_request(client_sockfd, method, uri, body, response);
        }
        // クライアントソケットを閉じる
        close(client_sockfd);
    }

    close(sockfd);
    return 0;
}
