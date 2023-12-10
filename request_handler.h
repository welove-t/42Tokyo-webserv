#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>
#include <map>

void parse_request_line(const std::string &request, std::string &method, std::string &uri, std::map<std::string, std::string> &headers);
void parse_headers_and_body(const std::string& request, std::map<std::string, std::string>& headers, std::string& body);
void handle_request(int client_sockfd, const std::string& method, const std::string& uri, const std::string& body, std::string &response);

#endif // REQUEST_HANDLER_H
