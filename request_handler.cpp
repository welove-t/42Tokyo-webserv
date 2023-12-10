#include "request_handler.h"
#include "cout_color.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/socket.h>

void parse_headers_and_body(const std::string& request, std::map<std::string, std::string>& headers, std::string& body) {
    std::istringstream request_stream(request);
    std::string line;
    bool header_section = true;

    while (std::getline(request_stream, line) && line != "\r") {
        if (header_section) {
            std::size_t delimiter_pos = line.find(": ");
            if (delimiter_pos != std::string::npos) {
                std::string key = line.substr(0, delimiter_pos);
                std::string value = line.substr(delimiter_pos + 2);
                headers[key] = value;
            } else {
                header_section = false;  // 空行に達した
            }
        } else {
            body += line + "\n";
        }
    }
}

// リクエストの解析
void parse_request_line(const std::string &request, std::string &method, std::string &uri, std::map<std::string, std::string> &headers) {
    std::istringstream request_stream(request);
    std::string request_line;
    std::getline(request_stream, request_line);

    std::istringstream request_line_stream(request_line);
    std::string http_version;
    request_line_stream >> method >> uri >> http_version;
    headers.clear();
    std::string header_line;
    while (std::getline(request_stream, header_line) && header_line != "\r" && header_line != "\n") {
        std::size_t delimiter_pos = header_line.find(": ");
        if (delimiter_pos != std::string::npos) {
            std::string header_key = header_line.substr(0, delimiter_pos);
            std::string header_value = header_line.substr(delimiter_pos + 2);
            headers[header_key] = header_value;
        }
    }

    // POSTリクエストの場合、ボディを読み取る処理がここに必要
    // std::cout << GREEN << "Method: " << method << ", URI: " << uri << ", Version: " << http_version << RESET << std::endl;
    // for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
    //     std::cout << it->first << ": " << it->second << std::endl;
    // }
}

// HTTPレスポンスの生成
std::string create_http_response(const std::string& status, const std::string& content, const std::string& content_type = "text/html") {
    std::string response = "HTTP/1.1 " + status + "\r\n";
    response += "Connection: keep-alive\r\n";
    response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
    response += "Content-Type: " + content_type + "\r\n";
    response += "\r\n";
    response += content;
    return response;
}

// POSTリクエスト処理
void handle_post_request(const std::string& body) {
    // POSTリクエストボディの処理
    // ここではシンプルにボディをコンソールに出力
    std::cout << "Received POST data: " << body << std::endl;

    // 実際には、フォームデータの解析、データベースへの保存、APIへの応答などの処理をここに実装
}

// レスポンス送信処理
void send_response(int client_sockfd, const std::string& response) {
    // レスポンスをクライアントに送信
    ssize_t bytes_sent = send(client_sockfd, response.c_str(), response.size(), 0);
    if (bytes_sent < 0) {
        // 送信エラーの処理
        std::cerr << "Error sending response to client." << std::endl;
    }
}

// HTTPリクエストを処理する関数
void handle_request(int client_sockfd, const std::string& method, const std::string& uri, const std::string& body, std::string &respons) {
    std::string response;
    (void) respons;
    if (method == "GET") {
        std::string file_path = "static" + uri;
        std::ifstream file(file_path.c_str(), std::ios::in | std::ios::binary);
        if (file) {
            std::ostringstream file_content;
            file_content << file.rdbuf();
            file.close();
            response = create_http_response("200 OK", file_content.str());
        } else {
            response = create_http_response("404 Not Found", "", "text/plain");
        }
    } else if (method == "POST") {
        // POSTリクエストの処理
        handle_post_request(body);
        response = create_http_response("200 OK", "POST request processed");
    } else if (method == "DELETE") {
        // DELETEリクエストの処理
    }
    // レスポンスの送信
    std::cout << BLUE << "response: " << response << RESET << std::endl;
    send_response(client_sockfd, response);
}

