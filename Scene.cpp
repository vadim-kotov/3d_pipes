#include "Scene.hpp"

glm::ivec3 Scene::all_dirs[6] = 
	{
		glm::ivec3(-1, 0, 0), glm::ivec3(1, 0, 0),
		glm::ivec3(0, -1, 0), glm::ivec3(0, 1, 0),
		glm::ivec3(0, 0, -1), glm::ivec3(0, 0, 1)
	};

Scene::Scene(struct Bounds &b, GLfloat sellwidth)
{
	bounds = b;
	cell = sellwidth;

	x = (bounds.right - bounds.left) / cell;
	y = (bounds.top - bounds.bot) / cell;
	z = (bounds.front - bounds.back) / cell;
	free_cells = x*y*z;

	max_line_len = std::min(z, std::min(x, y)) / 4;

	cells = new bool**[x];
	for(int i = 0; i < x; i++)
	{
		cells[i] = new bool*[y];
		for(int j = 0; j < y; j++)
		{
			cells[i][j] = new bool[z];
			for(int k = 0; k < z; k++)
			{
				cells[i][j][k] = false;
			}
		}
	}
	direction = glm::ivec3(0);

	pipe_num = rand() % 3 + 5;

	state = PIPE_END;
}

int Scene::Vertex()		//VERTEX
{
	std::vector<glm::ivec3> poss_dirs;

	glm::ivec3 next_pos;
	glm::ivec3 cur_dir;
	for(int i = 0; i < DIRS_NUM; i++)
	{
		cur_dir = all_dirs[i];
		if(direction != glm::ivec3(0, 0, 0) && (cur_dir == direction || cur_dir == -direction))
			continue;

		next_pos = pos_cell + cur_dir;
		if((cur_dir.x && (next_pos.x >= x || next_pos.x < 0)) ||
			(cur_dir.y && (next_pos.y >= y || next_pos.y < 0)) ||
			(cur_dir.z && (next_pos.z >= z || next_pos.z < 0)) ||
			cells[next_pos.x][next_pos.y][next_pos.z])
			continue;

		poss_dirs.push_back(cur_dir);
	}

	if(poss_dirs.empty())
	{
		return -1;
	}
	direction = poss_dirs[rand() % poss_dirs.size()];

	int poss_len = 1;
	next_pos = pos_cell + 2 * direction;
	while(1)
	{
		if((direction.x && (next_pos.x >= x || next_pos.x < 0)) ||
			(direction.y && (next_pos.y >= y || next_pos.y < 0)) ||
			(direction.z && (next_pos.z >= z || next_pos.z < 0)) ||
			cells[next_pos.x][next_pos.y][next_pos.z])
		{
			break;
		}

		next_pos += direction;
		poss_len++;
	}

	pos_cell += direction;
	int res;

	struct Model model;
	if(direction == glm::ivec3(0, 1, 0) || direction == glm::ivec3(0, -1, 0))
	{
		model.rot_axis = glm::vec3(1.0f, 0.0f, 0.0f);
		model.rot = 0.0f;
	}
	else
	{
		model.rot_axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(direction));
		model.rot = glm::radians(90.0f);
	}
	struct Material material =
	{
		.col = cur_color
	};

	if(poss_len == 1)
	{
		cur_line_len = 0;

		model.pos = glm::vec3(cell2world(pos_cell) - glm::vec3(direction) * (3.0f/4));
		model.scale = glm::vec3(cell/2);

		Objects.push_back(new Cylinder(model, material));

		return 0;
	}
	else
	{
		if(poss_len > max_line_len)
		{
			res = rand() % (max_line_len - 1) + 1;
		}
		else
		{
			res = rand() % (poss_len - 1) + 1;
		}

		cur_line_len = 1;

		next_pos = pos_cell;	
		for(int i = 0; i < res; i++)
		{
			cells[next_pos.x][next_pos.y][next_pos.z] = true;
			free_cells--;
			next_pos += direction;
		}

		model.pos = cell2world(pos_cell) - glm::vec3(direction) * (1.0f/4);
		model.scale = glm::vec3(cell/2, cell * (3.0f/2), cell/2);
		Objects.push_back(new Cylinder(model, material));	
	
		if(res == 1)
		{

			state = LINE_END;
		}
		else
		{
			state = LINE;
		}
		
		return res;
	}
}

void Scene::Line()
{
	cur_line_len++;

	Figure *figure = Objects.back();
	struct Model *model = &figure->model;
	figure->ChangeModelPS(model->pos + glm::vec3(direction) * cell / 2.0f, glm::vec3(model->scale.x, (cur_line_len + 0.5) * cell, model->scale.z));

	if(cur_line_len == line_len)
	{
		state = LINE_END;
	}
	else
	{
		state = LINE;
	}

	return;
}

void Scene::LineEnd()
{
	pos_cell += direction * line_len;

	Figure *prev_figure = Objects.back();
	struct Model *prev_model = &prev_figure->model;
	prev_figure->ChangeModelPS(prev_model->pos + glm::vec3(direction) * cell / 4.0f, glm::vec3(prev_model->scale.x, (cur_line_len + 1) * cell, prev_model->scale.z));

	struct Model model = 
	{
		.pos = cell2world(pos_cell),
		.scale = glm::vec3(cell * ((rand() % 2 + 2) / 4.0f)),
		.rot_axis = glm::vec3(1.0f, 0.0f, 0.0f),
		.rot = 0.0f
	};
	struct Material material = 
	{
		.col = cur_color
	};
	Objects.push_back(new Sphere(model, material));
	cells[pos_cell.x][pos_cell.y][pos_cell.z] = true;
	free_cells--;

	line_num--;
	if(line_num)
	{
		state = VERTEX;
	}
	else
	{
		direction = glm::ivec3(0);
		state = PIPE_END;
	}

	return;
}

void Scene::PipeEnd()
{
	pipe_num--;
	if(!pipe_num)
	{
		for(std::vector<Figure *>::iterator it = Objects.begin(); it != Objects.end(); it++)
		{
			delete [] *it;
		}
		Objects.clear();

		for(int i = 0; i < x; i++)
			for(int j = 0; j < y; j++)
				for(int k = 0; k < z; k++)
					cells[i][j][k] = false;

		free_cells = x*y*z;
		pipe_num = rand() % 3 + 5;
	}

	int cell_num = rand() % free_cells;

	for(int i = 0; i < x; i++)
		for(int j = 0; j < y; j++)
			for(int k = 0; k < z; k++)
			{
				if(cells[i][j][k])
					continue;

				if(!cell_num)
				{
					pos_cell = glm::ivec3(i, j, k);
					cells[i][j][k] = true;
					free_cells--;
					struct Model model =
					{
						.pos = cell2world(pos_cell),
						.scale = glm::vec3(cell * ((rand() % 2 + 2) / 4.0f)),
						.rot_axis = glm::vec3(1.0f),
						.rot = 0.0f
					};
					cur_color = glm::vec3(static_cast<float> (rand()) / RAND_MAX, static_cast<float> (rand()) / RAND_MAX, static_cast<float> (rand()) / RAND_MAX);

					struct Material material =
					{
						.col = 	cur_color
					};
					Objects.push_back(new Sphere(model, material));

					line_num = rand() % 50 + 10;	

					state = VERTEX;

					return;
				}

				cell_num--;
			}
}

void Scene::GenNext()
{
	if(state == VERTEX)
	{
		line_len = Vertex();
		if(line_len < 0)
		{
			direction = glm::ivec3(0);
			state = PIPE_END;
			PipeEnd();
		}
		else if(!line_len)
		{
			state = LINE_END;
			LineEnd();
		}
	}
	else if(state == LINE)
	{
		Line();
	}
	else if(state == LINE_END)
	{
		LineEnd();
	}
	else if (state == PIPE_END)
	{
		PipeEnd();
	}
}

void Scene::Draw(ShaderProgram &program)
{
	for(std::vector<Figure *>::iterator it = Objects.begin(); it != Objects.end(); it++)
	{
		(*it)->Draw(program);
	}
}

glm::vec3 Scene::cell2world(glm::ivec3 cell_pos)
{
	glm::vec3 res;

	res = glm::vec3(bounds.left, bounds.bot, bounds.back) + glm::vec3(cell_pos.x * cell, cell_pos.y * cell, cell_pos.z * cell) + glm::vec3(cell/2);
	return res;
}


