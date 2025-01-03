/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:27:21 by vagarcia          #+#    #+#             */
/*   Updated: 2025/01/03 14:40:34 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	ft_atoi(const char *str)
{
	unsigned long	number;
	int				i;
	int				sign;

	i = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	if (!(str[i] >= '0' && str[i] <= '9'))
		return (1);
	number = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		number = number * 10 + str[i++] - '0';
		if (sign == 1 && number >= 9223372036854775807)
			return (-1);
		else if (sign == -1 && number >= (unsigned long)9223372036854775807 + 1)
			return (0);
	}
	return ((int)number * sign);
}

void	send_signal(pid_t pid, unsigned char c)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		if (c & (1 << i))
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		usleep(200);
		i++;
	}
}

int	main(int argc, char **argv)
{
	pid_t				server_pid;
	struct sigaction	sa;
	unsigned char		*message;
	int					i;

	if (argc != 3)
		return (write(2, "./client <server_pid> <message>\n", 33), 1);
	i = 0;
	server_pid = ft_atoi(argv[1]);
	if (server_pid <= 0)
		return (write(STDERR_FILENO, "Invalid PID\n", 13), 1);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = 0; 
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (perror("sigaction"), exit(EXIT_FAILURE), 1);
	message = (unsigned char *)argv[2];
	while (message[i] != '\0')
		send_signal(server_pid, message[i++]);
	send_signal(server_pid, '\0');
	return (0);
}
