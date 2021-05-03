//
//  main.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/9/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "main.hpp"

int Client::_trashFileID = 0;

int main(int argc, char* argv[]) {
	char* fileName = NULL;
	if (argc > 1)
		fileName = argv[1];
	WebServer* webserv = new WebServer();
	confParser(webserv, fileName);
	webserv->openListenSock();
	printEvent("START SERVER", G_COL);
	webserv->startServer();
	delete webserv;
	return 0;
}
