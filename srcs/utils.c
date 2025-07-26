/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:55:21 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/26 17:24:47 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void	free_data(t_data **data) {
	if (data && *data) {
		if ((*data)->sockfd)
			close((*data)->sockfd);
		if ((*data)->packet)
			free((*data)->packet);
		if ((*data)->previous)
			free((*data)->previous);
		free(*data);
		*data = NULL;
	}
}

int		print_domain_name(t_data *data, char *addr) {
	char				s[BUFFER_SIZE] = "";
	struct sockaddr_in	target;
	int					ret;

	inet_pton(AF_INET, addr, &target.sin_addr);
	target.sin_family = AF_INET;
	if ((ret = getnameinfo((struct sockaddr *)&target, sizeof(target), s, sizeof(s), NULL, 0, 0))) {
		sprintf(s, "getnameinfo: %s\n", gai_strerror(ret));
		return (error_perso(data, s));
	}
	else
		printf("%s", s);
	return (EXIT_SUCCESS);
}

int		check_atoi(char *s) {
	int	check = 1;
	int i = 0;

	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\v' ||
	s[i] == '\f' || s[i] == '\r' || s[i] == '\n')
		i++;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9') {
		if (check)
			check = 0;
		i++;
	}
	if (s[i])
		check = 1;
	return (check);
}

unsigned short	get_checksum(unsigned short *packet, size_t size) {
	u_int32_t	checksum = 0;
	
	while (size > 1) {
		checksum += *(packet++);
		size -= 2;
	}
	if (size == 1)
		checksum += *((unsigned char *)packet);
	checksum = (checksum >> 16) + (checksum & 0xffff);
	return (~checksum);
}
