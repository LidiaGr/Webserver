//
//  Server.cpp
//  webserv
//
//  Created by Shira Broderick on 09/03/2021.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "Server.hpp"

Server::Server() : _name(std::string("default")), _serv_port(8080), _serv_ip(std::string()), _errorPage(std::string("/notFound.html")),  _location(std::vector<Location*>()), _listenFd(-1) {
	char* str = getcwd(NULL, 0);
	_root = str;
	free(str);
}

Server::~Server() {}

void Server::setName(std::string str) {
	if (_name != "default")
		printErrorAndExit("Dublicated field \"server_name\" in config file", true, 1);
	else if (str.find(':') + 1 != str.length())
		this->_name = str.substr(str.find(':') + 1);
}

void Server::setPort(std::string str) {
	if (_serv_port != 8080)
		printErrorAndExit("Dublicated field \"port\" in config file", true, 1);
	this->_serv_port = std::stoi(str.substr(str.find(':') + 1));
}

void Server::setIp(std::string str) {
	if (_serv_ip != "")
		printErrorAndExit("Dublicated field \"ip\" in config file", true, 1);
	this->_serv_ip = str.substr(str.find(':') + 1);
}

void Server::setRoot(std::string str, std::string path) {
	char* tmp = getcwd(NULL, 0);
	std::string res = tmp;
	free(tmp);
	if (_root != res)
		printErrorAndExit("Dublicated field \"root\" in config file", true, 1);
	path += str.substr(str.find(':') + 1);
	this->_root = path;
}

void Server::setErrorPage(std::string str, std::string path) {
	if (_errorPage != "/notFound.html")
		printErrorAndExit("Dublicated field \"error_page\" in config file", true, 1);
	path += str.substr(str.find(':') + 1);
	this->_errorPage = path;
}

void Server::setLocation(Location* src) {
	_location.push_back(src);
}

void Server::setListenFd(int fd) {
	this->_listenFd = fd;
}


std::string Server::getName() const {
	return this->_name;
}
unsigned short int Server::getPort() const {
	return this->_serv_port;
}
std::string Server::getIp() const {
	return this->_serv_ip;
}
std::string Server::getRoot() const {
	return this->_root;
}
std::string Server::getErrorPage() const {
	return this->_errorPage;
}
std::vector<Location*> Server::getLocation() const {
	return this->_location;
}
int Server::getListenFd() const {
	return this->_listenFd;
}


void Server::printData() const {
	std::cout << this->getName() << std::endl;
	std::cout << this->getPort() << std::endl;
	std::cout << this->getIp() << std::endl;
}
