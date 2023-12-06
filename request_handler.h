#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>
#include <map>

void parse_request(const std::string &request);
void parse_headers_and_body(const std::string& request, std::map<std::string, std::string>& headers, std::string& body);

#endif // REQUEST_HANDLER_H
