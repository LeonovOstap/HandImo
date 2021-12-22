// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#include <GL/glew.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"

#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <iostream>
#include <string>
#include "GL/GLU.h"
#include "mat4x4.hpp"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

#include "Bones.h"
#include "camera.h"
#include "Leap/LeapInterleave.h"
#include "Line.h"
#include "Octahedron.h"
#include "Leap/Leap2Handimo.h"


Camera camera;

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
    if(width > 0 && height > 0)
        camera.SetViewport(0, 0, width, height);
}

void window_mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        double x;
        double y;
        glfwGetCursorPos(window,&x,&y);
        camera.mouse_position = glm::vec3(x,y,0);
        camera.Move2D(x,y);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if( yoffset > 0)
        camera.Move(FORWARD);
    else if (yoffset < 0)
        camera.Move(BACK);
        
}

void window_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case 87:
        camera.Move(FORWARD);
        break;
    case 83:
        camera.Move(BACK);
        break;
    case 65:
        camera.Move(LEFT);
        break;
    case 68:
        camera.Move(RIGHT);
        break;
    case 69:
        camera.Move(UP);
        break;
    case 81:
        camera.Move(DOWN);
        break;
    default: ;
    }
}


int main(int a, char** b)
{
    // Setup window
    if (!glfwInit())
        return 1;
    
#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Create window with graphics contex
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Test Application 1", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, window_key_callback);
    glfwSetMouseButtonCallback(window, window_mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);
    

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    

    LeapInterleave interleave = LeapInterleave();
    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);

    camera.SetMode(FREE);
    camera.SetPosition(glm::vec3(0, 6, -1));
    camera.SetLookAt(glm::vec3(0, 2, 0));
    camera.SetClipping(.1, 1200);
    camera.SetFOV(45);
    camera.SetViewport(0,0,1280,720);
    

    Vector3f HandBonesColor(1.f,0.f,0.f);
    Vector3f HandPointColor(1,1,1);
    char RecordingDirectory[256] = "N:/GemDev/KinectRecordning/fuck.bvh";
    char RecordingPrefix[256] = "HandImo";

    GLuint Preview_Image;
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.15, 0.15, 0.15, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glTranslatef(0,0,0);
        float r[3] = {1,0,0};
        float g[3] = {0,1,0};
        float b[3] = {0,0,1};

        DrawLine(Vector3f(0,0,0), Vector3f(1,0,0), r);
        DrawLine(Vector3f(0,0,0), Vector3f(0,1,0), g);
        DrawLine(Vector3f(0,0,0), Vector3f(0,0,1), b);
        
        glm::mat4 model, view, projection;
        camera.Update();
        camera.GetMatricies(projection,model,view);

        glm::mat4 mvp = projection* view * model;	//Compute the mvp matrix
        
        glLoadMatrixf(glm::value_ptr(mvp));

        for(int x = -100; x < 100; ++x)
        {
            for(int y = -100; y < 100; ++y)
            {
                Vector3f Start = Vector3f(y, 0, x);
                Vector3f Color(0.5,0.5,0.5);
                DrawLine(Start, Vector3f(y+1,0,x), Color.mdata);
                DrawLine(Start, Vector3f(y,0,x+1), Color.mdata);
            }
        }

        int RMBState  = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

        if(RMBState == GLFW_PRESS)
        {
            double x;
            double y;
            glfwGetCursorPos(window,&x,&y);
            camera.Move2D(x,y);
        }else
        {
            camera.mouse_position = glm::vec3(0,0,0);
        }
        
 
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {

            ImGui::Begin("Leap Motion Test");                          // Create a window called "Hello, world!" and append into it.
            auto bababuy = "Hands in FOV " + std::to_string(interleave.Hands.size());
            ImGui::Text(bababuy.c_str());
            
            if(ImGui::Button("Start Leap Tracking"))
                interleave.StartTracking();

            ImGui::SameLine();
            
            if(ImGui::Button("Stop Leap Tracking"))
                interleave.StopTracking();


            ImGui::Separator();

            const char* items[] = { "Desktop", "HMD" };
            static const char* current_item = NULL;

            if (ImGui::BeginCombo("Tracking Mode", current_item)) // The second parameter is the label previewed before opening the combo.
                {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
                    if (ImGui::Selectable(items[n], is_selected))
                        current_item = items[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                }
                ImGui::EndCombo();
                }

            interleave.SetTrackingMode(current_item == "HMD");
            if(ImGui::CollapsingHeader("Skeleton Settings"))
            {
                ImGui::Indent(16.f);
                ImGui::InputText("Arm Name", (char*)Bones::ARM.BoneName.data(), Bones::ARM.BoneName.length()+1);
                ImGui::Separator();
                if(ImGui::CollapsingHeader("Thumb Bones"))
                {
                    ImGui::Indent(16.f);
                    ImGui::InputText("Thumb Metacarpal Name", (char*)Bones::THUMB_Metacarpal.BoneName.data(), Bones::THUMB_Metacarpal.BoneName.length());
                    ImGui::InputText("Thumb Proximal Name", (char*)Bones::THUMB_Proximal.BoneName.data(), Bones::THUMB_Proximal.BoneName.length());
                    ImGui::InputText("Thumb Intermediate Name", (char*)Bones::THUMB_Intermediate.BoneName.data(), Bones::THUMB_Intermediate.BoneName.length());
                    ImGui::InputText("Thumb Distal Name", (char*)Bones::THUMB_Distal.BoneName.data(), Bones::THUMB_Distal.BoneName.length());
                    ImGui::Unindent(16.f);
                }
                ImGui::Separator();
                if(ImGui::CollapsingHeader("Index Bones"))
                {
                    ImGui::Indent(16.f);
                    ImGui::InputText("Index Metacarpal Name", (char*)Bones::INDEX_Metacarpal.BoneName.data(), Bones::INDEX_Metacarpal.BoneName.length());
                    ImGui::InputText("Index Proximal Name", (char*)Bones::INDEX_Proximal.BoneName.data(), Bones::INDEX_Proximal.BoneName.length());
                    ImGui::InputText("Index Intermediate Name", (char*)Bones::INDEX_Intermediate.BoneName.data(), Bones::INDEX_Intermediate.BoneName.length());
                    ImGui::InputText("Index Distal Name", (char*)Bones::INDEX_Distal.BoneName.data(), Bones::INDEX_Distal.BoneName.length());
                    ImGui::Unindent(16.f);
                }
                ImGui::Separator();
                if(ImGui::CollapsingHeader("Middle Bones"))
                {
                    ImGui::Indent(16.f);
                    ImGui::InputText("Middle Metacarpal Name", (char*)Bones::MIDDLE_Metacarpal.BoneName.data(), Bones::MIDDLE_Metacarpal.BoneName.length());
                    ImGui::InputText("Middle Proximal Name", (char*)Bones::MIDDLE_Proximal.BoneName.data(), Bones::MIDDLE_Proximal.BoneName.length());
                    ImGui::InputText("Middle Intermediate Name", (char*)Bones::MIDDLE_Intermediate.BoneName.data(), Bones::MIDDLE_Intermediate.BoneName.length());
                    ImGui::InputText("Middle Distal Name", (char*)Bones::MIDDLE_Distal.BoneName.data(), Bones::MIDDLE_Distal.BoneName.length());
                    ImGui::Unindent(16.f);
                }
                ImGui::Separator();
                if(ImGui::CollapsingHeader("Ring Bones"))
                {
                    ImGui::Indent(16.f);
                    ImGui::InputText("Ring Metacarpal Name", (char*)Bones::RING_Metacarpal.BoneName.data(), Bones::RING_Metacarpal.BoneName.length());
                    ImGui::InputText("Ring Proximal Name", (char*)Bones::RING_Proximal.BoneName.data(), Bones::RING_Proximal.BoneName.length());
                    ImGui::InputText("Ring Intermediate Name", (char*)Bones::RING_Intermediate.BoneName.data(), Bones::RING_Intermediate.BoneName.length());
                    ImGui::InputText("Ring Distal Name", (char*)Bones::RING_Distal.BoneName.data(), Bones::RING_Distal.BoneName.length());
                    ImGui::Unindent(16.f);
                }
                ImGui::Separator();
                if(ImGui::CollapsingHeader("Pinkie Bones"))
                {
                    ImGui::Indent(16.f);
                    ImGui::InputText("Pinkie Metacarpal Name", (char*)Bones::PINKIE_Metacarpal.BoneName.data(), Bones::PINKIE_Metacarpal.BoneName.length());
                    ImGui::InputText("Pinkie Proximal Name", (char*)Bones::PINKIE_Proximal.BoneName.data(), Bones::PINKIE_Proximal.BoneName.length());
                    ImGui::InputText("Pinkie Intermediate Name", (char*)Bones::PINKIE_Intermediate.BoneName.data(), Bones::PINKIE_Intermediate.BoneName.length());
                    ImGui::InputText("Pinkie Distal Name", (char*)Bones::PINKIE_Distal.BoneName.data(), Bones::PINKIE_Distal.BoneName.length());
                    ImGui::Unindent(16.f);
                }
                
                ImGui::Unindent(16.f);
            }

            ImGui::Separator();
            if(ImGui::CollapsingHeader("Hand Settings"))
            {
                ImGui::Indent(16.f);
                ImGui::ColorEdit3("Hands draw color",HandBonesColor.mdata);
                ImGui::Unindent(16.f);
            }

            ImGui::Separator();

            if(ImGui::CollapsingHeader("Recording Settings"))
            {
                ImGui::Indent(16.f);
                ImGui::InputText("Recording directory", RecordingDirectory, IM_ARRAYSIZE(RecordingDirectory));
                ImGui::InputText("Recording prefix", RecordingPrefix, IM_ARRAYSIZE(RecordingPrefix));
                ImGui::Unindent(16.f);
            }

            if(ImGui::Button("Start Recording"))
                interleave.StartRecording();
            ImGui::SameLine();
            if(ImGui::Button("Stop Recording"))
                interleave.StopRecording(RecordingDirectory);
            
            ImGui::End();

            
        }
        ImGui::Begin("Leap Motion Data");

        ImGui::PushStyleColor(ImGuiCol_Text, interleave.IsTracking? IM_COL32(0,255,0,255) : IM_COL32(255,0,0,255));
        ImGui::Text("TRACKING IS %s", interleave.IsTracking? "ACTIVE" : "INACTIVE");
        ImGui::PopStyleColor();
        ImGui::Separator();
        if(interleave.IsRecording)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, interleave.IsRecording? IM_COL32(0,255,0,255) : IM_COL32(255,0,0,255));
            ImGui::Text("RECORDING IS %s", interleave.IsRecording? "ACTIVE" : "INACTIVE");
            ImGui::PopStyleColor();
            ImGui::Separator();
        }

        
        /* GLint viewport[ 4 ];
         glGetIntegerv( GL_VIEWPORT, viewport );
 
         float vw = viewport[ 2 ];
         float vh = viewport[ 3 ];
 
         //90 = fov
         double fy = tan( 0.5 * 90 * 3.14159265359 / 180.0 ) * 6;
         double fx = fy * vw / vh;
         
             
         glMatrixMode(GL_PROJECTION_MATRIX);
         glLoadIdentity();
         glm::perspective(90.f, vw/vh,6.f, 1200.f);
         glTranslatef(0, -300, -500); */

  
        


        if( interleave.IsTracking)
        {
            
            glDeleteTextures(1, &Preview_Image);
            glGenTextures(1, &Preview_Image);
            glBindTexture(GL_TEXTURE_2D, Preview_Image);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 640, 240, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, interleave.Image[0].data);
            
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 640, 240, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &interleave.Image);
            ImGui::Image((void*)(intptr_t)Preview_Image,ImVec2(640, 240));
            
        }

        glMatrixMode(GL_MODELVIEW_MATRIX);
        
        if(interleave.Hands.size() > 0)
        {
            const vector<LEAP_HAND> HandsRef = interleave.Hands;
            glScalef(0.01f,0.01f,0.01f);
            for(int h = 0; h < HandsRef.size(); ++h)
            {
                auto Hand = HandsRef[h];
                ImGui::Text("%s Hand", Hand.type == eLeapHandType_Right? "Right" : "Left");
                ImGui::Separator();
                ImGui::Indent(16);
                ImGui::Text("Pinch %f", Hand.pinch_strength);
                ImGui::Text("Grab %f", Hand.grab_strength);
                ImGui::Text("Tilt x:%f y:%f z:%f w:%f", Hand.arm.rotation.x, Hand.arm.rotation.y, Hand.arm.rotation.z, Hand.arm.rotation.w);
                ImGui::Unindent(16.0f);

                glPushMatrix();
                DrawLine(Leap2ImoVec(Hand.arm.prev_joint),Leap2ImoVec(Hand.arm.next_joint), HandBonesColor.mdata);
                glTranslatef(Hand.arm.prev_joint.x, Hand.arm.prev_joint.y, Hand.arm.prev_joint.z);
                DrawCube(HandPointColor);
                glPopMatrix();
                
                glPushMatrix();
                glTranslatef(Hand.arm.next_joint.x, Hand.arm.next_joint.y, Hand.arm.next_joint.z);
                DrawCube(HandPointColor);
                glPopMatrix();
               
               //todo::fix rotation
                for(int i = 0; i < 5; ++i)
                {
                    auto Fingers = Hand.digits[i];
                    
                    glPushMatrix();
                    DrawLine(Leap2ImoVec(Hand.arm.next_joint), Leap2ImoVec(Fingers.metacarpal.prev_joint), HandBonesColor.mdata);
                    glPopMatrix();
                    for(int f = 0; f < 4; f++)
                    {
                        auto bone = Fingers.bones[f];
                        glPushMatrix();
                        DrawLine(Leap2ImoVec(bone.prev_joint), Leap2ImoVec(bone.next_joint), HandBonesColor.mdata);
                        glTranslatef(bone.next_joint.x, bone.next_joint.y, bone.next_joint.z);
                        DrawCube(HandPointColor);
                        glPopMatrix();
                    
                    }
                }
            }
            
        }
        glFlush();
        ImGui::End();
        
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}