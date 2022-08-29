//internal includes
#include "common.h"
#include "ShaderProgram.h"

//External dependencies
#define GLFW_DLL
#include <GLFW/glfw3.h>
//#include <random>
#include <cstdlib>
//#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Figure.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

const GLuint WIDTH = 800, HEIGHT = 600; //размеры окна
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int initGL()
{
	int res = 0;
	//грузим функции opengl через glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: "  << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return 0;
}

bool keys[1024];
GLfloat deltaTime = 0.0f;

void Movement(Camera &camera)
{
	if(keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD);
	if(keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD);
	if(keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT);
	if(keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT);
	if(keys[GLFW_KEY_R])
		camera.ProcessKeyboard(RIGHT_VEER);
	if(keys[GLFW_KEY_F])
		camera.ProcessKeyboard(LEFT_VEER);
}

GLuint Cylinder::vertexLocation = 0;
GLuint Cylinder::normalLocation = 1;
GLuint Sphere::vertexLocation = 0;
GLuint Sphere::normalLocation = 1;

int main(int argc, char** argv)
{
	srand(time(NULL));

	if(!glfwInit())
		return -1;

	//запрашиваем контекст opengl версии 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //сообщ GLFW ,что 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //используем OpenGL3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  //выкючение изменения размера окна

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL basic sample", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();		//clearing allocated resources
		return -1;
	}
	
	glfwMakeContextCurrent(window); 
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetKeyCallback(window, key_callback);

	if(initGL() != 0) 
		return -1;
	
  	//Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	//создание шейдерной программы из двух файлов с исходниками шейдеров
	//используется класс-обертка ShaderProgram

	std::unordered_map < GLenum, std::string > shaders;
	shaders[GL_VERTEX_SHADER]   = "vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "fragment.glsl";
	ShaderProgram program(shaders); 
	GL_CHECK_ERRORS;

  	glfwSwapInterval(1); // force 60 frames per second

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);	//get window size form GLFW instead of using const's
	glViewport(0, 0, width, height); 			//Размер отрисовываемой области
 
	glEnable(GL_DEPTH_TEST);
 
	glm::vec3 lightPos(-30.0f, 30.0f, 30.0f);
	glm::vec3 lightCol(1.0f, 1.0f, 1.0f);

	GLfloat time_old = glfwGetTime(), time_new;

	glm::mat4 projection;

	projection = glm::perspective(glm::radians(60.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	program.StartUseShader();
	program.SetUniform("projection", projection);

	program.SetUniform("lightPos", lightPos);
	program.SetUniform("lightCol", lightCol);
	program.StopUseShader();

	struct Bounds bounds = 
	{
		.bot = -20.0f,
		.top = 20.0f,
		.back = -20.0f,
		.front = 20.0f,
		.left = -20.0f,
		.right = 20.0f
	};
	Scene S(bounds, 1.0f);
	float radius = std::max(std::max((bounds.top - bounds.bot), (bounds.right - bounds.left)), (bounds.front - bounds.back));
	Camera camera(radius);

	glm::mat4 view;

	system("xset r off");

	GLfloat lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;	
		
		glfwPollEvents();				//event handling
		
		Movement(camera);

		// очистка и заполнение экрана цветом
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);

		time_new = glfwGetTime();
		if(time_new - time_old > 0.1f)
		{
			time_old = time_new;
			S.GenNext();
		}
	
		program.StartUseShader();
		view = camera.GetViewMatrix();
		program.SetUniform("view", view);
		S.Draw(program);
		program.StopUseShader();
		
		glfwSwapBuffers(window); 
	}

	//очищаем vboи vao перед закрытием программы
	//

	glDeleteVertexArrays(1, &Sphere::Sphere_VAO);
	glDeleteVertexArrays(1, &Cylinder::Cylinder_VAO);
	glDeleteBuffers(1, &Sphere::Sphere_VBO);
	glDeleteBuffers(1, &Cylinder::Cylinder_VBO);

	glfwTerminate();		//clearing allocated resources

	system("xset r on");

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if(key >= 0 && key < 1024)
	{
		if(action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if(action == GLFW_RELEASE)
		{
			keys[key] = false;	
			glfwGetKey(window, key);
		}
	}
}
