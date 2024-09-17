#pragma once
#include "Utility.h"
#include <windows.h>
#include <gl/GL.h>

#pragma comment(lib, "opengl32")

const unsigned short int GL_ARRAY_BUFFER = 0x8892;
const unsigned short int GL_ELEMENT_ARRAY_BUFFER = 0x8893;
const unsigned short int GL_FRAGMENT_SHADER = 0x8B30;
const unsigned short int GL_STATIC_DRAW = 0x88E4;
const unsigned short int GL_VERTEX_SHADER = 0x8B31;

const unsigned short int LeftMouseButton = 0x01;
const unsigned short int RightMouseButton = 0x02;
const unsigned short int Shiftkey = 0x10;
const unsigned short int Escapekey = 0x1b;
const unsigned short int Spacebar = 0x20;

typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

extern void (*glAttachShader) (GLuint, GLuint);
extern void (*glBindBuffer) (GLenum, GLuint);
extern void (*glBufferData) (GLenum, GLsizeiptr, const GLvoid*, GLenum);
extern void (*glCompileShader) (GLuint);
extern GLuint(*glCreateProgram) (void);
extern GLuint(*glCreateShader) (GLenum);
extern void (*glEnableVertexAttribArray) (GLuint);
extern void (*glGenBuffers) (GLsizei, GLuint*);
extern GLint(*glGetUniformLocation) (GLuint, const GLchar*);
extern void (*glLinkProgram) (GLuint);
extern void (*glShaderSource) (GLuint, GLsizei, const GLchar* const*, const GLint*);
extern void (*glUniformMatrix4fv) (GLint, GLsizei, GLboolean, const GLfloat*);
extern void (*glUseProgram) (GLuint);
extern void (*glGenerateMipmap) (GLenum);
extern void (*glDeleteShader) (GLuint);
extern void (*glBindVertexArray) (GLuint);
extern void (*glGenVertexArrays) (GLsizei, GLuint*);
extern void (*glVertexAttribPointer) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);

void Update(float time);
void Start();

extern Vector2 mousePosition;
extern bool KeyPressed(char key);

extern Vector2 screenSize;
extern Vector2 screenRatio;
