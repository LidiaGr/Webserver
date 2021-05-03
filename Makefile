# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ttarsha <ttarsha@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/03 13:18:22 by ttarsha           #+#    #+#              #
#    Updated: 2021/02/19 17:10:05 by ttarsha          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

OBJS_DIR	= bin/
SRCS_DIR    = program/srcs/
SRCS		= main \
				requestHandler \
				monitoringReadingProcess \
				cgiHandler \
				Client \
				WebServer \
				configParser \
				StatusCodeHTTP \
				dataAndTimeCreator \
				Server \
				reqestAndResponseUtils \
				monitoringWritingProcess \
				base64 \
				autoIndexPageCreator \
				Location \
				clientsAuth \
				utils

OBJS		= $(addprefix $(OBJS_DIR), $(SRCS:=.o))
INCLUDES	= program/includes/

CC			= clang++
GCC_FLAGS	= -I $(INCLUDES) -g -Wall -Wextra -Werror -std=c++98 -pedantic


#deb:
#				echo $(OBJS)

all:			$(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	mkdir -p	$(OBJS_DIR)

$(NAME):		$(OBJS)
	$(CC) 		$(OBJS) -o $(NAME)

$(OBJS):		$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	$(CC)		$(GCC_FLAGS) -c $< -o $@


clean:
	rm -rf		$(OBJS_DIR)

fclean:			clean
	rm -f		$(NAME)

re:				fclean all


.PHONY: all clean fclean re
