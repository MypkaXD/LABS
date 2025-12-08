#include <opencv2/opencv.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

float coef = 10.0f;
int current_width = 800;
int current_height = 800;
int count_of_symbols_in_real = 2452;

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

void morphology_erode(const cv::Mat& input, cv::Mat& output, const cv::Mat& kernel, int count_of_iters = 1) {

    output = input.clone();

    int kernel_rows = kernel.rows;
    int kernel_cols = kernel.cols;
    int kernel_center_x = kernel_cols / 2;
    int kernel_center_y = kernel_rows / 2;

    cv::Mat current_mat;

    for (int current_iters = 0; current_iters < count_of_iters; ++current_iters) {

        current_mat = output.clone();

#pragma omp parallel for collapse(2)
        for (int idx_i = kernel_center_x; idx_i < current_mat.cols - kernel_center_x; ++idx_i) {
            for (int idx_j = kernel_center_y; idx_j < current_mat.rows - kernel_center_y; ++idx_j) {

                std::uint8_t min_b = (int)(1 << 8) - 1;
                std::uint8_t min_g = (int)(1 << 8) - 1;
                std::uint8_t min_r = (int)(1 << 8) - 1;

                for (int idx_kernel_x = 0; idx_kernel_x < kernel_cols; ++idx_kernel_x) {
                    for (int idx_kernel_y = 0; idx_kernel_y < kernel_rows; ++idx_kernel_y) {

                        cv::Vec3b input_current_pixel = current_mat.at<cv::Vec3b>(idx_j + idx_kernel_y - kernel_center_y, idx_i + idx_kernel_x - kernel_center_x);

                        if (input_current_pixel[0] < min_b)
                            min_b = input_current_pixel[0];
                        if (input_current_pixel[1] < min_g)
                            min_g = input_current_pixel[1];
                        if (input_current_pixel[2] < min_r)
                            min_r = input_current_pixel[2];
                    }
                }

                cv::Vec3b& output_current_pixel = output.at<cv::Vec3b>(idx_j, idx_i);
                output_current_pixel = { min_b, min_g, min_r };
            }
        }
    }
}
void morphology_dilate(const cv::Mat& input, cv::Mat& output, const cv::Mat& kernel, int count_of_iters = 1) {

    output = input.clone();

    int kernel_rows = kernel.rows;
    int kernel_cols = kernel.cols;
    int kernel_center_x = kernel_cols / 2;
    int kernel_center_y = kernel_rows / 2;

    cv::Mat current_mat;

    for (int current_iters = 0; current_iters < count_of_iters; ++current_iters) {

        current_mat = output.clone();

#pragma omp parallel for collapse(2)
        for (int idx_i = kernel_center_x; idx_i < current_mat.cols - kernel_center_x; ++idx_i) {
            for (int idx_j = kernel_center_y; idx_j < current_mat.rows - kernel_center_y; ++idx_j) {

                std::uint8_t max_b = 0;
                std::uint8_t max_g = 0;
                std::uint8_t max_r = 0;

                for (int idx_kernel_x = 0; idx_kernel_x < kernel_cols; ++idx_kernel_x) {
                    for (int idx_kernel_y = 0; idx_kernel_y < kernel_rows; ++idx_kernel_y) {

                        cv::Vec3b input_current_pixel = current_mat.at<cv::Vec3b>(idx_j + idx_kernel_y - kernel_center_y, idx_i + idx_kernel_x - kernel_center_x);

                        if (input_current_pixel[0] > max_b)
                            max_b = input_current_pixel[0];
                        if (input_current_pixel[1] > max_g)
                            max_g = input_current_pixel[1];
                        if (input_current_pixel[2] > max_r)
                            max_r = input_current_pixel[2];
                    }
                }

                cv::Vec3b& output_current_pixel = output.at<cv::Vec3b>(idx_j, idx_i);
                output_current_pixel = { max_b, max_g, max_r };
            }
        }
    }
}
void morphology_close(const cv::Mat& input, cv::Mat& output, const cv::Mat& kernel, int count_of_iters = 1) {
    cv::Mat current_mat;
    morphology_dilate(input, current_mat, kernel, count_of_iters);
    morphology_erode(current_mat, output, kernel, count_of_iters);
}
void morphology(std::vector<cv::Mat>& container) {
    cv::Mat after_morphology;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(container.back(), after_morphology, cv::MORPH_CLOSE, kernel, cv::Point(-1, -1), 1);
    container.emplace_back(after_morphology);
}

void brightness_increase(const cv::Mat& input, cv::Mat& output, double alpha, double beta) {
    output.create(input.size(), input.type());
    const int total_pixels = input.rows * input.cols;
    const cv::Vec3b* input_data = input.ptr<cv::Vec3b>();
    cv::Vec3b* output_data = output.ptr<cv::Vec3b>();
#pragma omp parallel for
    for (int i = 0; i < total_pixels; ++i) {
        for (int c = 0; c < 3; ++c) {
            int val = static_cast<int>(input_data[i][c] * alpha + beta);
            output_data[i][c] = static_cast<uchar>((std::max)(0, (std::min)(255, val)));
        }
    }
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

void draw_lines_of_text(const cv::Mat& input, cv::Mat& output, const std::vector<cv::Rect>& rects, int& count_of_lines, int param_min_distance_between_lines = 40) {

    if (rects.size() == 0)
        return;

    cv::cvtColor(input, output, cv::COLOR_GRAY2BGR);

    std::vector<int> y_coords(rects.size());
    for (std::size_t idx = 0; idx < rects.size(); ++idx) {
        y_coords[idx] = rects[idx].y;
    }

    std::sort(y_coords.begin(), y_coords.end());

    std::vector<int> y_lines;
    y_lines.emplace_back(y_coords[0]);
    for (std::size_t idx = 1; idx < y_coords.size(); ++idx) {
        if (std::abs(y_coords[idx] - y_lines.back()) >= param_min_distance_between_lines)
            y_lines.emplace_back(y_coords[idx]);
    }

    count_of_lines = y_lines.size();

    for (std::size_t idx = 0; idx < y_lines.size(); ++idx) {
        cv::line(output, cv::Point(0, y_lines[idx]), cv::Point(output.cols, y_lines[idx]), cv::Scalar(0, 255, 0), 3);
    }

}

cv::Mat createDocumentMask(const cv::Mat& warped, int border_margin = 40) {

    cv::Mat mask = cv::Mat::zeros(warped.size(), warped.type());

    cv::Rect document_roi(border_margin, border_margin,
        warped.cols - 2 * border_margin,
        warped.rows - 2 * border_margin);

    mask(document_roi) = 255;

    cv::Mat result;
    warped.copyTo(result, mask);
    return result;
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
    cv::resize(container[0], resized_image, cv::Size((float)container[0].cols / coef, (float)container[0].rows / coef));
    container.emplace_back(resized_image);

    // high up bright
    cv::Mat brigth_image;
    brightness_increase(container.back(), brigth_image, 1.3, 10);
    // OpenCV impl
    // container.back().convertTo(brigth_image, -1, 1.3, 10);
    container.emplace_back(brigth_image);

    // Morphology close ~ dilate and erode
    cv::Mat close;
    morphology_close(container.back(), close, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)), 1);
    container.emplace_back(close);
    // OpenCV impl
    // morphology(container);

    grab_cut(container);

    cv::Mat gray;
    cv::cvtColor(container.back(), gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(11, 11), 0);
    container.emplace_back(gray);

    cv::Mat canny;
    cv::Canny(container.back(), canny, 50, 150);
    cv::dilate(canny, canny, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)), cv::Point(-1, -1), 1);
    container.emplace_back(canny);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(container.back(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point> approx;
    cv::approxPolyDP(contours[0], approx, 5.0, true);

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

    cv::Mat gray_mat;
    cv::cvtColor(container.back(), gray_mat, cv::COLOR_BGR2GRAY);
    container.emplace_back(gray_mat);

    cv::Mat binary_mat;
    cv::threshold(container.back(), binary_mat, 128, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    container.emplace_back(binary_mat);

    container.emplace_back(createDocumentMask(container.back()));

    // Create MSER detector
    cv::Ptr<cv::MSER> mser = cv::MSER::create(1, 100, 2000, 0.5, 0.1);
    std::vector<std::vector<cv::Point>> msers;
    std::vector<cv::Rect> rects;
    mser->detectRegions(container.back(), msers, rects);

    cv::Mat mser_mat;
    cv::cvtColor(container.back(), mser_mat, cv::COLOR_GRAY2BGR);

    // Draw bounding rectangles
    for (const auto& rect : rects) {
        cv::rectangle(mser_mat, rect, cv::Scalar(255, 0, 0), 2);
    }

    float error = (float)std::abs((int)msers.size() - count_of_symbols_in_real) / count_of_symbols_in_real;
    int count_of_lines = 0;
    std::cout << "Number of symbols: " << msers.size() << std::endl;
    std::cout << "Number of real symbols: " << count_of_symbols_in_real << std::endl;
    std::cout << "Error: " << error << std::endl;
    
    container.emplace_back(mser_mat);

    // draw lines
    cv::Mat lines_mat;
    draw_lines_of_text(container[9], lines_mat, rects, count_of_lines, 50);
    container.emplace_back(lines_mat);

    std::cout << "Count of lines: " << count_of_lines << std::endl;

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

        ImGui::SetNextWindowPos(ImVec2(0, 0));
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
        ImGui::Text("Number of symbols: %ld", msers.size());
        ImGui::Text("Number of real symbols: %ld", count_of_symbols_in_real);
        ImGui::Text("Error: %lf",error);
        ImGui::Text("Count of lines: %lf", count_of_lines);
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