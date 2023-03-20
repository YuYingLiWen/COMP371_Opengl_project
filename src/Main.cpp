
#include <cstdio>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "AppTime.h"

namespace AppTime { extern void UpdateTime(); }


void ScrollCallback(GLFWwindow*, double, double);
extern void KeyCallback(GLFWwindow*, int, int, int, int);
extern void UserInputs();
static void CustomImGui(ImGuiIO&, bool&, bool&, ImVec4&);

static const unsigned int HEIGHT = 720;
static const unsigned int WIDTH = 1280;
static const float FOV = 65.0f;

glm::mat4 view = glm::mat4(1.0f);


static void glfwErrorCallback(int error, const char* description)
{
    printf("GLFW Error %d: %s\n", error, description);
}

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

    // glfw Callbacks
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetErrorCallback(glfwErrorCallback);

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

    //gl enables
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // The default anyway

    // Debug
#ifdef _DEBUG
    EnableDebug();
#endif

    PRINT_LOG(glGetString(GL_VERSION)); // Prints Opengl version

    float positions[] = {

        // Front face
        -1.0f, -1.0f, 1.0f,   // 0 Bot Left
         1.0f, -1.0f, 1.0f,   // 1 Bot Right
         1.0f,  1.0f, 1.0f,   // 2 Top Right
        -1.0f,  1.0f, 1.0f,    // 3 Top Left

        // Back face
        -1.0f, -1.0f, -1.0f,   // 4 Bot Left
         1.0f, -1.0f, -1.0f,   // 5 Bot Right
         1.0f,  1.0f, -1.0f,   // 6 Top Right
        -1.0f,  1.0f, -1.0f    // 7 Top Left
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


    VertexArray vao2;
    VertexBuffer vbo2(positions, sizeof(positions), GL_STATIC_DRAW);
    ElementBuffer ebo2(indices, sizeof(indices));
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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");


    glm::mat4 projection = glm::perspective(glm::radians(FOV), ((float)WIDTH / (float)HEIGHT), 0.1f, 100.0f);

    // Force the camera to be a the following settings
    view = glm::rotate(view, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));


    glm::mat4 model = glm::mat4(1.0f);
    

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(5.0f, 0.0f, 0.0f));


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        AppTime::UpdateTime();

        renderer.Clear();

        UserInputs();

        /* Render here */

        renderer.Draw(vao, ebo, shader_program);

        

        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        shader_program.SetUniformValueMat4f("u_projection", projection);
        shader_program.SetUniformValueMat4f("u_model", model);
        shader_program.SetUniformValueMat4f("u_view", view);
        shader_program.SetUniformValue4f("u_color", r, 1.0f, 0.0f, 0.0f);


        renderer.Draw(vao2, ebo2, shader_program);
        model2 = glm::rotate(model2, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        shader_program.SetUniformValueMat4f("u_projection", projection);
        shader_program.SetUniformValueMat4f("u_view", view);

        shader_program.SetUniformValueMat4f("u_model", model2);

        shader_program.SetUniformValue4f("u_color", r, 0.0f, 1.0f, 0.0f);


        if (r > 1.0f || r < 0.0f) increment = -increment;

        r += increment;
        
        shader_program.Unbind();


        // Render ImGui on top of everything
        CustomImGui(io,show_demo_window,show_another_window,clear_color);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        glfwSetFramebufferSizeCallback(window, WindowSizeCallback);
    }

    //Cleanups
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


static void CustomImGui(ImGuiIO& io, bool& show_demo_window, bool& show_another_window, ImVec4& clear_color)
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
