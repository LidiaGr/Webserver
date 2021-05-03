//
//  monitoringReadingProcess.hpp
//  webserv
//
//  Created by Shira Broderick on 4/3/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef monitoringReadingProcess_hpp
#define monitoringReadingProcess_hpp

#include "main.hpp"

void readingData(Client *client);
void handleChunkedBuffer(Client *client);

#endif
