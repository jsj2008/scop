
#include "scop.h"

static bool		m_texture_json_loop(t_m_texture *m_texture,
									cJSON *json_textures)
{
	(void)m_texture;(void)json_textures;
	t_texture		*texture;(void)texture;
	int				index;(void)index;
	cJSON			*json_texture;(void)json_texture;
	char			*path_tex;(void)path_tex;

	path_tex = NULL;
	index = 0;
	json_texture = json_textures->child;
	if (!json_texture)
		return (ft_bool_error("JSON textures is undefined ", NULL, NULL));
	while (json_texture)
	{
		if (!json_parse_string(json_texture, "path", &path_tex))
		{
			return (dprintf(2, "JSON texture[%i]: the path is Undefined\n",
					index) == 0);
		}
		if (!(texture = m_texture->new(m_texture, MATERIAL_MAP_AMBIENT, path_tex)))
			return (false);
		json_texture = json_texture->next;
		index++;
	}
	return (true);
}

bool			m_texture_json_parse(t_m_texture *m_texture,
									cJSON *get,
									const char *key)
{
	cJSON	*source;

	source = cJSON_GetObjectItemCaseSensitive(get, key);
	if (source)
	{
		if (!(m_texture_json_loop(m_texture, source)))
		{
			return (ft_bool_error("Erreur: Le parsing de m_texture_hidden a échoué",
					NULL, NULL));
		}
	}
	return (true);
}