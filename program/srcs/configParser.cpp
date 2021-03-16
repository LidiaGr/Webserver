//
//  configParser.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/9/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "../includes/main.hpp"

void configFileErrorHendler(size_t *stringNumber, std::string str) {
	std::cout << "Config error: string " << *stringNumber << " " << "\"" << str << "\"" <<  std::endl;
	exit(1);
}
//MARK: - dublicate variable?
Server *addNewServer(std::vector<std::string> conf, size_t *stringNumber) {
	Server *server = new Server();
	//MARK: create default port 8080!
	server->setPort("8080");
	bool isConfig = false;
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
		else if (!it->compare(0, 9, "location:") && isConfig == true) {
			it++;
			*stringNumber += 1;
			while (it->size() == 0) {
				*stringNumber += 1;
				it++;
			}
			server->setLocation(*it);
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

int 	confParser(WebServer* webserv) {
	int fd;
	char *cwd = getcwd(NULL, 0);
	char *path = ft_strjoin(cwd, "/program/data/conf");
	if ((fd = open(path, O_RDONLY)) < 0) {
		strerror(errno);
		exit(1);
	}
	free(cwd);
	free(path);
	char *buf = NULL;
	std::vector<std::string> conf;
	std::string str;
	size_t stringNumber = 1;
	while ((gnl(fd, &buf) > 0)) {
		str += buf;
		conf.push_back(str);
		if (*buf == '}') {
			webserv->setServer(addNewServer(conf, &stringNumber));
			conf.clear();
		}
		free(buf);
		str.clear();
	}
	if (buf)
		free(buf);
	if (webserv->getServer().size() == 0) {
		std::cout << "Config file is empty!" << std::endl;
		exit(1);
	}
	return 0;
}
