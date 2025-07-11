/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:01:17 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/03 17:44:21 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int		print_usage(t_data *data) {
	printf("Usage:\n  ft_traceroute host [ packetlen ]\nOptions:\n  -m max_ttl");
	printf("                  Set the max number of hops (max TTL to be\n");
	printf("                              reached). Default is 30\n  --help                      Read this help and exit\n\nArguments:\n");
	printf("+     host          The host to traceroute to\n      packetlen     The full packet length (default is the length of an IP");
	printf("\n                    header plus 40). Can be ignored or increased to a minimal\n                    allowed value\n");
	free_data(&data);
	return (EXIT_FAILURE);
}

int		error_errno(t_data *data, char *s) {
	fprintf(stderr, "%s: %s\n", s, strerror(errno));
	free_data(&data);
	return (EXIT_FAILURE);
}

int		error_perso(t_data *data, char *s) {
	fprintf(stderr, "%s\n", s);
	free_data(&data);
	return (EXIT_FAILURE);
}
