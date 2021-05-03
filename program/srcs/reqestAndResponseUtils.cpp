//
//  reqestAndResponseUtils.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/29/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "reqestAndResponseUtils.hpp"

std::string createBasicResponseHeaders() {
	std::string res;
	res += "Server: webserv/1.0\r\n";
	res += "Date: " + getCurrentDate() + "\r\n";
	res += "Accept-Charsets: utf-8\r\n";
	res += "Accept-Language: en-US\r\n";
	return res;
}

std::string createAllowMethodsStr(Location* src) {
	std::string res;
	res += "Allow: ";
	int i = 0;
	std::string method[4] = {"GET", "HEAD", "POST", "PUT"};
	if (src)
		while(i < 4) {
			if (src->_methods[i] == true)
				res += method[i] + " ";
			i++;
		}
	res += "\r\n";
	return res;
}

bool isLocationExists(std::string target, Client *client, std::vector<std::string> request) {
	Location *src = rightLocation(client->getServer()->getLocation(), target);
	std::string path = createPath(src, target, request);
	int fd = 0;
	if ((fd = open(path.c_str(), O_RDONLY)) > 0) {
		close(fd);
		return true;
	}
	return false;
}

Location* rightLocation(std::vector<Location*> locations, std::string target) {
	Location* src = NULL;
	bool defaultTarget = true;
	for (size_t id = 0; id != locations.size(); id++){
		src = locations.at(id);
		if ((target.find(src->getName()) != std::string::npos) && (src->getName() != "/")) {
			defaultTarget = false;
			break;
		}
	}
	if (defaultTarget == true)
		for (size_t i = 0; i != locations.size(); i++) {
			src = locations.at(i);
			if (src->getName() == "/")
				break;
		}
	return src;
}

std::string cutTarget(Location* src, std::string target) {
	size_t id = target.find(src->getName());
	std::string tmpTarget = target;
	if (src->getName() != "/") {
		size_t start = id + src->getName().size();
		tmpTarget.clear();
		tmpTarget = target.substr(start, target.size() - start);
	}
	return tmpTarget;
}

std::string createPath(Location* src, std::string target, std::vector<std::string> request) {
	std::string tmpTarget = cutTarget(src, target);
	
	// create path
	std::string path;
	std::string cwd = src->getRoot();
	
	if (target == "/" || tmpTarget.empty())
		path = cwd + "/" + src->getIndex();
	else if (tmpTarget.back() == '/' && tmpTarget.find('.') == std::string::npos)
		path = cwd + tmpTarget + src->getIndex();
	else if (tmpTarget.find('.') == std::string::npos && request.at(0).find("GET") != std::string::npos)
		path = cwd + tmpTarget + "/" + src->getIndex();
	else
		path = cwd + tmpTarget;
	
	return path;
}

char* allocateResponse(std::string response) {
	char* res = NULL;
	if ((res = (char*)malloc(sizeof(char) * (response.size() + 1))) == NULL)
		printErrorAndExit("Allocate response problem", true, 1);
	int i = 0;
	for (std::string::iterator it = response.begin(); it != response.end(); it++) {
		res[i] = *it;
		i++;
	}
	res[i] = '\0';
	return res;
}
