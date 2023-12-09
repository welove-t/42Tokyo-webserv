#include "request_handler.h"
#include "cout_color.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>

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
void parse_request(const std::string &request) {
    std::istringstream request_stream(request);
    std::string request_line;
    std::getline(request_stream, request_line);

    std::istringstream request_line_stream(request_line);
    std::string method, uri, http_version;
    request_line_stream >> method >> uri >> http_version;

    std::map<std::string, std::string> headers;
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

    std::cout << GREEN << "Method: " << method << ", URI: " << uri << ", Version: " << http_version << RESET << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

// HTTPリクエストを処理する関数
void handle_request(const std::string& method, const std::string& uri, const std::map<std::string, std::string>& headers, const std::string& body) {
    std::string response;
    if (method == "GET") {
        // 静的ファイルのパスを決定（例: "static/" + uri）
        std::string file_path = "static" + uri;  // "static"は静的ファイルが格納されているディレクトリ

        // ファイルのオープン
        std::ifstream file(file_path.c_str(), std::ios::in | std::ios::binary);
        if (file) {
            // ファイル内容を読み込む
            std::ostringstream file_content;
            file_content << file.rdbuf();
            file.close();

            // レスポンスの生成
            response = "HTTP/1.1 200 OK\r\n";
            response += "Content-Length: " + std::to_string(file_content.str().size()) + "\r\n";
            response += "Content-Type: text/html\r\n";  // 適切なContent-Typeを設定
            response += "\r\n";
            response += file_content.str();
        } else {
            // ファイルが見つからない場合のエラーレスポンス
            response = "HTTP/1.1 404 Not Found\r\n";
            response += "Content-Length: 0\r\n";
            response += "\r\n";
        }
    } else if (method == "POST") {
        // POSTリクエストの処理
        // フォームデータの受け取り、APIエンドポイントへの応答など
    } else if (method == "DELETE") {
    // その他のHTTPメソッドに対する処理もここに追加

    // レスポンスの生成と送信
    // std::string response = "HTTP/1.1 200 OK\r\nContent-Length: ... \r\n\r\n[レスポンスボディ]";
    // レスポンスの送信
}
