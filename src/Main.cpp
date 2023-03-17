


#include "Renderer.h"

extern void KeyCallback(GLFWwindow*, int, int, int, int);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    // Set Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) 
    { 
        PRINT_LOG("GLEW INIT ERROR."); 
        return -1;
    }
    else
        PRINT_LOG("GLEW INIT SUCCESSFUL.");

    // User Inputs
    glfwSetKeyCallback(window, KeyCallback);

    // Debug
#ifdef _DEBUG
    EnableDebug();
#endif

    PRINT_LOG(glGetString(GL_VERSION)); // Prints Opengl version

    float positions[8] = {
    -0.5f, -0.5f, // 0 Bot Left
     0.5f, -0.5f, // 1 Bot Right
     0.5f,  0.5f, // 2 Top Right
    -0.5f,  0.5f, // 3 Top Left
    };

    // Creating index buffer
    unsigned int indices[6]=
    {
        0, 1, 2,
        2, 0, 3
    };

    VertexArray vao;
    VertexBuffer vbo(positions, sizeof(positions), GL_STATIC_DRAW);
    ElementBuffer ebo(indices, sizeof(indices));
    vao.SetLayout(0, 2, GL_FLOAT, 2);

    ShaderProgram shader_program;
    try {
        shader_program.Attach(GL_VERTEX_SHADER, "res\\shaders\\vertex.shader");
        shader_program.Attach(GL_FRAGMENT_SHADER, "res\\shaders\\fragment.shader");
    }
    catch (std::exception e)
    {
        PRINT_LOG(e.what());
        PRINT_LOG("Press Any Key to exit.");
        std::cin.get();
        return -1;
    }
    
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float r = 0.0f;
    float increment = 0.1f;

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Resize window
        glfwSetFramebufferSizeCallback(window, WindowSizeCallback);


        /* Render here */
        renderer.Clear();
        
        renderer.SetDrawMode(DrawMode::WIREFRAME);
        renderer.Draw(vao, ebo, shader_program);


        shader_program.SetUniformValue4f("u_color", r, 0.1f, 0.5f, 1.0f);

        if (r > 1.0f || r < 0.0f) increment = -increment;

        r += increment;
        
        //shader_program.Unbind();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
