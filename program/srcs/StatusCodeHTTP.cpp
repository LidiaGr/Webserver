//
//  StatusCodeHTTP.cpp
//  webserv
//
//  Created by Shira Broderick on 10/03/2021.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "StatusCodeHTTP.hpp"

StatusCodeHTTP::StatusCodeHTTP() {
	_codes = std::map<int, std::string>();
	setCodes();
}

StatusCodeHTTP::~StatusCodeHTTP() {
	_codes.clear();
}

void StatusCodeHTTP::setCodes() {
	_codes[100] = "Continue";
	_codes[101] = "Switching Protocols";
	_codes[102] = "Processing";
	_codes[103] = "Early Hints";
	
	_codes[200] = "OK";
	_codes[201] = "Created";
	_codes[202] = "Accepted";
	_codes[203] = "Non-Authoritative Information";
	_codes[204] = "No Content";
	_codes[205] = "Reset Content";
	_codes[206] = "Partial Content";
	_codes[207] = "Multi-Status";
	_codes[208] = "Already Reported";
	_codes[226] = "IM Used";
	
	_codes[300] = "Multiple Choices";
	_codes[301] = "Moved Permanently";
	_codes[302] = "Found";
	_codes[303] = "See Other";
	_codes[304] = "Not Modified";
	_codes[305] = "Use Proxy";
	_codes[307] = "Temporary Redirect";
	_codes[308] = "Permanent Redirect";
	
	_codes[400] = "Bad Request";
	_codes[401] = "Unauthorized";
	_codes[402] = "Payment Required";
	_codes[403] = "Forbidden";
	_codes[404] = "Not Found";
	_codes[405] = "Method Not Allowed";
	_codes[406] = "Not Acceptable";
	_codes[407] = "Proxy Authentication Required";
	_codes[408] = "Request Timeout";
	_codes[409] = "Conflict";
	_codes[410] = "Gone";
	_codes[411] = "Length Required";
	_codes[412] = "Precondition Failed";
	_codes[413] = "Payload Too Large";
	_codes[414] = "URI Too Long";
	_codes[415] = "Unsupported Media Type";
	_codes[416] = "Range Not Satisfiable";
	_codes[417] = "Expectation Failed";
	_codes[418] = "I’m a teapot";
	_codes[419] = "Authentication Timeout";
	_codes[421] = "Misdirected Request";
	_codes[422] = "Unprocessable Entity";
	_codes[423] = "Locked";
	_codes[424] = "Failed Dependency";
	_codes[425] = "Too Early";
	_codes[426] = "Upgrade Required";
	_codes[428] = "Precondition Required";
	_codes[429] = "Too Many Requests";
	_codes[431] = "Request Header Fields Too Large";
	_codes[449] = "Retry With";
	_codes[451] = "Unavailable For Legal Reasons";
	_codes[499] = "Client Closed Request";
	
	_codes[500] = "Internal Server Error";
	_codes[501] = "Not Implemented";
	_codes[502] = "Bad Gateway";
	_codes[503] = "Service Unavailable";
	_codes[504] = "Gateway Timeout";
	_codes[505] = "HTTP Version Not Supported";
	_codes[506] = "Variant Also Negotiates";
	_codes[507] = "Insufficient Storage";
	_codes[508] = "Loop Detected";
	_codes[509] = "Bandwidth Limit Exceeded";
	_codes[510] = "Not Extended";
	_codes[511] = "Network Authentication Required";
	_codes[520] = "Unknown Error";
	_codes[521] = "Web Server Is Down";
	_codes[522] = "Connection Timed Out";
	_codes[523] = "Origin Is Unreachable";
	_codes[524] = "A Timeout Occurred";
	_codes[525] = "SSL Handshake Failed";
	_codes[526] = "Invalid SSL Certificate";
}

std::pair<int, std::string> StatusCodeHTTP::getPair(int code) const {
	std::pair<int, std::string> result;
	std::map<int, std::string>::const_iterator it;
	it = _codes.find(code);
	if (it == _codes.end()) {
		result.first = -1;
		result.second = "Bad status code/ Not found";
	} else
		result = *it;
	return result;
}

std::string StatusCodeHTTP::getStatus(int code) const {
	return getPair(code).second;
}

std::string StatusCodeHTTP::getCodeAndStatus(int code) const {
	std::string result;
	std::pair<int, std::string> pair = getPair(code);
	result += intToString(pair.first);
	result += " ";
	result += pair.second;
	return result + "\r\n";
}
