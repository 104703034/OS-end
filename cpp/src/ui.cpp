
#include "ui.h"

#include "bankState.h"

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <sstream>
#include <set>

#define SCALE_FACTOR (2.5)

using std::string;
using std::set;

static int scale_dpi(int p) {
    return p*SCALE_FACTOR;
}

static string int2identifier(int i, string additional_label = "") {
  using namespace std;
  stringstream ss;
  ss << "##" << i << additional_label;
  return ss.str();
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int ui_loop(BankState* bs) {
    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(scale_dpi(1280), scale_dpi(720),
                                          "banker's problem", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    // Setup style
    ImGui::StyleColorsClassic();
    //ImGui::StyleColorsDark();

    // Load Fonts
    ImGuiIO& io = ImGui::GetIO();
    // TODO: change to absolute path
    io.Fonts->AddFontFromFileTTF("imgui/extra_fonts/Roboto-Medium.ttf", scale_dpi(12.0f));

    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        {
            ImGui::SetNextWindowSize(ImVec2(scale_dpi(600), scale_dpi(100)), ImGuiWindowFlags_NoTitleBar);
            ImGui::Begin("banker's problem", nullptr);
            ImGui::Text("Demo for banker's problem");
            ImGui::End();
        }

        {
            ImGui::SetNextWindowSize(ImVec2(scale_dpi(800), scale_dpi(480)));
            ImGui::Begin("banker's window", nullptr, ImGuiWindowFlags_NoTitleBar |
                                                     ImGuiWindowFlags_NoResize);
            using std::pair;
            pair<int, int> bm = bs->get_banker_money();
            char buf[32];
            sprintf(buf, "%d/%d", bm.first, bm.second);
            ImGui::ProgressBar(bm.first*1.0 / bm.second, ImVec2(-1.0f, 0.f), buf);
            // TODO: need a lock to protect bs.client_money
            static float f[30] = {0};
            static float all_f;
            float old_all_f = all_f;
            bs->lock_state();
            {
              set<int> S;
              for(auto p: bs->get_client_money())
                S.insert(p.first);
              for(int i = 0 ; i <= 20 ; i++)
                if(!S.count(i))
                  f[i] = all_f;
            }
            for(auto p: bs->get_client_money()) {
              auto rat = p.second;
              ImGui::SetCursorPos(ImVec2(scale_dpi(5), scale_dpi(10 + 20*p.first)));
              ImGui::Text("%d.", p.first);
              ImGui::SetCursorPos(ImVec2(scale_dpi(20), scale_dpi(10 + 20*p.first)));
              sprintf(buf, "%d/%d (-%d)", rat.first, rat.second, rat.second-rat.first);
              ImGui::ProgressBar(rat.first*1.0 / rat.second, ImVec2(0.f, 0.f), buf);
              ImGui::SetCursorPos(ImVec2(scale_dpi(550), scale_dpi(10 + 20*p.first)));
              ImGui::Text(",   lambda = ");
              ImGui::SetCursorPos(ImVec2(scale_dpi(610), scale_dpi(10 + 20*p.first)));
              ImGui::PushItemWidth(scale_dpi(150));
              ImGui::SliderFloat(int2identifier(p.first, "sdfkjl").c_str(), f + p.first, 0.0f, 10.0f);
              ImGui::PopItemWidth();
              if(rat.first && rat.second-rat.first <= bm.first) { // can potentially increase banker's balance
                ImGui::SetCursorPos(ImVec2(scale_dpi(755), scale_dpi(10 + 20*p.first)));
                ImGui::Text(".");
              }
            }
            ImGui::SetCursorPos(ImVec2(scale_dpi(100), scale_dpi(440)));
            ImGui::SliderFloat("  adjust all lambda values at once", &all_f, 0.0f, 10.0f);
            if(old_all_f != all_f)
              for(int i = 0 ; i <= 20 ; i++)
                f[i] = all_f;
            for(int i = 0 ; i <= 20 ; i++)
              bs->set_token_mu(i, f[i]);
            bs->unlock_state();
            ImGui::End();
        }

        ImGui::ShowMetricsWindow(nullptr);

        if (false)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
    return 0;
}

