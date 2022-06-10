#include <iostream>
#include <filesystem>
#include <set>

#include <GLFW/glfw3.h>
#include "../vendor/imgui_impl_glfw.h"
#include "../vendor/imgui_impl_opengl3.h"
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <../lib/mapbox/earcut.hpp>

#include "utils.h"
#include "gerberProcessing/GerberFile.h"
#include "gerberProcessing/Parser.h"

#define GLSL_VERSION "#version 330"

// Layer names as string
const std::map<int, std::string> layerNames = {
        {0, "Top copper"},
        {1, "Top soldermask"},
        {2, "Top silkscreen"},
        {3, "Outline"},
        {4, "Bottom copper"},
        {5, "Bottom soldermask"},
        {6, "Bottom silkscreen"}};

// Colors for layers by default
std::vector<std::pair<GerberFile, int>> loadedFiles = {};
std::map<int, ImVec4> layerColors = {
        {0, ImVec4(204.0f / 255.0f, 81.0f / 255.0f, 66.0f / 255.0f, 255.0f / 255.0f)},
        {1, ImVec4(91.0f / 255.0f, 170.0f / 255.0f, 83.0f / 255.0f, 255.0f / 255.0f)},
        {2, ImVec4(141.0f / 255.0f, 112.0f / 255.0f, 201.0f / 255.0f, 255.0f / 255.0f)},
        {3, ImVec4(154.0f / 255.0f, 153.0f / 255.0f, 62.0f / 255.0f, 255.0f / 255.0f)},
        {4, ImVec4(200.0f / 255.0f, 88.0f / 255.0f, 140.0f / 255.0f, 255.0f / 255.0f)},
        {5, ImVec4(73.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f, 255.0f / 255.0f)},
        {6, ImVec4(201.0f / 255.0f, 133.0f / 255.0f, 66.0f / 255.0f, 255.0f / 255.0f)}};

std::map<int, bool> layerVisibility = {{0, true},{1, true},{2, true},{3, true},{4, true},{5, true},{6, true}};
std::vector<int> layerOrder = {0,1,2,3,4,5,6};

void drop_callback(GLFWwindow* window, int count, const char** paths)
{
    for (int i = 0; i < count; ++i) {
        if(std::filesystem::is_directory(paths[i]))
        {
            for (const auto & file : std::filesystem::directory_iterator(paths[i]))
            {
                if(!std::filesystem::is_directory(file.path()))
                {
                    loadedFiles.emplace_back(Parser::fromFile(file.path()), 0);
                }
            }
        }
        else
        {
            loadedFiles.emplace_back(Parser::fromFile(paths[i]), 0);
        }
    }
}

void function_to_restore_shader_or_blend_state(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void function_to_restore_shader_or_blend_state2(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}
void function_to_select_shader_or_blend_state(const ImDrawList* parent_list, const ImDrawCmd* cmd)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow *window = glfwCreateWindow(1200, 800, "GerberViewer",
                                          nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Could not create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetDropCallback(window, drop_callback);             // sets up the drop function which is used for files

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Could not initialize GLAD" << std::endl;
        return -1;
    }

    ImGui::CreateContext();

    int detailFileIndex = 0;
    bool showLayerDetail = false;

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = "imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    ImVec2 scrolling(500.0f, 500.0f);
    bool opt_enable_context_menu = true;
    bool adding_line = false;
    double factor = 10;


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        static ImGuiID dockspaceID = 0;


        bool active = true;
        int width, height;
        static bool opt_enable_grid = true;
        glfwGetWindowSize(window, &width, &height);

        ImGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width+2, height+2), ImGuiCond_Always);
        if (ImGui::Begin("Master Window", &active, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("View"))
                {
                    if (ImGui::MenuItem(opt_enable_grid ? "Disable grid" : "Enable grid")) {opt_enable_grid = !opt_enable_grid;}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Layout"))
                {
                    if (ImGui::MenuItem("Reset to default")) {

                        int dockspace_id = 6;

                        ImGui::DockBuilderRemoveNode(dockspaceID);
                        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
                        ImGui::DockBuilderSetNodeSize(dockspaceID, io.DisplaySize);

                        ImGuiID dock_main_id = dockspaceID;
                        const ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, .11f, nullptr, &dock_main_id);
                        ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, .30f, nullptr, &dock_main_id);
                        const ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, .22f, nullptr, &dock_main_id);
                        const ImGuiID dock_id_bottomleft = ImGui::DockBuilderSplitNode(dock_id_bottom, ImGuiDir_Left, .5f, nullptr, &dock_main_id);
                        ImGui::DockBuilderDockWindow("Layers", dock_id_right);
                        ImGui::DockBuilderDockWindow("Main View", dock_id_bottom);
                        ImGui::DockBuilderDockWindow("Window3" , dock_main_id);
                        ImGui::DockBuilderDockWindow("Metrics", dock_main_id);
                        ImGui::DockBuilderDockWindow("Loaded files", dock_id_bottomleft);
                        ImGui::DockBuilderFinish(dockspaceID);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Layers"))
                {
                    if (ImGui::MenuItem("Reset default order")) {
                        std::sort (layerOrder.begin(), layerOrder.end());
                    }
                    ImGui::EndMenu();
                }

                // display cursor position in the right corner if the window is focused
                if(glfwGetWindowAttrib(window, GLFW_FOCUSED))
                {
                    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
                    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
                    canvas_p0 = ImVec2(canvas_p0.x, canvas_p0.y);
                    auto string = "(" + std::to_string((io.MousePos.x - scrolling.x-7)/factor) + ";" + std::to_string((-io.MousePos.y + scrolling.y + 50)/factor) + ")";
                    ImGui::SameLine(ImGui::GetWindowWidth()-ImGui::CalcTextSize(string.c_str()).x-10);
                    ImGui::Text("%s", string.c_str());
                }

                ImGui::EndMainMenuBar();
            }
        }
        if (active)
        {
            dockspaceID = ImGui::GetID("Master Window");
            ImGui::DockSpace(dockspaceID , ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None|ImGuiDockNodeFlags_PassthruCentralNode/*|ImGuiDockNodeFlags_NoResize*/);
        }
        ImGui::End();

        static int drag_i = 50;
        ImGui::Begin("Layers");
        {

            static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

            // check if layerTypeOrder matches up with present layers
            std::set<int> presentLayers = {};
            for (auto i = 0; i < loadedFiles.size(); i++) {
                presentLayers.insert(loadedFiles[i].second);
            }

            for (auto i = 0; i < layerOrder.size(); i++)
            {
                ImGui::PushID(("##" + std::to_string(i)).c_str());
                if(!layerVisibility.find(layerOrder[i])->second)
                {
                    if (ImGui::SmallButton("S")) {
                        std::cout << "Showing";
                        layerVisibility[layerOrder[i]] = true;
                    }
                }
                else
                {
                    if (ImGui::SmallButton("H")) {
                        std::cout << "Hiding";
                        layerVisibility[layerOrder[i]] = false;
                    }
                }
                ImGui::PopID();
                ImGui::SameLine();
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::ColorEdit3(std::to_string(layerOrder[i]).c_str(), (float*)&(layerColors.find(layerOrder[i])->second), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::PopStyleVar();
                ImGui::SameLine();
                const int item = layerOrder[i];
                if(!presentLayers.contains(layerOrder[i]))
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                    ImGui::Selectable(layerNames.find(item)->second.c_str());
                    ImGui::PopStyleColor();
                }
                else
                {
                    ImGui::Selectable(layerNames.find(item)->second.c_str());
                }

                if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
                {
                    int n_next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                    if (n_next >= 0 && n_next < layerOrder.size())
                    {
                        layerOrder[i] = layerOrder[n_next];
                        layerOrder[n_next] = item;
                        ImGui::ResetMouseDragDelta();
                    }
                }
            }

        }
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Main View", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
        {

            // get the canvas dimensions
            ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
            ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
            canvas_p0 = ImVec2(canvas_p0.x, canvas_p0.y+7);
            if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
            if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
            ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);


            // This will catch our interactions
            ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
            const bool is_hovered = ImGui::IsItemHovered(); // Hovered
            const bool is_active = ImGui::IsItemActive();   // Held
            const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
            const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

            // Handle zooming
            //TODO: zoom with mouse position in mind
            auto newfactor = factor + io.MouseWheel/10;
            factor = newfactor;

            //Initialise drawlist
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            //Draw background
            draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));

            // Panning the canvas using mouse
            const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
            if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
            {
                scrolling.x += io.MouseDelta.x;
                scrolling.y += io.MouseDelta.y;
            }

            // Draw grid + all lines in the canvas
            draw_list->PushClipRect(canvas_p0, canvas_p1, true);
            if (opt_enable_grid && factor > 0.3)
            {
                const float GRID_STEP = 5.0f;
                for (float x = fmodf(scrolling.x, GRID_STEP*factor); x < canvas_sz.x; x += GRID_STEP*factor)
                    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
                for (float y = fmodf(scrolling.y, GRID_STEP*factor); y < canvas_sz.y; y += GRID_STEP*factor)
                    draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
            }

            // Draw the objects of the layer files
            for (int layerIndex = layerOrder.size()-1; layerIndex >= 0; layerIndex--)
            {
                draw_list->AddCallback(function_to_restore_shader_or_blend_state2, NULL);
                if(layerVisibility[layerOrder[layerIndex]])
                {
                    auto layerColor = IM_COL32(int(layerColors[layerOrder[layerIndex]].x*255), int(layerColors[layerOrder[layerIndex]].y*255),int(layerColors[layerOrder[layerIndex]].z*255),int(layerColors[layerOrder[layerIndex]].w*255));
                    for (auto & loadedFile : loadedFiles)
                    {
                        if(loadedFile.second == layerOrder[layerIndex])
                        {
                            auto file = loadedFile.first;
                            for (int n = file.objects.size()-1; n >= 0 ; n--)
                            {
                                //identify type and draw it
                                if (std::holds_alternative<Line>(file.objects[n]))              //Draw Line
                                {
                                    Line& myline = std::get<Line>(file.objects[n]);
                                    if(myline.polarity == Clear)
                                    {
                                        draw_list->AddCallback(function_to_select_shader_or_blend_state,nullptr);
                                    }
                                    else
                                    {
                                        draw_list->AddCallback(function_to_restore_shader_or_blend_state2, nullptr);
                                    }

                                    auto diameter = file.aperatures[myline.aperture]->getDiameter() * factor;
                                    //std::cout << "Displaying at pos: " << std::to_string(origin.x + myline.startX) << " " << std::to_string(origin.x + myline.endX) << " " << std::to_string(origin.x + myline.startY) << " " << std::to_string(origin.x + myline.endY)<< " mouse in pos " << mouse_pos_in_canvas.x << " " << mouse_pos_in_canvas.y << std::endl;

                                    draw_list->AddLine(ImVec2(origin.x + myline.start.x*factor , origin.y - myline.start.y*factor), ImVec2(origin.x + myline.end.x*factor, origin.y - myline.end.y*factor), layerColor, diameter);
                                }
                                else if(std::holds_alternative<Arc>(file.objects[n]))           //Draw Arc
                                {
                                    Arc& myarc = std::get<Arc>(file.objects[n]);

                                    if(myarc.polarity == Clear)
                                    {
                                        draw_list->AddCallback(function_to_select_shader_or_blend_state,nullptr);
                                    }
                                    else
                                    {
                                        draw_list->AddCallback(function_to_restore_shader_or_blend_state2, nullptr);
                                    }

                                    myarc.GeneratePath();
                                    auto diameter = file.aperatures[myarc.aperture]->getDiameter() * factor;
                                    for (int i = 1; i < myarc.pathPoints.size(); i++) {
                                        draw_list->AddLine(ImVec2(origin.x +myarc.pathPoints[i-1].x*factor , origin.y - myarc.pathPoints[i-1].y*factor), ImVec2(origin.x +myarc.pathPoints[i].x*factor, origin.y - myarc.pathPoints[i].y*factor), layerColor, diameter);
                                    }
                                }
                                else if(std::holds_alternative<Contour>(file.objects[n]))       //Draw contour
                                {
                                    auto& myContour = std::get<Contour>(file.objects[n]);

                                    if(myContour.polarity == Clear)
                                    {
                                        draw_list->AddCallback(function_to_select_shader_or_blend_state,nullptr);
                                    }
                                    else
                                    {
                                        draw_list->AddCallback(function_to_restore_shader_or_blend_state2, nullptr);
                                    }

                                    for (int i = 0; i < myContour.indices.size(); i+=3) {
                                        draw_list->AddTriangleFilled(ImVec2(origin.x + myContour.polygon[myContour.indices[i]].x*factor,origin.y - myContour.polygon[myContour.indices[i]].y*factor),
                                                                     ImVec2(origin.x + myContour.polygon[myContour.indices[i+1]].x*factor,origin.y - myContour.polygon[myContour.indices[i+1]].y*factor),
                                                                     ImVec2(origin.x + myContour.polygon[myContour.indices[i+2]].x*factor,origin.y - myContour.polygon[myContour.indices[i+2]].y*factor),
                                                                     layerColor);
                                    }

                                }
                                else if(std::holds_alternative<Flash>(file.objects[n]))             //Draw Flash
                                {
                                    auto& myFlash = std::get<Flash>(file.objects[n]);

                                    if(myFlash.polarity == Clear)
                                    {
                                        draw_list->AddCallback(function_to_select_shader_or_blend_state,nullptr);
                                    }
                                    else
                                    {
                                        draw_list->AddCallback(function_to_restore_shader_or_blend_state2, nullptr);
                                    }

                                    if(file.aperatures.find(myFlash.aperture) != file.aperatures.end())
                                    {
                                        auto points = file.aperatures[myFlash.aperture]->pathPoints;
                                        draw_list->PathClear();
                                        for (auto & point : points) {
                                            draw_list->PathLineTo(ImVec2(origin.x +point.x*factor + myFlash.position.x*factor,origin.y - point.y*factor - myFlash.position.y*factor));
                                        }
                                        draw_list->PathFillConvex(layerColor);
                                    }
                                    else
                                    {
                                        //std::cout << "Unknown aparature " << myFlash.aperture << std::endl;
                                    }
                                }
                            }
                        }
                    }
                }
                draw_list->AddCallback(function_to_restore_shader_or_blend_state, nullptr);
            }

            draw_list->PopClipRect();
        }
        ImGui::PopStyleVar(2);
        ImGui::End();

        ImGui::Begin("Window3");
        {
            ImGui::Text("Hello, Dear ImGUI!");
        }
        ImGui::End();


        ImGui::Begin("Metrics");
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
            ImGui::Text("%d active windows (%d visible)", io.MetricsActiveWindows, io.MetricsRenderWindows);
            ImGui::Text("%d active allocations", io.MetricsActiveAllocations);
        }
        ImGui::End();

        //window which shows loaded files
        ImGui::Begin("Loaded files");
        {
            if(loadedFiles.empty())
            {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),"Drag & drop files or directories to load");
            }
            else
            {
                for (int i = 0; i < loadedFiles.size(); ++i) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.9f, 0.0f, 0.0f, 1.0f});
                    if (ImGui::SmallButton(("X##" + std::to_string(i)).c_str())) {
                        loadedFiles.erase(loadedFiles.begin() + i);
                        i--;
                    }
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::Text("%s", loadedFiles[i].first.filename.c_str());
                    ImGui::SameLine(ImGui::GetWindowWidth()-190);
                    const char* items[] = { "Top copper", "Top soldermask", "Top silkscreen", "Outline", "Bottom copper", "Bottom soldermask", "Bottom silkscreen"};
                    ImGui::SetNextItemWidth(100);
                    ImGui::Combo(("##" + std::to_string(i)).c_str(),  &loadedFiles[i].second, items, IM_ARRAYSIZE(items));
                    ImGui::SameLine();
                    if (ImGui::SmallButton(("Details##" + std::to_string(i)).c_str())) {
                        if(!showLayerDetail || detailFileIndex == i)
                        {
                            showLayerDetail = !showLayerDetail;
                        }
                        detailFileIndex = i;
                    }
                }
            }
        }
        ImGui::End();


        if(showLayerDetail)
        {
            ImGui::Begin("File details");
            {
                ImGui::Text("%s", ("Filename:" + loadedFiles[detailFileIndex].first.filename).c_str());
                std::string units = loadedFiles[detailFileIndex].first.metricUnit ? "Units: Metric" : "Units: Imperial";
                ImGui::Text("%s", units.c_str());
                if (ImGui::TreeNode("Comments"))
                {
                    for (auto & comment : loadedFiles[detailFileIndex].first.comments) {
                        ImGui::Text("%s", comment.c_str());
                    }
                    ImGui::TreePop();
                    ImGui::Separator();
                }
                if (ImGui::TreeNode("Aparatures"))
                {
                    ImGui::Text("Total count: %lu", loadedFiles[detailFileIndex].first.aperatures.size());
                    for (const auto& e: loadedFiles[detailFileIndex].first.aperatures) {
                        ImGui::Text("%i %s",e.first, std::string(*e.second).c_str());
                    }
                    ImGui::TreePop();
                    ImGui::Separator();
                }
                if (ImGui::TreeNode("Objects"))
                {
                    ImGui::Text("Total count: %lu", loadedFiles[detailFileIndex].first.objects.size());
                    ImGui::TreePop();
                    ImGui::Separator();
                }
            }
            ImGui::End();
        }
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
