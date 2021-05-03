//
//  autoIndexPageCreator.cpp
//  webserv
//
//  Created by Frenica Torn on 4/3/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "main.hpp"

char* autoIndexPageCreator(std::string dir, Client *client, std::string target) {
	DIR *dirp = opendir(dir.c_str());
	if (dirp == NULL)
		return NULL;
	std::string page;
	page += "<!DOCTYPE html>\n<html lang=en>\n<meta charset=utf-8>\n<head>\n";
	page += "</head>\n<body>\n";
	while (dirent *res = readdir(dirp)) {
		page += "<a href='";
		if (target.back() == '/')
			page += target;
		else
			page += target + "/";
		page += res->d_name;
		page += "'>";
		DIR *check = opendir((dir + "/" + res->d_name).c_str());
		if (check != NULL) {
			closedir(check);
			page += res->d_name;
			page += "/";
		}
		else
			page += res->d_name;
		page += "</a><br>\n";
	}
	closedir(dirp);
	page += "</body>";
	client->setResponseSizeBody(page.size());
	char* res = (char*)malloc(sizeof(char) * page.size() + 1);
	for (size_t i = 0; i < page.size(); i++) {
		res[i] = page.at(i);
	}
	res[page.size()] = '\0';
	return res;
}

//entered 	:^  |=  |+  | |а  |б  |в  |г  |д
//received  :%5E|%3D|%2B|+|%E0|%E1|%E2|%E3|%E4
