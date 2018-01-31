/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntoniolo <ntoniolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/19 22:16:06 by ntoniolo          #+#    #+#             */
/*   Updated: 2018/01/31 22:24:20 by ntoniolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static bool	size_of_bmp_texture(t_texture *texture, const int fd, uint32_t *size_texture)
{
	read(fd, &texture->width, sizeof(uint32_t));
	read(fd, &texture->height, sizeof(uint32_t));
	*size_texture = texture->width * texture->height * 4;
	if (texture->width > 4996 || texture->height > 4996)
		return (false);
	return (true);
}

static unsigned char	*import_texture(t_texture *texture, const char *path)
{
	unsigned char	*data_texture;
	int				fd;
	uint32_t		size_texture;

	if (!(fd = open(path, O_RDONLY)))
		return (NULL);
	if (!size_of_bmp_texture(texture, fd, &size_texture) ||
		!(data_texture = ft_memalloc(size_texture)))
	{
		close(fd);
		return (NULL);
	}
	if ((read(fd, data_texture, size_texture)) != size_texture)
	{
		ft_memdel((void *)data_texture);
		close(fd);
		return (NULL);
	}
	close(fd);
	return (data_texture);
}

void			*textures_destroy(t_m_textures **textures)
{
	if (!textures || !*textures)
		return (NULL);
	ft_memdel((void *)textures);
	return (NULL);
}

void			*texture_destroy(t_texture **texture)
{
	if (!texture || !*texture)
		return (NULL);
	ft_memdel((void*)texture);
	return (NULL);
}
t_texture			*texture_construct(const char *texture_path)
{
	t_texture			*texture;
	unsigned char	*data_texture;

	if (!(texture = ft_memalloc(sizeof(t_texture))))
		return (NULL);
	if (!(data_texture = import_texture(texture, texture_path)))
		return (texture_destroy(&texture));
	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
								texture->width,
								texture->height,
								0, GL_BGRA, GL_UNSIGNED_BYTE,
								data_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	ft_memdel((void *)&data_texture);
	return (texture);
}

bool	texture_add(t_m_textures *textures,
				const char *texture_path)
{
	if (!(textures->texture = realloc(textures->texture, sizeof(t_texture **) * (textures->size + 1))))
		return (false);
	textures->texture[textures->size] = texture_construct(texture_path);
	textures->size++;
	return (true);
}
/*
void	texture_set(const t_texture *textures,
					const t_mesh *mesh,
					const t_shader *shader,
					const char *name_uniform)
{
	glUniform1i(glGetUniformLocation(shader->program, name_uniform), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->tex->id);
}*/

t_m_textures		*textures_construct()
{
	t_m_textures		*textures;

	if (!(textures = ft_memalloc(sizeof(t_m_textures))))
		return (textures_destroy(&textures));
	textures->add = &texture_add;
	return (textures);
}
