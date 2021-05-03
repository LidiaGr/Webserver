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
	int				_socket;
	Status			_status;
	char*			_response;
	std::string		_buff;
	Server*			_server;
	size_t			_responseSizeBody;
	size_t			_responseSizeHead;
	bool			_isChunked;
	std::string		_requestHead;
	std::string		_requestBody;
	ssize_t			_chunkSize;
	size_t			_lastChunk;
	size_t			_totalWriteSize;
	size_t			_writeBytes;
	bool			_endChunkFlag;
	bool			_fullHeader;
	bool			_bodySeparatorFlag;
	bool			_requestEnd;
	int				_needAuth;
	bool			_connectionClose;
	std::vector<std::string> _auth;
	
	static int		_trashFileID;
	
	Client();
	
private:
	void createUser(std::string login, std::string pass);
	
public:
	Client(int socket, Server *server);
	~Client();
	
	int				getSocket() const;
	Status			getStatus() const;
	char*			getResponse() const;
	std::string		getBuff() const;
	Server*			getServer() const;
	size_t			getResponseSizeBody () const;
	size_t			getResponseSizeHead () const;
	std::string		getRequestHead() const;
	std::string		getRequestBody() const;
	ssize_t			getChunkSize() const;
	size_t			getLastChunkSize() const;
	size_t			getTotalWriteSize() const;
	size_t			getWriteBytes() const;
	bool			getEndChunkFlag() const;
	bool			getFullHeaderFlag() const;
	bool			getBodySeparatorFlag() const;
	bool			getRequestEndFlag() const;
	int				getTrashFileID() const;
	int				getNeedAuth() const;
	bool			getConnectionClose() const;
	std::vector<std::string> getAuth() const;
	
	
	void			setChunkStatus(bool status);
	void			setStatus(Status status);
	void			setResponse(char* str);
	void			addToBuff(char* buff);
	void			setBuff(std::string buff);
	void			setBuff(char* buff);
	void			setResponseSizeBody(size_t size);
	void			setResponseSizeHead(size_t size);
	void			setRequestHead(std::string head);
	void			setRequestBody(std::string body);
	void			setChunkSize(size_t size);
	void			setLastChunkSize(size_t size);
	void			setTotalWriteSize(size_t size);
	void			setWriteBytes(size_t size);
	void			setEndChunkFlag(bool value);
	void			setFullHeader(bool value);
	void			setRequestEndFlag(bool value);
	void			setBodySeparatorFlag(bool value);
	void			setNeedAuth(int value);
	void			setConnectionClose(bool flag);
	
	bool			checkConnectionValue();
	bool			isChunked() const;
	void			addToRequestBody(std::string part);
	void			addToRequestHead(std::string part);
	void			addToLastChunkSize(size_t size);
	void			divToLastChunkSize(size_t size);
	void			addToBuff(std::string str);
	void			addToTotalWriteSize(ssize_t size);
	void			addToWriteBytes(size_t size);
	void			clearBuff();
	void			addToBuffWithSize(char* str, long valread);
};

#endif /* Client_hpp */
