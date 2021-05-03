//
//  monitoringReadingProcess.cpp
//  webserv
//
//  Created by Shira Broderick on 4/3/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "monitoringReadingProcess.hpp"

#define BUF_SIZE 100000

void handleBodySeparator(Client *client, size_t position) {
	std::string body;
	client->setRequestHead(client->getBuff().substr(0, position + 4));
	std::cout << std::endl << client->getRequestHead() << std::endl;
	client->setFullHeader(true);
	if (stringToLowercase(client->getRequestHead()).find("connection: close") != std::string::npos) {
		client->setConnectionClose(true);
	}
	if (stringToLowercase(client->getRequestHead()).find("transfer-encoding: chunked") != std::string::npos) {
		client->setChunkStatus(true);
		client->setBuff(client->getBuff().substr(position + 2));
	}
	else
		client->setBuff(client->getBuff().substr(position + 4));
}

bool cutChunk(Client *client) {
//	std::cout << "READED: " << client->getBuff().size() << std::endl;
	size_t pos = std::string::npos;
	if (client->getChunkSize() != 0 && client->getBuff().size() >= (size_t)client->getChunkSize() && client->getEndChunkFlag() == false) {
		client->addToRequestBody(client->getBuff().substr(0, client->getChunkSize()));
		client->setBuff(client->getBuff().erase(0, client->getChunkSize()));
		client->setChunkSize(-1);
	}
	else if ((pos = client->getBuff().find("\r\n")) != std::string::npos) {
		if (client->getEndChunkFlag() == false) {
			client->setEndChunkFlag(true);
			client->setBuff(client->getBuff().erase(0, pos + 2));
		}
		if ((pos = client->getBuff().find("\r")) != std::string::npos) {
			if (client->getChunkSize() == -1)
				client->setChunkSize(stoiBase(client->getBuff(), HEX));
			if (client->getChunkSize() == 0) {
				client->setRequestEndFlag(true);
			}
			client->setBuff(client->getBuff().erase(0, pos));
			if ((pos = client->getBuff().find("\r\n")) != std::string::npos) {
				client->setEndChunkFlag(false);
				client->setBuff(client->getBuff().erase(0, pos + 2));
				if (client->getChunkSize() != 0 && client->getBuff().size() >= (size_t)client->getChunkSize()) {
					client->addToRequestBody(client->getBuff().substr(0, client->getChunkSize()));
					client->setBuff(client->getBuff().erase(0, client->getChunkSize()));
					client->setChunkSize(-1);
				}
			}
		}
	}
	else
		return (false);
	return (true);
}

void handleChunkedBuffer(Client *client) {
	while (cutChunk(client))
		;
	if (client->getChunkSize() == 0 && client->getBuff().size() == 0 && client->getRequestEndFlag() == true) {
//		std::cout << "BODY SIZE: " << client->getRequestBody().size() << std::endl;
		client->setRequestEndFlag(false);
		client->setChunkSize(-1);
		client->setResponse(handleRequestAndCreateResponse(client));
		client->setStatus(responsing);
		client->setEndChunkFlag(false);
		client->clearBuff();
		
		char trash[10];
		while (read(client->getSocket(), trash, 10) > 0)
			;
	}
}

void handleNOTChunkedBuffer(Client *client) {
	size_t position = std::string::npos;
	if ((position = client->getBuff().find(BODY_SEPARATOR)) != std::string::npos) {
		handleBodySeparator(client, position);
	}
	if (client->getFullHeaderFlag() == true && client->isChunked() == true) {
		handleChunkedBuffer(client);
		return;
	}
	if (client->getBuff().empty() == false && client->getFullHeaderFlag() == true) {
		client->addToRequestBody(client->getBuff());
		client->clearBuff();
	}
		
	if (client->getBuff().empty() == true && client->getFullHeaderFlag() == true) {
		client->setResponse(handleRequestAndCreateResponse(client));
		client->setStatus(responsing);
		client->clearBuff();
	}
}

void readingData(Client *client) {
	char buffer[BUF_SIZE] = {0};
	long valread = -1;

	if ((valread = read(client->getSocket(), buffer, BUF_SIZE)) == -1) {
		printEvent("read connection closed", R_COL);
		client->setResponseSizeBody(0);
		client->setResponseSizeHead(0);
		client->setResponse(NULL);
		client->setStatus(closing);
		client->addToBuff(NULL);
		return;
	} else if (valread == 0) {
		client->setStatus(responsing);
		return;
	}
	
	client->addToBuffWithSize(buffer, valread);
		
	if (client->isChunked() == false)
		handleNOTChunkedBuffer(client);
	else
		handleChunkedBuffer(client);
}
