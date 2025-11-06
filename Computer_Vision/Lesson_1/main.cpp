#include <opencv2/opencv.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <vector>

float coef = 10.0f;
int current_width = 800;
int current_height = 800;

std::vector<cv::Mat> container;
std::vector<unsigned int> texutres;
std::size_t count_of_textures;

void update_textures(std::vector<unsigned int>& textures, const std::size_t count_of_textures, std::vector<cv::Mat>& container) {
    for (std::size_t idx = 0; idx < count_of_textures; ++idx) {
        glBindTexture(GL_TEXTURE_2D, textures[idx]);
        cv::Mat current_mat = container[idx].clone();
        cv::cvtColor(current_mat, current_mat, cv::COLOR_RGB2BGR);
        cv::resize(current_mat, current_mat, cv::Size(current_width / 2, current_height - 40));
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, current_mat.cols, current_mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, current_mat.ptr());
    }
}
void create_textures(std::vector<unsigned int>& textures, const std::size_t count_of_textures, std::vector<cv::Mat>& container) {
    textures.resize(count_of_textures);
    glGenTextures(count_of_textures, textures.data());
    for (std::size_t idx = 0; idx < count_of_textures; ++idx) {
        glBindTexture(GL_TEXTURE_2D, textures[idx]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    current_width = width;
    current_height = height;
    update_textures(texutres, count_of_textures, container);
}


void morphology(std::vector<cv::Mat>& container) {
    cv::Mat after_morphology;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(container.back(), after_morphology, cv::MORPH_CLOSE, kernel, cv::Point(-1, -1), 1);
    container.emplace_back(after_morphology);
}

void grab_cut(std::vector<cv::Mat>& container) {

    cv::Mat bg_model = cv::Mat::zeros(1, 65, CV_64FC1);
    cv::Mat fg_model = cv::Mat::zeros(1, 65, CV_64FC1);
    cv::Mat mask = cv::Mat::zeros(container.back().rows, container.back().cols, CV_8UC1);
    cv::Mat after_grab;
    cv::grabCut(container.back(), mask, cv::Rect(150.0f / coef, 600.0f / coef, 2750.0f / coef, 3000.0f / coef), bg_model, fg_model, 5, cv::GC_INIT_WITH_RECT);
    //cv::grabCut(container.back(), mask, cv::Rect(30, 70, 450, 390), bg_model, fg_model, 5, cv::GC_INIT_WITH_RECT);

    cv::Mat mask2 = (mask == 1) + (mask == 3);  // 0 = cv::GC_BGD, 1 = cv::GC_FGD, 2 = cv::PR_BGD, 3 = cv::GC_PR_FGD
    cv::Mat dest;
    container.back().copyTo(dest, mask2);

    container.emplace_back(dest);
}

int main()
{
    const std::string image_path = "C:\\dev\\Source\\LABS\\Computer_Vision\\Lesson_1\\data\\image.jpeg";

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Computer_Vision_1", NULL, NULL);
    if (window == nullptr) {
        std::cout << "ERROR: Can't creat window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERROR: Can't init GLAD!" << std::endl;
        glfwTerminate();
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // original image
    container.emplace_back(cv::imread(image_path, cv::IMREAD_COLOR));

    // resized image for fast proccessing
    cv::Mat resized_image;
    cv::resize(container.back(), resized_image, cv::Size((float)container.back().cols / coef, (float)container.back().rows / coef));
    container.emplace_back(resized_image);

    // high up bright
    cv::Mat brigth_image;
    container.back().convertTo(brigth_image, -1, 1.3, 1.3);
    container.emplace_back(brigth_image);
    
    morphology(container);
    
    grab_cut(container);

    cv::Mat gray;
    cv::cvtColor(container.back(), gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(11, 11), 0);
    container.emplace_back(gray);

    cv::Mat canny;
    cv::Canny(container.back(), canny, 0, 200);
    cv::dilate(canny, canny, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)), cv::Point(-1, -1), 1);
    container.emplace_back(canny);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(container.back(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point> approx;
    cv::approxPolyDP(contours[0], approx, 5.0, true); // epsilon = 5 пикселей

    cv::Mat contour_points = container.back().clone();
    cv::cvtColor(contour_points, contour_points, cv::COLOR_GRAY2BGR);
    std::vector<cv::Point2f> new_points = { approx[1] * coef, approx[0] * coef, approx[3] * coef, approx[2] * coef };
    for (std::size_t idx = 0; idx < new_points.size(); ++idx) {
        cv::circle(contour_points, new_points[idx] / coef, 5, cv::Scalar(0, 0, 255), -1);
        cv::putText(contour_points, std::to_string(idx), new_points[idx] / coef, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0));
    }
    container.emplace_back(contour_points);
    
    std::vector<cv::Point2f> result_points = {
        cv::Point2f(0,0), cv::Point2f(container[0].cols, 0), cv::Point2f(container[0].cols, container[0].rows), cv::Point2f(0, container[0].rows)
    };
    cv::Mat M = cv::getPerspectiveTransform(new_points, result_points);
    cv::Mat final;
    cv::warpPerspective(container[0], final, M, cv::Size(container[0].cols, container[0].rows));
    container.emplace_back(final);

    cv::imwrite("Outpur.png", container.back());

    count_of_textures = container.size();
    create_textures(texutres, count_of_textures, container);
    update_textures(texutres, count_of_textures, container);

    int current_show_index = 0;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(current_width / 2, current_height));
        ImGui::Begin("Output_images", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::Image(texutres[current_show_index], ImVec2(current_width / 2, current_height - 40));
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(current_width / 2, 0));
        ImGui::SetNextWindowSize(ImVec2(current_width / 2, current_height));
        ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        if (ImGui::InputInt("Input number of image", &current_show_index, 1)) {
            if (current_show_index == count_of_textures)
                current_show_index = 0;
            if (current_show_index < 0)
                current_show_index = count_of_textures - 1;
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}