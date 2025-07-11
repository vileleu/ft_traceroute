/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 16:38:32 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/10 23:27:27 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int		run_traceroute(t_data *data) {
	int		count = 0;

	printf("ft_traceroute to %s (", data->target);
	if (print_ip(data, data->target))
		return (EXIT_FAILURE);
	printf("), %d hops max, %d byte packets\n", data->max_ttl, (int)(data->packet_size + sizeof(struct ip)));
	while (data->ttl <= data->max_ttl && !data->addr_reached) {
		if ((setsockopt(data->sockfd, IPPROTO_IP, IP_TTL, &data->ttl, sizeof(data->ttl))) < 0)
			return (error_errno(data, "setsockopt"));
		printf("%d  ", data->ttl);
		count = 3;
		while (count--) {
			data->reply_true = 0;
			if (handle_packet(data))
				return (EXIT_FAILURE);
			if (count)
				printf(" ");
		}
		printf("\n");
		data->ttl++;
	}
	return (EXIT_SUCCESS);
}

int		main(int ac, char **av) {
	t_data	*data = NULL;

	if (ac == 1)
		return (print_usage(data));
	if (!(data = create_data()))
		return (error_perso(data, "ERROR MALLOC"));
	if (parse(data, av + 1, ac - 1))
		return (EXIT_FAILURE);
	if (create_packet(data))
		return (EXIT_FAILURE);
	if (create_socket(data))
		return (EXIT_FAILURE);
	if (run_traceroute(data))
		return (EXIT_FAILURE);
	free_data(&data);
	return (EXIT_SUCCESS);
}
