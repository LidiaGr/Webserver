//
//  Location.hpp
//  webserv
//
//  Created by Frenica Torn on 3/24/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef Location_hpp
#define Location_hpp

#include "main.hpp"

class Location {
private:
	std::string _name;
	std::string _root;
	std::string _index;
	std::string _cgi;
	std::string _cgiExtension;
	int			_bodyLimit;
	bool		_autoIndex;
	
public:
	bool		_methods[4]; //GET HEAD POST PUT
	
	std::string getName() const;
	bool getAutoindex() const;
	std::string getRoot() const;
	std::string getIndex() const;
	std::string getCgi() const;
	std::string getExtension() const;
	int getLimit() const;
	
	void setName(std::string name);
	void setAutoindex(bool src);
	void setRoot(std::string root);
	void setIndex(std::string index);
	void setCgi(std::string cgi);
	void setExtension(std::string extension);
	void setLimit(int limit);
	
	Location();
	~Location();
};

#endif /* Location_hpp */
