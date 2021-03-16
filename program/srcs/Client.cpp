//
//  Client.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/11/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "Client.hpp"

Client::Client(int socket, unsigned int short port, std::string ip, std::string location) : _socket(socket), _port(port), _ip(ip), _status(requesting), _response(NULL), _buff(std::string()), _location(location) {};

Client::~Client() {};

int					Client::getSocket() const { return this->_socket; };
unsigned int short	Client::getPort() const { return this->_port; };
std::string			Client::getIp() const { return this->_ip; };
Status				Client::getStatus() const { return this->_status; };
char*				Client::getResponse() const { return this->_response; };
std::string			Client::getBuff() const { return this->_buff; };
std::string			Client::getLocation() const { return this->_location; };

void	Client::setStatus(Status status) { _status = status; }
void	Client::setResponse(char* str) {
	if (str)
		_response = str;
	else {
		free(_response);
		_response = NULL;
	}
}
void	Client::setBuff(char* buff) {
	if (!buff)
		_buff.clear();
	else
		_buff += buff;
}
