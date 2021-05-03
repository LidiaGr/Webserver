//
//  clientsAuth.cpp
//  webserv
//
//  Created by Temple Tarsha on 4/5/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "clientsAuth.hpp"

bool checkClient(std::string data, Client* client) {
	//cut data
	size_t val = 0;
	std::string authType = data.substr(0, (val = data.find(" ")));
	data = data.substr(val + 1);
	data.pop_back();
	data.pop_back();
	
	//check access
	std::vector<std::string> users = client->getAuth();
	for (size_t i = 0; i < users.size(); i++) {
		if (data == users.at(i)) {
			std::cout << "Access garanted" << std::endl;
			return true;
		}
	}
	return false;
}

size_t needAccess(std::vector<std::string> request, std::string target, Client *client) {
	bool access = false;
	if (target == "/private_data" || target == "/private_data/private.html" ) {
		std::cout << "Need access" << std::endl;
		for (size_t i = 0; i < request.size(); i++) {
			if (request.at(i).find("Authorization:") != std::string::npos) {
				access = checkClient(request.at(i).substr(15), client);
				if (access == true)
					return AUTH_ACCEPTED;
			}
		}
		return NEED_AUTH;
	}
	return NOT_NEED_AUTH;
}

