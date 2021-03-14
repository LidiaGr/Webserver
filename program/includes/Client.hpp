//
//  Client.hpp
//  webserv
//
//  Created by Temple Tarsha on 3/11/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp

#include "main.hpp"

enum Status {requesting, responsing, waiting, closing};

class Client {
private:
	int					_socket;
	unsigned int short	_port;
	std::string			_ip;
	Status				_status;
	const char*			_response;
	std::string			_buff;
	std::string			_location;
	
	Client();
	
public:
	Client(int socket, unsigned int short port, std::string ip, std::string location);
	~Client();
	
	int						getSocket() const;
	unsigned int short		getPort() const;
	std::string				getIp() const;
	Status					getStatus() const;
	const char*				getResponse() const;
	std::string				getBuff() const;
	std::string 			getLocation() const;
	
	void					setStatus(Status status);
	void					setResponse(const char* str);
	void					setBuff(char* buff);
};

#endif /* Client_hpp */
