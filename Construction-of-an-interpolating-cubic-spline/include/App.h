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

    bool isShowZeroDifDif = false;
    bool isShowFirstDifDif = false;
    bool isShowSecondDifDif = false;

    int separate_n = 10; // число разбиений для S_i(x)
    int item_current_idx = 0;
    int N = 20;

    double max = 0;
    int index_max = -1;

    double max_first = 0;
    int index_max_first = -1;

    double max_second = 0;
    int index_max_second = -1;

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

    std::vector<double> zero_dif;
    std::vector<double> first_dif;
    std::vector<double> second_dif;

    std::vector<double> y_max;
    std::vector<double> y_max_first;
    std::vector<double> y_max_second;

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
                //std::cout << "ASDA" << std::endl;
                create_data_for_analytical_solution();
                //std::cout << "ASDA" << std::endl;
                create_data_for_analytical_solution_first_dif();
                //std::cout << "ASDA" << std::endl; 
                create_data_for_analytical_solution_second_dif();
                //std::cout << "ASDA" << std::endl;
                create_data_for_difference_of_solutions();
            }
            else if (item_current_idx == 1) {

                cs.set_border(2, 4);

                get_data_for_first_task();

                create_x_coords_for_analytical_solution();
                create_data_for_analytical_solution();
                create_data_for_analytical_solution_first_dif();
                create_data_for_analytical_solution_second_dif();
                create_data_for_difference_of_solutions();
            }
            else if (item_current_idx == 2) {

                cs.set_border(2, 4);

                get_data_for_second_task();

                create_x_coords_for_analytical_solution();
                create_data_for_analytical_solution();
                create_data_for_analytical_solution_first_dif();
                create_data_for_analytical_solution_second_dif();
                create_data_for_difference_of_solutions();
            }

            //std::cout << "ASDA" << std::endl;

            createWindowForInputN();
            createWindowForSettings();
            createWindowForSelectingTasks();
            createWindowForInfo();

            createGraph();
            createTableForCoefs();
            createTableForComparison();

            ImGui::End();

            m_window.clear({ 255,255,255,100 });
            render();
        }

        //std::cout << "HERE" << std::endl;
    }

    void create_data_for_difference_of_solutions() {

        zero_dif.clear();
        first_dif.clear();
        second_dif.clear();

        zero_dif.resize(x_of_numerical_solution.size());
        first_dif.resize(x_of_numerical_solution.size());
        second_dif.resize(x_of_numerical_solution.size());

        //std::cout << "y_of_analytical_solution: " << y_of_analytical_solution.size() << std::endl;
        //std::cout << "y_of_analytical_solution_first_dif: " << y_of_analytical_solution_first_dif.size() << std::endl;
        //std::cout << "y_of_analytical_solution_second_dif: " << y_of_analytical_solution_second_dif.size() << std::endl;

        //std::cout << x_of_numerical_solution.size() << std::endl;

        for (size_t count = 0; count < x_of_numerical_solution.size(); ++count) {
            zero_dif[count] = y_of_analytical_solution[count] - y_of_numerical_solution[count];
            first_dif[count] = y_of_analytical_solution_first_dif[count] - y_of_numerical_solution_first_dif[count];
            second_dif[count] = y_of_analytical_solution_second_dif[count] - y_of_numerical_solution_second_dif[count];

            //std::cout << count << std::endl;
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
        ImGui::Checkbox(u8"Отображать F(x) - S(x)", &isShowZeroDifDif);
        ImGui::Checkbox(u8"Отображать F'(x) - S'(x)", &isShowFirstDifDif);
        ImGui::Checkbox(u8"Отображать F''(x) - S''(x)", &isShowSecondDifDif);

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
            isAnalyticalSolutionSet = false;
            isAnalyticalSolutionFirstDifSet = false;
            isAnalyticalSolutionSecondDifSet = false;
        }

        ImGui::End();

    }
    void createWindowForInfo() {

        ImGui::SetNextWindowPos({ 321,0 });
        ImGui::SetNextWindowSize({ 639,232 });

        ImGui::Begin("Info", 0, flags_for_window);

        if (isNumericalSolutionSet) {

            ImGui::Text(u8"Справка:");
            ImGui::Text(u8"Сетка сплайна: n = %d", cs.get_n());
            ImGui::Text(u8"Конрольная сетка: N = %d", N);
            ImGui::Text(u8"Погрешность сплайна на контрольной сетке:");
            ImGui::Text(u8"max|F(x_j)-S(x_j)| = %.12lf \tпри x = %lf", max, x_of_numerical_solution_with_multiple_n[index_max]);
            ImGui::Text(u8"Погрешность производной на контрольной сетке:");
            ImGui::Text(u8"max|F'(x_j)-S'(x_j)| = %.12lf \tпри x = %lf", max_first, x_of_numerical_solution_with_multiple_n[index_max_first]);
            ImGui::Text(u8"Погрешность второй производной на контрольной сетке (опция):");
            ImGui::Text(u8"max|F''(x_j)-S''(x_j)| = %.12lf \tпри x = %lf", max_second, x_of_numerical_solution_with_multiple_n[index_max_second]);
        }

        ImGui::End;
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
            
            // Разность решений
            if (isShowZeroDifDif)
                ImPlot::PlotLine("F(x) - S(x)", x_of_numerical_solution.data(), zero_dif.data(), x_of_numerical_solution.size()); // Отрисовываем линию
            if (isShowFirstDifDif)
                ImPlot::PlotLine("F'(x) - S'(x)", x_of_numerical_solution.data(), first_dif.data(), x_of_numerical_solution.size()); // Отрисовываем линию
            if (isShowSecondDifDif)
                ImPlot::PlotLine("F''(x) - S''(x)", x_of_numerical_solution.data(), second_dif.data(), x_of_numerical_solution.size()); // Отрисовываем линию

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
                    ImGui::Text("%lf", cs.get_a()[row]);
                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("%lf", cs.get_b()[row]);
                    ImGui::TableSetColumnIndex(5);
                    ImGui::Text("%lf", cs.get_c()[row+1]);
                    ImGui::TableSetColumnIndex(6);
                    ImGui::Text("%lf", cs.get_d()[row]);
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

        //std::cout << x_of_numerical_solution_with_multiple_n.size() / separate_n << std::endl;

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

                //std::cout << "N: " << N << std::endl;
                
                for (int row = 0; row <= N; row++)
                {
                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", row);

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lf", x_of_numerical_solution_with_multiple_n[row * separate_n]);

                    //std::cout << "1" << std::endl;
                    //
                    //std::cout << "x_of_numerical_solution_with_multiple_n: " << x_of_numerical_solution_with_multiple_n.size() << std::endl;
                    //std::cout << "y_of_analytical_solution_for_graph: " << y_of_analytical_solution_for_graph.size() << std::endl;
                    //std::cout << "y_of_numerical_solution_with_multiple_n: " << y_of_numerical_solution_with_multiple_n.size() << std::endl;
                    //std::cout << "y_max: " << y_max.size() << std::endl;
                    //
                    //std::cout << y_of_analytical_solution_for_graph[y_of_analytical_solution_for_graph.size() - 1] << std::endl;

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%lf", y_of_analytical_solution_for_graph[row * separate_n]);
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%lf", y_of_numerical_solution_with_multiple_n[row * separate_n]);
                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("%.12lf", y_max[row]);

                    //std::cout << "2" << std::endl;

                    ImGui::TableSetColumnIndex(5);
                    ImGui::Text("%lf", y_of_analytical_solution_first_dif_for_graph[row * separate_n]);
                    ImGui::TableSetColumnIndex(6);
                    ImGui::Text("%lf", y_of_numerical_solution_first_dif_with_multiple_n[row * separate_n]);
                    ImGui::TableSetColumnIndex(7);
                    ImGui::Text("%.12lf", y_max_first[row]);

                    //std::cout << "3" << std::endl;

                    ImGui::TableSetColumnIndex(8);
                    ImGui::Text("%lf", y_of_analytical_solution_second_dif_for_graph[row * separate_n]);
                    ImGui::TableSetColumnIndex(9);
                    ImGui::Text("%lf", y_of_numerical_solution_second_dif_with_multiple_n[row * separate_n]);
                    ImGui::TableSetColumnIndex(10);
                    ImGui::Text("%.12lf", y_max_second[row]);

                    //std::cout << "4" << std::endl;
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

        //std::cout << "x_of_numerical_solution_with_multiple_n.size(): " << x_of_numerical_solution_with_multiple_n.size() << std::endl;

        y_of_analytical_solution_for_graph.resize(x_of_numerical_solution_with_multiple_n.size());
        y_of_analytical_solution_first_dif_for_graph.resize(x_of_numerical_solution_with_multiple_n.size());
        y_of_analytical_solution_second_dif_for_graph.resize(x_of_numerical_solution_with_multiple_n.size());

        for (size_t count = 0; count < x_of_numerical_solution_with_multiple_n.size(); ++count) {
            
            double x = x_of_numerical_solution_with_multiple_n[count];

            if (item_current_idx == 0) {

                //std::cout << x_of_numerical_solution_with_multiple_n[count * separate_n] << std::endl;
                
                if (x_of_numerical_solution_with_multiple_n[count] >= -1 && x_of_numerical_solution_with_multiple_n[count] <= 0) {

                    y_of_analytical_solution_for_graph[count] = x * x * x + 3 * x * x;

                    y_of_analytical_solution_first_dif_for_graph[count] = 3 * x * x + 6 * x;

                    y_of_analytical_solution_second_dif_for_graph[count] = 6 * x + 6;

                    //std::cout << "x: " << x << "\ty_of_analytical_solution_for_graph[count]: " << y_of_analytical_solution_for_graph[count] << std::endl;

                }
                else {

                    y_of_analytical_solution_for_graph[count] = -x * x * x + 3 * x * x;

                    y_of_analytical_solution_first_dif_for_graph[count] = -3 * x * x + 6 * x;

                    y_of_analytical_solution_second_dif_for_graph[count] = -6 * x + 6;

                    //std::cout << "x: " << x << "\ty_of_analytical_solution_for_graph[count]: " << y_of_analytical_solution_for_graph[count] << std::endl;

                }
            }
            else if (item_current_idx == 1) {

                y_of_analytical_solution_for_graph[count] = (double)log(x + 1) / x;

                y_of_analytical_solution_first_dif_for_graph[count] = ((double)1 / (x * x + x)) - (log(x + 1) / (x * x));

                y_of_analytical_solution_second_dif_for_graph[count] = ((2 * log(x + 1)) / (x * x * x)) - ((3 * x + 2) / (x * x * x * x + 2 * x * x * x + x * x));

            }
            else if (item_current_idx == 2) {

                y_of_analytical_solution_for_graph[count] = (double)log(x + 1) / x + cos(10 * x);

                y_of_analytical_solution_first_dif_for_graph[count] = ((double)1 / (x * x + x)) - (log(x + 1) / (x * x)) - 10 * sin(x);

                y_of_analytical_solution_second_dif_for_graph[count] = ((2 * log(x + 1)) / (x * x * x)) - ((3 * x + 2) / (x * x * x * x + 2 * x * x * x + x * x)) - 100 * cos(10 * x);

            }
        }

    }
    void create_data_for_coefs() {
        
        if (item_current_idx == 0)
            cs.set_phi_for_test();
        else if (item_current_idx == 1)
            cs.set_phi_for_first_task();
        else if (item_current_idx == 2)
            cs.set_phi_for_second_task();

        cs.set_c();
        cs.set_a(item_current_idx);
        cs.set_b(item_current_idx);
        cs.set_d();
    }
    void find_max() {

        y_max.clear();
        y_max.resize(N+1, 0);
        y_max_first.clear();
        y_max_first.resize(N+1, 0);
        y_max_second.clear();
        y_max_second.resize(N+1, 0);

        max = 0;
        index_max = -1;

        max_first = 0;
        index_max_first = -1;

        max_second = 0;
        index_max_second = -1;

        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j <= separate_n; ++j) {

                if (abs(y_of_analytical_solution_for_graph[i * separate_n + j] - y_of_numerical_solution_with_multiple_n[i * separate_n + j]) >= y_max[i])
                    y_max[i] = abs(y_of_analytical_solution_for_graph[i * separate_n + j] - y_of_numerical_solution_with_multiple_n[i * separate_n + j]);

                if (abs(y_of_analytical_solution_first_dif_for_graph[i * separate_n + j] - y_of_numerical_solution_first_dif_with_multiple_n[i * separate_n + j]) >= y_max_first[i])
                    y_max_first[i] = abs(y_of_analytical_solution_first_dif_for_graph[i * separate_n + j] - y_of_numerical_solution_first_dif_with_multiple_n[i * separate_n + j]);

                if (abs(y_of_analytical_solution_second_dif_for_graph[i * separate_n + j] - y_of_numerical_solution_second_dif_with_multiple_n[i * separate_n + j]) >= y_max_second[i])
                    y_max_second[i] = abs(y_of_analytical_solution_second_dif_for_graph[i * separate_n + j] - y_of_numerical_solution_second_dif_with_multiple_n[i * separate_n + j]);

                if (abs(y_of_analytical_solution_for_graph[i*separate_n + j] - y_of_numerical_solution_with_multiple_n[i * separate_n + j]) >= max) {
                    index_max = i * separate_n + j;
                    max = abs(y_of_analytical_solution_for_graph[i * separate_n + j] - y_of_numerical_solution_with_multiple_n[i * separate_n + j]);
                }
                if (abs(y_of_analytical_solution_first_dif_for_graph[i * separate_n + j] - y_of_numerical_solution_first_dif_with_multiple_n[i * separate_n + j]) >= max_first) {
                    index_max_first = i * separate_n + j;
                    max_first = abs(y_of_analytical_solution_first_dif_for_graph[i * separate_n + j] - y_of_numerical_solution_first_dif_with_multiple_n[i * separate_n + j]);
                }
                if (abs(y_of_analytical_solution_second_dif_for_graph[i * separate_n + j] - y_of_numerical_solution_second_dif_with_multiple_n[i * separate_n + j]) >= max_second) {
                    index_max_second = i * separate_n + j;
                    max_second = abs(y_of_analytical_solution_second_dif_for_graph[i * separate_n + j] - y_of_numerical_solution_second_dif_with_multiple_n[i * separate_n + j]);
                }
            }
        }
    }

    // Аналитическое решение
    void create_x_coords_for_analytical_solution() {

        if (!isAnalyticalSolutionSet) {

            size_t size_of_analytical_solution = x_of_numerical_solution.size(); // Получаем размер вектора точек для аналитического решения

            //std::cout << "const static size_t size_of_analytical_solution = x_of_numerical_solution.size(): " << x_of_numerical_solution.size() << std::endl;
            
            x_of_analytical_solution.clear();

            x_of_analytical_solution.resize(size_of_analytical_solution);

            double h_for_analytical_solution = abs(cs.get_border().second - cs.get_border().first) / (size_of_analytical_solution - 1);

            for (size_t count = 0; count < size_of_analytical_solution; ++count)
                x_of_analytical_solution[count] = cs.get_border().first + h_for_analytical_solution * count;

            //std::cout << x_of_analytical_solution.back() << std::endl;
        }

    }
    void create_data_for_analytical_solution() {

        if (!isAnalyticalSolutionSet) {

            size_t size_of_analytical_solution = x_of_numerical_solution.size(); // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution.clear();

            y_of_analytical_solution.resize(size_of_analytical_solution);

            if (item_current_idx == 0) {

                for (size_t count = 0; count < (size_of_analytical_solution - 1) / 2; ++count) {
                    double x = x_of_analytical_solution[count];
                    y_of_analytical_solution[count] = x * x * x + 3 * x * x;
                }

                for (size_t count = (size_of_analytical_solution - 1) / 2; count < size_of_analytical_solution; ++count) {
                    double x = x_of_analytical_solution[count];
                    y_of_analytical_solution[count] = -x * x * x + 3 * x * x;
                }

            }
            else if (item_current_idx == 1) {

                for (size_t count = 0; count < size_of_analytical_solution; ++count) {
                    y_of_analytical_solution[count] = (double)log(x_of_analytical_solution[count] + 1) / (x_of_analytical_solution[count]);
                }

            }
            else if (item_current_idx == 2) {

                for (size_t count = 0; count < size_of_analytical_solution; ++count) {
                    y_of_analytical_solution[count] = (double)log(x_of_analytical_solution[count] + 1) / (x_of_analytical_solution[count])
                        + cos(10 * x_of_analytical_solution[count]);
                }

            }

            isAnalyticalSolutionSet = true;

        }
    }
    void create_data_for_analytical_solution_first_dif() {

        if (!isAnalyticalSolutionFirstDifSet) {

            size_t size_of_analytical_solution = x_of_numerical_solution.size(); // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution_first_dif.clear();

            y_of_analytical_solution_first_dif.resize(size_of_analytical_solution);

            if (item_current_idx == 0) {

                for (size_t count = 0; count < (size_of_analytical_solution - 1) / 2; ++count) {
                    double x = x_of_analytical_solution[count];
                    y_of_analytical_solution_first_dif[count] = 3 * x * x + 6 * x;
                }

                for (size_t count = (size_of_analytical_solution - 1) / 2; count < size_of_analytical_solution; ++count) {
                    double x = x_of_analytical_solution[count];
                    y_of_analytical_solution_first_dif[count] = -3 * x * x + 6 * x;
                }

            }
            else if (item_current_idx == 1) {
                for (size_t count = 0; count < size_of_analytical_solution; ++count)
                    y_of_analytical_solution_first_dif[count] = ((double)1 / (x_of_analytical_solution[count] * x_of_analytical_solution[count] +
                        x_of_analytical_solution[count])) - (log(x_of_analytical_solution[count] + 1) / (x_of_analytical_solution[count] *
                            x_of_analytical_solution[count]));

            }
            else if (item_current_idx == 2) {
                for (size_t count = 0; count < size_of_analytical_solution; ++count)
                    y_of_analytical_solution_first_dif[count] = ((double)1 / (x_of_analytical_solution[count] * x_of_analytical_solution[count] +
                        x_of_analytical_solution[count])) - (log(x_of_analytical_solution[count] + 1) / (x_of_analytical_solution[count] *
                            x_of_analytical_solution[count])) - sin(10 * x_of_analytical_solution[count]) * 10;

            }

            isAnalyticalSolutionFirstDifSet = true;

        }
    }
    void create_data_for_analytical_solution_second_dif() {

        if (!isAnalyticalSolutionSecondDifSet) {

            size_t size_of_analytical_solution = x_of_numerical_solution.size(); // Получаем размер вектора точек для аналитического решения

            y_of_analytical_solution_second_dif.clear();

            y_of_analytical_solution_second_dif.resize(size_of_analytical_solution);

            if (item_current_idx == 0) {

                for (size_t count = 0; count < (size_of_analytical_solution - 1) / 2; ++count) {
                    double x = x_of_analytical_solution[count];
                    y_of_analytical_solution_second_dif[count] = 6 * x + 6;
                }

                for (size_t count = (size_of_analytical_solution - 1) / 2; count < size_of_analytical_solution; ++count) {
                    double x = x_of_analytical_solution[count];
                    y_of_analytical_solution_second_dif[count] = -6 * x + 6;
                }

            }
            else if (item_current_idx == 1) {
                for (size_t count = 0; count < size_of_analytical_solution; ++count)
                    y_of_analytical_solution_second_dif[count] = ((2 * log(x_of_analytical_solution[count] + 1)) / (x_of_analytical_solution[count] * x_of_analytical_solution[count] *
                        x_of_analytical_solution[count])) -
                    ((3 * x_of_analytical_solution[count] + 2) / (x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] *
                        x_of_analytical_solution[count] + 2 * x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] +
                        x_of_analytical_solution[count] * x_of_analytical_solution[count]));
            }
            else if (item_current_idx == 2) {
                for (size_t count = 0; count < size_of_analytical_solution; ++count)
                    y_of_analytical_solution_second_dif[count] = ((2 * log(x_of_analytical_solution[count] + 1)) / (x_of_analytical_solution[count] * x_of_analytical_solution[count] *
                        x_of_analytical_solution[count])) -
                    ((3 * x_of_analytical_solution[count] + 2) / (x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] *
                        x_of_analytical_solution[count] + 2 * x_of_analytical_solution[count] * x_of_analytical_solution[count] * x_of_analytical_solution[count] +
                        x_of_analytical_solution[count] * x_of_analytical_solution[count])) - 100 * cos(10 * x_of_analytical_solution[count]);

            }


            isAnalyticalSolutionSecondDifSet = true;

        }
    }
    /////////

    // Численное решение для тестовой задачи
    void get_data_for_test_task() {

        if (!isNumericalSolutionSet) {

            int n = cs.get_n();

            cs.set_n(N);
            cs.set_h();

            create_data_for_coefs();

            create_x_coords_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_first_dif_with_multiple_n();
            create_data_for_numerical_solution_second_dif_with_multiple_n();
            create_data_for_analytical_solution_for_graph();

            find_max();

            cs.set_n(n);
            cs.set_h();

            create_data_for_coefs();

            //std::cout << "PHI:::" << std::endl;

            //cs.print_phi();

            //std::cout << "=============" << std::endl;

            create_x_coords_for_numerical_solution();
            //std::cout << "after create_x_coords_for_numerical_solution" << std::endl;
            create_data_for_numerical_solution();
            create_data_for_numerical_solution_first_dif();
            create_data_for_numerical_solution_second_dif();

            isNumericalSolutionSet = true;
        }
    }
    /////////

    // Численное решение для первой основной задачи
    void get_data_for_first_task() {

        if (!isNumericalSolutionSet) {

            int n = cs.get_n();

            cs.set_n(N);
            cs.set_h();

            create_data_for_coefs();

            create_x_coords_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_first_dif_with_multiple_n();
            create_data_for_numerical_solution_second_dif_with_multiple_n();
            create_data_for_analytical_solution_for_graph();

            find_max();

            cs.set_n(n);
            cs.set_h();

            create_data_for_coefs();

            create_x_coords_for_numerical_solution();
            create_data_for_numerical_solution();
            create_data_for_numerical_solution_first_dif();
            create_data_for_numerical_solution_second_dif();

            isNumericalSolutionSet = true;
        }
    }
    /////////

    // Численное решение для второй основной задачи
    void get_data_for_second_task() {

        if (!isNumericalSolutionSet) {

            int n = cs.get_n();

            cs.set_n(N);
            cs.set_h();

            create_data_for_coefs();

            create_x_coords_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_with_multiple_n();
            create_data_for_numerical_solution_first_dif_with_multiple_n();
            create_data_for_numerical_solution_second_dif_with_multiple_n();
            create_data_for_analytical_solution_for_graph();

            find_max();

            cs.set_n(n);
            cs.set_h();

            create_data_for_coefs();

            create_x_coords_for_numerical_solution();
            create_data_for_numerical_solution();
            create_data_for_numerical_solution_first_dif();
            create_data_for_numerical_solution_second_dif();

            isNumericalSolutionSet = true;
        }
    }
    /////////

    // Численное решение для любой задачи с n
    void create_x_coords_for_numerical_solution() {

        x_of_numerical_solution.clear();

        x_of_numerical_solution.resize(cs.get_n() * separate_n + 1);

        //std::cout << "BORDERS: " << cs.get_border().first << "\t" << cs.get_border().second << std::endl;
        //std::cout << "cs.get_n(): " << cs.get_n() << std::endl;
        //std::cout << abs(cs.get_border().second - cs.get_border().first) / cs.get_n() / separate_n << std::endl;

        double h = abs(cs.get_border().second - cs.get_border().first) / cs.get_n() / separate_n;

        for (size_t count = 0; count < cs.get_n(); ++count) {
            for (size_t i = 0; i <= separate_n; ++i)
                x_of_numerical_solution[count * separate_n + i] = (cs.get_border().first + count * 
                    (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()) + i * h);
        }

        //for (size_t count = 0; count < x_of_numerical_solution.size(); ++count) {
        //    std::cout << x_of_numerical_solution[count] << std::endl;
        //}

        //std::cout << "x_of_numerical_solution.size(): " << x_of_numerical_solution.size() << std::endl;
    }
    void create_data_for_numerical_solution() {

        y_of_numerical_solution.clear();

        y_of_numerical_solution.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            //std::cout << "x_current: " << x_current << std::endl;

            for (size_t i = 0; i <= separate_n; ++i) {

                //std::cout << "x_of_numerical_solution[count * separate_n + i]: " << x_of_numerical_solution[count * separate_n + i] << std::endl;

                double delta = x_of_numerical_solution[count * separate_n + i] - x_current;

                y_of_numerical_solution[count * separate_n + i] = cs.get_a()[count] + cs.get_b()[count] * delta
                    + cs.get_c()[count + 1] / 2 * delta * delta + cs.get_d()[count] / 6 * delta * delta * delta;
            }
        }
    }
    void create_data_for_numerical_solution_first_dif() {

        y_of_numerical_solution_first_dif.clear();

        y_of_numerical_solution_first_dif.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            for (size_t i = 0; i <= separate_n; ++i) {

                double delta = x_of_numerical_solution[count * separate_n + i] - x_current;

                y_of_numerical_solution_first_dif[count * separate_n + i] = cs.get_b()[count] + 
                cs.get_c()[count + 1] * delta + cs.get_d()[count] / 2 * delta * delta;

            }
        }
    }
    void create_data_for_numerical_solution_second_dif() {

        y_of_numerical_solution_second_dif.clear();

        y_of_numerical_solution_second_dif.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            for (size_t i = 0; i <= separate_n; ++i) {

                double delta = x_of_numerical_solution[count * separate_n + i] - x_current;

                y_of_numerical_solution_second_dif[count * separate_n + i] = cs.get_c()[count + 1] + cs.get_d()[count] * delta;
            }
        }
    }
    /////////

    // Численное решение для любой задачи с N
    void create_x_coords_for_numerical_solution_with_multiple_n() {

        x_of_numerical_solution_with_multiple_n.clear();

        //std::cout << "N in cs.get_n(): " << cs.get_n() << std::endl;

        x_of_numerical_solution_with_multiple_n.resize(cs.get_n() * separate_n + 1);
        
        double h = abs(cs.get_border().second - cs.get_border().first) / cs.get_n() / separate_n;

        //std::cout << "H: " << h << std::endl;

        for (size_t count = 0; count < cs.get_n(); ++count) {
            for (size_t i = 0; i <= separate_n; ++i)
                x_of_numerical_solution_with_multiple_n[count * separate_n + i] = (cs.get_border().first + count *
                    (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()) + i * h);
        }

        //for (size_t count = 0; count < x_of_numerical_solution_with_multiple_n.size(); ++count) {
        //    std::cout << "x_of_numerical_solution_with_multiple_n[count]: " << x_of_numerical_solution_with_multiple_n[count] << std::endl;
        //}

    }
    void create_data_for_numerical_solution_with_multiple_n() {

        y_of_numerical_solution_with_multiple_n.clear();

        y_of_numerical_solution_with_multiple_n.resize(cs.get_n() * separate_n + 1);

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            //std::cout << "x_current: " << x_current << std::endl;

            for (size_t i = 0; i <= separate_n; ++i) {

                double delta = x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current;

                y_of_numerical_solution_with_multiple_n[count * separate_n + i] = cs.get_a()[count] + cs.get_b()[count] * delta + 
                    cs.get_c()[count + 1] / 2 * delta * delta + cs.get_d()[count] / 6 * delta * delta * delta;
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

            for (size_t i = 0; i <= separate_n; ++i) {

                double delta = x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current;

                y_of_numerical_solution_first_dif_with_multiple_n[count * separate_n + i] = cs.get_b()[count] +
                cs.get_c()[count + 1] * delta + cs.get_d()[count] / 2 * delta * delta;

            }
        }
    }
    void create_data_for_numerical_solution_second_dif_with_multiple_n() {

        y_of_numerical_solution_second_dif_with_multiple_n.clear();

        y_of_numerical_solution_second_dif_with_multiple_n.resize(cs.get_n() * separate_n + 1);

        //std::cout << cs.get_n() << std::endl;

        for (size_t count = 0; count < cs.get_n(); ++count) {

            double x_current = (cs.get_border().first + (count + 1) * (abs(cs.get_border().second - cs.get_border().first) / cs.get_n()));

            for (size_t i = 0; i <= separate_n; ++i) {

                double delta = x_of_numerical_solution_with_multiple_n[count * separate_n + i] - x_current;

                y_of_numerical_solution_second_dif_with_multiple_n[count * separate_n + i] = cs.get_c()[count + 1] + cs.get_d()[count] * delta;
            
            }
        }
    }
    /////////

};