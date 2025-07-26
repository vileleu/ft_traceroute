/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_socket.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:32:30 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/26 19:32:38 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static int	receive_packet(t_data *data, uint8_t *recv_ok) {
	unsigned char		recv_buffer[BUFFER_SIZE_RECV] = "";
	struct sockaddr_in	from;
	socklen_t			from_size = sizeof(from);
	struct icmphdr		*icmp;
	struct icmphdr		*packet;
	char				*addr = NULL;

	if ((recvfrom(data->sockfd, recv_buffer, BUFFER_SIZE_RECV, 0, (struct sockaddr *)&from, &from_size)) < 0)
		return (error_errno(data, "recvfrom"));
	icmp = (struct icmphdr *)(recv_buffer + sizeof(struct ip));
	packet = (struct icmphdr *)data->packet;
	if (icmp->type != ICMP_ECHOREPLY)
		icmp = (struct icmphdr *)(recv_buffer + (sizeof(struct ip) + sizeof(struct ip) + sizeof(struct icmphdr)));
	if (packet->id != icmp->id)
		return (EXIT_SUCCESS);
	gettimeofday(&data->end, NULL);
	*recv_ok = 1;
	data->reply_true = 1;
	addr = inet_ntoa(from.sin_addr);
	if (strcmp(addr, data->previous)) {
		if (print_domain_name(data, addr))
			return (EXIT_FAILURE);
		printf(" (%s) ", addr);
	}
	free(data->previous);
	if (!(data->previous = strdup(addr)))
		return (error_perso(data, "ERROR MALLOC"));
	icmp = (struct icmphdr *)(recv_buffer + sizeof(struct ip));
	if (icmp->type == ICMP_ECHOREPLY)
		data->addr_reached = 1;
	return (EXIT_SUCCESS);
}

int		send_packet(t_data *data) {
	fd_set				socks;
	struct timeval		timeout;
	int					ret_select = 0;
	uint8_t				recv_ok = 0;

	timeout.tv_sec = 0;
	timeout.tv_usec = 500000;
	FD_ZERO(&socks);
	FD_SET(data->sockfd, &socks);
	if (sendto(data->sockfd, data->packet, data->packet_size, 0, (struct sockaddr *)&data->dst, sizeof(data->dst)) < 0)
		return (error_errno(data, "sendto"));
	while (!recv_ok) {
		if ((ret_select = select(data->sockfd + 1, &socks, NULL, NULL, &timeout)) < 0)
			return (error_errno(data, "select"));
		else if (!ret_select) {
			recv_ok = 1;
			printf("*");
		}
		else {
			if (receive_packet(data, &recv_ok))
				return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int		handle_packet(t_data *data) {
	struct timeval	start;

	bzero(&start, sizeof(struct timeval));
	bzero(&data->end, sizeof(struct timeval));
	gettimeofday(&start, NULL);
	if (send_packet(data))
		return (EXIT_FAILURE);
	timersub(&data->end, &start, &data->end);
	if (data->reply_true)
		printf(" %.3f ms", (float)data->end.tv_usec / 1000);
	return (EXIT_SUCCESS);
}

int		create_socket(t_data *data) {
	if ((data->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
		return (error_errno(data, "socket"));
	return (EXIT_SUCCESS);
}
