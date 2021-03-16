
//  WebServer.cpp
//  webserv
//
//  Created by Shira Broderick on 09/03/2021.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "../includes/WebServer.hpp"

WebServer::WebServer() : _servers(std::vector<Server *>()), _clients(std::vector<Client *>()) {
	_statCodes = new StatusCodeHTTP();
};
WebServer::~WebServer() { delete _statCodes; };

std::vector<Server *> WebServer::getServer() const {
	return this->_servers;
};

void WebServer::setServer(Server * server) {
	for (size_t i = 0; i < _servers.size(); i++) {
		if (getServer().at(i)->getIp() == server->getIp() && getServer().at(i)->getPort() == server->getPort()) {
			std::cout << "IP \"" << _servers[i]->getIp() << "\" and Port \"" << _servers[i]->getPort() << "\" are already binded. Check config file." << std::endl;
			exit(1);
		}
		else if (server->getIp().size() == 0) {
			std::cout << server->getName() << " server has no IP" << std::endl;
			exit(1);
		}
		else if (server->getPort() == 0) {
			std::cout << server->getName() << " server has no port" << std::endl;
			exit(1);
		}
	}
	this->_servers.push_back(server);
};

void WebServer::openListenSock() {
	for (size_t i = 0; i < _servers.size(); i++) {
		_servers.at(i)->setListenFd(socket(AF_INET, SOCK_STREAM, 0));
		int server_fd = _servers.at(i)->getListenFd();
		if (server_fd < 0) {
			strerror(errno);
			exit(1);
		}
		struct sockaddr_in addr;
		ft_memset((char *)&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(_servers.at(i)->getPort());	//FIXME: check if forbidden
		addr.sin_addr.s_addr = htonl(INADDR_ANY); 			//FIXME: check if forbidden
		
		
		//MARK: --SOCKOPT
		int opt = 1;
		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		
		if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			strerror(errno);
			exit(1);
		}
		if (listen(server_fd, 3) < 0) {
			strerror(errno);
			exit(EXIT_FAILURE);
		}
	}
};

void WebServer::startServer() {
	for (;;) {
		fd_set readFds, writeFds;
		FD_ZERO(&readFds);
		FD_ZERO(&writeFds);
		
		//add in readFD SERVERS listen-sockets
		for (size_t i = 0; i < _servers.size(); i++)
			FD_SET(_servers.at(i)->getListenFd(), &readFds);
		
		int maxFd = _servers.back()->getListenFd();
		
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 300000;
		
		//add in readFD CLIENTS sockets
		for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
			FD_SET((*it)->getSocket(), &readFds);
			if ((*it)->getStatus() == responsing)
				FD_SET((*it)->getSocket(), &writeFds);
			if ((*it)->getSocket() > maxFd)
				maxFd = (*it)->getSocket();
		}
		
		select(maxFd + 1, &readFds, &writeFds, NULL, &timeout);
		
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
					int flags = fcntl(newSocket, F_GETFL);
					fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
					
//					printEvent("add new client");
					_clients.push_back(new Client(newSocket, _servers.at(i)->getPort(), _servers.at(i)->getIp(), _servers.at(i)->getLocation()));
				}
			}
		}
		
		//check all clients sockets and read requests
		for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
			if (FD_ISSET((*it)->getSocket(), &readFds)) {
				
				
				char buffer[8000] = {0};
				long valread;
				valread = read((*it)->getSocket(), buffer, 8000);
				
				(*it)->setBuff(buffer);
				
				if ((*it)->getBuff().find("\r\n\r\n") != -1) {
					std::cout << (*it)->getBuff() << std::endl;
					(*it)->setResponse(handleRequestAndCreateResponse((*it)->getBuff(), (*it)->getLocation()));
					(*it)->setStatus(responsing);
				}
			}
			if (FD_ISSET((*it)->getSocket(), &writeFds)) {
				if ((*it)->getStatus() == responsing) {
					send((*it)->getSocket(), (*it)->getResponse(), ft_strlen((*it)->getResponse()) , 0);
					(*it)->setResponse(NULL);
					(*it)->setStatus(closing);
					(*it)->setBuff(NULL);
					
					//close socket when client recieve response and delete current client
					FD_CLR((*it)->getSocket(), &readFds);
					close((*it)->getSocket());
					it = _clients.erase(it);
					if (it == _clients.end())
						break;
				}
			}
		}
	}
}

std::string getHtmlBody(const char* location) {
	char* cwd = getcwd(NULL, 0);
	char* path = ft_strjoin(cwd, location + 1);
	free(cwd);
	int fd;
	if ((fd = open(path, O_RDONLY)) < 0) {
		strerror(errno);
		exit(1);
	}
	char buf[2048];
	read(fd, &buf, 2048);
	return buf;
};

std::string getCurrentDate() {  		//FIXME: check if forbidden
	struct timeval time;
	time_t t;
	struct tm *info;
	std::string res;
	gettimeofday(&time, DST_NONE);
	t = time.tv_sec;
	
	info = localtime(&t);
	return res += asctime(info);
}

char* allocateResponse(std::string response) {
	char* res = NULL;
	if ((res = (char*)malloc(sizeof(char) * (response.size() + 1))) == NULL) {
		strerror(errno);
		exit(1);
	}
	int i = 0;
	for (std::string::iterator it = response.begin(); it != response.end(); it++) {
		res[i] = *it;
		i++;
	}
	res[i] = '\0';
	return res;
}


std::string WebServer::requestSelector(std::vector<std::string> request, std::string location) {
	std::vector<std::string>::iterator it = request.begin();
	std::string res;
	if (!it->compare(0, 5, "HEAD ")) {
		res = handleGetAndHeadRequest(request, location);
		return ((res + "\r\n"));
	} else if (!it->compare(0, 4, "GET ")) {
		res = handleGetAndHeadRequest(request, location);
		res += getHtmlBody(location.c_str());
		return ((res + "\r\n"));
	} else {
		res = _statCodes->getCodeAndStatus(400);
		return ((res + "\r\n"));
	}
	return ("");
}

//FIXME: --- Походу надо все эти поля проинициализировать где-то при создании сервера!
//Возможно хранить всю эту муть по частям в готовых строках(е) и добавлять только то что изменяется
//Типа текущей даты и тд.
//Так как порядок хедеров не важен можно в конце добавлять все изменяемое

std::string WebServer::handleGetAndHeadRequest(std::vector<std::string> request, std::string location) {
	std::string res;
	res += "HTTP/1.1 " + _statCodes->getCodeAndStatus(200);
	res += "Server: webserv/1.0\r\n";
	res += "Date: " + getCurrentDate();
	res += "Host: localhost\r\n";
	res += "Accept-Charsets: utf-8\r\n";
	res += "Accept-Language: en-US\r\n\r\n";
	return (res);
}

char* WebServer::handleRequestAndCreateResponse(std::string str, std::string location) {
	StatusCodeHTTP code = StatusCodeHTTP();
	std::string res;
	size_t start = 0;
	size_t finish = 0;
	std::vector<std::string> request;
	if (str.size() > 8000)
		return (allocateResponse(code.getCodeAndStatus(414) + "\r\n"));
	while (str.size() >= (finish + 1)) {
		finish = str.find('\n');
		request.push_back(str.substr(start, finish + 1));
		str = str.substr(str.find('\n') + 1, str.size());
		finish = 0;
	}
	//MARK: added str.clear()
	
	if (request.back() == "\r\n") {
		res = requestSelector(request, location);
		str.clear();
		return (allocateResponse(res + "\r\n"));
	}
	
//	for (std::vector<std::string>::iterator it = request.begin(); it != request.end(); it++) {
//		if (!it->compare(0, 16, "GET / HTTP/1.1\r\n") && (request.back() == "\r\n")) {
//			res += "HTTP/1.1 " + code.getCodeAndStatus(200);
//			res += "Server: webserv/1.0\r\n";
//			res += "Date: " + getCurrentDate();
//			res += "Host: localhost\r\n";
//			res += "Accept-Charsets: utf-8\r\n";
//			res += "Accept-Language: en-US\r\n\r\n";
//			res += getHtmlBody(location.c_str());
//			str.clear();
//			return (allocateResponse(res + "\r\n"));
//		}
//	}
	str.clear();
	return (allocateResponse(code.getCodeAndStatus(501) + "\r\n"));
};
