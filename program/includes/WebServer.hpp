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

class Server;
class Client;
class StatusCodeHTTP;

class WebServer {
protected:
	std::vector<Server *>	_servers;
	std::vector<Client *>	_clients;

public:
	WebServer();
	~WebServer();
	
	std::vector<Server *> getServer() const;
	void setServer(Server * server);
	
	void openListenSock();
	void startServer();
};

#endif /* WebServer_hpp */
