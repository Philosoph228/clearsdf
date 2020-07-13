#include <filesystem>
#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
  Shader(GLuint shaderId) : m_shaderId(shaderId) {};
  ~Shader() {
    glDeleteShader(m_shaderId);
  }
  
  GLuint GetId() {
    return m_shaderId;
  }

private:
  GLuint m_shaderId;
};

Shader LoadShader(std::string filename, GLenum type)
{
  std::ifstream shaderFile(filename);

  if (!shaderFile) {
    std::cout << "Can't open shader file: \"" << filename << '\"' << std::endl;
    throw "Pizda";
  }

  shaderFile.ignore(std::numeric_limits<std::streamsize>::max());
  auto size = shaderFile.gcount();

  if (size > 0x10000)
    throw "Pizda";

  shaderFile.clear();
  shaderFile.seekg(0, std::ios_base::beg);

  std::string shaderText{std::istreambuf_iterator<char> {shaderFile}, {}};


  GLuint shaderId = glCreateShader(type); 

  const char* shaderTextBuf = shaderText.c_str();
  glShaderSource(shaderId, 1, static_cast<const GLchar**> (&shaderTextBuf), nullptr);
  glCompileShader(shaderId);

  GLint success = 0;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLint maxLength = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

    std::cout << "Shader" << filename << "compilation error: " << std::string(errorLog.begin(), errorLog.end()) << std::endl;

    glDeleteShader(shaderId);
    throw "Mda";
  }

  return Shader(shaderId);
}

int main(int argc, char* argv[])
{
  GLFWwindow* window;

  if (!glfwInit())
    return -1;

  window = glfwCreateWindow(640, 480, "Hello World", nullptr,
    nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    throw "pizda";
  } else if (
      !GLEW_ARB_shading_language_100 ||
      !GLEW_ARB_vertex_shader ||
      !GLEW_ARB_fragment_shader ||
      !GLEW_ARB_shader_objects
    ) {
    std::cout << "Shaders not vailable" << std::endl; 
  }

  Shader surfaceShader = LoadShader("surface.vert", GL_VERTEX_SHADER);
  Shader roundShader = LoadShader("round_corners.frag", GL_FRAGMENT_SHADER);

  GLuint vertexArrayId;
  glGenVertexArrays(1, &vertexArrayId);
  glBindVertexArray(vertexArrayId);

  static const GLfloat vertexBufferData[] = {
    -1.0f, -1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,
    -1.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  0.0f,
  };

  GLuint vertexBuffer;
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

  GLuint programId = glCreateProgram();
  glAttachShader(programId, surfaceShader.GetId());
  glAttachShader(programId, roundShader.GetId());
  glLinkProgram(programId);

  GLint isLinked = 0;
  glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
  if (!isLinked) {
    GLint maxLength = 0;
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);
    
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);

    std::cout << "Program linking error: " << std::string(infoLog.begin(), infoLog.end()) << std::endl;
    
    throw "error";

    glDeleteProgram(programId);  
  }

  glUseProgram(programId);

  
  while (!glfwWindowShouldClose(window))
  {
    glUseProgram(programId);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
