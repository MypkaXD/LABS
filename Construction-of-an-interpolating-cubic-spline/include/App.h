#include "imgui.h"
#include "imgui-SFML.h"

#include "implot.h"
#include "implot_internal.h"

#include "imconfig.h"
#include "imconfig-SFML.h"

#include "SFML/Graphics.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "Construction_of_an_interpolating_cubic_spline.h"
#include "Sweep_method.h"

#pragma once

class App {
private:
    sf::RenderWindow m_window;
    ImGuiIO io;

    bool isRecreateData = true;
    bool isShow = false;
    
    bool isAnalyticalSolutionSet = false; // просчитано ли аналитическое решение
    bool isShowAnalyticalSolution = false; // показывать ли аналитическое решение

    bool isAnalyticalSolutionFirstDifSet = false;
    bool isShowAnalyticalSolutionFirstDif = false;

    bool isAnalyticalSolutionSecondDifSet = false;
    bool isShowAnalyticalSolutionSecondDif = false;

    bool isNumericalSolutionSet = false; // просчитано ли численное решение
    bool isShowNumericalSolution = false; // показывать ли численное решение

    int separate_n = 10; // число разбиений дл€ S_i(x)
    int item_current_idx = 0;

    std::vector<double> x_of_analytical_solution;

    std::vector<double> y_of_analytical_solution;
    std::vector<double> y_of_analytical_solution_first_dif;
    std::vector<double> y_of_analytical_solution_second_dif;

    std::vector<double> x_of_numerical_solution;
    std::vector<double> y_of_numerical_solution;

    ImGuiWindowFlags flags_for_window = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    Construction_of_an_interpolating_cubic_spline cs;



public:
    App()
    {
    }
    ~App() {
        ImGui::SFML::Shutdown();
    }

    void init() {
        m_window.create(sf::VideoMode(1920, 1080, 32), "Cauchy problem");
        m_window.setFramerateLimit(60);
        ImGui::SFML::Init(m_window);
        io = ImGui::GetIO();
        ImPlot::CreateContext();
    }

    void run() {

        sf::Clock deltaClock;
        while (m_window.isOpen()) {
            ImPlot::SetCurrentContext(ImPlot::GetCurrentContext());
            sf::Event event;
            while (m_window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(m_window, event);

                if (event.type == sf::Event::Closed) {
                    m_window.close();
                }
            }
            ImGui::SFML::Update(m_window, deltaClock.restart());

            if (item_current_idx == 0) {
                cs.set_border(-1, 1);
                get_data_for_test_task();
                create_x_coords_for_analytical_solution();
                create_data_for_analytical_solution();
                create_data_for_analytical_solution_first_dif();
                create_data_for_analytical_solution_second_dif();
            }
            else if (item_current_idx == 1) {
                cs.set_border(0.2, 2);
                get_data_for_first_task();
                create_x_coords_for_analytical_solution_for_first_task();
                create_data_for_analytical_solution_for_first_task();
                create_data_for_analytical_solution_for_first_task_first_dif();
                create_data_for_analytical_solution_for_first_task_second_dif();
            }


            createWindowForInputN();
            //createWindowForInputBorder();
            createWindowForSettings();
            createWindowForSelectingTasks();

            createGraph();

            //std::cout << cs.get_border().first << "\t" << cs.get_border().second << std::endl;

            m_window.clear();
            render();
        }
    }

    void createWindowForSelectingTasks() {

        ImGui::SetNextWindowPos({ 0, 500 });
        ImGui::SetNextWindowSize({ 500, 500 });

        ImGui::Begin("Select_Tasks", 0, flags_for_window);

        const char* items[] = { "Test", "First", "Second" }; // создаем массив с названием задача
        const char* combo_preview_value = items[item_current_idx];  // отображает предыдущую выбранную задачу

        if (ImGui::BeginCombo("Select the task", combo_preview_value)) // создаем выплывающий список из задача
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++) // проходи циклом по массиву задач
            {
                const bool is_selected = (item_current_idx == n); // если задача в массиве, совпадающа€ с item_current_idx -> true
                if (ImGui::Selectable(items[n], is_selected)) { // устанавливаем индекс n дл€ item_current_idx, если выбрана задача n
                    item_current_idx = n;

                    isAnalyticalSolutionSet = false;
                    isAnalyticalSolutionFirstDifSet = false;
                    isAnalyticalSolutionSecondDifSet = false;
                    
                    isNumericalSolutionSet = false;
                }
                if (is_selected) // если is_selected
                    ImGui::SetItemDefaultFocus(); // устанавливаем фокус на выбранную задачу
            }
            ImGui::EndCombo(); // удал€ем выплывающее окно
        }

        ImGui::End();

    }

    void createWindowForSettings() {

        ImGui::SetNextWindowPos({ 1000,0 });
        ImGui::SetNextWindowSize({ 500,500 });

        ImGui::Begin("Settings", 0, flags_for_window);

        ImGui::Checkbox("Show AnalSolut", &isShowAnalyticalSolution);
        ImGui::Checkbox("Show NumericalSolut", &isShowNumericalSolution);
        ImGui::Checkbox("Show S'(x)", &isShowAnalyticalSolutionFirstDif);
        ImGui::Checkbox("Show S''(x)", &isShowAnalyticalSolutionSecondDif);

        ImGui::End();

    }

    void createWindowForInputBorder() {

        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::SetNextWindowSize({ 500,500 });

        ImGui::Begin("Input_Borders", 0, flags_for_window);

        double a_border = cs.get_border().first + 0.001;
        double b_border = cs.get_border().second - 0.001;

        static double max_value = INFINITY;
        static double min_value = -INFINITY;

        if (ImGui::DragScalar("Input left border", ImGuiDataType_Double, &cs.get_left_border(), 0.1f, &min_value, &b_border, "%.10f", ImGuiSliderFlags_AlwaysClamp)
            || ImGui::DragScalar("Input right border", ImGuiDataType_Double, &cs.get_right_border(), 0.1f, &a_border, &max_value, "%.10f", ImGuiSliderFlags_AlwaysClamp)) {

            isRecreateData = true;

            isAnalyticalSolutionSet = false;
            isNumericalSolutionSet = false;
        }

        ImGui::End();

    }

    void createWindowForInputN() {

        ImGui::SetNextWindowPos({ 500,0 });
        ImGui::SetNextWindowSize({ 500,500 });

        ImGui::Begin("Input_N", 0, flags_for_window);

        if (ImGui::DragInt("Input N", &cs.get_n(), 1, 1, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp)) {
            cs.set_h();
            isNumericalSolutionSet = false;
        }

        ImGui::End();

    }

    void get_data_for_first_task() {

        if (!isNumericalSolutionSet) {

            std::cout << "GET DATA FOR TEST TASK" << std::endl;

            cs.set_phi_for_first_task();
            cs.set_c();

            //cs.print_phi();

            cs.set_a();
            cs.set_b();
            cs.set_d();

            std::cout << "A B C D" << std::endl;

            cs.print_a();
            cs.print_b();
            cs.print_d();
            cs.print_c();

            create_data_for_numerical_solution();

            isNumericalSolutionSet = true;

            std::cout << "AFTER CREATE NUM SOL" << std::endl;
        }
    }

    void get_data_for_test_task() {

        if (!isNumericalSolutionSet) {

            std::cout << "GET DATA FOR TEST TASK" << std::endl;

            cs.set_phi_for_test();
            cs.set_c();

            //cs.print_phi();

            cs.set_a();
            cs.set_b();
            cs.set_d();
            
            std::cout << "A B C D" << std::endl;

            cs.print_a();
            cs.print_b();
            cs.print_d();
            cs.print_c();

            create_data_for_numerical_solution();

            isNumericalSolutionSet = true;

            std::cout << "AFTER CREATE NUM SOL" << std::endl;
        }
    }

    void create_data_for_numerical_solution() {

        std::cout << "BEFORE CREATE NUM SOL" << std::endl;

        x_of_numerical_solution.clear();
        y_of_numerical_solution.clear();

        x_of_numerical_solution.resize(cs.get_n() * separate_n);
        y_of_numerical_solution.resize(cs.get_n() * separate_n);

        std::cout << "N: " << cs.get_n() << std::endl;

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double h = (double)(abs((cs.get_border().first + count * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n())) -
                (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()))) / separate_n); // шаг в интервале дл€ S_i(x)

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));
            
            std::cout << "X_CUR: " << x_current << std::endl;

            std::cout << "H is " << h << std::endl;

            for (size_t i = 0; i < separate_n; ++i) {

                x_of_numerical_solution[count * separate_n + i] = (cs.get_border().first + count * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()) + i * h);
                y_of_numerical_solution[count * separate_n + i] = cs.get_a()[count] + cs.get_b()[count] *
                    (x_of_numerical_solution[count * separate_n + i] - x_current) + cs.get_c()[count] *
                    (x_of_numerical_solution[count * separate_n + i] - x_current) * (x_of_numerical_solution[count * separate_n + i] - x_current) +
                    cs.get_d()[count] / 6 * (x_of_numerical_solution[count * separate_n + i] - x_current) * (x_of_numerical_solution[count * separate_n + i] - x_current) *
                    (x_of_numerical_solution[count * separate_n + i] - x_current);

            }

        }
    }

    void createGraph() { // функци€ дл€ отрисовки графика

        ImGui::Begin("Graph");

        if (ImPlot::BeginPlot("Solution schedule", "x", "y", { 940,780 })) { // ќтрисовываем график
            
            // јналитическое решение
            if (isAnalyticalSolutionSet && isShowAnalyticalSolution)
                ImPlot::PlotLine("F(x)", x_of_analytical_solution.data(), y_of_analytical_solution.data(), x_of_analytical_solution.size()); // ќтрисовываем линию
            if (isAnalyticalSolutionFirstDifSet && isShowAnalyticalSolutionFirstDif)
                ImPlot::PlotLine("F'(x)", x_of_analytical_solution.data(), y_of_analytical_solution_first_dif.data(), x_of_analytical_solution.size()); // ќтрисовываем линию
            if (isAnalyticalSolutionSecondDifSet && isShowAnalyticalSolutionSecondDif)
                ImPlot::PlotLine("F''(x)", x_of_analytical_solution.data(), y_of_analytical_solution_second_dif.data(), x_of_analytical_solution.size()); // ќтрисовываем линию
            
            // „исленное решение
            if (isNumericalSolutionSet && isShowNumericalSolution)
                ImPlot::PlotLine("S(x)", x_of_numerical_solution.data(), y_of_numerical_solution.data(), x_of_numerical_solution.size()); // ќтрисовываем линию


            
            ImPlot::EndPlot(); // «аканчиваем отрисовку графика
        }
        ImGui::End(); // ”дал€ем окно

    }

    void render() {
        ImGui::SFML::Render(m_window);
        m_window.display();
    }

private:
    
    // јналитическое решение дл€ тестовой задачи
    void create_x_coords_for_analytical_solution() {

        if (!isAnalyticalSolutionSet) {

            const static size_t size_of_analytical_solution = 1000; // ѕолучаем размер вектора точек дл€ аналитического решени€

            x_of_analytical_solution.clear();

            x_of_analytical_solution.resize(size_of_analytical_solution + 1);

            double h_for_analytical_solution = abs(cs.get_border().second - cs.get_border().first) / size_of_analytical_solution;

            for (size_t count = 0; count < size_of_analytical_solution / 2; ++count)
                x_of_analytical_solution[count] = cs.get_border().first + h_for_analytical_solution * count;

            for (size_t count = size_of_analytical_solution / 2; count <= size_of_analytical_solution; ++count)
                x_of_analytical_solution[count] = cs.get_border().first + h_for_analytical_solution * count;

        }

    }
    void create_data_for_analytical_solution() {

        if (!isAnalyticalSolutionSet) {

            const static size_t size_of_analytical_solution = 1000; // ѕолучаем размер вектора точек дл€ аналитического решени€

            y_of_analytical_solution.clear();

            y_of_analytical_solution.resize(size_of_analytical_solution + 1);

            double h_for_analytical_solution = abs(cs.get_border().second - cs.get_border().first) / size_of_analytical_solution;

            for (size_t count = 0; count < size_of_analytical_solution / 2; ++count)
                y_of_analytical_solution[count] = x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] + 3 * x_of_analytical_solution[count] * x_of_analytical_solution[count];

            for (size_t count = size_of_analytical_solution / 2; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution[count] = -x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] + 3 * x_of_analytical_solution[count] * x_of_analytical_solution[count];

            isAnalyticalSolutionSet = true;

        }
    }
    void create_data_for_analytical_solution_first_dif() {

        if (!isAnalyticalSolutionFirstDifSet) {

            const static size_t size_of_analytical_solution = 1000; // ѕолучаем размер вектора точек дл€ аналитического решени€

            y_of_analytical_solution_first_dif.clear();

            y_of_analytical_solution_first_dif.resize(size_of_analytical_solution + 1);

            double h_for_analytical_solution = abs(cs.get_border().second - cs.get_border().first) / size_of_analytical_solution;

            for (size_t count = 0; count < size_of_analytical_solution / 2; ++count)
                y_of_analytical_solution_first_dif[count] = 3 * x_of_analytical_solution[count] * x_of_analytical_solution[count] + 6 * x_of_analytical_solution[count];

            for (size_t count = size_of_analytical_solution / 2; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution_first_dif[count] = -3 * x_of_analytical_solution[count] * x_of_analytical_solution[count] + 6 * x_of_analytical_solution[count];

            isAnalyticalSolutionFirstDifSet = true;

        }
    }
    void create_data_for_analytical_solution_second_dif() {

        if (!isAnalyticalSolutionSecondDifSet) {

            const static size_t size_of_analytical_solution = 1000; // ѕолучаем размер вектора точек дл€ аналитического решени€

            y_of_analytical_solution_second_dif.clear();

            y_of_analytical_solution_second_dif.resize(size_of_analytical_solution + 1);

            double h_for_analytical_solution = abs(cs.get_border().second - cs.get_border().first) / size_of_analytical_solution;

            for (size_t count = 0; count < size_of_analytical_solution / 2; ++count)
                y_of_analytical_solution_second_dif[count] = 6 * x_of_analytical_solution[count] + 6;

            for (size_t count = size_of_analytical_solution / 2; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution_second_dif[count] = -6 * x_of_analytical_solution[count] + 6;

            isAnalyticalSolutionSecondDifSet = true;

        }
    }

    // јналитическое решение дл€ первой задачи
    void create_x_coords_for_analytical_solution_for_first_task() {
        if (!isAnalyticalSolutionSet) {

            const static size_t size_of_analytical_solution = 1000; // ѕолучаем размер вектора точек дл€ аналитического решени€

            x_of_analytical_solution.clear();

            x_of_analytical_solution.resize(size_of_analytical_solution + 1);

            double h_for_analytical_solution = abs(cs.get_border().second - cs.get_border().first) / size_of_analytical_solution;

            for (size_t count = 0; count <= size_of_analytical_solution; ++count) {
                x_of_analytical_solution[count] = cs.get_border().first + h_for_analytical_solution * count;
            }

        }
    }
    void create_data_for_analytical_solution_for_first_task() {

        if (!isAnalyticalSolutionSet) {

            const static size_t size_of_analytical_solution = 1000; // ѕолучаем размер вектора точек дл€ аналитического решени€

            y_of_analytical_solution.clear();

            y_of_analytical_solution.resize(size_of_analytical_solution + 1);

            double h_for_analytical_solution = abs(cs.get_border().second - cs.get_border().first) / size_of_analytical_solution;

            for (size_t count = 0; count <= size_of_analytical_solution; ++count) {
                y_of_analytical_solution[count] = log(x_of_analytical_solution[count] + 1) / (x_of_analytical_solution[count] + 1);
            }

            isAnalyticalSolutionSet = true;

        }
    }
    void create_data_for_analytical_solution_for_first_task_first_dif() {

        if (!isAnalyticalSolutionFirstDifSet) {

            const static size_t size_of_analytical_solution = 1000; // ѕолучаем размер вектора точек дл€ аналитического решени€

            y_of_analytical_solution_first_dif.clear();

            y_of_analytical_solution_first_dif.resize(size_of_analytical_solution + 1);

            double h_for_analytical_solution = abs(cs.get_border().second - cs.get_border().first) / size_of_analytical_solution;

            for (size_t count = 0; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution_first_dif[count] = (double)1 / (x_of_analytical_solution[count] * x_of_analytical_solution[count] +
                    2 * x_of_analytical_solution[count] + 1) - log(x_of_analytical_solution[count] + 1) / (x_of_analytical_solution[count] *
                        x_of_analytical_solution[count] + 2 * x_of_analytical_solution[count] + 1);

            isAnalyticalSolutionFirstDifSet = true;

        }
    }
    void create_data_for_analytical_solution_for_first_task_second_dif() {

        if (!isAnalyticalSolutionSecondDifSet) {

            const static size_t size_of_analytical_solution = 1000; // ѕолучаем размер вектора точек дл€ аналитического решени€

            y_of_analytical_solution_second_dif.clear();

            y_of_analytical_solution_second_dif.resize(size_of_analytical_solution + 1);

            double h_for_analytical_solution = abs(cs.get_border().second - cs.get_border().first) / size_of_analytical_solution;

            for (size_t count = 0; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution_second_dif[count] = (2 * log(x_of_analytical_solution[count] + 1)) / (x_of_analytical_solution[count] * x_of_analytical_solution[count] *
                    x_of_analytical_solution[count] + 3 * x_of_analytical_solution[count] * x_of_analytical_solution[count] + 3 * x_of_analytical_solution[count] + 1) -
                (double)3 / (x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] + 3 * x_of_analytical_solution[count] * x_of_analytical_solution[count] +
                    3 * x_of_analytical_solution[count] + 1);

            isAnalyticalSolutionSecondDifSet = true;

        }
    }


};