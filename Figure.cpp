#include "Figure.hpp"

/*==============================Cylinder==============================*/

GLuint Cylinder::Cylinder_VAO = 0;
GLuint Cylinder::Cylinder_VBO = 0;

std::vector<GLfloat> Cylinder::CylinderVertexes;

void Cylinder::GenVertexes()
{
	std::vector<glm::vec3> UpperVertexes, DownVertexes;

	GLfloat dlt_angle = 2 * glm::pi<float>() / CYL_SECTS;
	GLfloat angle = 0.0f;
	for(int i = 0; i < (CYL_SECTS + 1); i++)
	{
		glm::vec3 v(cos(angle) / 2, 0.5f, sin(angle) / 2);
		UpperVertexes.push_back(v);
		DownVertexes.push_back(v - glm::vec3(0.0f, 1.0f, 0.0f));
		angle += dlt_angle;
	}
	
	for(std::vector<glm::vec3>::iterator upper_it = UpperVertexes.begin(), down_it = DownVertexes.begin(); upper_it != UpperVertexes.end() - 1; upper_it++, down_it++)
	{
		PushVec3(glm::vec3(0.0f, 0.5f, 0.0f));
		PushVec3(glm::vec3(0.0f, 1.0f, 0.0f));
		
		PushVec3(*upper_it);
		PushVec3(glm::vec3(0.0f, 1.0f, 0.0f));

		PushVec3(*(upper_it+1));
		PushVec3(glm::vec3(0.0f, 1.0f, 0.0f));



		PushVec3(glm::vec3(0.0f, -0.5f, 0.0f));
		PushVec3(glm::vec3(0.0f, -1.0f, 0.0f));
		
		PushVec3(*down_it);
		PushVec3(glm::vec3(0.0f, -1.0f, 0.0f));

		PushVec3(*(down_it+1));
		PushVec3(glm::vec3(0.0f, -1.0f, 0.0f));

		

		glm::vec3 prev_n = *upper_it - glm::vec3(0.0f, 0.5f, 0.0f);
		glm::vec3 next_n = *(upper_it + 1) - glm::vec3(0.0f, 0.5f, 0.0f);

		PushVec3(*upper_it);
		PushVec3(prev_n);

		PushVec3(*down_it);
		PushVec3(prev_n);

		PushVec3(*(down_it+1));
		PushVec3(next_n);


		PushVec3(*upper_it);
		PushVec3(prev_n);

		PushVec3(*(upper_it + 1));
		PushVec3(next_n);

		PushVec3(*(down_it + 1));
		PushVec3(next_n);
	}
	
	return;
}

void Cylinder::PushVec3(const glm::vec3 &vect)
{
	CylinderVertexes.push_back(vect.x);
	CylinderVertexes.push_back(vect.y);
	CylinderVertexes.push_back(vect.z);
	
	return;
}

Cylinder::Cylinder(struct Model &cylinder_model, struct Material &cylinder_material)
{
	model = cylinder_model;
	material = cylinder_material;

	modelMatrix = glm::translate(glm::mat4(1.0f), model.pos);
	modelMatrix = glm::rotate(modelMatrix, model.rot, model.rot_axis);
	modelMatrix = glm::scale(modelMatrix, model.scale);

	normalMatrix = glm::rotate(glm::mat4(1.0f), model.rot, model.rot_axis);
	
	if(!Cylinder_VAO)
	{
		GenVertexes();

		glGenVertexArrays(1, &Cylinder_VAO);
		glGenBuffers(1, &Cylinder_VBO);
		glBindVertexArray(Cylinder_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, Cylinder_VBO);

		glBufferData(GL_ARRAY_BUFFER, CylinderVertexes.size() * sizeof(GLfloat), CylinderVertexes.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);                          
		glEnableVertexAttribArray(vertexLocation);                                                

		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(normalLocation);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);	
	}

}

void Cylinder::ChangeModelPS(glm::vec3 pos, glm::vec3 scale)
{
	model.pos = pos;
	model.scale = scale;

	modelMatrix = glm::translate(glm::mat4(1.0f), model.pos);
	modelMatrix = glm::rotate(modelMatrix, model.rot, model.rot_axis);
	modelMatrix = glm::scale(modelMatrix, model.scale);

	return;
}

void Cylinder::Draw(ShaderProgram &program)
{
	program.SetUniform("model", modelMatrix);
	program.SetUniform("normalMatrix", normalMatrix);
	program.SetUniform("objectCol", material.col);
	glBindVertexArray(Cylinder_VAO);
	glDrawArrays(GL_TRIANGLES, 0, CylinderVertexes.size() / 2);
	glBindVertexArray(0);
}

/*=================================Sphere================================*/

GLuint Sphere::Sphere_VAO = 0;
GLuint Sphere::Sphere_VBO = 0;

std::vector<GLfloat> Sphere::SphereVertexes;

Sphere::Sphere(struct Model &sphere_model, struct Material &sphere_material)
{
	model = sphere_model;
	material = sphere_material;

	modelMatrix = glm::translate(glm::mat4(1.0f), model.pos);
	modelMatrix = glm::rotate(modelMatrix, model.rot, model.rot_axis);
	modelMatrix = glm::scale(modelMatrix, model.scale);

	normalMatrix = glm::rotate(glm::mat4(1.0f), model.rot, model.rot_axis);
	
	if(!Sphere_VAO)
	{
		GenVertexes();

		glGenVertexArrays(1, &Sphere_VAO);
		glGenBuffers(1, &Sphere_VBO);
		glBindVertexArray(Sphere_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, Sphere_VBO);

		glBufferData(GL_ARRAY_BUFFER, SphereVertexes.size() * sizeof(GLfloat), SphereVertexes.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);                          
		glEnableVertexAttribArray(vertexLocation);                                                

		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(normalLocation);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);	
	}
}

void Sphere::GenVertexes()
{
	glm::vec3 octahedron[] = 
	{
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 1.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f, 0.0f, -1.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 1.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f, 0.0f, -1.0f)
	};

	std::vector<glm::vec3> VecVertexes;

	for(int i = 0; i < 8; i++)
	{
		TriangleSegmentation(octahedron[i * 3 + 0], octahedron[i * 3 + 1], octahedron[i * 3 + 2], VecVertexes, REC_DEPTH);
	}

	SphereVertexes.reserve(VecVertexes.size() * 3 * 2);
	for(int i = 0; i < VecVertexes.size(); i++)
	{
		glm::vec3 v = glm::normalize(VecVertexes[i]);
		SphereVertexes.push_back(v.x / 2);
		SphereVertexes.push_back(v.y / 2);
		SphereVertexes.push_back(v.z / 2);

		SphereVertexes.push_back(v.x);
		SphereVertexes.push_back(v.y);
		SphereVertexes.push_back(v.z);
	}
}

void Sphere::TriangleSegmentation(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, std::vector<glm::vec3> &res, int rec_depth)
{
	glm::vec3 segs[] = 
	{
		a, (a + b) / 2.0f, (a + c) / 2.0f, 
		(a + b) / 2.0f, b, (b + c) / 2.0f,
		(a + b) / 2.0f, (a + c) / 2.0f, (b + c) / 2.0f,
		(a + c) / 2.0f, (b + c) / 2.0f, c
	};

	if(!rec_depth)
	{
		int N = sizeof(segs) / sizeof(segs[0]);
		res.insert(res.end(), segs, segs + N);
		return;
	}

	std::vector<glm::vec3> next_res[4];
	for(int i = 0; i < 4; i++)
	{
		TriangleSegmentation(segs[i * 3 + 0], segs[i * 3 + 1], segs[i * 3 + 2], next_res[i], rec_depth - 1);
		res.insert(res.end(), next_res[i].begin(), next_res[i].end());
	}

	return;
}

void Sphere::ChangeModelPS(glm::vec3 pos, glm::vec3 scale)
{
	model.pos = pos;
	model.scale = scale;

	modelMatrix = glm::translate(glm::mat4(1.0f), model.pos);
	modelMatrix = glm::rotate(modelMatrix, model.rot, model.rot_axis);
	modelMatrix = glm::scale(modelMatrix, model.scale);

	return;
}

void Sphere::Draw(ShaderProgram &program)
{
	program.SetUniform("model", modelMatrix);
	program.SetUniform("normalMatrix", normalMatrix);
	program.SetUniform("objectCol", material.col);
	glBindVertexArray(Sphere_VAO);
	glDrawArrays(GL_TRIANGLES, 0, SphereVertexes.size() / 2);
	glBindVertexArray(0);
}

