/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 14:24:08 by martalop          #+#    #+#             */
/*   Updated: 2024/10/04 14:34:05 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	int	*int_arr;
	int	i;

	i = 0;
	int_arr = malloc(sizeof(int) * 3);
	while (i < 3)
	{
		int_arr[i] = i + 1;
		i++;
	}
	i = 0;
	while (i < 3)
	{
		printf("int_arr[%d]: %d\n", i, int_arr[i]);
		i++;
	}
	printf("%d\n", int_arr[0]);
	return (0);
}
