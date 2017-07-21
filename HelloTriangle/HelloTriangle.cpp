#include "HelloTriangle.h"
#include <sstream>

#include "../GLBase/Application.h"

using namespace std;

const char* vertex_shader = "# version 410 \n" \
	"in vec3 vp;" \
	"void main () {" \
	" gl_Position = vec4 (vp, 1.0);" \
	"}";
const char* fragment_shader = "# version 410 \n" \
	"out vec4 frag_colour;" \
	"void main () {" \
	" frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);" \
	"}";


int CALLBACK wWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPWSTR lpCmdLine,
	int nCmdShow
) {
	Application<HelloTriangle> app;
	return app.Run();
}

HelloTriangle::HelloTriangle()
{
}


HelloTriangle::HelloTriangle(const GLWindow* window)
	: GLWindow(window)
{

}


HelloTriangle::~HelloTriangle()
{
}

void HelloTriangle::Init()
{
	stringstream stream;
	stream << glGetString(GL_RENDERER) << endl;
	stream << glGetString(GL_VERSION) << endl;
	MessageBoxA(Handle(), stream.str().c_str(), "GL Info", MB_OK);

	glClearColor(0.5, 0.55, 0.5, 1);

	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	vao = 0;
	glGenVertexArrays(1, &vao); 
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL); 
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs); 
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

}

void HelloTriangle::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_programme);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	GLenum error = glGetError();
}