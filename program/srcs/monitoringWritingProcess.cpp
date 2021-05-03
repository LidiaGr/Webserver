//
//  monitoringWritingProcess.cpp
//  webserv
//
//  Created by Shira Broderick on 4/6/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "monitoringWritingProcess.hpp"

void writingData(Client *client) {
	if (client->getStatus() == responsing) {
		//		size_t size = 50000;
		size_t size = client->getResponseSizeBody() + client->getResponseSizeHead() - client->getTotalWriteSize();
		ssize_t writeBytes = write(client->getSocket(), client->getResponse() + client->getTotalWriteSize(), size);
		client->addToTotalWriteSize(writeBytes);
		std::cout << Y_COL << "WRITTEN: " << DEF_COL << client->getTotalWriteSize() << " socket " << client->getSocket() << std::endl;
		if (writeBytes <= 0 || client->getNeedAuth() == NEED_AUTH) {
			client->setResponseSizeBody(0);
			client->setResponseSizeHead(0);
			client->setResponse(NULL);
			client->setStatus(closing);
			client->addToBuff(NULL);
			return;
		} else if (client->getTotalWriteSize() < (client->getResponseSizeBody() + client->getResponseSizeHead())) {
			client->setStatus(responsing);
		} else {
			std::cout << B_LIGHT_COL << "All Data transfered" << DEF_COL << std::endl;
			if (client->getConnectionClose() == false)
				client->setStatus(requesting);
			else
				client->setStatus(closing);
			client->setTotalWriteSize(0);
			client->setRequestHead("");
			client->setRequestBody("");
			client->setResponseSizeBody(0);
			client->setResponseSizeHead(0);
			client->setResponse(NULL);
			client->addToBuff(NULL);
			client->setChunkStatus(false);
			client->setFullHeader(false);
		}
	}
}
