
#include <cstdio>
#include <unordered_set>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "AppTime.h"

#include "Camera.h"
#include "SceneObject.h"
#include "ShaderProgram.h"

//#include "assimp/import"

namespace AppTime { extern void UpdateTime(); }

Camera camera(780, 1280, 65.0f);



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
    window = glfwCreateWindow(camera.width, camera.height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // glfw Callbacks
    glfwSetScrollCallback(window, camera.ScrollCallback);
    glfwSetKeyCallback(window, camera.KeyCallback);
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

    //float positions[] = {

    //    // Front face
    //    -0.5f, -0.5f, 0.5f,   // 0 Bot Left
    //     0.5f, -0.5f, 0.5f,   // 1 Bot Right
    //     0.5f,  0.5f, 0.5f,   // 2 Top Right
    //    -0.5f,  0.5f, 0.5f,    // 3 Top Left

    //    // Back face
    //    -0.5f, -0.5f, -0.5f,   // 4 Bot Left
    //     0.5f, -0.5f, -0.5f,   // 5 Bot Right
    //     0.5f,  0.5f, -0.5f,   // 6 Top Right
    //    -0.5f,  0.5f, -0.5f    // 7 Top Left
    //};

    //// Creating index buffer
    //unsigned int indices[]=
    //{
    //    // front face
    //    0, 1, 2,
    //    0, 2, 3,

    //    //Back face
    //    4,5,6,
    //    4,6,7,

    //    // Top face
    //    3,2,6,
    //    3,6,7,

    //    // Bottom face
    //    0,1,5,
    //    0,5,4,

    //    //Left face
    //    1,5,6,
    //    1,6,2,

    //    //Right face
    //    4,0,3,
    //    4,3,7
    //};

    //SceneObject cube1(positions, sizeof(positions), indices, sizeof(indices));
    //cube1.SetLayout(0, 3, GL_FLOAT, 3);
    //cube1.Unbind();

    //SceneObject cube2(positions, sizeof(positions), indices, sizeof(indices));
    //cube2.SetLayout(0, 3, GL_FLOAT, 3);
    //cube2.Unbind();


    const int square = 100;
    //// Perlin noise map
    unsigned int size_x = square;
    unsigned int size_z = square;

    unsigned int map_position_size = square * square * 3;
    float map_positions[square * square * 3];

    float half_x = size_x * 0.5f;
    float half_z = size_z * 0.5f;

    unsigned int data_block = 0;
    unsigned int block_size = 3;

    for (size_t z = 0; z < size_z; z++)
    {
        for (size_t x = 0; x < size_x; x++)
        {
            map_positions[data_block * block_size + 0] = x -half_x;
            map_positions[data_block * block_size + 1] = 0.0f;
            map_positions[data_block * block_size + 2] = z -half_z;

            data_block++;
        }
    }
    
    const unsigned int map_ebo_size = (square - 1) * (square - 1) * 6;
    unsigned int map_ebo[map_ebo_size]{};// 6 because, 3 to form a triangle, 6 to form a rectangle
    data_block = 0;
    block_size = 6;

    for (unsigned int z = 0; z < size_z - 1; z++)
    {
        for (unsigned int x = 0; x < size_x - 1; x++)
        {
            // Triangle 1
            map_ebo[data_block * block_size + 0] = x + z * size_z; // P0
            map_ebo[data_block * block_size + 1] = x + z * size_z + 1; // P1
            map_ebo[data_block * block_size + 2] = x + z * size_z + 1 + size_z; // P2

            // Triangle 2
            map_ebo[data_block * block_size + 3] = x + z * size_z; // P0
            map_ebo[data_block * block_size + 4] = x + z * size_z + 1 + size_z; // P1
            map_ebo[data_block * block_size + 5] = x + z * size_z + size_z; // P2

            data_block++;
        }
    }
    PRINT_LOG(map_position_size);
    PRINT_LOG(map_ebo_size);

    //unsigned int vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float), 0);
    //
    //unsigned int vbo;
    //glGenBuffers(1, &vbo);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(float), map_positions, GL_STATIC_DRAW);

    //unsigned int ebo;
    //glGenBuffers(1, &ebo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(unsigned int), map_ebo, GL_STATIC_DRAW);

    SceneObject map(map_positions, map_position_size, map_ebo, map_ebo_size);
    map.SetLayout(0, 3, GL_FLOAT, 0);



    //End perlin noise map
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

    Renderer renderer;

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(5.0f, 0.0f, 0.0f));

    glm::mat4 model3 = glm::mat4(1.0f);
    //model3 = glm::translate(model3, glm::vec3(-5.0f, 0.0f, 0.0f));

    glm::mat4 projection = camera.GetProjection();
    glm::mat4 view;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    // ImGui state
    bool show_demo_window = false;

    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        AppTime::UpdateTime();

        renderer.Clear();
        camera.UserInputs();

        view = camera.GetView();

        shader_program.Bind();

        /* Render here */
        //model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //shader_program.SetUniformValueMat4f("u_projection", projection);
        //shader_program.SetUniformValueMat4f("u_model", model);
        //shader_program.SetUniformValueMat4f("u_view", view);
        //shader_program.SetUniformValue4f("u_color", 1.0f, 0.0f, 0.0f, 1.0f);
        //renderer.Draw(cube1);
        //cube1.Unbind();

        //model2 = glm::translate(model2, glm::vec3(0.0f, glm::sin(glfwGetTime()) * AppTime::DeltaTime(), 0.0f));
        //model2 = glm::rotate(model2, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //shader_program.SetUniformValueMat4f("u_projection", projection);
        //shader_program.SetUniformValueMat4f("u_view", view);
        //shader_program.SetUniformValueMat4f("u_model", model2);
        //shader_program.SetUniformValue4f("u_color", 0.0f, 1.0f, 0.0f, 1.0f);
        //renderer.Draw(cube2);
        
        //unsigned int data[36];
        //glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 36 * sizeof(unsigned int), data);
        //for (int i = 0; i < 36; i++)
        //{
        //    PRINT_LOG(data[i]);
        //}
        //cube2.Unbind();

        shader_program.SetUniformValueMat4f("u_projection", projection);
        shader_program.SetUniformValueMat4f("u_view", view);
        shader_program.SetUniformValueMat4f("u_model", model3);
        shader_program.SetUniformValue4f("u_color", 0.0f, 0.0f, 1.0f, 1.0f);
        renderer.Draw(map);
        map.Unbind();

        
        // Render ImGui on top of everything
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("DEBUG");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::SliderFloat("Keyboard Move Speed", &camera.GetKeySpeed(), 0.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Reset Camera"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                Camera::ResetCamera(camera);
            ImGui::SameLine();
            ImGui::Text("same line example");

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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


static void CustomImGui(ImGuiIO& io, float& key_speed, ImVec4& clear_color)
{

}

