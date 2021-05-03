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
class Location;
class Client;

void*			ft_memset(void *s, int c, size_t n);
int				gnl(int fd, char **line);
int				confParser(WebServer* webserv, char* fileName);
size_t			ft_strlen(const char *s);
char*			ft_strjoin(char const *s1, char const *s2);
void			printEvent(std::string event, std::string color);
std::string		intToString(int num);
std::string		getCurrentDate();
char*			deleteSpaces(char *str, bool needFree);
std::string		splitStr(std::string *str);
int				stoiBase(std::string str, int base);
void			*ft_calloc(size_t count, size_t size);
void			ft_strlcpy(char *dst, const char *src, size_t size);
char* 			autoIndexPageCreator(std::string dir, Client *client, std::string target);
std::string		stringToLowercase(std::string str);
std::string		decodeSpec(std::string src);
void 			printErrorAndExit(std::string str, bool allowErrno, int exitCode);
void			freeDoubleArray(char **arr);
size_t			cStrFind(const char *big, const char *little, size_t len);

#endif /* utils_hpp */
