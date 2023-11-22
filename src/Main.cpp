#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Camera.hpp"

#include "Chunk.hpp"

#include <vector>

#include <stb_image.h>

#include <unordered_map>

static Camera camera(90, 1920.0f / 1080.0f);

void glfwFramebufferSizeCallback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
    camera.aspect = (float)w / h;
}

float last_x = 1920 / 2.0f;
float last_y = 1080 / 2.0f;

void glfwCursorCallback(GLFWwindow* window, double x, double y)
{
    float delta_x = x - last_x;
    float delta_y = y - last_y;
    last_x = x;
    last_y = y;

    camera.pitch -= delta_y * 0.25;
    camera.yaw -= delta_x * 0.25;
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam)
{
    std::cerr << message << "\n";
}

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW.\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Minecraft Clone", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window.\n";
        return 1;
    }

    InitBlockProperties();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSetCursorPosCallback(window, glfwCursorCallback);

    glfwMakeContextCurrent(window);

    if(!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to load OpenGL functions.\n";
        return 1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, NULL);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    Shader shader("res/shaders/chunk.vert.glsl", "res/shaders/chunk.frag.glsl");
    
    GLuint atlas;
    
    glGenTextures(1, &atlas);
    glBindTexture(GL_TEXTURE_2D, atlas); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;

    unsigned char *data = stbi_load("res/textures/atlas.png", &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    Chunk chunk(0, 0);

    for(int x = 0; x < CHUNK_SIZE; x++)
    {
        for(int y = 0; y < CHUNK_SIZE; y++)
        {
            for(int z = 0; z < CHUNK_SIZE; z++)
            {
                chunk.SetBlock(x, y, z, Block::DIRT);
            }
        }
    }

    float current_time = glfwGetTime();
    float last_time = current_time;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        current_time = glfwGetTime();
        float delta_time = current_time - last_time;
        last_time = current_time;

        if(glfwGetKey(window, GLFW_KEY_W))
        {
            camera.position += camera.GetForward() * delta_time * 10.0f;
        }
        if(glfwGetKey(window, GLFW_KEY_S))
        {
            camera.position -= camera.GetForward() * delta_time * 10.0f;
        }
        if(glfwGetKey(window, GLFW_KEY_A))
        {
            camera.position += camera.GetRight() * delta_time * 10.0f;
        }
        if(glfwGetKey(window, GLFW_KEY_D))
        {
            camera.position -= camera.GetRight() * delta_time * 10.0f;
        }

        camera.Update();
        chunk.Update();

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atlas);
        shader.SetUniform("u_sampler", 0);

        shader.SetUniform("u_view", camera.GetView());
        shader.SetUniform("u_proj", camera.GetProj());
 
        chunk.Draw();

        shader.Unbind();

        glfwSwapBuffers(window);
    }

    return 0;
}