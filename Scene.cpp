#include "Scene.h"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;"
"layout (location = 1) in vec2 _textCords;"
"out vec2 textCords;"
"uniform mat4 transformationMatrix;"
"void main()"
"{"
"   gl_Position = transformationMatrix * vec4(aPos.x, aPos.y, aPos.z, 1);"
"	textCords = _textCords;"
"}";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;"
"in vec2 textCords;"
"uniform sampler2D ourTexture;"
"void main()"
"{"
"FragColor = texture(ourTexture,textCords);"
"}";

void Scene::SetUpScene() {
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Scene::RenderScene() {
	glClearColor(135.0 / 256.0, 206.0 / 256.0, 235.0 / 256.0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	for (Sprite *sprite : spriteList)
	{
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transformationMatrix");

		float rotationValues[16] = {
			std::cos(sprite->rotation), -std::sin(sprite->rotation), 0, 0,
			std::sin(sprite->rotation), std::cos(sprite->rotation) , 0, 0,
			0						  , 0						   , 1, 0,
			0						  , 0						   , 0, 1
		};
		Matrix4x4 rotationmatrix(rotationValues);

		float translationValues[16] = {
			1, 0, 0, sprite->position.x - camera->position.x,
			0, 1, 0, sprite->position.y - camera->position.y,
			0, 0, 1, 0										,
			0, 0, 0, 1										
		};
		Matrix4x4 translationMatrix(translationValues);

		float scaleValues[16] = {
			sprite->scale.x, 0				, 0, 0,
			0			   , sprite->scale.y, 0, 0,
			0			   , 0				, 1, 0,
			0			   , 0				, 0, 1
		};
		Matrix4x4 scaleMatrix(scaleValues);

		Matrix4x4 matrix = translationMatrix * rotationmatrix * scaleMatrix;
		glUniformMatrix4fv(transformLoc, 1, true, *matrix.values);

		glBindVertexArray(sprite->VAO);
		glBindTexture(GL_TEXTURE_2D, sprite->textureID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void Scene::AddSpriteToScene(Sprite* sprite) {
	spriteList.insert(spriteList.begin(), sprite);
}

void Scene::AddCameraToScene(Camera* _camera) {
	camera = _camera;
}