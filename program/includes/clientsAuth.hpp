//
//  clientsAuth.hpp
//  webserv
//
//  Created by Temple Tarsha on 4/5/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef clientsAuth_hpp
#define clientsAuth_hpp

#include "main.hpp"

bool checkClient(std::string data, Client* client);
size_t needAccess(std::vector<std::string> request, std::string target, Client *client);

#endif /* clientsAuth_hpp */
