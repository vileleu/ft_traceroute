/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:13:33 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/26 17:23:00 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static int	opt_m(t_data *data, t_parse *parsing, int i) {
	char	s[BUFFER_SIZE] = "";
	int		nb = 1;

	if (parsing->av[i][2] == '\0') {
		if (i < parsing->ac - 1) {
			if (!check_atoi(parsing->av[i + 1])) {
				nb = atoi(parsing->av[i + 1]);
				parsing->ignore_next = 1;
			}
			else
				sprintf(s, "Cannot handle `-m' option with arg `%s' (argc %d)", parsing->av[i + 1], i + 2);	
		}
		else
			sprintf(s, "Option `-m' (argc %d) requires an argument: `-m max_ttl'", i + 1);
	}
	else if (!check_atoi(parsing->av[i] + 2))
		nb = atoi(parsing->av[i] + 2);
	else
		sprintf(s, "Cannot handle `-m' option with arg `%s' (argc %d)", parsing->av[i] + 2, i + 1);
	if (nb <= 0)
		sprintf(s, "first hop out of range");
	else if (nb >= 256)
		sprintf(s, "max hops cannot be more than 255");
	if (s[0])
		return (error_perso(data, s));
	parsing->m = 1;
	parsing->max_ttl = nb;
	return (EXIT_SUCCESS);
}

static int	get_opt(t_data *data, t_parse *parsing, int i) {
	char	s[BUFFER_SIZE] = "";

	if (!strncmp(parsing->av[i], "-m", 2)) {
		if (opt_m(data, parsing, i))
			return (EXIT_FAILURE);
	}
	else
		sprintf(s, "Bad option `%s' (argc %d)", parsing->av[i], i + 1);
	if (s[0])
		return (error_perso(data, s));
	return (EXIT_SUCCESS);
}

static int	get_host(t_data *data, t_parse *parsing, int i) {
	char			s[BUFFER_SIZE] = "";
	struct addrinfo	hints, *res;
	int				ret;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	if ((ret = getaddrinfo(parsing->av[i], NULL, &hints, &res)))
		sprintf(s, "getaddrinfo: %s\nCannot handle \"host\" cmdline arg `%s' on position 1 (argc %d)", \
			gai_strerror(ret), parsing->av[i], i + 1);
	else {
		data->dst = *(struct sockaddr_in *)res->ai_addr;
		freeaddrinfo(res);
		parsing->check_host = 1;
		data->target = parsing->av[i];
	}
	if (s[0])
		return (error_perso(data, s));
	return (EXIT_SUCCESS);
}

int		get_arg(t_data *data, t_parse *parsing, int i) {
	char			s[BUFFER_SIZE] = "";

	if (!parsing->check_host)
		return (get_host(data, parsing, i));
	else if (!parsing->check_packetlen) {
		if (check_atoi(parsing->av[i]))
			sprintf(s, "Cannot handle \"packetlen\" cmdline arg `%s' on position 2 (argc %d)", parsing->av[i], i + 1);
		else {
			parsing->check_packetlen = 1;
			parsing->packetlen = atoi(parsing->av[i]);
			if (parsing->packetlen > 65000)
				sprintf(s, "too big packetlen %d specified", parsing->packetlen);
		}
	}
	else
		sprintf(s, "Extra arg `%s' (position 3, argc %d)", parsing->av[i], i + 1);
	if (s[0])
		return (error_perso(data, s));
	return (EXIT_SUCCESS);
}

int		parse(t_data *data, char **av, int ac) {
	t_parse	parsing;
	int		i = 0;

	bzero(&parsing, sizeof(t_parse));
	while (i < ac) {
		if (!(strcmp(av[i], "--help")))
			return (print_usage(data));
		i++;
	}
	parsing.av = av;
	parsing.ac = ac;
	i = 0;
	while (i < parsing.ac) {
		parsing.ignore_next = 0;
		if (parsing.av[i][0] == '-') {
			if (get_opt(data, &parsing, i))
				return (EXIT_FAILURE);
		}
		else {
			if (get_arg(data, &parsing, i))
				return (EXIT_FAILURE);
		}
		i += 1 + parsing.ignore_next;
	}
	if (parsing.m)
		data->max_ttl = parsing.max_ttl;
	if (parsing.check_packetlen)
		data->packetlen = parsing.packetlen;
	return (EXIT_SUCCESS);
}
