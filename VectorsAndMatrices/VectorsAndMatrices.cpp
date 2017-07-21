#include "VectorsAndMatrices.h"

#include "../GLBase/Application.h"

const char* vertex_shader = "# version 410 \n" \
"in vec3 vertex_position;  \n" \
"in vec3 vertex_colour; \n" \
"uniform mat4 matrix; // our matrix \n" \
"out vec3 colour;  \n" \
"void main () {  \n" \
"	colour = vertex_colour; \n" \
"	gl_Position = matrix * vec4 (vertex_position, 1.0); \n" \
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
	Application<VectorsAndMatrices> app;
	return app.Run();
}

VectorsAndMatrices::VectorsAndMatrices()
	: matrix{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.0f, 0.0f, 1.0f}
	, speed (1.0f)
	, lastPosition (0.0)
{
}


VectorsAndMatrices::VectorsAndMatrices(const GLWindow* window)
	: GLWindow(window)
	, matrix{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.0f, 0.0f, 1.0f }
	, speed(1.0f)
	, lastPosition(0.0)
{
}

VectorsAndMatrices::~VectorsAndMatrices()
{
}

void VectorsAndMatrices::Init()
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
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

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

void VectorsAndMatrices::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int matrix_location = glGetUniformLocation(shader_programme, "matrix");
	glUseProgram(shader_programme);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void VectorsAndMatrices::TimeElapsed(ULONGLONG elapsedTime)
{
	if (fabs(lastPosition) > 1.0f) {
		speed = -speed;
	}
	float time = (elapsedTime / 5000.f);
	matrix[12] = time * speed + lastPosition;
	lastPosition = matrix[12];
}