//
//  reqestAndResponseUtils.hpp
//  webserv
//
//  Created by Temple Tarsha on 3/29/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef reqestAndResponseUtils_hpp
#define reqestAndResponseUtils_hpp

#include "main.hpp"

std::string		createBasicResponseHeaders();
std::string		createAllowMethodsStr(Location* src);
bool			isLocationExists(std::string target, Client *client, std::vector<std::string> request);
Location*		rightLocation(std::vector<Location*> locations, std::string target);
std::string		createPath(Location* src, std::string target, std::vector<std::string> request);
std::string 	cutTarget(Location* src, std::string target);
char*			allocateResponse(std::string response);

#endif /* reqestAndResponseUtils_hpp */
