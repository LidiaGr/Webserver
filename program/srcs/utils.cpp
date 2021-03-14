//
//  utils.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/9/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "../includes/utils.hpp"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = (unsigned char *)s;
	while (i < n)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

size_t     ft_strlen(const char *s)
{
    int len = 0;
    
    while (s && s[len])
        len++;
    return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstr;
	size_t	len;
	size_t	i;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	i = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	newstr = (char *)malloc(sizeof(char) * len);
	if (!newstr)
		return (NULL);
	while (*s1 != '\0')
		newstr[i++] = *s1++;
	while (*s2 != '\0')
		newstr[i++] = *s2++;
	newstr[i] = '\0';
	return (newstr);
}

char    *ft_strdup(char *s)
{
   char *cpy;
   int i = 0;

    if (!(cpy = (char *)malloc(sizeof(char) * ft_strlen(s) + 1)))
        return (NULL);
    while (s && s[i])
    {
        cpy[i] = s[i];
        i++;
    }
   cpy[i] = '\0';
   return (cpy);
}

char    *ft_addsym(char *s, char *c)
{
    char    *newS;
    int     i = 0;
    
    if(!(newS = (char *)malloc(sizeof(char) * ft_strlen(s) + 2)))
        return (NULL);
    while (s && *s)
        newS[i++] = *s++;
    newS[i++] = c[0];
    newS[i] = '\0';
    return (newS);
}

char* deleteSpaces(char *str)
{
	int i = 0;
	int j;
	char *tmp;
	char *res;
	tmp = str;
	while (*tmp) {
		if (*tmp != ' ' && *tmp != '\t')
			i++;
		tmp++;
	}
	res = (char*)malloc(sizeof(char) * i);
	i = 0;
	j = 0;
	while (str[i]) {
		if (str[i] != ' ' && str[i] != '\t') {
			res[j] = str[i];
			j++;
		}
		i++;
	}
	res[j] = '\0';
	free(str);
	return (res);
}

int    ft_getline(char **str, char **line, ssize_t bytes)
{
    if (bytes == 0)
    {
        *line = ft_strdup(*str);
        free(*str);
        *str = NULL;
    }
    if (bytes < 0)
        return (-1);
    return (bytes ? 1 : 0);
}

int    gnl(int fd, char **line)
{
    char            buf[1];
    static char     *str;
    ssize_t         bytes;
    char            *tmp;
    
    while ((bytes = read(fd, buf, 1) > 0))
    {
        if (*buf && *buf != '\n')
        {
            tmp = str;
            str = ft_addsym(str, buf);
            free(tmp);
        }
        else
        {
            *line = ft_strdup(str);
            free(str);
            str = NULL;
            break ;
        }
    }
	if (bytes > 0)
		*line = deleteSpaces(*line);
    return (ft_getline(&str, line, bytes));
}

void printEvent(std::string event) {
	std::cout << std::endl << "*** EVENT: " << event << " ***" << std::endl << std::endl;
}
