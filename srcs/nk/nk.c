#include "scop_nk.h"
#include "scop.h"

/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
/*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
/*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
/*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
/*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
/*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
/*nk_style_load_all_cursors(nk->, atlas->cursors);*/
/*nk_style_set_font(nk->, &droid->handle);*/
/*set_style(nk->, THEME_WHITE);*/
/*set_style(nk->, THEME_RED);*/
/*set_style(nk->, THEME_BLUE);*/
/*set_style(nk->, THEME_DARK);*/

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

t_nk				*nk_construct(GLFWwindow *win)
{
	t_nk *nk;

	if (!(nk = ft_memalloc(sizeof(t_nk))))
		return (NULL);
	nk->win = win;
	nk->ctx = nk_glfw3_init(nk->win, NK_GLFW3_INSTALL_CALLBACKS);
	nk_glfw3_font_stash_begin(&nk->atlas);
	nk_glfw3_font_stash_end();
	return (nk);
}

void				*nk_destruct(t_nk **nk)
{
	if (nk && *nk)
	{
		ft_memdel((void **)nk);
	}
	return (NULL);
}

void				nk_update(t_nk *nk)
{
	(void)nk;
	nk_glfw3_new_frame();

}
void				nk_render(t_nk *nk)
{
	(void)nk;
	nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

#define MENU_NONE (1 << 0)
#define MENU_FILE (1 << 1)

void				nk_position(t_nk *nk, t_vector *vector)
{
	struct nk_context		*ctx;

	ctx = nk->ctx;
	nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
	nk_layout_row_push(ctx, 70);
	nk_label(ctx, "Position", NK_TEXT_LEFT);
	nk_layout_row_push(ctx, 90);
	nk_property_float(ctx, "X:", -2000.f, &vector->x, 2000.f, 0.1f, 0.2f);
	nk_layout_row_push(ctx, 90);
	nk_property_float(ctx, "Y:", -2000.f, &vector->y, 2000.f, 0.1f, 0.2f);
	nk_layout_row_push(ctx, 90);
	nk_property_float(ctx, "Z:", -2000.f, &vector->z, 2000.f, 0.1f, 0.2f);
	nk_layout_row_end(ctx);
}
/*
void				nk_direction(t_nk *nk, t_vector *vector, t_cam *cam)
{
	struct nk_context		*ctx;

	ctx = nk->ctx;
	nk_layout_row_begin(ctx, NK_STATIC, 20, 4);
	nk_layout_row_push(ctx, 70);
	nk_label(ctx, "Position", NK_TEXT_LEFT);
	nk_layout_row_push(ctx, 90);
	nk_property_float(ctx, "X:", 0.f, &vector->x, 1.f, 0.1f, 0.2f);
	nk_layout_row_push(ctx, 90);
	nk_property_float(ctx, "Y:", 0.f, &vector->y, 1.f, 0.1f, 0.2f);
	nk_layout_row_push(ctx, 90);
	nk_property_float(ctx, "Z:", 0.f, &vector->z, 1.f, 0.1f, 0.2f);
	nk_layout_row_end(ctx);
	(void)cam;
	//cam->first_callback = false;
}*/


void				nk_direction(t_nk *nk, t_vector *vector, t_cam *cam)
{
	struct nk_context		*ctx;

	ctx = nk->ctx;
	nk_layout_row_static(ctx, 25, 200, 1);
	sprintf(nk->buffer, "%.2f, %.2f, %.2f", vector->x, vector->y, vector->z);
	if (nk_combo_begin_label(ctx, nk->buffer, nk_vec2(200,200)))
	{
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_property_float(ctx, "Yaw", -100.f, &cam->yaw, 100.f, 0.1f, 0.05f);
		nk_property_float(ctx, "Pitch", -100.f, &cam->pitch, 100.f, 0.1f, 0.05f);
		nk_combo_end(ctx);
		cam->first_callback = false;
		cam->front = cam_get_front(cam->pitch, cam->yaw);
	}
}

void				nk_camera(t_nk *nk, t_cam *cam)
{
	struct nk_context		*ctx;

	ctx = nk->ctx;
	if (nk_begin(ctx, "Camera", nk_rect(10, 10, 400, 600),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
		NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
	{
		nk_menubar_begin(ctx);
		nk_layout_row_begin(ctx, NK_STATIC, 25, 5);
		nk_layout_row_push(ctx, 45);
		if (nk_menu_begin_label(ctx, "MENU", NK_TEXT_LEFT, nk_vec2(120, 200)))
		{
			nk->state = (nk->menu_state == MENU_FILE) ? NK_MAXIMIZED: NK_MINIMIZED;
			if (nk_tree_state_push(ctx, NK_TREE_TAB, "FILE", &nk->state))
			{
				nk->menu_state = MENU_FILE;
				nk_menu_item_label(ctx, "New", NK_TEXT_LEFT);
				nk_menu_item_label(ctx, "Open", NK_TEXT_LEFT);
				nk_menu_item_label(ctx, "Save", NK_TEXT_LEFT);
				nk_menu_item_label(ctx, "Save As", NK_TEXT_LEFT);
				nk_menu_item_label(ctx, "Exit", NK_TEXT_LEFT);
				nk_tree_pop(ctx);
			}
			else
				nk->menu_state = (nk->menu_state == MENU_FILE) ? MENU_NONE: nk->menu_state;
			nk_menu_end(ctx);
		}

		if (nk_tree_push(ctx, NK_TREE_TAB, "Camera", NK_MINIMIZED)) {
			nk_layout_row_dynamic(ctx, 20, 1);
			nk_labelf(ctx, NK_TEXT_LEFT, "FOV [%.2f]", cam->fov);
			nk_slider_float(ctx, 20.f, &cam->fov, 90.f, 0.1f);


			nk_position(nk, &cam->position);
			nk_direction(nk, &cam->angle, cam);

			nk_tree_pop(ctx);

		}

	}
	nk_menubar_end(ctx);
	nk_end(ctx);
}

void				nk_example(t_nk *nk)
{
	nk->bg.r = 0.10f, nk->bg.g = 0.18f, nk->bg.b = 0.24f, nk->bg.a = 1.0f;
	if (nk_begin(nk->ctx, "Demo", nk_rect(50, 50, 230, 250),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
		NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
	{
		enum {EASY, HARD};
		static int op = EASY;
		static int property = 20;
		nk_layout_row_static(nk->ctx, 30, 80, 1);
		if (nk_button_label(nk->ctx, "button"))
			fprintf(stdout, "button pressed\n");

		nk_layout_row_dynamic(nk->ctx, 30, 2);
		if (nk_option_label(nk->ctx, "easy", op == EASY)) op = EASY;
		if (nk_option_label(nk->ctx, "hard", op == HARD)) op = HARD;

		nk_layout_row_dynamic(nk->ctx, 25, 1);
		nk_property_int(nk->ctx, "Compression:", 0, &property, 100, 10, 1);

		nk_layout_row_dynamic(nk->ctx, 20, 1);
		nk_label(nk->ctx, "background:", NK_TEXT_LEFT);
		nk_layout_row_dynamic(nk->ctx, 25, 1);
		if (nk_combo_begin_color(nk->ctx, nk_rgb_cf(nk->bg), nk_vec2(nk_widget_width(nk->ctx),400))) {
			nk_layout_row_dynamic(nk->ctx, 120, 1);
			nk->bg = nk_color_picker(nk->ctx, nk->bg, NK_RGBA);
			nk_layout_row_dynamic(nk->ctx, 25, 1);
			nk->bg.r = nk_propertyf(nk->ctx, "#R:", 0, nk->bg.r, 1.0f, 0.01f,0.005f);
			nk->bg.g = nk_propertyf(nk->ctx, "#G:", 0, nk->bg.g, 1.0f, 0.01f,0.005f);
			nk->bg.b = nk_propertyf(nk->ctx, "#B:", 0, nk->bg.b, 1.0f, 0.01f,0.005f);
			nk->bg.a = nk_propertyf(nk->ctx, "#A:", 0, nk->bg.a, 1.0f, 0.01f,0.005f);
			nk_combo_end(nk->ctx);
		}
	}
	nk_end(nk->ctx);
}
