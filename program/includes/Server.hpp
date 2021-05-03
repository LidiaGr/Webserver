//
//  Server.hpp
//  webserv
//
//  Created by Shira Broderick on 09/03/2021.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp
#include "main.hpp"

class Location;

class Server {
private:
	std::string					_name;
	unsigned short int			_serv_port;
	std::string					_serv_ip;
	std::string					_root;
	std::string					_errorPage;
	std::vector<Location*>		_location;
	int							_listenFd;

public:
	Server();
	~Server();

	void	setName(std::string str);
	void 	setPort(std::string str);
	void	setRoot(std::string str, std::string path);
	void 	setErrorPage(std::string str, std::string path);
	void 	setIp(std::string str);
	void 	setLocation(Location* src);
	void 	setListenFd(int fd);

	std::string 					getName() const;
	unsigned short int 				getPort() const;
	std::string 					getIp() const;
	std::string 					getRoot() const;
	std::string 					getErrorPage() const;
	std::vector<Location*>	 		getLocation() const;
	int				 				getListenFd() const;
	
	void printData() const;
};

#endif /* Server_hpp */
