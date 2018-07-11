/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntoniolo <ntoniolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 20:15:15 by ntoniolo          #+#    #+#             */
/*   Updated: 2018/07/11 16:19:35 by ntoniolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include "scop_glfw.h"
# include "libft.h"
# include "matrix.h"
# include "material.h"

# include <math.h>
# include <sys/types.h>
# include <sys/time.h>
# include <stdint.h>
# include <stdio.h>
#include <stdlib.h>

# define DEBUG 1

# define BUFFER_OBJ (2048)

# define WIDTH 1920
# define HEIGHT 1080

# define SCOP_V (1 << 1)
# define SCOP_VT (1 << 2)
# define SCOP_VN (1 << 3)
# define F_TEXTURE (1 << 4)

#define MAX_SOURCE_SIZE 8000
#define BUFFER_LOG 2048


typedef struct		s_fps
{
	struct timeval	step2;
	struct timeval	step;
	struct timeval	cur;
	float			delta_time;
	unsigned int	fps;
	unsigned int	ret_fps;
}					t_fps;
t_fps 				*fps_construct();
void 				*fps_destruct(t_fps **e);
void				fps_update(t_fps *fps, float *ptr);


/*						*/
/*		  SHADER		*/
/*						*/
typedef struct		s_shader
{
	GLuint			program;

	void			(*use)(struct s_shader *);
}					t_shader;
void				*shader_destruct(t_shader **shader);
t_shader			*shader_construct(const char *vertex_shader_path,
									const char *fragment_shader_path);

typedef struct		s_m_shader
{
	unsigned int	size;
	t_shader		**shader;
	bool			(*add)(struct s_m_shader *, const char *, const char *);
}					t_m_shader;
t_m_shader			*m_shader_construct();
void				*m_shader_destruct(t_m_shader **m_shader);
/*bool				m_shader_add(t_m_shader *m_shader,
								const char *vertex_shader_path,
								const char *fragment_shader_path);*/

/*						*/
/*		  MESH			*/
/*						*/
typedef struct		s_mesh
{
	GLuint			VBO;
	GLuint			VNBO;
	GLuint			VTBO;
	GLuint			EBO;
	GLuint			VAO;

	//t_material		*material;

	GLfloat			*indexed_v;
	GLfloat			*indexed_vt;
	GLfloat			*indexed_vn;
	GLuint			*indices;

	GLint			nb_faces;
	GLint			nb_indices;
	int				flag;
}					t_mesh;
void				*mesh_destruct(t_mesh **mesh);
t_mesh				*mesh_construct();
void				mesh_gen_gl_buffers(t_mesh *mesh);
typedef struct		s_m_mesh
{
	unsigned int	size;
	t_mesh			**mesh;
	bool			(*add)(struct s_m_mesh *, t_mesh *);
	t_mesh			*(*new)(struct s_m_mesh *);
}					t_m_mesh;
t_m_mesh			*m_mesh_construct();
void				*m_mesh_destruct(t_m_mesh **m_mesh);

/*						*/
/*		  MODEL			*/
/*						*/
typedef struct		s_model
{
	//index_material
	//index_shader
	t_m_mesh		*m_mesh;
	t_matrix		transform;
	GLenum			type_draw;
	long			flag;
	t_vector		min;
	t_vector		max;
	t_vector		center;
	t_vector		negative_center;
}					t_model;
void				model_setup_scaling(t_model *model);
void				*model_destruct(t_model **model);
t_model				*model_construct();

typedef struct		s_m_model
{
	unsigned int	size;
	t_model			**model;
	bool			(*add)(struct s_m_model *, t_model *);
}					t_m_model;
t_m_model			*m_model_construct();
void				*m_model_destruct(t_m_model **m_model);
void				model_gen_gl_buffers(t_model *model);

/*						*/
/*		  CAM			*/
/*						*/

typedef struct		s_cam
{
	bool			first_callback;
	float			pitch;
	float			yaw;
	float			sensitivity;
	t_vector		front;
	t_vector		last_cursor_position;
	t_vector		position;
	t_vector		to;
	t_vector		up;
	t_matrix		view;
	t_matrix		projection;
}					t_cam;
t_cam 		*cam_construct();
void		*cam_destruct(t_cam **cam);
void		cam_update(t_cam *cam, const t_glfw *glfw, const float delta_time);

/*						*/
/*		  SCENE			*/
/*						*/
typedef struct		s_scene
{
	t_cam			*cam;

	t_m_shader		*m_shader;
	t_m_mesh		*m_mesh;
	t_m_model		*m_model;
	bool			(*shader_add)(struct s_scene*, const char *, const char *);
	bool			(*mesh_add)(struct s_scene*, t_mesh *);
	bool			(*model_add)(struct s_scene*, t_model *);
}					t_scene;
void				*scene_destruct(t_scene **scene);
t_scene				*scene_construct();

typedef struct		s_env
{
	t_fps			*fps;
	float			delta_time;
	t_glfw			*glfw;

	t_scene			*scene;

	int64_t			flag;
}					t_env;
t_env 				*env_construct();
void 				*env_destruct(void *ptr);


typedef struct		s_lm
{
	t_model			*model;
	t_mesh			*mesh;

	GLint			nb_v;
	GLint			nb_vt;
	GLint			nb_vn;
	GLfloat			*v;
	GLfloat			*vt;
	GLfloat			*vn;

	const char		*path;
	int				fd;

	char			type[10];
	int				buffer_index_v[4];
	int				buffer_index_vt[4];
	int				buffer_index_vn[4];
	float			buffer_v[4];
	float			buffer_vt[4];
	float			buffer_vn[4];

	uint32_t		mem_len_indices;

	uint32_t		mem_len_v;
	uint32_t		mem_len_vt;
	uint32_t		mem_len_vn;
	uint32_t		mem_len_indexed_v;
	uint32_t		mem_len_indexed_vn;
	uint32_t		mem_len_indexed_vt;

	uint32_t		last_index_v;
	uint32_t		last_index_vt;
	uint32_t		last_index_vn;
	uint32_t		last_index;
	char			*line;
}					t_lm;
bool		lm_get_vertex(t_lm *lm);
bool		lm_get_vnormal(t_lm *lm);
bool		lm_get_vtexel(t_lm *lm);
bool		lm_check_realloc(t_lm *lm);
bool		lm_add_mesh(t_lm *lm);
bool		lm_get_face(t_lm *lm);
void		*lm_destruct(t_lm **c_lm);
t_lm		*lm_construct(t_model *model, const char *path_obj);


int					flag(int64_t *f, int argc, char **argv);
bool				loop(t_env *e);



t_vector			*singleton_mouse_position();
void				event_mouse(GLFWwindow *window, double pos_x, double pos_y);

float				get_degrees(const float radians);
float				get_radians(const float degrees);



t_matrix		matrixgl_view(t_cam *cam);
t_matrix		look_at_gl(const t_vector *position,
						const t_vector *to,
						const t_vector *up);
t_matrix matrixgl_get_projection(const float fov,
									const float ratio,
									const float near,
									const float far);


bool		load_model(t_scene *scene, const char * path_obj);
void		scene_render(t_scene *scene);
#endif
