//
//  requestHandler.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/29/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "requestHandler.hpp"

StatusCodeHTTP _statCodes;

char* getHtmlBody(Client *client, std::string target, std::vector<std::string> request) {
	int fd = -1;
	
	Location *src = rightLocation(client->getServer()->getLocation(), target);
	
	std::string cwd = createPath(src, target, request);
		
	fd = open(cwd.c_str(), O_RDONLY);
	char tmp;
	if (read(fd, &tmp, 1) == -1) {
		close(fd);
		fd = -1;
	}
	else
		lseek(fd, 0, SEEK_SET);
	
	if (fd == -1) {
		DIR *isDir;
		cwd = cwd.substr(0, cwd.find_last_of("/"));
		if (src->getAutoindex() == true && (isDir = opendir(cwd.c_str())) != NULL) {
			closedir(isDir);
			return autoIndexPageCreator(src->getRoot() + target, client, target);
		}
		if ((fd = open(client->getServer()->getErrorPage().c_str(), O_RDONLY)) < 0)
			printErrorAndExit("Error page open problem in GetHtmlBody", true, 1);
	}
	
	// read file
	struct stat buffer;
	if (fstat(fd, &buffer) == -1)
		printErrorAndExit("Fstat problem in GetHtmlBody", true, 1);
	ssize_t size = buffer.st_size;
	char* buf = (char*)malloc(sizeof(char) * size);
	ssize_t res;
	if ((res = read(fd, buf, size)) == -1)
		printErrorAndExit("Read problem in GetHtmlBody", false, 1);
	
	client->setResponseSizeBody(res);
	client->setRequestBody(buf);
	
	// CGI
	if (src->getCgi() != "" && request.size() > 0 && target.find(src->getExtension()) != std::string::npos) {
		free(buf);
		return handleCgi(client, target, request, fd);
	}
	
	close(fd);
	return buf;
}

std::string handleNotAllowedRequest(Client *client, Location* src) {
	std::string res = "HTTP/1.1 ";
	res += _statCodes.getCodeAndStatus(405);
	res += createBasicResponseHeaders();
	res += createAllowMethodsStr(src);
	
	res += "Content-Length: ";
	res += intToString(int(client->getResponseSizeBody()));
	res += BODY_SEPARATOR;
	client->setResponseSizeHead(res.size());
	return res;
}

std::string handleGetAndHeadRequest(std::vector<std::string> request, std::string target, Client *client) {
	std::string res;
	int code = 200;
	if (!isLocationExists(target, client, request))
		code = 404;
	size_t access = needAccess(request, target, client);
	if (access == NEED_AUTH) {
		code = 401;
		client->setNeedAuth(NEED_AUTH);
	}
	res += "HTTP/1.1 " + _statCodes.getCodeAndStatus(code);
	res += createBasicResponseHeaders();
	if (code == 401 && access == NEED_AUTH) {
		res += "WWW-Authenticate: Basic";
		res += BODY_SEPARATOR;
		client->setResponseSizeHead(res.size());
		client->setResponseSizeBody(0);
		return res;
	}
	
	if (request.at(0).find("GET") != std::string::npos) {
		char* tmp = getHtmlBody(client, target, request);
		res += "Content-Length: ";
		res += intToString(int(client->getResponseSizeBody()));
		res += BODY_SEPARATOR;
		client->setResponseSizeHead(res.size());
		res.insert(res.end(), tmp, tmp + client->getResponseSizeBody());
		free(tmp);
	} else {
		res += BODY_SEPARATOR;
		client->setResponseSizeHead(res.size());
		client->setResponseSizeBody(0);
	}
	return res;
}

std::string handlePutAndPostRequest(std::vector<std::string> request, std::string target, Client *client) {
	Location* src = rightLocation(client->getServer()->getLocation(), target);
	
	int code = 200;
	if (!isLocationExists(target, client, request))
		code = 201;
	size_t access = needAccess(request, target, client);
	if (access == NEED_AUTH) {
		code = 401;
		client->setNeedAuth(NEED_AUTH);
	}
	if (src->getLimit() != -1 && (int)client->getRequestBody().size() > src->getLimit())
		code = 413;
	
	std::string path;
	std::string tmpTarget = cutTarget(src, target);
	if (tmpTarget.empty() == true)
		path = createPath(src, target, request);
	else
		path = src->getRoot() + tmpTarget;
	
	int fd = -1;
	if (code != 413) {
		if ((fd = open(path.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644)) < 0)
			printErrorAndExit("Open problem in handlePutAndPostRequest", true, 1);
		if (write(fd, client->getRequestBody().c_str(), client->getRequestBody().size()) == -1) {
			printErrorAndExit("Write problem in file (handlePutAndPostRequest)", false, 1);
		}
	}
	
	std::string res;
	res += "HTTP/1.1 " + _statCodes.getCodeAndStatus(code);
	res += createBasicResponseHeaders();
		
	if (code == 401 && access == NEED_AUTH) {
		res += "WWW-Authenticate: Basic";
		res += BODY_SEPARATOR;
		client->setResponseSizeHead(res.size());
		client->setResponseSizeBody(0);
		return res;
	}

	char *tmp = NULL;
	if (code != 413) {
		if (src->getCgi() != "" && request.size() > 0 && target.find(src->getExtension()) != std::string::npos)
			tmp = handleCgi(client, target, request, fd);
		else
			close(fd);
	}
	res += "Content-Length: ";
	if (tmp) {
		res += intToString(int(client->getResponseSizeBody()));
		res += BODY_SEPARATOR;
		client->setResponseSizeHead(res.size());
		res.insert(res.end(), tmp, tmp + client->getResponseSizeBody());
		free(tmp);
	} else {
		res += intToString(int(client->getRequestBody().size()));
		res += BODY_SEPARATOR;
		client->setResponseSizeHead(res.size());
		res += client->getRequestBody();
		client->setResponseSizeBody(client->getRequestBody().size());
	}
	return res;
}

std::string requestSelector(std::vector<std::string> request, Client *client) {
	std::string res;
	std::string str = request.front();
	std::string method = splitStr(&str);
	std::string target = splitStr(&str);
	target = decodeSpec(target);
	std::string protocol = splitStr(&str);
	Location* src = rightLocation(client->getServer()->getLocation(), target);
	
	if (protocol != "HTTP/1.1\r\n" || !str.empty())
		return handleNotAllowedRequest(client, NULL);
	if (method == "GET" && src->_methods[0] == true)
		res = handleGetAndHeadRequest(request, target, client);
	else if (method == "HEAD" && src->_methods[1] == true)
		res = handleGetAndHeadRequest(request, target, client);
	else if (method == "POST" && src->_methods[2] == true)
		res = handlePutAndPostRequest(request, target, client);
	else if (method == "PUT" && src->_methods[3] == true)
		res = handlePutAndPostRequest(request, target, client);
	else
		return handleNotAllowedRequest(client, src);
//	MARK: - The code below is never used with "allow_methods" implementation
//	else if (method == "DELETE" || method == "CONNECT" || method == "OPTIONS"
//			 || method == "TRACE")
//		return _statCodes->getCodeAndStatus(501);
	return res;
}

char* handleRequestAndCreateResponse(Client *client) {
	//create request vector
	size_t start = 0;
	size_t finish = 0;
	std::vector<std::string> request;
	std::string str = client->getRequestHead() + client->getRequestBody();
	while (str.size() >= (finish + 1)) {
		if ((finish = str.find('\n')) == std::string::npos)
			break;
		request.push_back(str.substr(start, finish + 1));
		str = str.substr(str.find('\n') + 1, str.size());
		finish = 0;
	}
	
	std::string res;
	res = requestSelector(request, client);
	return (allocateResponse(res));
}
