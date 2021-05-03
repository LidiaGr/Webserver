//
//  configParser.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/9/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "main.hpp"

void configFileErrorHendler(size_t *stringNumber, std::string str) {
	printErrorAndExit("Config error: string " + intToString(int(*stringNumber)) + " " + "\"" + str + "\"", true, 1);
}

Server *addNewServer(std::vector<std::string> conf, size_t *stringNumber) {
	Server *server = new Server();
	bool isConfig = false;
	std::string path;
	char* cwd = getcwd(NULL, 0);
	path += cwd;
	free(cwd);
	for (std::vector<std::string>:: iterator it = conf.begin(); it != conf.end(); it++) {
		if (!it->compare(0, 1, "#"))
			;
		else if (!it->compare(0, 7, "server{") && isConfig == false)
			isConfig = true;
		else if (!it->compare(0, 7, "server{") && isConfig == true)
			configFileErrorHendler(stringNumber, *it);
		else if (!it->compare(0, 12, "server_name:") && isConfig == true)
			server->setName(*it);
		else if (!it->compare(0, 5, "port:") && isConfig == true)
			server->setPort(*it);
		else if (!it->compare(0, 3, "ip:") && isConfig == true)
			server->setIp(*it);
		else if (!it->compare(0, 11, "error_page:") && isConfig == true) {
			server->setErrorPage(*it, server->getRoot() + "/program/data");
		}
		else if (!it->compare(0, 9, "location:") && *(--(*it).end()) == '{' && isConfig == true) {
			// new parser for location field. we need it for CGI config
			Location* newOne = new Location();
			std::string path = (*it).substr((*it++).find(':') + 1);
			*stringNumber += 1;
			path.erase(path.end() - 1);
			newOne->setName(path);
			while (*it != "}") {
				if (!it->compare(0, 1, "#"))
					;
				else if (!it->compare(0, 5, "root:")) {
					char* path = getcwd(NULL, 0);
					newOne->setRoot(path + (*it).substr((*it).find(':') + 1));
					free(path);
				}
				else if (!it->compare(0, 6, "index:"))
					newOne->setIndex((*it).substr((*it).find(':') + 1));
				else if (!it->compare(0, 14, "allow_methods:")) {
					if ((*it).find("GET") != std::string::npos)
						newOne->_methods[0] = true;
					if ((*it).find("HEAD") != std::string::npos)
						newOne->_methods[1] = true;
					if ((*it).find("POST") != std::string::npos)
						newOne->_methods[2] = true;
					if ((*it).find("PUT") != std::string::npos)
						newOne->_methods[3] = true;
				}
				else if (!it->compare(0, 12, "autoindex:on"))
					newOne->setAutoindex(true);
				else if (!it->compare(0, 9, "cgi_pass:"))
					newOne->setCgi((*it).substr((*it).find(':') + 1));
				else if (!it->compare(0, 14, "cgi_extension:"))
					newOne->setExtension((*it).substr((*it).find(':') + 1));
				else if (!it->compare(0, 23, "limit_client_body_size:"))
					newOne->setLimit(std::stoi((*it).substr((*it).find(':') + 1)));
				else if (it->size() > 0)
					configFileErrorHendler(stringNumber, *it);
				else if (it->size() == 0)
					;
				it++;
				*stringNumber += 1;
			}
			server->setLocation(newOne);
		}
		else if (!it->compare(0, 1, "}") && isConfig == true) {
			isConfig = false;
			*stringNumber += 1;
			break ;
		}
		else if (it->size() > 0)
			configFileErrorHendler(stringNumber, *it);
		*stringNumber += 1;
	}
	return server;
}

int 	confParser(WebServer* webserv, char* fileName) {
	int brackets = 0;
	int fd;
	char *cwd = getcwd(NULL, 0);
	char *path;
	if (fileName == NULL)
		path = ft_strjoin(cwd, "/program/data/configs/conf");
	else {
		fileName = ft_strjoin("/", fileName);
		path = ft_strjoin(cwd, fileName);
		free(fileName);
	}
	if ((fd = open(path, O_RDONLY)) < 0)
		printErrorAndExit("Config open problem", true, 1);
	free(cwd);
	free(path);
	char *buf = NULL;
	std::vector<std::string> conf;
	std::string str;
	size_t stringNumber = 1;
	while ((gnl(fd, &buf) > 0)) {
		str += buf;
		if (str.find('{') != std::string::npos)
			brackets++;
		if (str.find('}') != std::string::npos)
			brackets--;
		conf.push_back(str);
		if (*buf == '}' && brackets == 0) {
			webserv->setServer(addNewServer(conf, &stringNumber));
			conf.clear();
		}
		free(buf);
		str.clear();
	}
	if (brackets != 0)
		printErrorAndExit("Invalid brackets in config file", true, 1);
	if (buf)
		free(buf);
	if (webserv->getServer().size() == 0)
		printErrorAndExit("Config file is empty!", true, 1);
	close(fd);
	return 0;
}
