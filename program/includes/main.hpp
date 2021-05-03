//
//  main.hpp
//  webserv
//
//  Created by Shira Broderick on 09/03/2021.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef main_hpp
#define main_hpp

#define SUCCESS 0
#define CHUNK_REQUEST_LIMIT_EXCEEDED 1
#define CUNK_SIZE_MISMATCH 2
#define HEX 16
#define DEC 10
#define OCT 8
#define BIN 2

#define NOT_NEED_AUTH 0
#define NEED_AUTH 1
#define AUTH_ACCEPTED 2

#define BODY_SEPARATOR "\r\n\r\n"

# define G_COL "\033[32m"
# define R_COL "\033[31m"
# define B_COL "\033[34m"
# define R_GRAD_COL "\033[41m"
# define B_GRAD_COL "\033[44m"
# define B_LIGHT_COL "\033[36m"
# define Y_COL "\033[33m"
# define P_COL "\033[35m"
# define P_GRAD_COL "\033[45m"
# define DEF_COL "\033[0m"


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
#include <dirent.h>
#include <sys/stat.h>

#include "utils.hpp"
#include "Server.hpp"
#include "WebServer.hpp"
#include "StatusCodeHTTP.hpp"
#include "Client.hpp"
#include "Location.hpp"
#include "cgiHandler.hpp"
#include "requestHandler.hpp"
#include "reqestAndResponseUtils.hpp"
#include "monitoringReadingProcess.hpp"
#include "monitoringWritingProcess.hpp"
#include "base64.hpp"
#include "clientsAuth.hpp"

#endif
