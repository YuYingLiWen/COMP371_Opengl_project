

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

extern void KeyCallback(GLFWwindow*, int, int, int, int);

static unsigned int HEIGHT = 600;
static unsigned int WIDTH = 800;


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
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
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

    float positions[] = {

        // Front face
        -1.0f, -1.0f, 0.0f,   // 0 Bot Left
         1.0f, -1.0f, 0.0f,   // 1 Bot Right
         1.0f,  1.0f, 0.0f,   // 2 Top Right
        -1.0f,  1.0f, 0.0f,    // 3 Top Left

        // Back face
        -1.0f, -1.0f, -2.0f,   // 4 Bot Left
         1.0f, -1.0f, -2.0f,   // 5 Bot Right
         1.0f,  1.0f, -2.0f,   // 6 Top Right
        -1.0f,  1.0f, -2.0f    // 7 Top Left
    };

    // Creating index buffer
    unsigned int indices[]=
    {
        // front face
        0, 1, 2,
        2, 0, 3,

        //Back face
        4,5,6,
        6,4,7,

        // Top face
        3,2,6,
        6,3,7,

        // Bottom face
        0,1,5,
        5,0,4,

        //Left face
        1,2,6,
        6,5,1,

        //Right face
        0,3,7,
        7,4,0


    };

    VertexArray vao;
    VertexBuffer vbo(positions, sizeof(positions), GL_STATIC_DRAW);
    ElementBuffer ebo(indices, sizeof(indices));
    vao.SetLayout(0, 3, GL_FLOAT, 3);

    ShaderProgram shader_program;
    try 
    {
        shader_program.Attach(GL_FRAGMENT_SHADER, "res\\shaders\\fragment.shader");
        shader_program.Attach(GL_VERTEX_SHADER, "res\\shaders\\vertex.shader");
    }
    catch (std::exception e)
    {
        PRINT_LOG(e.what());
        PRINT_LOG("Press Any Key to exit.");
        std::cin.get();
        return -1;
    }
    
    shader_program.LinkAndValidate();

    vao.Unbind();
    shader_program.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    float r = 0.0f;
    float increment = 0.1f;

    Renderer renderer;

    float fov = 45.0f;

    glm::mat4 projection = glm::perspective(glm::radians(fov), ((float)WIDTH / (float)HEIGHT), 0.1f, 100.0f);
    //projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, 1.0f, 100.0f); //glm::perspective(glm::radians(fov), (float)(WIDTH / HEIGHT), 0.1f, 100.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

    glm::mat4 model = glm::mat4(1.0f);
    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Resize window
        glfwSetFramebufferSizeCallback(window, WindowSizeCallback);

        /* Render here */
        renderer.Clear();
        
        renderer.Draw(vao, ebo, shader_program);

        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 1.0f));

        shader_program.SetUniformValueMat4f("u_projection", projection);
        shader_program.SetUniformValueMat4f("u_model", model);
        shader_program.SetUniformValueMat4f("u_view", view);


        shader_program.SetUniformValue4f("u_color", r, 0.1f, 0.5f, 1.0f);

        if (r > 1.0f || r < 0.0f) increment = -increment;

        r += increment;
        
        shader_program.Unbind();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
