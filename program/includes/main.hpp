//
//  main.hpp
//  webserv
//
//  Created by Shira Broderick on 09/03/2021.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef main_hpp
#define main_hpp

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "utils.hpp"
#include "Server.hpp"
#include "WebServer.hpp"
#include "StatusCodeHTTP.hpp"
#include "Client.hpp"

#endif
