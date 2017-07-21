#include "VertexBufferObjects.h"

#include "../GLBase/Application.h"

const char* vertex_shader = "# version 410 \n" \
"layout( location = 0) in vec3 vertex_position; \n" \
"layout( location = 1) in vec3 vertex_colour; \n" \
"out vec3 colour; \n" \
"void main () { \n" \
"colour = vertex_colour; \n" \
"gl_Position = vec4 (vertex_position, 1.0); \n" \
"}";

const char* fragment_shader = "# version 410 \n" \
"in vec3 colour; \n" \
"out vec4 frag_colour; \n" \
"void main () { \n" \
"frag_colour = vec4 (colour, 1.0); \n" \
"}";

int CALLBACK wWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPWSTR lpCmdLine,
	int nCmdShow
) {
	Application<VertexBufferObjects> app;
	return app.Run();
}

VertexBufferObjects::VertexBufferObjects()
{
}

VertexBufferObjects::VertexBufferObjects(const GLWindow* window)
	: GLWindow(window)
{

}


VertexBufferObjects::~VertexBufferObjects()
{
}

void VertexBufferObjects::Init()
{

	glClearColor(0.5, 0.55, 0.5, 1);

	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLuint points_vbo = 0;
	glGenBuffers(1, &points_vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint colors_vbo = 0;
	glGenBuffers (1, &colors_vbo); 
	glBindBuffer (GL_ARRAY_BUFFER, colors_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof (colors), colors, GL_STATIC_DRAW);

	vao = 0;
	glGenVertexArrays(1, &vao); 
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);

	glBindAttribLocation(shader_programme, 0, "vertex_position"); 
	glBindAttribLocation(shader_programme, 1, "vertex_colour");

	glLinkProgram(shader_programme);

}

void VertexBufferObjects::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_programme);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}