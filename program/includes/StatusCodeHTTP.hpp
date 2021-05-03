//
//  StatusCodeHTTP.hpp
//  webserv
//
//  Created by Shira Broderick on 10/03/2021.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef StatusCodeHTTP_HPP
#define StatusCodeHTTP_HPP
#include "main.hpp"

class StatusCodeHTTP {
private:
	std::map<int, std::string> _codes;
	
	void setCodes();
	
public:
	StatusCodeHTTP();
	~StatusCodeHTTP();
	std::pair<int, std::string> getPair(int code) const;
	std::string getStatus(int code) const;
	std::string getCodeAndStatus(int code) const;
};

#endif
