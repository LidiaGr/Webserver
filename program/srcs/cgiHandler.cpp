//
//  cgiHandler.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/29/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "cgiHandler.hpp"

std::string 	findContent(std::vector<std::string> src, std::string toFind)
{
	std::string res = std::string();
	for (size_t i = 0; i < src.size(); i++) {
		if (src.at(i).find(toFind) != std::string::npos) {
			size_t j = src.at(i).find(':') + 2;
			res = src.at(i).substr(j, src.at(i).size() - j);
			res.pop_back();
			return res;
		}
	}
	return res;
}

bool fieldFinder(std::vector<std::string> request, std::string field) {
	for (std::vector<std::string>::iterator it = request.begin(); it != request.end(); it++) {
		if ((*it).find(field) != std::string::npos)
			return true;
	}
	return false;
}

char** envConvertor(std::vector<std::string> src) {
	char** env;
	env = (char**)malloc(sizeof(char*) * src.size() + 1);
	int i = 0;
	for (std::vector<std::string>::iterator it = src.begin(); it != src.end(); it++) {
		env[i] = (char*)malloc(sizeof(char) * (*it).size() + 1);
		ft_strlcpy(env[i], (*it).c_str(), (*it).size() + 1);
		i++;
	}
	env[i] = NULL;
	return env;
}

std::vector<std::string> envCreator(Client *client, std::string target, std::vector<std::string> request) {
	std::vector<std::string> res;
	Location* cgiLocation = rightLocation(client->getServer()->getLocation(), target);
	Location* rootLocation = rightLocation(client->getServer()->getLocation(), "/");
	res.insert(res.end(), "AUTH_TYPE=basic");
	res.insert(res.end(), "GATEWAY_INTERFACE=CGI/1.1");
	res.insert(res.end(), "SERVER_NAME=localhost");
	res.insert(res.end(), "SERVER_PORT=" + intToString(client->getServer()->getPort()));
	res.insert(res.end(), "SERVER_PROTOCOL=HTTP/1.1");
	res.insert(res.end(), "SERVER_SOFTWARE=webserv/1.0");
	res.insert(res.end(), "REQUEST_URI=" + target);
	res.insert(res.end(), "REQUEST_METHOD=" + request.at(0).substr(0, request.at(0).find(' ')));
	res.insert(res.end(), "CONTENT_LENGTH=" + intToString(int(client->getRequestBody().size())));
	res.insert(res.end(), "CONTENT_TYPE=" + findContent(request, "Content-Type:"));
	res.insert(res.end(), "PATH_INFO=" + target);
	res.insert(res.end(), "PATH_TRANSLATED=" + rootLocation->getRoot() + cgiLocation->getCgi() + "/");
	res.insert(res.end(), "SCRIPT_NAME=" + cgiLocation->getCgi().substr(cgiLocation->getCgi().find_last_of('/') + 1));
	res.insert(res.end(), "REMOTE_ADDR=" + findContent(request, "Host:"));
	res.insert(res.end(), "REMOTE_IDENT=" + findContent(request, "Remote-ident:"));
	res.insert(res.end(), "REMOTE_USER=" + findContent(request, "Remote-user:"));
	if (target.find("?") != std::string::npos)
		res.insert(res.end(), "QUERY_STRING=" + target.substr(target.find("?")));
	else
		res.insert(res.end(), "QUERY_STRING=/");
	if (fieldFinder(request, "X-Secret-Header-For-Test") == true)
		res.insert(res.end(), "HTTP_X_SECRET_HEADER_FOR_TEST=1");
	return res;
}

char* handleCgi(Client *client, std::string target, std::vector<std::string> request, int fd) {
	int origFds[2];
	origFds[0] = dup(0);
	origFds[1] = dup(1);
	
	Location* rootLocation = rightLocation(client->getServer()->getLocation(), "/");
	Location* cgiLocation = rightLocation(client->getServer()->getLocation(), target);
	char **env = envConvertor(envCreator(client, target, request));
//	*** PRINT ENV ***
//	int i = 0;
//	while(env[i])
//		std::cout << env[i++] << std::endl;
	char* res = NULL;
	lseek(fd, 0, SEEK_SET);
	int status;
	
	std::string fileName = rootLocation->getRoot() + "/trash/tmp_";
	fileName += intToString(client->getTrashFileID());
	int fdNew;
	if ((fdNew = open(fileName.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644)) < 0)
		printErrorAndExit("Open problem in handleCGI", true, 1);
	
	std::string path = rootLocation->getRoot() + cgiLocation->getCgi();
	pid_t pid;
	pid = fork();
	if (pid == 0) {
		dup2(fd, 0);
		dup2(fdNew, 1);
		execve(path.c_str(), NULL, env);
		exit(0);
	}
	else if (pid == -1)
		printErrorAndExit("Fork problem", true, 1);
	else {
		waitpid(pid, &status, WUNTRACED);
		lseek(fdNew, 0, SEEK_SET);
		struct stat buffer;
		if (fstat(fdNew, &buffer) == -1)
			printErrorAndExit("Fstat problem in handleCGI", true, 1);
		ssize_t size = buffer.st_size;
		res = (char*)ft_calloc(size, sizeof(char));
		
		char tmp[1000];
		if (read(fdNew, tmp, 1000) < 0)
			printErrorAndExit("Read problem in tmp handleCGI", false, 2);
		size_t pos = cStrFind(tmp, "\r\n\r\n", size);
		if (pos != std::string::npos) {
			pos += 4;
			lseek(fdNew, pos, SEEK_SET);
			size -= pos;
		}
		else {
			pos = 0;
			lseek(fdNew, pos, SEEK_SET);
		}
		if ((read(fdNew, res, size)) == -1)
			printErrorAndExit("Read problem in handleCGI", false, 1);
		client->setResponseSizeBody(size);
		
	}
	close(fd);
	close(fdNew);
	close(origFds[0]);
	close(origFds[1]);
	freeDoubleArray(env);
	return res;
}
