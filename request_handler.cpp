#include "request_handler.h"
#include "cout_color.h"
#include <sstream>
#include <iostream>

// リクエストの解析
void parse_request(const std::string &request) {
    std::istringstream request_stream(request);
    std::string request_line;
    std::getline(request_stream, request_line);

    // リクエストラインの解析
    std::istringstream request_line_stream(request_line);
    std::string method;
    std::string uri;
    std::string http_version;
    request_line_stream >> method >> uri >> http_version;

    // ヘッダーの解析
    std::string header_line;
    while (std::getline(request_stream, header_line) && header_line != "\r") {
        // ヘッダーの処理（例: "Host: localhost:8080"）
        // ...
    }

    // ボディの処理（POSTリクエストの場合）
    // ...

    // リクエスト情報の出力（デバッグ用）
    std::cout << GREEN << "Method: " << method << ", URI: " << uri << ", Version: " << http_version << RESET << std::endl;
}
