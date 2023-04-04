
#include <memory>
#include <cstdio>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Renderer.h"
#include "AppTime.h"

#include "Camera.h"
#include "SceneObject.h"
#include "ShaderProgram.h"

#include "CustomRandom.h"

#include "assimp/Importer.hpp"
#include "assimp/Exporter.hpp"

#include "MeshGenerator.h"
#include "AppTime.h"

#include "TerrainGenerator.h"
#include "TerrainObject.h"

#include "ParticleSystem.h"

static bool toggle_grid = true;
static bool toggle_terrain_normal = true;


Camera camera(780, 1280, 65.0f);


glm::i32vec2 terrain_dimensions(200, 200);
int iter = 7;
float amplitude = 70.0f;
int split = 8;


extern std::vector<unsigned int> cube_indexes;
extern std::vector<glm::vec3> cube_pos;


extern bool WIREMESH_TOGGLE;

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
    window = glfwCreateWindow(camera.width, camera.height, "", NULL, NULL);
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

    //SceneObject cube1(&cube_pos, &cube_indexes);
    ////cube1.SetLayout(0, 3, GL_FLOAT, 3 * sizeof(float));
    //cube1.Transform().SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    //
    //SceneObject cube2(&cube_pos, &cube_indexes);
    ////cube2.SetLayout(0, 3, GL_FLOAT, 3 * sizeof(float));
    //cube2.Transform().SetPosition(glm::vec3(5.0f, 0.0f, 0.0f));




    TerrainGenerator tg;
    MeshGenerator mg;

    
    //// Perlin noise map
    auto terrain_data = tg.GeneratePerlinTerrain(terrain_dimensions.x, terrain_dimensions.y, iter, amplitude, split);//tg.Generate(terrain_dimensions, glm::vec2((float)terrain_dimensions.x * 0.5f, (float)terrain_dimensions.y * 0.5f), terrain_dimensions.x * 0.5f, 50.0f);
    auto base_mesh = mg.Generate(terrain_dimensions);
    TerrainObject* terrain2 = nullptr;
    SceneObject* terrain_normals = nullptr;
    if (terrain_data)
    {
        terrain2 = new TerrainObject(terrain_data, base_mesh->positions.get());

        terrain_normals = new SceneObject(terrain_data->positions.get(), terrain_data->indexes.get(), terrain_data->normals.get());
        terrain2->Transform().Translate(LEFT * (terrain_dimensions.x * 0.5f));
        terrain2->Transform().Translate(BACK * (terrain_dimensions.y * 0.5f));
    }


    //// Scene Grid
    auto grid_data = mg.GenerateGrid(300, 20);
    SceneObject* grid = nullptr;
    if (grid_data)
    {
        grid = new SceneObject(grid_data->positions.get(), grid_data->indexes.get());
        grid->Transform().Translate(LEFT * 150.0f * 10.0f);
        grid->Transform().Translate(BACK * 150.0f * 10.0f);
    }

    //End perlin noise map
    ShaderProgram simple_shader;
    simple_shader.Attach("res\\shaders\\vertex.shader", "res\\shaders\\fragment.shader");

    ShaderProgram terrain_shader;
    terrain_shader.Attach("res\\shaders\\terrain_vertex.shader", "res\\shaders\\terrain_fragment.shader");

    ShaderProgram grid_shader;
    grid_shader.Attach("res\\shaders\\grid_vs.shader", "res\\shaders\\grid_fs.shader");

    ParticleData data;
    ParticleSystem ps(3, data);
    //ps.Play();

    ParticleData cloud_data;
    cloud_data.scale = glm::vec3(30.0f, 8.0f, 30.0f);
    cloud_data.life_time = 30.0f;
    cloud_data.color_begin = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    cloud_data.color_end = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    cloud_data.direction = glm::vec3(1.0f, 0.0f, 0.0f);

    ParticleSystem cloud_ps(1, 1.0f, glm::vec3(-150.0f, 65.0f ,0.0f),glm::vec3(0.0f, 10.0f, 80.0f), cloud_data);
    cloud_ps.Play();


    ParticleData rain_data;
    rain_data.scale = glm::vec3(0.2f, 5.0f, 0.2f);
    rain_data.life_time = 1.0f;
    rain_data.color_begin = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    rain_data.color_end = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    rain_data.direction = glm::vec3(0.0f, -10.0f, 0.0f);

    ParticleSystem rain_ps(10, 0.2f, glm::vec3(0.0f, 65.0f, 0.0f), glm::vec3(50.0f, 0.0f, 50.0f), rain_data);
    rain_ps.Play();

    Renderer renderer;

    glm::mat4 model3 = glm::mat4(1.0f);


    glm::mat4 projection = camera.GetProjection();
    glm::mat4 view;
    float rotation = 0.0f;

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

    glm::vec3 directional_light(0.5f, -1.0f, -3.0f);
    
    enum TerrainLerpState {TOWARDS, BACKWARDS};

    float terrain_t = 0.0f;
    float terrain_lerp_time_elapsed = 0.0f;
    float terrain_lerp_duration = 1.3f;
    TerrainLerpState lerp_state = TOWARDS;
    bool terrain_is_lerping = true;

    bool is_demonstrating = true;
    float cam_rot_speed = 15.0f;
    

    // Weather control
    bool has_clouds = false;
    bool has_rain = false;

    while (!glfwWindowShouldClose(window))
    {
        AppTime::UpdateTime();

        renderer.Clear();

        if (is_demonstrating)
        {
            view = camera.GetView(glm::vec3(0.0f, 0.0f, 0.0f));
            rotation = camera.GetTransform().Rotation().x;
            camera.GetTransform().Rotate(cam_rot_speed * AppTime::DeltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else
        {
            camera.UserInputs(window);

            glm::vec3 rot = glm::vec3(rotation, camera.GetTransform().Rotation().y, camera.GetTransform().Rotation().z);
            camera.GetTransform().SetRotation(rot);
            view = camera.GetView();
        }




        glm::vec2 val = CustomRandom::GetInstance().RandomCircle();
        /* Render here */

        if (toggle_grid)
        {
            grid_shader.Bind();
            grid_shader.SetUniformMat4f("u_projection", projection);
            grid_shader.SetUniformMat4f("u_view", view);
            if (grid != nullptr) grid_shader.SetUniformMat4f("u_model", grid->GetModel());
            if (grid != nullptr) renderer.Draw(GL_LINES, *grid);
        }

        //simple_shader.Bind();
        //cube1.Transform().Rotate(glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //simple_shader.SetUniformMat4f("u_projection", projection);
        //simple_shader.SetUniformMat4f("u_model", cube1.GetModel());
        //simple_shader.SetUniformMat4f("u_view", view);
        //simple_shader.SetUniform4f("u_color", 1.0f, 0.0f, 0.0f, 1.0f);
        //renderer.Draw(cube1);

        //cube2.Transform().Rotate(glm::radians(15.0f) , glm::vec3(0.0f, 1.0f, 0.0f));
        //cube2.Transform().Translate(glm::vec3(0.0f, glm::sin(glfwGetTime())* AppTime::DeltaTime(), 0.0f));
        //
        //simple_shader.SetUniformMat4f("u_projection", projection);
        //simple_shader.SetUniformMat4f("u_view", view);
        //simple_shader.SetUniformMat4f("u_model", cube2.GetModel());
        //simple_shader.SetUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
        //renderer.Draw(cube2);

        if (terrain2)
        {
            terrain_shader.Bind();
            terrain_shader.SetUniformMat4f("u_projection", projection);
            terrain_shader.SetUniformMat4f("u_view", view);
            terrain_shader.SetUniformMat4f("u_model", terrain2->GetModel());
            terrain_shader.SetUniformInt("u_use_wiremesh", WIREMESH_TOGGLE);
            //terrain_shader.SetUniformFloat("u_time", AppTime::Time());

            terrain_shader.SetUniform3f("u_light", directional_light);
            terrain_shader.SetUniform4f("u_color", 1.0f, 1.0f, 1.0f, 1.0f);

            if (terrain_is_lerping)
            {
                terrain_lerp_time_elapsed += AppTime::DeltaTime();

                if (lerp_state == TOWARDS)
                {
                    terrain_t = glm::clamp(terrain_lerp_time_elapsed / terrain_lerp_duration, 0.0f, 1.0f) ;
                }
                else // Backwards 
                {
                    terrain_t = glm::clamp(1.0f - (terrain_lerp_time_elapsed / terrain_lerp_duration), 0.0f, 1.0f) ;
                }

                terrain2->Lerp(terrain_t);

                if (terrain_t >= 1.0f) // Reached destination
                {
                    terrain_is_lerping = false;
                    terrain_lerp_time_elapsed = 0.0f;
                }
                else if (terrain_t <= 0.0f) // Back to origin
                {
                    delete terrain2;
                    terrain_data = tg.GeneratePerlinTerrain(terrain_dimensions.x, terrain_dimensions.y, iter, amplitude, split);
                    base_mesh = mg.Generate(terrain_dimensions);
                    terrain2 = new TerrainObject(terrain_data, base_mesh->positions.get());

                    terrain2->Transform().Translate(LEFT* (terrain_dimensions.x * 0.5f));
                    terrain2->Transform().Translate(BACK* (terrain_dimensions.y * 0.5f));

                    terrain_lerp_time_elapsed = 0.0f;
                    lerp_state = TOWARDS;
                }
            }

            renderer.Draw(*terrain2);
        }

        if (has_clouds) cloud_ps.Update();
        if (has_rain) rain_ps.Update();

        // Render ImGui on top of everything
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        {
            ImGui::Begin("DEBUG");                          

            ImGui::Checkbox("Demo Window", &show_demo_window);     

            if (ImGui::CollapsingHeader("Keyboard & Camera Controls"))
            {
                /// CAMERA
                ImGui::SeparatorText("Camera");

                if (ImGui::Button("Demo Style"))
                {
                    is_demonstrating = !is_demonstrating;

                    if(is_demonstrating) Camera::Reset(camera);
                }
                ImGui::SameLine();
                ImGui::Text("State: %s", (is_demonstrating? "True" : "False"));

                ImGui::SliderFloat("Rotation Speed", &cam_rot_speed, -90.0f, 90.0f);
                ImGui::SliderFloat("Pitch", &rotation, -180.0f, 180.0f);
                if (ImGui::Button("Reset"))
                    Camera::Reset(camera);
                ImGui::SameLine(); ImGui::Text("Resets To Default Values");

                /// KEYBOARD
                ImGui::SeparatorText("Keyboard");
                ImGui::SliderFloat("Keyboard Move Speed", &camera.GetKeySpeed(), 0.0f, 100.0f);
                ImGui::NewLine();
            }
       
            if (ImGui::CollapsingHeader("Terrain & Weather Controls"))
            {
                ImGui::SeparatorText("Terrain");

                ImGui::SliderInt("Gradient Grid Division", &split, 1, 10);
                ImGui::SliderFloat("Amplitude", &amplitude, 0.0f, 100.0f);
                ImGui::SliderInt("Iteration", &iter, 1, 100);

                ImGui::InputInt2("Terrain Dimension", glm::value_ptr(terrain_dimensions));

                if (ImGui::Button("Generate Terrain"))
                {
                    terrain_is_lerping = true;
                    lerp_state = BACKWARDS;
                }

                ImGui::SeparatorText("Weather");
                ImGui::Checkbox("Rain", &has_rain);
                ImGui::SameLine();
                if (ImGui::Button("Pause Rain")) rain_ps.Pause();
                ImGui::SameLine();
                if (ImGui::Button("Play Rain")) rain_ps.Play();
                ImGui::NewLine();
                
                ImGui::Checkbox("Clouds", &has_clouds);
                ImGui::SameLine();
                if (ImGui::Button("Pause Clouds")) cloud_ps.Pause();
                ImGui::SameLine();
                if (ImGui::Button("Play Clouds")) cloud_ps.Play();
                ImGui::NewLine();
            }

            if (ImGui::CollapsingHeader("Others"))
            {
                ImGui::ColorEdit3("clear color", (float*)&clear_color);

                ImGui::DragFloat3("Directional Light", glm::value_ptr(directional_light), 0.01f, -100.0f, 100.0f);

                if (ImGui::Button("Toggle Grid"))
                    toggle_grid = !toggle_grid;
                ImGui::SameLine(); ImGui::Text("Toggles World Grid ON/OFF");
                ImGui::NewLine();
            }
            
            ImGui::NewLine();
            ImGui::SeparatorText("App Data");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Text("Delta Time: %.5f sec", AppTime::DeltaTime());
            
            ImGui::NewLine();
            ImGui::SeparatorText("Particle System");
            ImGui::Text("Particles: %d", ps.ParticlesCount());
            ImGui::Text("Active: %d", ps.ActiveParticlesCount());
            ImGui::Text("Inactive: %d", ps.InactiveParticlesCount());


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

