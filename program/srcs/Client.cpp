//
//  Client.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/11/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "Client.hpp"

Client::Client(int socket, Server *server) : _socket(socket), _status(requesting), _response(NULL), _buff(std::string()), _server(server), _responseSizeBody(0), _responseSizeHead(0), _isChunked(false), _requestHead(std::string()), _requestBody(std::string()), _chunkSize(-1), _lastChunk(0), _totalWriteSize(0), _writeBytes(0), _endChunkFlag(false), _fullHeader(false), _bodySeparatorFlag(false), _requestEnd(false), _needAuth(NOT_NEED_AUTH), _connectionClose(false), _auth(std::vector<std::string>()) {
	createUser("user", "pass");
	_trashFileID += 1;
//	std::cout << "Client with socket " << socket << " created" << std::endl;
}

Client::~Client() {
//	std::cout << "Client with socket " << getSocket() << " destroyed" << std::endl;
}

void Client::createUser(std::string login, std::string pass) {
	std::string data = login + ':' + pass;
	std::string val = b64encode(data.c_str(), data.size());
	_auth.push_back(val);
}

int				Client::getSocket() const { return this->_socket; }
Status			Client::getStatus() const { return this->_status; }
char*			Client::getResponse() const { return this->_response; }
std::string		Client::getBuff() const { return this->_buff; }
Server*			Client::getServer() const { return this->_server; }
size_t			Client::getResponseSizeBody () const { return _responseSizeBody; }
size_t			Client::getResponseSizeHead () const { return _responseSizeHead; }
std::string		Client::getRequestHead() const { return _requestHead; }
std::string		Client::getRequestBody() const { return _requestBody; }
ssize_t			Client::getChunkSize() const { return _chunkSize; }
size_t			Client::getLastChunkSize() const { return _lastChunk; }
size_t			Client::getTotalWriteSize () const { return _totalWriteSize; }
size_t			Client::getWriteBytes () const { return _writeBytes; }
bool			Client::getEndChunkFlag() const { return _endChunkFlag; }
bool			Client::getFullHeaderFlag() const { return _fullHeader; }
bool			Client::getBodySeparatorFlag() const { return _bodySeparatorFlag; }
bool			Client::getRequestEndFlag() const { return _requestEnd; }
int				Client::getTrashFileID() const { return _trashFileID; }
int				Client::getNeedAuth() const { return _needAuth; }
bool			Client::getConnectionClose() const {return _connectionClose; }
std::vector<std::string> Client::getAuth() const { return _auth; }

void			Client::setChunkStatus(bool status) {_isChunked = status; }
void			Client::setResponseSizeBody (size_t size) { _responseSizeBody = size; }
void			Client::setResponseSizeHead (size_t size) { _responseSizeHead = size; }
void			Client::setStatus(Status status) { _status = status; }
void			Client::setRequestHead(std::string head) { _requestHead = head; }
void			Client::setRequestBody(std::string body) { _requestBody = body; }
void			Client::setChunkSize(size_t size) { _chunkSize = size; }
void			Client::setLastChunkSize(size_t size) { _lastChunk = size; }
void			Client::setTotalWriteSize(size_t size) { _totalWriteSize = size; }
void			Client::setWriteBytes(size_t size) { _writeBytes = size; }
void			Client::setEndChunkFlag(bool value) { _endChunkFlag = value; }
void			Client::setFullHeader(bool value) { _fullHeader = value; }
void			Client::setRequestEndFlag(bool value) { _requestEnd = value; }
void			Client::setBodySeparatorFlag(bool value) { _bodySeparatorFlag = value; }
void			Client::setNeedAuth(int value) { _needAuth = value; }
void			Client::setConnectionClose(bool flag) {_connectionClose = flag; }
void			Client::setResponse(char* str) {
	if (_response) {
		free(_response);
		_response = NULL;
	}
	if (str)
		_response = str;
}
void			Client::addToBuff(char* buff) {
	if (!buff)
		_buff.clear();
	else
		_buff += std::string(buff);
}

void			Client::setBuff(char* buff) {
	if (_buff.empty())
		_buff = buff;
	else {
		_buff.clear();
		_buff = buff;
	}
}

void			Client::setBuff(std::string buff) {
	if (_buff.empty())
		_buff = buff;
	else {
		_buff.clear();
		_buff = buff;
	}
}

void			Client::addToBuffWithSize(char* str, long valread) {
	_buff.insert(_buff.end(), str, str + valread);
}


bool			Client::isChunked() const { return _isChunked; }
void			Client::addToRequestBody(std::string part) { _requestBody += part; }
void			Client::addToRequestHead(std::string part) { _requestHead += part; }
void			Client::addToLastChunkSize(size_t size) { _lastChunk += size; }
void			Client::divToLastChunkSize(size_t size) { _lastChunk -= size; }
void			Client::addToBuff(std::string str) { _buff += str; }
void			Client::clearBuff() { _buff.clear(); }
void			Client::addToTotalWriteSize(ssize_t size) { _totalWriteSize += size; }
void			Client::addToWriteBytes(size_t size) { _writeBytes += size; }
bool 			Client::checkConnectionValue() {
	size_t res = 0;
	if ((res = _buff.find("Connection:")) != std::string::npos) {
		res = res + 11;
		while (res == '\t' || res == ' ')
			res++;
		if (_buff.find("close", res) != std::string::npos)
			return true;
	}
	return false;
}

