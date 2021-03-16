//
//  main.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/9/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "../includes/main.hpp"

int main() {
	WebServer* webserv = new WebServer();
	confParser(webserv);
	
//	for (int i = 0; i < webserv->getServer().size(); i++)
//		webserv->getServer().at(i)->printData();
	
	webserv->openListenSock();
	printEvent("START SERVER");
	webserv->startServer();
	
	return 0;
}
