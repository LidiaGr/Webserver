//
//  utils.cpp
//  webserv
//
//  Created by Temple Tarsha on 3/9/21.
//  Copyright © 2021 Temple Tarsha. All rights reserved.
//

#include "utils.hpp"

void	freeDoubleArray(char **arr)
{
	int		i;

	i = 0;
	if (*arr)
	{
		while (arr[i]) {
			free(arr[i]);
			arr[i++] = NULL;
		}
		free(arr);
		arr = NULL;
	}
}


void*	ft_memset(void *s, int c, size_t n)
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

size_t	ft_strlen(const char *s)
{
    int len = 0;
    
    while (s && s[len])
        len++;
    return (len);
}

char*	ft_strjoin(char const *s1, char const *s2)
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

char*	ft_strdup(char *s)
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

char*	ft_addsym(char *s, char *c)
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

char*	deleteSpaces(char *str, bool needFree)
{
	int i = 0;
	int j = 0;
	char *tmp;
	char *res;
	tmp = str;
	while (*tmp) {
		if (*tmp != ' ' && *tmp != '\t')
			i++;
		tmp++;
	}
	res = (char*)malloc(sizeof(char) * (i + 1));
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
	if (needFree)
		free(str);
	return (res);
}

int		ft_getline(char **str, char **line, ssize_t bytes)
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
    
    while ((bytes = read(fd, buf, 1)) > 0)
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
		*line = deleteSpaces(*line, true);
    return (ft_getline(&str, line, bytes));
}

void printEvent(std::string event, std::string color) {
	std::cout << std::endl << color << "*** EVENT: " << event << " ***" << DEF_COL << std::endl << std::endl;
}

/**
The function converts an integer type to a std::string

- Warning: The function does not handle the Int overflow.

- Returns: A new std::string.
*/
std::string intToString(int num) {
	std::string result;
	bool isNegative;
	if (num < 0) {
		isNegative = true;
		num = -num;
	}
	else
		isNegative = false;
	while (num / 10 > 0) {
		result.insert(result.begin(), ('0' + num % 10));
		num /= 10;
	}
	result.insert(result.begin(), ('0' + num % 10));
	if (isNegative)
		result.insert(result.begin(), '-');
	return result;
}

std::string splitStr(std::string *str) {
	size_t i = 0;
	while((*str)[i] != '\t' && (*str)[i] != ' ' && i != (*str).length())
		i++;
	std::string splited = (*str).substr(0, i);
	while((*str)[i] == '\t' || (*str)[i] == ' ')
		i++;
	*str = (*str).substr(i, (*str).length() - i);
	return splited;
}

/**
The function converts a std::string type to a  integer (base: HEX, OCT, DEC, BIN)
 
- Warning: Function is not case sensitive. The function does not handle the Int overflow.

- Returns: An Int value. If base not HEX, OCT, DEC or BIN return 0.
*/

int stoiBase(std::string str, int base) {
	
	int result = 0;
	std::map<char, int> literal;
	literal['A'] = 10;
	literal['B'] = 11;
	literal['C'] = 12;
	literal['D'] = 13;
	literal['E'] = 14;
	literal['F'] = 15;
	literal['a'] = 10;
	literal['b'] = 11;
	literal['c'] = 12;
	literal['d'] = 13;
	literal['e'] = 14;
	literal['f'] = 15;
	
	bool isNegative = false;
	int factor = 1;
	std::string::iterator it = str.begin();
	while (it != str.end() && (*it == ' ' || *it == '\t'))
		it++;
	for (; it != str.end(); it++) {
		if (*it == '-') {
			isNegative = true;
			continue;
		}
		
		if (base == BIN) {
			if (*it >= '0' && *it <= '1') {
				result *= factor;
				if (factor == 1)
				factor = base;
				result += (*it - '0');
			}
			else {
				if (isNegative)
					result = -result;
				return result;
			}
		} else if (base == OCT) {
			if (*it >= '0' && *it <= '7') {
				result *= factor;
				if (factor == 1)
					factor = base;
				result += (*it - '0');
			}
			else {
				if (isNegative)
					result = -result;
				return result;
			}
		} else if (base == DEC) {
			if (*it >= '0' && *it <= '9') {
				result *= factor;
				if (factor == 1)
					factor = base;
				result += (*it - '0');
			}
			else {
				if (isNegative)
					result = -result;
				return result;
			}
		} else if (base == HEX) {
			if (*it >= '0' && *it <= '9') {
				result *= factor;
				if (factor == 1)
					factor = base;
				result += (*it - '0');
			} else if ((*it >= 'A' && *it <= 'F') || (*it >= 'a' && *it <= 'f')) {
				result *= factor;
				if (factor == 1)
					factor = base;
				result += literal[*it];
			} else {
				if (isNegative)
					result = -result;
				return result;
			}
		}
	}
	if (isNegative)
		result = -result;
	return result;
}

void	*ft_calloc(size_t count, size_t size)
{
	size_t			i;
	unsigned char	*array;

	i = 0;
	array = (unsigned char *)malloc(size * count);
	if (!array)
		return (NULL);
	while (i < count * size)
	{
		array[i] = 0;
		i++;
	}
	return (array);
}

void	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	i = 0;
	while (src[i] && (i + 1) < size)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

static int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

std::string stringToLowercase(std::string str) {
	std::string::iterator it = str.begin();
	for (; it != str.end(); it++) {
		*it = ft_tolower(*it);
	}
	return str;
}

std::string decodeSpec(std::string src) {
	std::string sym;
//	int code[] = {20, 21, 23, 24, 25, 0x5E, 26, 28, 29, 0x3D, 0x2B};
//	char symbol[] = {' ', '!', '#', '$', '%', '^', '&', '(', ')', '=', '+'};
	int code[] = {20, 23};
	char symbol[] = {' ', '#'};
	for (size_t i = 0; i < 2; i++) {
		size_t pos = std::string::npos;
		while ((pos = src.find("%" + intToString(code[i]))) != std::string::npos) {
			src.replace(pos, 3, sym + symbol[i]);
		}
	}
	return src;
}

void printErrorAndExit(std::string str, bool allowErrno, int exitCode) {
	std::cout << std::endl << R_COL << "ERROR: " << str << std::endl;
	if (allowErrno == true)
		std::cout << std::string(strerror(errno)) << std::endl;
	std::cout << DEF_COL;
	exit(exitCode);
}

size_t	cStrFind(const char *big, const char *little, size_t len)
{
	size_t	counter;
	size_t	index;

	counter = 0;
	index = 0;
	while (counter < len && big[counter] != '\0')
	{
		while (big[counter + index] == little[index] && counter + index < len
				&& little[index] != '\0' && big[counter + index] != '\0')
			index++;
		if (little[index] != '\0')
			index = 0;
		else
			return (counter);
		counter++;
	}
	return (-1);
}
