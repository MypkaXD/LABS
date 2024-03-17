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
    bool isShowNumericalSolutionFirstDif = false;
    bool isShowNumericalSolutionSecondDif = false;

    int separate_n = 10; // число разбиений для S_i(x)
    int item_current_idx = 0;
    int N = 20;

    std::vector<double> x_of_analytical_solution;

    std::vector<double> y_of_analytical_solution;
    std::vector<double> y_of_analytical_solution_first_dif;
    std::vector<double> y_of_analytical_solution_second_dif;

    std::vector<double> x_of_numerical_solution;

    std::vector<double> y_of_numerical_solution;
    std::vector<double> y_of_numerical_solution_first_dif;
    std::vector<double> y_of_numerical_solution_second_dif;

    std::vector<double> x_of_numerical_solution_with_multiple_n;

    std::vector<double> y_of_numerical_solution_with_multiple_n;
    std::vector<double> y_of_numerical_solution_first_dif_with_multiple_n;
    std::vector<double> y_of_numerical_solution_second_dif_with_multiple_n;

    std::vector<double> y_of_analytical_solution_for_graph;
    std::vector<double> y_of_analytical_solution_first_dif_for_graph;
    std::vector<double> y_of_analytical_solution_second_dif_for_graph;

    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> d;

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
        m_window.create(sf::VideoMode(1920, 1080, 32), "Construction of an interpolating cubic spline");
        m_window.setFramerateLimit(60);
        ImGui::SFML::Init(m_window);
        io = ImGui::GetIO();
        ImPlot::CreateContext();

        io.Fonts->Clear();
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 13.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

        ImGui::SFML::UpdateFontTexture();

        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 100.0f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.4f, 0.8f, 1.0f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(255, 255, 255, 100);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(255, 255, 255, 1.0f);
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(255, 255, 255, 1.0f);

        ImPlotStyle& plotStyle = ImPlot::GetStyle();
        //plotStyle.Colors[ImPlotCol_Line] = ImVec4(255, 0.0f, 0.0f, 1.0f);
        plotStyle.LineWeight = 3;
    }

    void run() {

        sf::Clock deltaClock;
        while (m_window.isOpen()) {
            ImPlot::SetCurrentContext(ImPlot::GetCurrentContext());
            sf::Event event;
            while (m_window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(m_window, event);

                if (event.type == sf::Event::Closed)
                    m_window.close();
            
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

                cs.set_border(2, 4);

                get_data_for_first_task();

                create_x_coords_for_analytical_solution_for_first_task();
                create_data_for_analytical_solution_for_first_task();
                create_data_for_analytical_solution_for_first_task_first_dif();
                create_data_for_analytical_solution_for_first_task_second_dif();

            }
            else if (item_current_idx == 2) {

                cs.set_border(2, 4);

                get_data_for_second_task();

                create_x_coords_for_analytical_solution_for_second_task();
                create_data_for_analytical_solution_for_second_task();
                create_data_for_analytical_solution_for_second_task_first_dif();
                create_data_for_analytical_solution_for_second_task_second_dif();

            }

            createWindowForInputN();
            createWindowForSettings();
            createWindowForSelectingTasks();

            createGraph();
            createTableForCoefs();
            createTableForComparison();

            m_window.clear({ 255,255,255,100 });
            render();
        }
    }

    void createWindowForSelectingTasks() {

        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::SetNextWindowSize({ 320, 40 });

        ImGui::Begin("Select_Tasks", 0, flags_for_window);

        const char* items[] = { u8"Тестовая", u8"Первая", u8"Вторая" }; // создаем массив с названием задача
        const char* combo_preview_value = items[item_current_idx];  // отображает предыдущую выбранную задачу

        if (ImGui::BeginCombo(u8"Выберете задачу", combo_preview_value)) // создаем выплывающий список из задача
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++) // проходи циклом по массиву задач
            {
                const bool is_selected = (item_current_idx == n); // если задача в массиве, совпадающая с item_current_idx -> true
                if (ImGui::Selectable(items[n], is_selected)) { // устанавливаем индекс n для item_current_idx, если выбрана задача n
                    item_current_idx = n;

                    isAnalyticalSolutionSet = false;
                    isAnalyticalSolutionFirstDifSet = false;
                    isAnalyticalSolutionSecondDifSet = false;
                    
                    isNumericalSolutionSet = false;
                }
                if (is_selected) // если is_selected
                    ImGui::SetItemDefaultFocus(); // устанавливаем фокус на выбранную задачу
            }
            ImGui::EndCombo(); // удаляем выплывающее окно
        }

        ImGui::End();

    }
    void createWindowForSettings() {

        ImGui::SetNextWindowPos({ 0,41 });
        ImGui::SetNextWindowSize({ 320, 150});

        ImGui::Begin("Settings", 0, flags_for_window);

        ImGui::Checkbox(u8"Отображать F(x)", &isShowAnalyticalSolution);
        ImGui::Checkbox(u8"Отображать F'(x)", &isShowAnalyticalSolutionFirstDif);
        ImGui::Checkbox(u8"Отображать F''(x)", &isShowAnalyticalSolutionSecondDif);
        ImGui::Checkbox(u8"Отображать S(x)", &isShowNumericalSolution);
        ImGui::Checkbox(u8"Отображать S'(x)", &isShowNumericalSolutionFirstDif);
        ImGui::Checkbox(u8"Отображать S''(x)", &isShowNumericalSolutionSecondDif);

        ImGui::End();

    }
    void createWindowForInputN() {

        ImGui::SetNextWindowPos({ 0, 192 });
        ImGui::SetNextWindowSize({ 320,40 });

        ImGui::Begin("Input_N", 0, flags_for_window);

        if (ImGui::DragInt(u8"Число разбиений n", &cs.get_n(), 1, 1, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp)) {
            cs.set_h();
            N = cs.get_n() * 2;
            isNumericalSolutionSet = false;
        }

        ImGui::End();

    }

    void createGraph() { // функция для отрисовки графика

        ImGui::SetNextWindowPos({ 961, 0 });
        ImGui::SetNextWindowSize({ 940,1080 });

        ImGui::Begin("Graph", 0, flags_for_window);

        if (ImPlot::BeginPlot(u8"График", "x", "y", { 930,1000})) { // Отрисовываем график
            
            // Аналитическое решение
            if (isAnalyticalSolutionSet && isShowAnalyticalSolution)
                ImPlot::PlotLine("F(x)", x_of_analytical_solution.data(), y_of_analytical_solution.data(), x_of_analytical_solution.size()); // Отрисовываем линию
            if (isAnalyticalSolutionFirstDifSet && isShowAnalyticalSolutionFirstDif)
                ImPlot::PlotLine("F'(x)", x_of_analytical_solution.data(), y_of_analytical_solution_first_dif.data(), x_of_analytical_solution.size()); // Отрисовываем линию
            if (isAnalyticalSolutionSecondDifSet && isShowAnalyticalSolutionSecondDif)
                ImPlot::PlotLine("F''(x)", x_of_analytical_solution.data(), y_of_analytical_solution_second_dif.data(), x_of_analytical_solution.size()); // Отрисовываем линию
            
            // Численное решение
            if (isNumericalSolutionSet && isShowNumericalSolution)
                ImPlot::PlotLine("S(x)", x_of_numerical_solution.data(), y_of_numerical_solution.data(), x_of_numerical_solution.size()); // Отрисовываем линию
            if (isNumericalSolutionSet && isShowNumericalSolutionFirstDif)
                ImPlot::PlotLine("S'(x)", x_of_numerical_solution.data(), y_of_numerical_solution_first_dif.data(), x_of_numerical_solution.size()); // Отрисовываем линию
            if (isNumericalSolutionSet && isShowNumericalSolutionSecondDif)
                ImPlot::PlotLine("S''(x)", x_of_numerical_solution.data(), y_of_numerical_solution_second_dif.data(), x_of_numerical_solution.size()); // Отрисовываем линию
            
            ImPlot::EndPlot(); // Заканчиваем отрисовку графика
        }

        ImGui::End(); // Удаляем окно

    }

    void createTableForCoefs() { // функция для отрисовки графика

        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable; // настройки для таблицы

        ImGui::SetNextWindowPos({ 0,233 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 960,395 }); // устанавливаем размер для создаваемого окна для таблицы

        ImGui::Begin("TableForCoefs", 0, flags_for_window); // создаем окно с заданными настройками

        if (isNumericalSolutionSet) {

            if (ImGui::BeginTable("TableForCoefs", 7, flags)) // создаем таблицу с заданными настройками
            {
                if (true)
                {
                    ImGui::TableSetupColumn("i", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("X_i-1", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("X_i", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("a_i", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("b_i", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("c_i", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("d_i", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableHeadersRow();
                }

                for (int row = 0; row < cs.get_n(); row++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", row+1);
                    ImGui::TableSetColumnIndex(1);
                    if (row == 0)
                        ImGui::Text("%lf", cs.get_border().first);
                    else
                        ImGui::Text("%lf", x_of_numerical_solution[row * separate_n]);
                    ImGui::TableSetColumnIndex(2);
                    if (row == cs.get_n()-1)
                        ImGui::Text("%lf", cs.get_border().second);
                    else
                        ImGui::Text("%lf", x_of_numerical_solution[(row+1) * separate_n]);
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%lf", a[row]);
                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("%lf", b[row]);
                    ImGui::TableSetColumnIndex(5);
                    ImGui::Text("%lf", c[row+1]);
                    ImGui::TableSetColumnIndex(6);
                    ImGui::Text("%lf", d[row]);
                }
                ImGui::EndTable();
            }

        }


        ImGui::End(); // Удаляем окно

    }
    void createTableForComparison() {
        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable; // настройки для таблицы

        ImGui::SetNextWindowPos({ 0,629 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 960,395 }); // устанавливаем размер для создаваемого окна для таблицы

        ImGui::Begin("TableForCompariso", 0, flags_for_window); // создаем окно с заданными настройками

        if (isNumericalSolutionSet) {

            if (ImGui::BeginTable("TableForCompariso", 11, flags)) // создаем таблицу с заданными настройками
            {
                if (true)
                {
                    ImGui::TableSetupColumn("j", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("x_j", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("F(x_j)", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("S(x_j)", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("|F(x_j)-S(x_j)|", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("F'(x_j)", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("S'(x_j)", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("|F'(x_j)-S'(x_j)|", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("F''(x_j)", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("S''(x_j)", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("|F''(x_j)-S''(x_j)|", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableHeadersRow();
                }

                for (int row = 0; row <= N; row++)
                {
                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", row);

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", x_of_numerical_solution_with_multiple_n[row * separate_n]);

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%lf", y_of_analytical_solution_for_graph[row]);
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%lf", y_of_numerical_solution_with_multiple_n[row * separate_n]);
                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("%lf", abs(y_of_analytical_solution_for_graph[row] - y_of_numerical_solution_with_multiple_n[row * separate_n]));


                    ImGui::TableSetColumnIndex(5);
                    ImGui::Text("%lf", y_of_analytical_solution_first_dif_for_graph[row]);
                    ImGui::TableSetColumnIndex(6);
                    ImGui::Text("%lf", y_of_numerical_solution_first_dif_with_multiple_n[row * separate_n]);
                    ImGui::TableSetColumnIndex(7);
                    ImGui::Text("%lf", abs(y_of_analytical_solution_first_dif_for_graph[row] - y_of_numerical_solution_first_dif_with_multiple_n[row * separate_n]));

                    ImGui::TableSetColumnIndex(8);
                    ImGui::Text("%lf", y_of_analytical_solution_second_dif_for_graph[row]);
                    ImGui::TableSetColumnIndex(9);
                    ImGui::Text("%lf", y_of_numerical_solution_second_dif_with_multiple_n[row * separate_n]);
                    ImGui::TableSetColumnIndex(10);
                    ImGui::Text("%lf", abs(y_of_analytical_solution_second_dif_for_graph[row] - y_of_numerical_solution_second_dif_with_multiple_n[row * separate_n]));
                }
                ImGui::EndTable();
            }

        }


        ImGui::End(); // Удаляем окно

    }

    void render() {
        ImGui::SFML::Render(m_window);
        m_window.display();
    }

private:
    
    void create_data_for_analytical_solution_for_graph() {

        y_of_analytical_solution_for_graph.clear();
        y_of_analytical_solution_first_dif_for_graph.clear();
        y_of_analytical_solution_second_dif_for_graph.clear();

        y_of_analytical_solution_for_graph.resize(N);
        y_of_analytical_solution_first_dif_for_graph.resize(N);
        y_of_analytical_solution_second_dif_for_graph.resize(N);

        for (size_t count = 0; count <= N; ++count) {
            if (item_current_idx == 0) {

                //std::cout << x_of_numerical_solution_with_multiple_n[count * separate_n] << std::endl;
                
                if (x_of_numerical_solution_with_multiple_n[count * separate_n] >= -1 && x_of_numerical_solution_with_multiple_n[count * separate_n] <= 0) {

                    y_of_analytical_solution_for_graph[count] = x_of_numerical_solution_with_multiple_n[count * separate_n] *
                        x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n] + 3 * x_of_numerical_solution_with_multiple_n[count * separate_n] *
                        x_of_numerical_solution_with_multiple_n[count * separate_n];

                    y_of_analytical_solution_first_dif_for_graph[count] = 3 * x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n]
                        + 6 * x_of_numerical_solution_with_multiple_n[count * separate_n];

                    y_of_analytical_solution_second_dif_for_graph[count] = 6 * x_of_numerical_solution_with_multiple_n[count * separate_n] + 6;

                }
                else if (x_of_numerical_solution_with_multiple_n[count * separate_n] <= 1 && x_of_numerical_solution_with_multiple_n[count * separate_n] > 0) {

                    y_of_analytical_solution_for_graph[count] = -x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n] *
                        x_of_numerical_solution_with_multiple_n[count * separate_n] + 3 * x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n];

                    y_of_analytical_solution_first_dif_for_graph[count] = -3 * x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n]
                        + 6 * x_of_numerical_solution_with_multiple_n[count * separate_n];

                    y_of_analytical_solution_second_dif_for_graph[count] = -6 * x_of_numerical_solution_with_multiple_n[count * separate_n] + 6;

                }
            }
            else if (item_current_idx == 1) {

                y_of_analytical_solution_for_graph[count] = (double)log(x_of_numerical_solution_with_multiple_n[count * separate_n] + 1)
                    / (x_of_numerical_solution_with_multiple_n[count * separate_n]);

                y_of_analytical_solution_first_dif_for_graph[count] = ((double)1 / (x_of_numerical_solution_with_multiple_n[count * separate_n] *
                    x_of_numerical_solution_with_multiple_n[count * separate_n] +
                    x_of_numerical_solution_with_multiple_n[count * separate_n])) - (log(x_of_numerical_solution_with_multiple_n[count * separate_n] + 1) /
                        (x_of_numerical_solution_with_multiple_n[count * separate_n] *
                            x_of_numerical_solution_with_multiple_n[count * separate_n]));

                y_of_analytical_solution_second_dif_for_graph[count] = ((2 * log(x_of_numerical_solution_with_multiple_n[count * separate_n] + 1)) /
                    (x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n] *
                        x_of_numerical_solution_with_multiple_n[count * separate_n])) -
                    ((3 * x_of_numerical_solution_with_multiple_n[count * separate_n] + 2) / (x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n]
                        * x_of_numerical_solution_with_multiple_n[count * separate_n] *
                        x_of_numerical_solution_with_multiple_n[count * separate_n] + 2 * x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n] *
                        x_of_numerical_solution_with_multiple_n[count * separate_n] +
                        x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n]));

            }
            else if (item_current_idx == 2) {

                y_of_analytical_solution_for_graph[count] = (double)log(x_of_numerical_solution_with_multiple_n[count * separate_n] + 1) / (x_of_numerical_solution_with_multiple_n[count * separate_n])
                    + cos(10 * x_of_numerical_solution_with_multiple_n[count * separate_n]);

                y_of_analytical_solution_first_dif_for_graph[count] = ((double)1 / (x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n] +
                    x_of_numerical_solution_with_multiple_n[count * separate_n])) - (log(x_of_numerical_solution_with_multiple_n[count * separate_n] + 1) / (x_of_numerical_solution_with_multiple_n[count * separate_n] *
                        x_of_numerical_solution_with_multiple_n[count * separate_n])) - sin(10 * x_of_numerical_solution_with_multiple_n[count * separate_n]) * 10;

                y_of_analytical_solution_second_dif_for_graph[count] = ((2 * log(x_of_numerical_solution_with_multiple_n[count * separate_n] + 1)) / (x_of_numerical_solution_with_multiple_n[count * separate_n] *
                    x_of_numerical_solution_with_multiple_n[count * separate_n] *
                    x_of_numerical_solution_with_multiple_n[count * separate_n])) -
                    ((3 * x_of_numerical_solution_with_multiple_n[count * separate_n] + 2) / (x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n] *
                        x_of_numerical_solution_with_multiple_n[count * separate_n] + 2 * x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n] +
                        x_of_numerical_solution_with_multiple_n[count * separate_n] * x_of_numerical_solution_with_multiple_n[count * separate_n])) - 100 * cos(10 * x_of_numerical_solution_with_multiple_n[count * separate_n]);
            
            }
        }

    }

    // Аналитическое решение для тестовой задачи
    void create_x_coords_for_analytical_solution() {

        if (!isAnalyticalSolutionSet) {

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

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

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution.clear();

            y_of_analytical_solution.resize(size_of_analytical_solution + 1);

            for (size_t count = 0; count < size_of_analytical_solution / 2; ++count)
                y_of_analytical_solution[count] = x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] + 3 * x_of_analytical_solution[count] * x_of_analytical_solution[count];

            for (size_t count = size_of_analytical_solution / 2; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution[count] = -x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] + 3 * x_of_analytical_solution[count] * x_of_analytical_solution[count];

            isAnalyticalSolutionSet = true;

        }
    }
    void create_data_for_analytical_solution_first_dif() {

        if (!isAnalyticalSolutionFirstDifSet) {

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution_first_dif.clear();

            y_of_analytical_solution_first_dif.resize(size_of_analytical_solution + 1);

            for (size_t count = 0; count < size_of_analytical_solution / 2; ++count)
                y_of_analytical_solution_first_dif[count] = 3 * x_of_analytical_solution[count] * x_of_analytical_solution[count] + 6 * x_of_analytical_solution[count];

            for (size_t count = size_of_analytical_solution / 2; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution_first_dif[count] = -3 * x_of_analytical_solution[count] * x_of_analytical_solution[count] + 6 * x_of_analytical_solution[count];

            isAnalyticalSolutionFirstDifSet = true;

        }
    }
    void create_data_for_analytical_solution_second_dif() {

        if (!isAnalyticalSolutionSecondDifSet) {

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution_second_dif.clear();

            y_of_analytical_solution_second_dif.resize(size_of_analytical_solution + 1);

            for (size_t count = 0; count < size_of_analytical_solution / 2; ++count)
                y_of_analytical_solution_second_dif[count] = 6 * x_of_analytical_solution[count] + 6;

            for (size_t count = size_of_analytical_solution / 2; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution_second_dif[count] = -6 * x_of_analytical_solution[count] + 6;

            isAnalyticalSolutionSecondDifSet = true;

        }
    }
    /////////
    
    // Аналитическое решение для первой задачи
    void create_x_coords_for_analytical_solution_for_first_task() {
        if (!isAnalyticalSolutionSet) {

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

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

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution.clear();

            y_of_analytical_solution.resize(size_of_analytical_solution + 1);

            for (size_t count = 0; count <= size_of_analytical_solution; ++count) {
                y_of_analytical_solution[count] = (double)log(x_of_analytical_solution[count] + 1) / (x_of_analytical_solution[count]);
            }

            isAnalyticalSolutionSet = true;

        }
    }
    void create_data_for_analytical_solution_for_first_task_first_dif() {

        if (!isAnalyticalSolutionFirstDifSet) {

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution_first_dif.clear();

            y_of_analytical_solution_first_dif.resize(size_of_analytical_solution + 1);

            for (size_t count = 0; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution_first_dif[count] = ((double)1 / (x_of_analytical_solution[count] * x_of_analytical_solution[count] +
                    x_of_analytical_solution[count])) - (log(x_of_analytical_solution[count] + 1) / (x_of_analytical_solution[count] *
                        x_of_analytical_solution[count]));

            isAnalyticalSolutionFirstDifSet = true;

        }
    }
    void create_data_for_analytical_solution_for_first_task_second_dif() {

        if (!isAnalyticalSolutionSecondDifSet) {

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution_second_dif.clear();

            y_of_analytical_solution_second_dif.resize(size_of_analytical_solution + 1);

            for (size_t count = 0; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution_second_dif[count] = ((2 * log(x_of_analytical_solution[count] + 1)) / (x_of_analytical_solution[count] * x_of_analytical_solution[count] *
                    x_of_analytical_solution[count])) -
                (( 3* x_of_analytical_solution[count] + 2) / (x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] * 
                    x_of_analytical_solution[count] + 2 * x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] +
                    x_of_analytical_solution[count] * x_of_analytical_solution[count]));

            isAnalyticalSolutionSecondDifSet = true;

        }
    }
    /////////

    // Аналитическое решение для второй задачи
    void create_x_coords_for_analytical_solution_for_second_task() {
        if (!isAnalyticalSolutionSet) {

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

            x_of_analytical_solution.clear();

            x_of_analytical_solution.resize(size_of_analytical_solution + 1);

            double h_for_analytical_solution = abs(cs.get_border().second - cs.get_border().first) / size_of_analytical_solution;

            for (size_t count = 0; count <= size_of_analytical_solution; ++count) {
                x_of_analytical_solution[count] = cs.get_border().first + h_for_analytical_solution * count;
            }

        }
    }
    void create_data_for_analytical_solution_for_second_task() {

        if (!isAnalyticalSolutionSet) {

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution.clear();

            y_of_analytical_solution.resize(size_of_analytical_solution + 1);

            for (size_t count = 0; count <= size_of_analytical_solution; ++count) {
                y_of_analytical_solution[count] = (double)log(x_of_analytical_solution[count] + 1) / (x_of_analytical_solution[count])
                    + cos(10 * x_of_analytical_solution[count]);
            }

            isAnalyticalSolutionSet = true;

        }
    }
    void create_data_for_analytical_solution_for_second_task_first_dif() {

        if (!isAnalyticalSolutionFirstDifSet) {

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution_first_dif.clear();

            y_of_analytical_solution_first_dif.resize(size_of_analytical_solution + 1);

            for (size_t count = 0; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution_first_dif[count] = ((double)1 / (x_of_analytical_solution[count] * x_of_analytical_solution[count] +
                    x_of_analytical_solution[count])) - (log(x_of_analytical_solution[count] + 1) / (x_of_analytical_solution[count] *
                        x_of_analytical_solution[count])) - sin(10 * x_of_analytical_solution[count]) * 10;

            isAnalyticalSolutionFirstDifSet = true;

        }
    }
    void create_data_for_analytical_solution_for_second_task_second_dif() {

        if (!isAnalyticalSolutionSecondDifSet) {

            const static size_t size_of_analytical_solution = 1000; // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution_second_dif.clear();

            y_of_analytical_solution_second_dif.resize(size_of_analytical_solution + 1);

            for (size_t count = 0; count <= size_of_analytical_solution; ++count)
                y_of_analytical_solution_second_dif[count] = ((2 * log(x_of_analytical_solution[count] + 1)) / (x_of_analytical_solution[count] * x_of_analytical_solution[count] *
                    x_of_analytical_solution[count])) -
                ((3 * x_of_analytical_solution[count] + 2) / (x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] *
                    x_of_analytical_solution[count] + 2 * x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] +
                    x_of_analytical_solution[count] * x_of_analytical_solution[count])) - 100 * cos(10 * x_of_analytical_solution[count]);

            isAnalyticalSolutionSecondDifSet = true;

        }
    }
    /////////

    // Численное решение для тестовой задачи
    void get_data_for_test_task() {

        if (!isNumericalSolutionSet) {

            cs.set_phi_for_test();
            cs.set_c();

            cs.set_a(item_current_idx);
            cs.set_b(item_current_idx);
            cs.set_d();

            create_x_coords_for_numerical_solution();
            create_data_for_numerical_solution();
            create_data_for_numerical_solution_first_dif();
            create_data_for_numerical_solution_second_dif();

            a.clear();
            a.resize(cs.get_n());
            a = cs.get_a();
            b.clear();
            b.resize(cs.get_n());
            b = cs.get_b();
            c.clear();
            c.resize(cs.get_n()+1);
            c = cs.get_c();
            d.clear();
            d.resize(cs.get_n());
            d = cs.get_d();

            int n = cs.get_n();

            cs.set_n(N);
            cs.set_h();

            cs.set_phi_for_test();
            cs.set_c();

            cs.set_a(item_current_idx);
            cs.set_b(item_current_idx);
            cs.set_d();

            create_x_coords_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_first_dif_with_multiple_n();
            create_data_for_numerical_solution_second_dif_with_multiple_n();
            create_data_for_analytical_solution_for_graph();

            cs.set_n(n);
            cs.set_h();

            isNumericalSolutionSet = true;
        }
    }
    /////////

    // Численное решение для первой основной задачи
    void get_data_for_first_task() {

        if (!isNumericalSolutionSet) {

            cs.set_phi_for_first_task();
            cs.set_c();

            cs.set_a(item_current_idx);
            cs.set_b(item_current_idx);
            cs.set_d();

            create_x_coords_for_numerical_solution();
            create_data_for_numerical_solution();
            create_data_for_numerical_solution_first_dif();
            create_data_for_numerical_solution_second_dif();

            a.clear();
            a.resize(cs.get_n());
            a = cs.get_a();
            b.clear();
            b.resize(cs.get_n());
            b = cs.get_b();
            c.clear();
            c.resize(cs.get_n() + 1);
            c = cs.get_c();
            d.clear();
            d.resize(cs.get_n());
            d = cs.get_d();

            int n = cs.get_n();

            cs.set_n(N);
            cs.set_h();

            cs.set_phi_for_test();
            cs.set_c();

            cs.set_a(item_current_idx);
            cs.set_b(item_current_idx);
            cs.set_d();

            create_x_coords_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_first_dif_with_multiple_n();
            create_data_for_numerical_solution_second_dif_with_multiple_n();
            create_data_for_analytical_solution_for_graph();

            cs.set_n(n);
            cs.set_h();

            isNumericalSolutionSet = true;
        }
    }
    /////////

    // Численное решение для второй основной задачи
    void get_data_for_second_task() {

        if (!isNumericalSolutionSet) {

            cs.set_phi_for_second_task();
            cs.set_c();

            cs.set_a(item_current_idx);
            cs.set_b(item_current_idx);
            cs.set_d();

            create_x_coords_for_numerical_solution();
            create_data_for_numerical_solution();
            create_data_for_numerical_solution_first_dif();
            create_data_for_numerical_solution_second_dif();

            a.clear();
            a.resize(cs.get_n());
            a = cs.get_a();
            b.clear();
            b.resize(cs.get_n());
            b = cs.get_b();
            c.clear();
            c.resize(cs.get_n() + 1);
            c = cs.get_c();
            d.clear();
            d.resize(cs.get_n());
            d = cs.get_d();

            int n = cs.get_n();

            cs.set_n(N);
            cs.set_h();

            cs.set_phi_for_test();
            cs.set_c();

            cs.set_a(item_current_idx);
            cs.set_b(item_current_idx);
            cs.set_d();

            create_x_coords_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_first_dif_with_multiple_n();
            create_data_for_numerical_solution_second_dif_with_multiple_n();
            create_data_for_analytical_solution_for_graph();

            cs.set_n(n);
            cs.set_h();

            isNumericalSolutionSet = true;
        }
    }
    /////////

    // Численное решение для любой задачи с n
    void create_x_coords_for_numerical_solution() {

        x_of_numerical_solution.clear();

        x_of_numerical_solution.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double h = (double)(abs((cs.get_border().first + count * (abs(cs.get_border().second -
                cs.get_border().first) / cs.get_n())) -
                (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first)
                    / cs.get_n()))) / separate_n); // шаг в интервале для S_i(x)

            for (size_t i = 0; i <= separate_n; ++i)
                x_of_numerical_solution[count * separate_n + i] = (cs.get_border().first + count * 
                    (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()) + i * h);

        }
    }
    void create_data_for_numerical_solution() {

        y_of_numerical_solution.clear();

        y_of_numerical_solution.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            for (size_t i = 0; i <= separate_n; ++i) {
                y_of_numerical_solution[count * separate_n + i] = cs.get_a()[count] + cs.get_b()[count] *
                    (x_of_numerical_solution[count * separate_n + i] - x_current) + cs.get_c()[count + 1] / 2 *
                    (x_of_numerical_solution[count * separate_n + i] - x_current) * 
                    (x_of_numerical_solution[count * separate_n + i] - x_current) +
                    cs.get_d()[count] / 6 * (x_of_numerical_solution[count * separate_n + i] - x_current) * 
                    (x_of_numerical_solution[count * separate_n + i] - x_current) *
                    (x_of_numerical_solution[count * separate_n + i] - x_current);
            }
        }
    }
    void create_data_for_numerical_solution_first_dif() {

        y_of_numerical_solution_first_dif.clear();

        y_of_numerical_solution_first_dif.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            for (size_t i = 0; i <= separate_n; ++i)
                y_of_numerical_solution_first_dif[count * separate_n + i] = cs.get_b()[count] + 
                cs.get_c()[count + 1] * (x_of_numerical_solution[count * separate_n + i] - x_current) +
                cs.get_d()[count] / 2 * (x_of_numerical_solution[count * separate_n + i] - x_current) * 
                (x_of_numerical_solution[count * separate_n + i] - x_current);
        }
    }
    void create_data_for_numerical_solution_second_dif() {

        y_of_numerical_solution_second_dif.clear();

        y_of_numerical_solution_second_dif.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            for (size_t i = 0; i <= separate_n; ++i)
                y_of_numerical_solution_second_dif[count * separate_n + i] = cs.get_c()[count + 1] +
                cs.get_d()[count] * (x_of_numerical_solution[count * separate_n + i] - x_current);
        }
    }
    /////////

    // Численное решение для любой задачи с N
    void create_x_coords_for_numerical_solution_with_multiple_n() {

        x_of_numerical_solution_with_multiple_n.clear();

        x_of_numerical_solution_with_multiple_n.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double h = (double)(abs((cs.get_border().first + count * (abs(cs.get_border().second -
                cs.get_border().first) / cs.get_n())) -
                (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first)
                    / cs.get_n()))) / separate_n); // шаг в интервале для S_i(x)

            for (size_t i = 0; i <= separate_n; ++i)
                x_of_numerical_solution_with_multiple_n[count * separate_n + i] = (cs.get_border().first + count *
                    (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()) + i * h);

        }
    }
    void create_data_for_numerical_solution_with_multiple_n() {

        y_of_numerical_solution_with_multiple_n.clear();

        y_of_numerical_solution_with_multiple_n.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            for (size_t i = 0; i <= separate_n; ++i) {
                y_of_numerical_solution_with_multiple_n[count * separate_n + i] = cs.get_a()[count] + cs.get_b()[count] *
                    (x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current) + cs.get_c()[count + 1] / 2 *
                    (x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current) *
                    (x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current) +
                    cs.get_d()[count] / 6 * (x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current) *
                    (x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current) *
                    (x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current);
            }
        }

        //std::cout << x_of_numerical_solution_with_multiple_n.size() << std::endl;
        //
        //for (size_t count = 0; count < x_of_numerical_solution_with_multiple_n.size(); ++count) {
        //    std::cout << x_of_numerical_solution_with_multiple_n[count] << std::endl;
        //}
        //for (size_t count = 0; count < y_of_numerical_solution_with_multiple_n.size(); ++count) {
        //    std::cout << y_of_numerical_solution_with_multiple_n[count] << std::endl;
        //    std::cout << count << std::endl;
        //}
    }
    void create_data_for_numerical_solution_first_dif_with_multiple_n() {

        y_of_numerical_solution_first_dif_with_multiple_n.clear();

        y_of_numerical_solution_first_dif_with_multiple_n.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            for (size_t i = 0; i <= separate_n; ++i)
                y_of_numerical_solution_first_dif_with_multiple_n[count * separate_n + i] = cs.get_b()[count] +
                cs.get_c()[count + 1] * (x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current) +
                cs.get_d()[count] / 2 * (x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current) *
                (x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current);
        }
    }
    void create_data_for_numerical_solution_second_dif_with_multiple_n() {

        y_of_numerical_solution_second_dif_with_multiple_n.clear();

        y_of_numerical_solution_second_dif_with_multiple_n.resize(cs.get_n() * separate_n + 1);

        //std::cout << cs.get_n() << std::endl;

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            for (size_t i = 0; i <= separate_n; ++i)
                y_of_numerical_solution_second_dif_with_multiple_n[count * separate_n + i] = cs.get_c()[count + 1] +
                cs.get_d()[count] * (x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current);
        }
    }
    /////////

};