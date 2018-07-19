/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_material_get_index.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntoniolo <ntoniolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 16:12:02 by ntoniolo          #+#    #+#             */
/*   Updated: 2018/07/19 22:57:15 by ntoniolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"



int				m_material_get_index(t_m_material *m_material, char *str)
{
	unsigned int i;

	if (!str || !(*str))
		return (0);
	i = 0;
	while (i < m_material->size)
	{
		printf("%s %s\n", m_material->material[i]->name, str);
		if (!strcmp(m_material->material[i]->name, str))
			return (i);
		i++;
	}
	return (0);
}