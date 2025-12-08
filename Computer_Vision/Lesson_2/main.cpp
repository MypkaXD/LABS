#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/all_layers.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <vector>
#include <algorithm>

int m_width = 800;
int m_height = 800;

cv::Mat image;
unsigned int texture;

void update_textures() {

    cv::resize(image, image, cv::Size(m_width, m_height));

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image.ptr());
}

void framebuffer_size_update(GLFWwindow* window, int width, int heigth) {
    m_width = width;
    m_height = heigth - 40;
    update_textures();
}

int main()
{

    std::string path = "C:\\dev\\Source\\LABS\\Computer_Vision\\Lesson_2\\data\\0-9.png";

    std::string model_path = "C:\\dev\\Source\\LABS\\Computer_Vision\\Lesson_2\\data\\conv_net.onnx";

    cv::dnn::Net net = cv::dnn::readNetFromONNX(model_path);

    std::size_t size_of_batch = 0;
    std::size_t count_of_channels = 0;
    std::size_t height = 0;
    std::size_t width = 0;
    std::vector<std::string> outs_names;
    cv::Mat image_for_nn;

    if (!net.empty()) {

        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        std::cout << "Model loaded with CPU backend." << std::endl;

        cv::dnn::MatShape net_input_shape;
        std::vector<cv::dnn::MatShape> input_layer_shapes;
        std::vector<cv::dnn::MatShape> output_layer_shapes;

        net.getLayerShapes(net_input_shape, 0, input_layer_shapes, output_layer_shapes);

        if (input_layer_shapes.size() != 1)
            return -1;

        std::vector<int> input_shape = input_layer_shapes[0];

        if (input_shape.size() != 4) {
            std::cout << "ERROR: get_data_from_input_shape" << std::endl;
            return -1;
        }
        else {
            size_of_batch = input_shape[0];
            count_of_channels = input_shape[1];
            height = input_shape[2];
            width = input_shape[3];

            image_for_nn = cv::Mat(28, 28, CV_8UC1);
        }

        outs_names = net.getUnconnectedOutLayersNames();
        for (int i = 0; i < outs_names.size(); ++i)
            std::cout << outs_names[i] << std::endl;
    }

    image = cv::imread(path, cv::IMREAD_GRAYSCALE);
    cv::cvtColor(image, image, cv::COLOR_GRAY2RGB);
    cv::resize(image, image, cv::Size(800, 800));

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Computer_Vision_2", NULL, NULL);
    if (window == nullptr) {
        std::cout << "ERROR: Can't creat window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_update);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERROR: Can't init GLAD!" << std::endl;
        glfwTerminate();
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    update_textures();

    bool is_drawing = false;
    bool is_clearing = false;

    int brush_size = 20;

    while (!glfwWindowShouldClose(window)) {


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto& io = ImGui::GetIO();
        

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(m_width, 40));
        ImGui::Begin("brush", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
        ImGui::SliderInt("BrushSize", &brush_size, 10, 30);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0, 40));
        ImGui::SetNextWindowSize(ImVec2(m_width, m_height));
        ImGui::Begin("paint", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
        ImGui::Image(texture, ImVec2(image.cols, image.rows));
        ImGui::End();

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            is_drawing = true;
        }
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            is_clearing = true;
        }
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            is_drawing = false;
        }
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
            is_clearing = false;
        }

        if (is_drawing || is_clearing) {
            int x = (int)io.MousePos.x;
            int y = (int)io.MousePos.y - 20;

            int center = brush_size / 2;
            if (x <= m_width && y > 40 && y <= m_height) {
                for (int i = y - center; i <= y + center; ++i) {
                    for (int j = x - center; j <= x + center; ++j) {
                        if (i >= 0 && i < image.rows && j >= 0 && j < image.cols) {
                            cv::Vec3b& px = image.at<cv::Vec3b>(i, j);
                            if (is_drawing)
                                px = { 0, 0, 0 };
                            if (is_clearing)
                                px = { 255, 255, 255 };
                        }
                    }
                }
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.cols, image.rows, GL_RGB, GL_UNSIGNED_BYTE, image.ptr());
            }
        }

        cv::Mat data_copy = image.clone();
        cv::Mat gray;
        cv::cvtColor(data_copy, gray, cv::COLOR_BGR2GRAY);
        cv::Mat binary;
        cv::threshold(gray, binary, 20, 255, cv::THRESH_BINARY_INV);
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (std::size_t idx = 0; idx < contours.size(); ++idx) {
            cv::Rect rect = cv::boundingRect(contours[idx]);
            if (rect.area() < 100)
                continue;
            cv::Mat roi = gray(rect);
            cv::Mat resized_roi;
            cv::resize(roi, resized_roi, cv::Size(28, 28));
            cv::Mat inverted;
            cv::bitwise_not(resized_roi, inverted);

            cv::Mat float_roi;
            inverted.convertTo(float_roi, CV_32F, 2.0 / 255.0, -1.0);

            cv::Mat blob = cv::dnn::blobFromImage(float_roi, 1.0, cv::Size(28, 28), cv::Scalar(), false, false);

            net.setInput(blob);
            cv::Mat output = net.forward();

            cv::Mat probabilities;
            cv::exp(output, probabilities);
            float sum = cv::sum(probabilities)[0];
            probabilities /= sum;

            cv::Point max_loc;
            double max_val;
            cv::minMaxLoc(probabilities, NULL, &max_val, NULL, &max_loc);
            if (max_val < 0.6)
                continue;

            int predicted_digit = max_loc.x;
            cv::rectangle(data_copy, rect, cv::Scalar(0, 0, 255), 2);

            std::string label = std::to_string(predicted_digit) + " (" + std::to_string((int)(max_val * 100)) + "%)";
            cv::putText(data_copy, label, cv::Point(rect.x, rect.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, data_copy.cols, data_copy.rows, GL_RGB, GL_UNSIGNED_BYTE, data_copy.ptr());

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