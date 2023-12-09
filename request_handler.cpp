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

// HTTPレスポンスの生成
std::string create_http_response(const std::string& status, const std::string& content, const std::string& content_type = "text/html") {
    std::string response = "HTTP/1.1 " + status + "\r\n";
    response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
    response += "Content-Type: " + content_type + "\r\n";
    response += "\r\n";
    response += content;
    return response;
}

// HTTPリクエストを処理する関数
void handle_request(const std::string& method, const std::string& uri, const std::map<std::string, std::string>& headers, const std::string& body) {
    std::string response;
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
    } else if (method == "DELETE") {
        // DELETEリクエストの処理
    }
    // レスポンスの送信（例: send関数の使用）
}

