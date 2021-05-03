
//  WebServer.cpp
//  webserv
//
//  Created by Shira Broderick on 09/03/2021.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "WebServer.hpp"

WebServer::WebServer() : _servers(std::vector<Server *>()), _clients(std::vector<Client *>()) {
}
WebServer::~WebServer() {}

std::vector<Server *> WebServer::getServer() const {
	return this->_servers;
}

void WebServer::setServer(Server * server) {
	for (size_t i = 0; i < _servers.size(); i++) {
		if (getServer().at(i)->getIp() == server->getIp() && getServer().at(i)->getPort() == server->getPort())
			printErrorAndExit("IP \"" + _servers[i]->getIp() + "\" and Port \"" + intToString(int(_servers[i]->getPort())) + "\" are already binded. Check config file", true, 1);
		else if (server->getIp().size() == 0)
			printErrorAndExit(server->getName() + " server has no IP", true, 1);
		else if (server->getPort() == 0)
			printErrorAndExit(server->getName() + " server has no port", true, 1);
	}
	this->_servers.push_back(server);
}

void WebServer::openListenSock() {
	for (size_t i = 0; i < _servers.size(); i++) {
		_servers.at(i)->setListenFd(socket(AF_INET, SOCK_STREAM, 0));
		int server_fd = _servers.at(i)->getListenFd();
		if (server_fd < 0)
			printErrorAndExit("Listen socket problem", true, 2);
		struct sockaddr_in addr;
		ft_memset((char *)&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(_servers.at(i)->getPort());
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		
		
		//MARK: --SOCKOPT
		int opt = 1;
		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		
		if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
			printErrorAndExit("Bind socket problem", true, 2);
		//MARK: - queue!!!
		if (listen(server_fd, 1024) < 0)
			printErrorAndExit("Listen socket problem", true, 3);
	}
}

void WebServer::startServer() {
	for (;;) {
		fd_set readFds, writeFds;
		FD_ZERO(&readFds);
		FD_ZERO(&writeFds);
		
		//add in readFD SERVERS listen-sockets
		for (size_t i = 0; i < _servers.size(); i++)
			FD_SET(_servers.at(i)->getListenFd(), &readFds);
		
		int maxFd = _servers.back()->getListenFd();
		
		//add in readFD CLIENTS sockets
		for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
			if ((*it)->getStatus() == responsing)
				FD_SET((*it)->getSocket(), &writeFds);
			else if ((*it)->getStatus() == requesting)
				FD_SET((*it)->getSocket(), &readFds);
				
			if ((*it)->getSocket() > maxFd)
				maxFd = (*it)->getSocket();
		}
		
		select(maxFd + 1, &readFds, &writeFds, NULL, NULL);
		
		// check if client already exists
		std::vector<Client *>::iterator it = _clients.begin();
		for (; it != _clients.end(); it++) {
			if (FD_ISSET((*it)->getSocket(), &readFds) || FD_ISSET((*it)->getSocket(), &writeFds)) {
				break;
			}
		}
		if (it == _clients.end()) {
			//find the socket(s) that triggered the event
			for (size_t i = 0; i < _servers.size(); i++) {
				if (FD_ISSET(_servers.at(i)->getListenFd(), &readFds)) {
					int newSocket;
					struct sockaddr_in addr;
					ft_memset((char *)&addr, 0, sizeof(addr));
					addr.sin_family = AF_INET;
					addr.sin_port = htons(_servers.at(i)->getPort());
					addr.sin_addr.s_addr = htonl(INADDR_ANY);
					
					//create socket for read/write
					if ((newSocket = accept(_servers.at(i)->getListenFd(), (struct sockaddr *)&addr, (socklen_t*)&addr)) > 0) {
						
						//non-block client socket
						fcntl(newSocket, F_SETFL, O_NONBLOCK);
						_clients.push_back(new Client(newSocket, _servers.at(i)));
					}
				}
			}
		}
		
		//check all clients sockets and read requests
		for (std::vector<Client *>::iterator client = _clients.begin(); client != _clients.end(); client++) {
			if (_clients.size() == 0)
				break;
			if (FD_ISSET((*client)->getSocket(), &readFds)) {
				readingData(*client);
				
				//close socket and erase client, when an error has occurred
				if ((*client)->getStatus() == closing) {
					close((*client)->getSocket());
					FD_CLR((*client)->getSocket(), &readFds);
					FD_CLR((*client)->getSocket(), &writeFds);
					delete *client;
					std::vector<Client *>::iterator tmp = client;
					if (tmp != _clients.begin()) {
						tmp--;
						_clients.erase(client);
						client = tmp;
					} else {
						_clients.erase(client);
						client = _clients.begin();
					}
				}
			}
			if (FD_ISSET((*client)->getSocket(), &writeFds)) {
				writingData(*client);
				
				//close socket and erase client, when an error has occurred, or client close connection
				if ((*client)->getStatus() == closing) {
					
					printEvent("Client closed connection", G_COL);
					close((*client)->getSocket());
					FD_CLR((*client)->getSocket(), &writeFds);
					FD_CLR((*client)->getSocket(), &readFds);
					delete *client;
					
					std::vector<Client *>::iterator tmp = client;
					if (tmp != _clients.begin()) {
						tmp--;
						_clients.erase(client);
						client = tmp;
					} else {
						_clients.erase(client);
						client = _clients.begin();
					}
				}
			}
		}
	}
}

