//
//  Server.cpp
//  webserv
//
//  Created by Shira Broderick on 09/03/2021.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "../includes/main.hpp"

Server::Server() : _name(std::string("default")), _serv_port(0), _serv_ip(std::string()), _location(std::string()), _listenFd(-1) {};
Server::~Server() {};

void Server::setName(std::string str) {
	if (str.find(':') + 1 != str.length())
		this->_name = str.substr(str.find(':') + 1);
}

void Server::setPort(std::string str) {
	this->_serv_port = std::stoi(str.substr(str.find(':') + 1));
}

void Server::setIp(std::string str) {
	this->_serv_ip = str.substr(str.find(':') + 1);
}

void Server::setLocation(std::string str) {
	this->_location = str.substr(str.find(':') + 1);
}

void Server::setListenFd(int fd) {
	this->_listenFd = fd;
}


std::string Server::getName() const {
	return this->_name;
}
unsigned short int Server::getPort() const {
	return this->_serv_port;
};
std::string Server::getIp() const {
	return this->_serv_ip;
};
std::string Server::getLocation() const {
	return this->_location;
};
int Server::getListenFd() const {
	return this->_listenFd;
}



void Server::printData() const {
	std::cout << this->getName() << std::endl;
	std::cout << this->getPort() << std::endl;
	std::cout << this->getIp() << std::endl;
	std::cout << this->getLocation() << std::endl;
};
