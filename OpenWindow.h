#pragma once
#include "Utility.h"
#include <windows.h>
#include <gl/GL.h>

#pragma comment(lib, "opengl32")

const unsigned short int GL_ARRAY_BUFFER = 0x8892;
const unsigned short int GL_ARRAY_BUFFER_BINDING = 0x8894;
const unsigned short int GL_COLOR_ATTACHMENT0 = 0x8CE0;
const unsigned short int GL_COMPILE_STATUS = 0x8B81;
const unsigned short int GL_CURRENT_PROGRAM = 0x8B8D;
const unsigned short int GL_DYNAMIC_DRAW = 0x88E8;
const unsigned short int GL_ELEMENT_ARRAY_BUFFER = 0x8893;
const unsigned short int GL_ELEMENT_ARRAY_BUFFER_BINDING = 0x8895;
const unsigned short int GL_FRAGMENT_SHADER = 0x8B30;
const unsigned short int GL_FRAMEBUFFER = 0x8D40;
const unsigned short int GL_FRAMEBUFFER_COMPLETE = 0x8CD5;
const unsigned short int GL_FUNC_ADD = 0x8006;
const unsigned short int GL_INVALID_FRAMEBUFFER_OPERATION = 0x0506;
const unsigned short int GL_MAJOR_VERSION = 0x821B;
const unsigned short int GL_MINOR_VERSION = 0x821C;
const unsigned short int GL_extern_DRAW = 0x88E4;
const unsigned short int GL_STREAM_DRAW = 0x88E0;
const unsigned short int GL_STATIC_DRAW = 0x88E4;
const unsigned short int GL_TEXTURE0 = 0x84C0;
const unsigned short int GL_VERTEX_SHADER = 0x8B31;
const unsigned short int GL_LINK_STATUS = 0x8B82;

const unsigned short int LeftMouseButton = 0x01;
const unsigned short int RightMouseButton = 0x02;
const unsigned short int Shiftkey = 0x10;
const unsigned short int Escapekey = 0x1b;
const unsigned short int Spacebar = 0x20;

typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

extern void (*glGetProgramiv)(GLuint, GLenum, GLint*);
extern void (*glActiveTexture) (GLenum);
extern void (*glAttachShader) (GLuint, GLuint);
extern void (*glBindBuffer) (GLenum, GLuint);
extern void (*glBindFramebuffer) (GLenum, GLuint);
extern void (*glBufferData) (GLenum, GLsizeiptr, const GLvoid*, GLenum);
extern void (*glBufferSubData) (GLenum, GLintptr, GLsizeiptr, const GLvoid*);
extern GLenum(*glCheckFramebufferStatus) (GLenum);
extern void (*glClearBufferfv) (GLenum, GLint, const GLfloat*);
extern void (*glCompileShader) (GLuint);
extern GLuint(*glCreateProgram) (void);
extern GLuint(*glCreateShader) (GLenum);
extern void (*glDeleteFramebuffers) (GLsizei, const GLuint*);
extern void (*glEnableVertexAttribArray) (GLuint);
extern void (*glDrawBuffers) (GLsizei, const GLenum*);
extern void (*glFramebufferTexture2D) (GLenum, GLenum, GLenum, GLuint, GLint);
extern void (*glGenBuffers) (GLsizei, GLuint*);
extern void (*glGenFramebuffers) (GLsizei, GLuint*);
extern GLint(*glGetAttribLocation) (GLuint, const GLchar*);
extern void (*glGetShaderInfoLog) (GLuint, GLsizei, GLsizei*, GLchar*);
extern void (*glGetShaderiv) (GLuint, GLenum, GLint*);
extern GLint(*glGetUniformLocation) (GLuint, const GLchar*);
extern void (*glLinkProgram) (GLuint);
extern void (*glShaderSource) (GLuint, GLsizei, const GLchar* const*, const GLint*);
extern void (*glUniform1i) (GLint, GLint);
extern void (*glUniform1f) (GLint, GLfloat);
extern void (*glUniform2f) (GLint, GLfloat, GLfloat);
extern void (*glUniform4f) (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
extern void (*glUniformMatrix4fv) (GLint, GLsizei, GLboolean, const GLfloat*);
extern void (*glUseProgram) (GLuint);
extern void (*glGenerateMipmap) (GLenum);
extern void (*glDeleteShader) (GLuint);
extern void (*glBindVertexArray) (GLuint);
extern void (*glGenVertexArrays) (GLsizei, GLuint*);
extern void (*glVertexAttribPointer) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
extern void (*glUniformMatrix3fv)(GLint, GLsizei, GLboolean, const GLfloat*);
extern void (*glDeleteBuffers) (GLsizei, const GLuint*);

void Update();
void Start();
extern Vector2 MousePosition;
extern bool KeyPressed(char key);