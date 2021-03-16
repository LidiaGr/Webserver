//
//  WebServer.hpp
//  webserv
//
//  Created by Shira Broderick on 09/03/2021.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef WebServer_hpp
#define WebServer_hpp

#include "main.hpp"

class Client;
class StatusCodeHTTP;

class WebServer {
protected:
	std::vector<Server *>	_servers;
	std::vector<Client *>	_clients;
	StatusCodeHTTP 			*_statCodes;

public:
	WebServer();
	~WebServer();
	
	std::vector<Server *> getServer() const;
	void setServer(Server * server);
	
	void openListenSock();
	void startServer();
	char* handleRequestAndCreateResponse(std::string str, std::string location);
	
private:
	std::string requestSelector(std::vector<std::string> request, std::string location);
	std::string handleGetAndHeadRequest(std::vector<std::string> request, std::string location);
};

#endif /* WebServer_hpp */
