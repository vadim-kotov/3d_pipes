#ifndef FIGURE_H
#define FIGURE_H

#include "common.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "ShaderProgram.h"

struct Model
{
	glm::vec3 pos;
	glm::vec3 scale; 
	glm::vec3 rot_axis;		//position
	GLfloat rot;
};

struct Material
{
	glm::vec3 col;
};

class Figure
{
	protected:
	glm::mat4 modelMatrix;
	glm::mat4 normalMatrix;
	
	public:
	struct Model model;
	struct Material material;

	virtual void Draw(ShaderProgram &program) = 0;
	virtual void ChangeModelPS(glm::vec3 pos, glm::vec3 scale) = 0;
};

#define CYL_SECTS 10

class Cylinder : public Figure
{
	private:
	void PushVec3(const glm::vec3 &vect);
	void GenVertexes();

	public:
	static GLuint Cylinder_VBO;
	static GLuint Cylinder_VAO;
	static std::vector<GLfloat> CylinderVertexes;
	static GLuint vertexLocation, normalLocation;

	void Draw(ShaderProgram &program);
	void ChangeModelPS(glm::vec3 pos, glm::vec3 scale);

	Cylinder(struct Model &cylinder_model, struct Material &cylinder_material);

};

#define REC_DEPTH 2

class Sphere : public Figure
{
	private:
	void PushVec3(const glm::vec3 &vect);
	void GenVertexes();
	void TriangleSegmentation(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, std::vector<glm::vec3> &res, int rec_depth);

	public:
	static GLuint Sphere_VBO;
	static GLuint Sphere_VAO;
	static std::vector<GLfloat> SphereVertexes;
	static GLuint vertexLocation, normalLocation;

	void Draw(ShaderProgram &program);
	void ChangeModelPS(glm::vec3 pos, glm::vec3 scale);

	Sphere(struct Model &sphere_model, struct Material &sphere_material);

};


#endif
