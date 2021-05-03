//
//  cgiHandler.hpp
//  webserv
//
//  Created by Temple Tarsha on 3/29/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef cgiHandler_hpp
#define cgiHandler_hpp

#include "main.hpp"

char*			handleCgi(Client *client, std::string target, std::vector<std::string> request, int fd);
char**			createEnv(Client *client, std::string target, std::vector<std::string> request);
std::string		findContent(std::vector<std::string> src, std::string toFind);

#endif /* cgiHandler_hpp */
