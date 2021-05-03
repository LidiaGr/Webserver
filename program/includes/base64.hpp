//
//  base64.hpp
//  webserv
//
//  Created by Temple Tarsha on 4/5/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef base64_hpp
#define base64_hpp

#include "main.hpp"

const std::string b64encode(const void* data, const size_t &len);
const std::string b64decode(const void* data, const size_t len);

#endif /* base64_hpp */
