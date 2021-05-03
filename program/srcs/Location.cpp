//
//  Location.cpp
//  webserv
//
//  Created by Frenica Torn on 3/24/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "main.hpp"

Location::Location() : _name(std::string()), _root(std::string()), _index(std::string()), _cgi(std::string()), _bodyLimit(-1), _autoIndex(false) {
	_methods[0] = false; //GET
	_methods[1] = false; //HEAD
	_methods[2] = false; //POST
	_methods[3] = false; //PUT
}

Location::~Location() {}

std::string Location::getName() const { return _name; }
bool Location::getAutoindex() const { return _autoIndex; }
std::string Location::getRoot() const { return _root; }
std::string Location::getIndex() const { return _index; }
std::string Location::getCgi() const { return _cgi; }
std::string Location::getExtension() const { return _cgiExtension; }
int Location::getLimit() const { return _bodyLimit; }

void Location::setName(std::string name) { _name = name; }
void Location::setAutoindex(bool src) { _autoIndex = src; }
void Location::setRoot(std::string root) { _root = root; }
void Location::setIndex(std::string index) { _index = index; }
void Location::setCgi(std::string cgi) { _cgi = cgi; }
void Location::setExtension(std::string extansion) { _cgiExtension = extansion; }
void Location::setLimit(int limit) { _bodyLimit = limit; }
