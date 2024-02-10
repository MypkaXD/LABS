#include "Sweep_method.h"

#pragma once

#include "imgui.h"
#include "imgui-SFML.h"

#include "implot.h"
#include "implot_internal.h"

#include "imconfig.h"
#include "imconfig-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

class App {

private:

	sf::RenderWindow m_window;
	ImGuiIO m_io;
	SweepMethod sm;

public:

	App() {
	}
	~App() {
		ImGui::SFML::Shutdown();
	}

	void init() {

        m_window.create(sf::VideoMode(1920, 1080, 32), "Lab2");
        m_window.setFramerateLimit(60);
        ImGui::SFML::Init(m_window);
        m_io = ImGui::GetIO();
        ImPlot::CreateContext();

        m_io.Fonts->Clear();
        m_io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 13.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

        ImGui::SFML::UpdateFontTexture();

        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 100.0f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.4f, 0.8f, 1.0f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(255, 255, 255, 100);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(255, 255, 255, 1.0f);
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(255, 255, 255, 1.0f);

        ImPlotStyle& plotStyle = ImPlot::GetStyle();
        plotStyle.LineWeight = 3;  // Толщина линии

	}

    void render() {

        ImGui::SFML::Render(m_window);
        m_window.display();
    
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

            static int N = 100;
            static int item_current_idx = 0;

            static bool isStart = false;
            static bool isRender = false;

            static double e = 0;
            static double x_e = -1;

            static ImGuiWindowFlags flagsForWindows = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

            static std::vector<double> first_x;
            static std::vector<double> first_y;

            static std::vector<double> second_x;
            static std::vector<double> second_y;

            static std::vector<double> anal_x;
            static std::vector<double> anal_y;

            renderWindowForSrartCalc(flagsForWindows, isStart, N, first_x, first_y, second_x, second_y, anal_x, anal_y, item_current_idx, isRender, e, x_e);
            renderWindowForN(flagsForWindows, N, isRender);
            renderWindowForInputTask(flagsForWindows, isStart, item_current_idx, isRender);
            renderWindowForInfo(flagsForWindows, isRender, e, x_e, item_current_idx, N);

            renderGraph(flagsForWindows, isRender, item_current_idx, N , first_x, first_y, second_x, second_y, anal_x, anal_y);
            renderTable(flagsForWindows, N, first_x, first_y, second_x, second_y,
                anal_x, anal_y, item_current_idx, isRender);
            m_window.clear({ 255,255,255,100 });
            render();
        }
    }

    void renderWindowForInfo(ImGuiWindowFlags& flagsForWindows, bool& isRender, double& e, double& x_e, int& item_current_idx, int& N) {
        
        ImGui::SetNextWindowSize({ 508, 157 });
        ImGui::SetNextWindowPos({ 451, 0 });
        
        ImGui::Begin("E", 0, flagsForWindows);

        ImGui::Text(u8"Вариант 5");
        ImGui::Text(u8"Точка разрыва: Pi/4");

        if (isRender) {
        
            ImGui::Text(u8"Справка:");
            ImGui::Text(u8"Для решения задачи использована равномерная сетка с числом разбиений N = %d", N);
            ImGui::Text(u8"Задача должна быть решена с заданной точностью e = 0.5*10^(-6)");
            ImGui::Text(u8"Задача решена с точностью e = %.6e", e);

            if (item_current_idx == 0) {
                ImGui::Text(u8"Максимальное отклонение аналитического и численного решений наблюдается в точке x = %.12lf", x_e);
            }
            else {
                ImGui::Text(u8"Максимальная разность численных в общих узлах сетки наблюдается в точке x = %.12lf", x_e);
            }
        }
        ImGui::End();
    }

    void renderWindowForInputTask(ImGuiWindowFlags& flagsForWindows, bool& isStart, int& item_current_idx, bool& isRender) {

        ImGui::SetNextWindowPos({ 0,61 }); // устанавливаем позицию в левом-верхнем углу (0,0)
        ImGui::SetNextWindowSize({ 450,40 }); // устанавливаем размеры для окна 350*40

        ImGui::Begin("Select_Task", 0, flagsForWindows); // создаем окно с выбранными настройками flagsForWindows

        const char* items[] = { u8"Тестовая", u8"Основная"}; // создаем массив с названием задача
        const char* combo_preview_value = items[item_current_idx];  // отображает предыдущую выбранную задачу

        if (ImGui::BeginCombo(u8"Выберите задачу", combo_preview_value)) // создаем выплывающий список из задача
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++) // проходи циклом по массиву задач
            {
                const bool is_selected = (item_current_idx == n); // если задача в массиве, совпадающая с item_current_idx -> true
                if (ImGui::Selectable(items[n], is_selected)) { // устанавливаем индекс n для item_current_idx, если выбрана задача n
                    item_current_idx = n;
                    isStart = false;
                    isRender = false;
                }
                if (is_selected) // если is_selected
                    ImGui::SetItemDefaultFocus(); // устанавливаем фокус на выбранную задачу
            }
            ImGui::EndCombo(); // удаляем выплывающее окно
        }

        ImGui::End(); // удаляем окно
    }

    void renderWindowForSrartCalc(ImGuiWindowFlags& flagsForWindows, bool& isStart, int& N, std::vector<double>& first_x, std::vector<double>& first_y, std::vector<double>& second_x, std::vector<double>& second_y,
        std::vector<double>& anal_x, std::vector<double>& anal_y, int& item_current_idx, bool& isRender, double& e, double& x_e) {

        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::SetNextWindowSize({ 450, 60 });

        ImGui::Begin("Start", 0, flagsForWindows);

        if (ImGui::Button(u8"Начать вычисления", { 150, 40 })) {
            isStart = true;

            first_x.clear();
            first_y.clear();
            second_x.clear();
            second_y.clear();

            first_x.resize(N);
            first_y.resize(N);

            if (item_current_idx == 0) {
                sm.set_coefs_for_test();
            }
            else 
                sm.set_coefs_for_main();

            sm.straight_stroke();
            sm.reverse_stroke();

            double h = 1 / (double)N;

            for (size_t count = 0; count < N+1; ++count) {
                first_x[count] = 0 + count * h;
                first_y[count] = sm.get_V()[count];
            }

            if (item_current_idx == 1) {

                sm.set_N(N*2);
                second_x.resize(N * 2);
                second_y.resize(N * 2);

                if (item_current_idx == 0) {
                    sm.set_coefs_for_test();
                }
                else
                    sm.set_coefs_for_main();

                sm.straight_stroke();
                sm.reverse_stroke();

                h = 1 / (double)(2*N);

                for (size_t count = 0; count < N*2+1; ++count) {
                    second_x[count] = 0 + count * h;
                    second_y[count] = sm.get_V()[count];
                }

                sm.set_N(N);

            }
            if (item_current_idx == 0) {
                anal_x.clear();
                anal_y.clear();
                anal_x.resize(N+1);
                anal_y.resize(N+1);

                sm.analytical_solution_init();

                for (size_t count = 0; count < N+1; ++count) {
                    anal_x[count] = sm.get_analytical_solution()[count].first;
                    anal_y[count] = sm.get_analytical_solution()[count].second;
                }
            }
            if (item_current_idx == 1) {
                e = 0;
                x_e = -1;
                for (size_t count = 0; count < N+1; ++count) {
                    if (abs(first_y[count] - second_y[2 * count]) > e) {
                        e = abs(first_y[count] - second_y[2 * count]);
                        x_e = first_x[count];
                    }
                    else continue;
                }
            }
            else if (item_current_idx == 0) {

                e = 0;
                x_e = -1;

                for (size_t count = 0; count < N+1; ++count) {
                    if (abs(first_y[count] - anal_y[count]) > e) {
                        e = abs(first_y[count] - anal_y[count]);
                        x_e = first_x[count];
                    }
                }
            }

            isRender = true;
        }

        ImGui::SameLine();
        ImGui::Text(u8"Нажмите, чтобы начать вычисления");

        ImGui::End();

    }

    void renderWindowForN(ImGuiWindowFlags& flagsForWindows, int& N, bool& isRender) {

        ImGui::SetNextWindowPos({0,102});
        ImGui::SetNextWindowSize({ 450, 55 });

        ImGui::Begin("Input_N", 0, flagsForWindows);

        if (ImGui::InputInt(u8"Введите число разбиений", &N, 1, 100, 0)) {
            if (N <= 0)
                N = 1;
            isRender = false;
        }

        ImGui::Text(u8"Шаг сетки: %.6lf", 1 / double(N));

        sm.set_N(N);

        ImGui::End();
    }

    void renderTable(ImGuiWindowFlags& flagsForWindows, int& N , std::vector<double>& first_x, std::vector<double>& first_y, std::vector<double>& second_x, std::vector<double>& second_y,
        std::vector<double>& anal_x, std::vector<double>& anal_y, int& item_current_idx, bool& isRender) {
        
        int size = 5;

        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg; // настройки для таблицы

        ImGui::SetNextWindowPos({ 0,158 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 959,860 }); // устанавливаем размер для создаваемого окна для таблицы
        ImGui::Begin("Table", 0, flagsForWindows); // создаем окно с заданными настройками

        if (isRender) {
            if (ImGui::BeginTable("table1", size, flags)) // создаем таблицу с заданными настройками
            {
                if (true)
                {
                    ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Xi");
                    ImGui::TableSetupColumn("Vi");
                    if (item_current_idx == 1) {
                        ImGui::TableSetupColumn("V^i");
                        ImGui::TableSetupColumn("|Vi-V^i|");
                    }
                    else {
                        ImGui::TableSetupColumn("Ui");
                        ImGui::TableSetupColumn("|Ui-Vi|");
                    }

                    ImGui::TableHeadersRow();
                }

                for (int row = 0; row < N+1; row++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", row);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%.12lf", first_x[row]);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%.12lf", first_y[row]);
                    if (item_current_idx == 0) {
                        ImGui::TableSetColumnIndex(3);
                        ImGui::Text("%.12lf", anal_y[row]);
                        ImGui::TableSetColumnIndex(4);
                        ImGui::Text("%.12lf", abs(first_y[row] - anal_y[row]));
                    }
                    else {
                        ImGui::TableSetColumnIndex(3);
                        ImGui::Text("%.12lf", second_y[2 * row]);
                        ImGui::TableSetColumnIndex(4);
                        ImGui::Text("%.12lf", abs(first_y[row]-second_y[2 * row]));
                    }
                }
                ImGui::EndTable();
            }
        }
        ImGui::End();
    }

    void renderGraph(ImGuiWindowFlags& flagsForWindows, bool& isRender, int& item_current_idx, int& N, std::vector<double>& first_x, std::vector<double>& first_y, std::vector<double>& second_x, std::vector<double>& second_y,
        std::vector<double>& anal_x, std::vector<double>& anal_y) {

        ImGui::SetNextWindowSize({ 960,1080}); // устанавливаем размер для окна с графиком
        ImGui::SetNextWindowPos({ 960,0 }); // устанавливаем позицию для окна с графиком

        ImGui::Begin("Graph", 0, flagsForWindows); // создаем окно с выбранными настройками

        if (isRender) {
            if (ImPlot::BeginPlot(u8"График", "x", "u", { 960,1000 })) {

                ImPlot::PlotLine(u8"Численное решение с числом разбиений N", first_x.data(), first_y.data(), N+1); // отрисовываем линию
                if (item_current_idx == 0)
                    ImPlot::PlotLine(u8"Аналитическое решение", anal_x.data(), anal_y.data(), N+1); // отрисовываем линию
                else
                    ImPlot::PlotLine(u8"Численное решение с числом разбиений N*2", second_x.data(), second_y.data(), N*2+1); // отрисовываем линию
                ImPlot::EndPlot(); // заканчиваем отрисовку графика
            }
        }
        ImGui::End(); // удаляем окно
    }

};