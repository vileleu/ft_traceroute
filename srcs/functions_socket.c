/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_socket.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:32:30 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/11 16:35:51 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static int	receive_packet(t_data *data) {
	unsigned char		recv_buffer[BUFFER_SIZE] = "";
	struct sockaddr_in	from;
	socklen_t			from_size = sizeof(from);
	struct icmphdr		*icmp;
	char				*addr;

	if ((recvfrom(data->sockfd, recv_buffer, BUFFER_SIZE, 0, (struct sockaddr *)&from, &from_size)) < 0)
			return (error_errno(data, "recvfrom"));
	data->reply_true = 1;
	addr = inet_ntoa(from.sin_addr);
	if (strcmp(addr, data->previous))
		printf("%s ", addr);
	free(data->previous);
	if (!(data->previous = strdup(addr)))
		return (error_perso(data, "ERROR MALLOC"));
	icmp = (struct icmphdr *)(recv_buffer + sizeof(struct ip));
	if (icmp->type != ICMP_TIMXCEED)
		data->addr_reached = 1;
	return (EXIT_SUCCESS);
}

int		send_packet(t_data *data) {
	fd_set				socks;
	struct timeval		timeout;
	int					ret_select = 0;

	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;
	FD_ZERO(&socks);
	FD_SET(data->sockfd, &socks);
	if (sendto(data->sockfd, data->packet, data->packet_size, 0, (struct sockaddr *)&data->dst, sizeof(data->dst)) < 0)
		return (error_errno(data, "sendto"));
	if ((ret_select = select(data->sockfd + 1, &socks, NULL, NULL, &timeout)) < 0)
		return (error_errno(data, "select"));
	else if (!ret_select)
		printf("*");
	else
		return (receive_packet(data));
	return (EXIT_SUCCESS);
}

int		handle_packet(t_data *data) {
	struct timeval	start;
	struct timeval	end;

	bzero(&start, sizeof(struct timeval));
	bzero(&end, sizeof(struct timeval));
	gettimeofday(&start, NULL);
	if (send_packet(data))
		return (EXIT_FAILURE);
	gettimeofday(&end, NULL);
	timersub(&end, &start, &end);
	if (data->reply_true)
		printf(" %.3f ms", (float)end.tv_usec / 1000);
	return (EXIT_SUCCESS);
}

int		create_socket(t_data *data) {
	if ((data->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		return (error_errno(data, "socket"));
	//if ((setsockopt(data->sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&timeout, sizeof(timeout))) < 0)
	//	return (error_errno(data, "setsockopt"));
	return (EXIT_SUCCESS);
}
