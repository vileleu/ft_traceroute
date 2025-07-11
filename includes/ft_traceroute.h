/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:39:24 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/10 23:24:02 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define START_TTL 1
#define BUFFER_SIZE 256

#define id un.echo.id          // for icmp->id
#define seq un.echo.sequence   // for icmp->seq

typedef struct	s_parse {
	char	**av;
	int		ac;
	char	check_host;
	char	check_packetlen;
	char	ignore_next;
	char	m;
	int		packetlen;
	int		max_ttl;
}				t_parse;

typedef struct	s_data {
	struct sockaddr_in	dst;
	char				*packet;
	char				*target;
	size_t				packet_size;
	int					packetlen;
	int					sockfd;
	int					ttl;
	int					max_ttl;
	int					addr_reached;
	int					reply_true;
	char				*previous;
}				t_data;

/*
** FUNCTIONS DATA
*/

t_data	*create_data();
int		create_packet(t_data *data);

/*
** FUNCTIONS PARSE
*/

int		parse(t_data *data, char **av, int ac);

/*
** FUNCTIONS SOCKET
*/

int		create_socket(t_data *data);
int		handle_packet(t_data *data);

/*
** FUNCTIONS ERROR
*/

int		print_usage(t_data *data);
int		error_errno(t_data *data, char *s);
int		error_perso(t_data *data, char *s);

/*
** FUNCTIONS UTILS
*/

void			free_data(t_data **data);
int				print_domain_name(t_data *data, char *addr);
int				print_ip(t_data *data, char *addr);
int				check_atoi(char *s);
unsigned short	get_checksum(unsigned short *packet, size_t size);

#endif
