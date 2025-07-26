/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:17:29 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/26 19:25:49 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int		create_packet(t_data *data) {
	struct icmphdr	*icmp;
	//uint8_t		c = 0;
	//size_t		i = sizeof(struct icmphdr);

	data->packet_size = sizeof(struct icmphdr) + sizeof(struct ip);	
	while ((int)data->packet_size < data->packetlen)
		data->packet_size++;
	if (data->packetlen == -1)
		data->packet_size = 60;
	data->packet_size -= sizeof(struct ip);
	if (!(data->packet = malloc(data->packet_size)))
		return (error_perso(data, "ERROR MALLOC"));
	bzero(data->packet, data->packet_size);
	icmp = (struct icmphdr *)data->packet;
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->id = htons(getpid());
	icmp->seq = 0;
	/*while (i < data->packet_size) {
		data->packet[i] = c++;
		i++;
	}*/
	icmp->checksum = get_checksum((unsigned short *)data->packet, data->packet_size);
	return (EXIT_SUCCESS);
}

t_data	*create_data() {
	t_data	*data = NULL;

	if (!(data = malloc(sizeof(t_data))))
		return (NULL);
	bzero(data, sizeof(t_data));
	data->packet = NULL;
	data->target = NULL;
	if (!(data->previous = strdup("")))
		return (NULL);
	data->ttl = START_TTL;
	data->max_ttl = 30;
	data->packetlen = -1;
	return (data);
}
