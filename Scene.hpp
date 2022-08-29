#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glm/glm.hpp>
#include <cstdlib>

#include "Figure.hpp"

#define DIRS_NUM 6

struct Bounds
{
	GLfloat bot, top;
	GLfloat back, front;
	GLfloat left, right;
};

enum State
{
	VERTEX,			//-->LINE (-->LINE_END)
	LINE,			//-->LINE, LINE_END
	LINE_END,		//-->VERTEX, PIPE_END
	PIPE_END,		//-->VERTEX
	SCENE_END
};

class Scene
{
	public:
	enum State state;
	struct Bounds bounds;

	int line_len;
	int cur_line_len;
	int line_num;
	glm::vec3 cur_color;
	int free_cells;
	int pipe_num;
	int max_line_len;

	GLfloat cell;
	bool ***cells;
	int x, y, z;
	glm::ivec3 pos_cell;

	glm::ivec3 direction;
	std::vector<Figure *> Objects;
	Scene(struct Bounds &b, GLfloat sellwidth);
	void GenNext();
	int Vertex();
	void Line();
	void LineEnd();
	void PipeEnd();
	void Draw(ShaderProgram &program);
	glm::vec3 cell2world(glm::ivec3 cell_pos);
	void PrintCells();

	static glm::ivec3 all_dirs[6];
	
	~Scene()
	{
		for(int i = 0; i < x; i++)
		{
			for(int j = 0; j < y; j++)
			{
				delete [] cells[i][j];
			}
			delete [] cells[i];
		}
		delete [] cells;
	
		for(std::vector<Figure *>::iterator it = Objects.begin(); it != Objects.end(); it++)
		{
			delete *it;
		}
	}
};

#endif
