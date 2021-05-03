//
//  requestHandler.hpp
//  webserv
//
//  Created by Temple Tarsha on 3/29/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef requestHandler_hpp
#define requestHandler_hpp

#include "main.hpp"

char*			getHtmlBody(Client *client, std::string target, std::vector<std::string> request);
std::string		handleNotAllowedRequest(Client *client, Location* src);
std::string		handleGetAndHeadRequest(std::vector<std::string> request, std::string target, Client *client);
std::string		handlePutAndPostRequest(std::vector<std::string> request, std::string target, Client *client);
std::string		requestSelector(std::vector<std::string> request, Client *client);
char*			handleRequestAndCreateResponse(Client *client);

#endif /* requestHandler_hpp */
