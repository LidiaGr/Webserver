//
//  utils.hpp
//  webserv
//
//  Created by Temple Tarsha on 3/9/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include "main.hpp"

class WebServer;

void	*ft_memset(void *s, int c, size_t n);
int		gnl(int fd, char **line);
int 	confParser(WebServer* webserv);
size_t  ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
void	printEvent(std::string event);

#endif /* utils_hpp */
