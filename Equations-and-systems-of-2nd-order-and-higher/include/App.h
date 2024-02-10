#include "imgui.h"
#include "imgui-SFML.h"

#include "implot.h"
#include "implot_internal.h"

#include "imconfig.h"
#include "imconfig-SFML.h"

#include "SFML/Graphics.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "RK_IV.h"

#pragma once

class App {
private:

    sf::RenderWindow m_window;
    ImGuiIO io;
    RK_IV rk;

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
        ImGui::SFML::Init(m_window, false);
        io = ImGui::GetIO();
        ImPlot::CreateContext();

        io.Fonts->Clear();
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 13.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

        ImGui::SFML::UpdateFontTexture();
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

            static double t0 = 0;
            static std::vector<double> u0 = { 1,1,1 };

            static double a1 = 1;
            static double a2 = 1;
            static double w1 = 1;
            static double w2 = 1;
            static double b1 = 1;
            static double b2 = 1;
            static double g1 = 1;
            static double g2 = 1;

            static bool isPressedTheButtonOfStartingCalculating = false;
            static bool isNotice = false;
            static bool isCalculatingWithConstH = false;
            static bool isDrawTable = true;
            static bool isDrawGraph = true;
            static bool isCheckLocalDown = true;
            static bool isFirst = true;

            const char* names[4] = { "T", "U_1", "U_2", "U_3" };
            static bool opened[4] = { true , true, false , false};

            static ImGuiWindowFlags flags_for_window = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

            createWindowForStartingCalculating(flags_for_window, t0, u0, a1, a2, w1, w2, b1, b2, g1, g2, isPressedTheButtonOfStartingCalculating, isNotice, isCalculatingWithConstH, isCheckLocalDown, isFirst);
            makeWindowForInputEAndH(flags_for_window);
            makeWindowForInputBorder(flags_for_window);
            createWindowForInputCountOfN(flags_for_window, isCalculatingWithConstH, isPressedTheButtonOfStartingCalculating);
            createWindowForDrawingGraphAndTable(flags_for_window, isDrawTable, isDrawGraph, names, opened);
            createWindowForCheckDownE(flags_for_window, isCheckLocalDown, isPressedTheButtonOfStartingCalculating);
            createWindowForInputStartingParamsOfTAndU(flags_for_window, t0, u0);
            createWindowForInputStartingParams(flags_for_window, a1, a2, w1, w2, b1, b2, g1, g2);
            createWindowInfo(flags_for_window, isPressedTheButtonOfStartingCalculating, isCalculatingWithConstH, t0, u0);

            if (isPressedTheButtonOfStartingCalculating) {
                if (isDrawGraph)
                    createGraph(flags_for_window, isFirst, names, opened);
                else
                    clearGraph(flags_for_window);
                if (isDrawTable)
                    createTable(flags_for_window, isCalculatingWithConstH);
                else
                    clearTable(flags_for_window);
            }

            if (isNotice)
                makeWindowNoticeAboutEndingCalculating(isNotice);
            
            //ImGui::ShowDemoWindow();

            m_window.clear();
            render();
        }
    }

    // Function to display a help marker with a given message
    void HelpMarker(const char* message) {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(450.0f);
            ImGui::TextUnformatted(message);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    void createWindowInfo(ImGuiWindowFlags& flags_for_window, bool& isPressedTheButtonOfStartingCalculating, bool& isCalculatingWithConstH, double& t0, std::vector<double>& u0) {

        ImGui::SetNextWindowPos({ 1261,0 });
        ImGui::SetNextWindowSize({ 658, 219 });

        ImGui::Begin("Info", 0, flags_for_window);

        ImGui::Text("du_1/dt = u_1 * (a1 - u_1 - w1*u_2 - b1*u_3)");
        ImGui::Text("du_2/dt = u_2 * (a2 - w2*u_1 - u_2 - b2*u_3)");
        ImGui::Text("du_3/dt = u_3 * (-1 + g1*u_1 + g2*u_2)");

        ImGui::Text(u8"Метод Рунге Кутта порядка p = %d", rk.getP());

        ImGui::Text("t0 = %.12lf", t0);
        ImGui::Text("u0_1 = %.12lf; u0_2 = %.12lf; u0_3 = %.12lf", u0[0], u0[1], u0[2]);
        ImGui::SameLine();
        ImGui::Text(u8"(Условаия задачи Коши)");

        ImGui::Text(u8"b = %lf", rk.get_border().second);
        ImGui::SameLine();
        ImGui::Text(u8"E_гр = %.12lf", rk.get_E_check_right());
        ImGui::SameLine();
        ImGui::Text(u8"(Условия остановки счёта)");

        ImGui::Text(u8"h_0 = %.12lf", rk.get_h_start());
        ImGui::SameLine();
        ImGui::Text(u8"N_max = %d", rk.getMaxN());
        ImGui::SameLine();
        ImGui::Text(u8"(Начальный шаг интегрирования и максимальное число шагов метода)");

        ImGui::Text(u8"E = %.12lf", rk.get_E_check_up());
        ImGui::SameLine();
        ImGui::Text(u8"E_min = %.12lf", rk.get_E_check_down());
        ImGui::SameLine();
        ImGui::Text(u8"(Контроль модуля локальной погрешности сверху и снизу)");    

        if (isPressedTheButtonOfStartingCalculating) {

            ImGui::Text(u8"Результат расчёта:");

            ImGui::Text(u8"Количество шагов: %d", rk.get_coords_for_first().size() - 1);
            ImGui::SameLine();
            ImGui::Text(u8"(Должно быть N <= N_max)");

            ImGui::Text("b - t_N = %.12lf", rk.get_border().second - rk.get_coords_for_first()[rk.get_coords_for_first().size() - 1].first);
            ImGui::SameLine();
            ImGui::Text(u8"(При N < N_max должно быть 0 < b - t_N <= E_гр)");

            ImGui::Text("t_N = %.12lf", rk.get_coords_for_first()[rk.get_coords_for_first().size() - 1].first);
            ImGui::SameLine();
            ImGui::Text(u8"V_N_итог_1 = %.12lf; V_N_итог_2 = %.12lf; V_N_итог_3 = %.12lf;", rk.get_coords_for_first()[rk.get_coords_for_first().size() - 1].second, rk.get_coords_for_second()[rk.get_coords_for_second().size() - 1].second, rk.get_coords_for_third()[rk.get_coords_for_third().size() - 1].second);
            ImGui::Text(u8"(Последняя найденная точка численной траектории)");

            auto it_for_S = std::minmax_element(rk.get_vector_of_S().begin()+1, rk.get_vector_of_S().end());
            int min_idx_of_S = std::distance(rk.get_vector_of_S().begin(), it_for_S.first);
            int max_idx_of_S= std::distance(rk.get_vector_of_S().begin(), it_for_S.second);
            ImGui::Text(u8"max норма |S| = %.12lf", *it_for_S.second);
            ImGui::SameLine();
            ImGui::Text(u8"при t_n = %.12lf", rk.get_coords_for_first()[max_idx_of_S].first);
            ImGui::SameLine();
            ImGui::Text(u8"(Должно быть |S| < E_гр)");
            ImGui::Text(u8"min норма |S| = %.12lf", *it_for_S.first);
            ImGui::SameLine();
            ImGui::Text(u8"при t_n = %.12lf", rk.get_coords_for_first()[min_idx_of_S].first);
            ImGui::SameLine();
            ImGui::Text(u8"(кроме t = t0)");

            if (!isCalculatingWithConstH) {
                
                int countC1 = 0;
                int countC2 = 0;

                for (size_t count = 0; count < rk.get_vector_of_C1().size(); ++count) {
                    countC1 += rk.get_vector_of_C1()[count];
                    countC2 += rk.get_vector_of_C2()[count];
                }
                
                ImGui::Text(u8"Всего уменьшений шага: %d", countC1);
                ImGui::Text(u8"Всего увеличений шага: %d", countC2);

            }

            auto it_for_H = std::minmax_element(rk.get_vector_of_h().begin() + 1, rk.get_vector_of_h().end());
            int min_idx_of_H = std::distance(rk.get_vector_of_h().begin(), it_for_H.first);
            int max_idx_of_H = std::distance(rk.get_vector_of_h().begin(), it_for_H.second);
            ImGui::Text("max h_n = %.12lf", *it_for_H.second);
            ImGui::SameLine();
            ImGui::Text(u8"при t_n+1 = %.12lf", rk.get_coords_for_first()[max_idx_of_H].first);
            ImGui::Text("min h_n = %.12lf", *it_for_H.first);
            ImGui::SameLine();
            ImGui::Text(u8"при t_n+1 = %.12lf", rk.get_coords_for_first()[min_idx_of_H].first);

        }

        ImGui::End();
    
    }

    void clearGraph(ImGuiWindowFlags& flags_for_window) {

        ImGui::SetNextWindowSize({ io.DisplaySize.x / 2,io.DisplaySize.y / 2 * 3 }); // устанавливаем размер для окна с графиком
        ImGui::SetNextWindowPos({ 960,220 }); // устанавливаем позицию для окна с графиком

        ImGui::Begin("Graph_clear", 0, flags_for_window); // создаем окно с выбранными настройками

        ImGui::End();
    }

    void clearTable(ImGuiWindowFlags& flags_for_window) {

        ImGui::SetNextWindowPos({ 0,301 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 960,720 }); // устанавливаем размер для создаваемого окна для таблицы
        
        ImGui::Begin("Table_clear", 0, flags_for_window); // создаем окно с заданными настройками
    
        ImGui::End();

    }

    void createWindowForInputStartingParams(ImGuiWindowFlags& flags_for_window, double& a1, double& a2, double& w1, double& w2, double& b1, double& b2, double& g1, double& g2) {

        ImGui::SetNextWindowPos({ 960,0 });
        ImGui::SetNextWindowSize({ 300,219 });

        ImGui::Begin("Input Starting Params", 0, flags_for_window);

        createWindowForInputA1(flags_for_window, a1);
        createWindowForInputA2(flags_for_window, a2);
        createWindowForInputW1(flags_for_window, w1);
        createWindowForInputW2(flags_for_window, w2);
        createWindowForInputB1(flags_for_window, b1);
        createWindowForInputB2(flags_for_window, b2);
        createWindowForInputG1(flags_for_window, g1);
        createWindowForInputG2(flags_for_window, g2);

        ImGui::End();
    }

    void createWindowForInputG2(ImGuiWindowFlags flags_for_window, double& g2) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input g2", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите параметр g2"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &g2, 0.01f, 1.0f, "%.8f"); // поле для ввода double u
        ImGui::SameLine(); HelpMarker(u8"Эффективность увсоения вида 2 для популяции 3");

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputG1(ImGuiWindowFlags flags_for_window, double& g1) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input g1", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите параметр g1"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &g1, 0.01f, 1.0f, "%.8f"); // поле для ввода double u
        ImGui::SameLine(); HelpMarker(u8"Эффективность увсоения вида 1 для популяции 3");

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputW2(ImGuiWindowFlags flags_for_window, double& w2) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input w2", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите параметр w2"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &w2, 0.01f, 1.0f, "%.8f"); // поле для ввода double u
        ImGui::SameLine(); HelpMarker(u8"Интенсивность межвидовой конкуренции в популяции 2");

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputW1(ImGuiWindowFlags flags_for_window, double& w1) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input w1", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите параметр w1"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &w1, 0.01f, 1.0f, "%.8f"); // поле для ввода double u
        ImGui::SameLine(); HelpMarker(u8"Интенсивность межвидовой конкуренции в популяции 1");

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputB2(ImGuiWindowFlags flags_for_window, double& b2) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input b2", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите параметр b2"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &b2, 0.01f, 1.0f, "%.8f"); // поле для ввода double u
        ImGui::SameLine(); HelpMarker(u8"Интенсивность взаимодействия полпуляции 2 с популяцией 3");

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputB1(ImGuiWindowFlags flags_for_window, double& b1) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input b1", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите параметр b1"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &b1, 0.01f, 1.0f, "%.8f"); // поле для ввода double u
        ImGui::SameLine(); HelpMarker(u8"Интенсивность взаимодействия полпуляции 1 с популяцией 3");

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputA2(ImGuiWindowFlags flags_for_window, double& a2) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input a2", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите параметр a2"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &a2, 0.01f, 1.0f, "%.8f"); // поле для ввода double u
        ImGui::SameLine(); HelpMarker(u8"Темпы прироста в популяции 2");

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputA1(ImGuiWindowFlags flags_for_window, double& a1) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input a1", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите параметр a1"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &a1, 0.01f, 1.0f, "%.8f"); // поле для ввода double u
        ImGui::SameLine(); HelpMarker(u8"Темпы прироста в популяции 1");

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputStartingParamsOfTAndU(ImGuiWindowFlags& flags_for_window, double& t0, std::vector<double>& u0) {

        ImGui::SetNextWindowPos({ 662,0 });
        ImGui::SetNextWindowSize({ 297,300 });

        ImGui::Begin("Input Starting Params of t and u0", 0, flags_for_window);

        createWindowForInputT0(flags_for_window, t0);
        createWindowForInputU0_1(flags_for_window, u0[0]);
        createWindowForInputU0_2(flags_for_window, u0[1]);
        createWindowForInputU0_3(flags_for_window, u0[2]);

        ImGui::End();
    }

    void createWindowForInputT0(ImGuiWindowFlags flags_for_window, double& t0) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input T0", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите t0"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &t0, 0.01f, 1.0f, "%.8f"); // поле для ввода double u

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputU0_1(ImGuiWindowFlags flags_for_window, double& u0_1) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input U0_1", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите U0_1"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &u0_1, 0.01f, 1.0f, "%.8f"); // поле для ввода double u

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputU0_2(ImGuiWindowFlags flags_for_window, double& u0_2) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input U0_2", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите U0_2"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &u0_2, 0.01f, 1.0f, "%.8f"); // поле для ввода double u

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputU0_3(ImGuiWindowFlags flags_for_window, double& u0_3) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input U0_3", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите U0_3"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &u0_3, 0.01f, 1.0f, "%.8f"); // поле для ввода double u

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForCheckDownE(ImGuiWindowFlags& flags_for_window, bool& isCheckLocalDown, bool& isPressedTheButtonOfStartingCalculating) {

        ImGui::SetNextWindowPos({ 0,262 });
        ImGui::SetNextWindowSize({ 330,38 });

        ImGui::Begin("Window of checkboxs of chech down E", 0, flags_for_window);

        if (ImGui::Checkbox(u8"Учитывать епсилон-нижнюю границу", &isCheckLocalDown))
            isPressedTheButtonOfStartingCalculating = false;

        ImGui::End();
    }

    void createWindowForDrawingGraphAndTable(ImGuiWindowFlags& flags_for_window, bool& isDrawTable, bool& isDrawGraph, const char* names[4], bool(&opened)[4]) {

        ImGui::SetNextWindowPos({ 331,232 });
        ImGui::SetNextWindowSize({ 330,68 });

        ImGui::Begin("Window of checkboxs of table and graph", 0, flags_for_window);

        ImGui::Checkbox(u8"Рисовать график", &isDrawGraph);
        
        ImGui::SameLine();
        ImGui::Text(u8"Выберите оси:");

        ImGui::Checkbox(u8"Рисовать таблицу", &isDrawTable);

        ImGui::SameLine();
        int counter = 0;


        for (size_t count = 0; count < IM_ARRAYSIZE(opened); ++count) {
            if (opened[count])
                ++counter;
            else
                --counter;
        }

        for (size_t count = 0; count < IM_ARRAYSIZE(opened); ++count) {
            if (count > 0)
                ImGui::SameLine();
            if (ImGui::Checkbox(names[count], &opened[count])) {

                ++counter;
                if (counter >= 1) {
                    if (!opened[0]) {
                        if (counter > 1 && opened[0] == false) {
                            for (size_t i = count; i < IM_ARRAYSIZE(opened); ++i)
                                opened[i] = false;
                        }

                        opened[count] = false;
                    }
                }
            }
        }

        ImGui::End();
    }

    void createWindowForInputCountOfN(ImGuiWindowFlags& flags_for_window, bool& isCalculatingWithConstH, bool& isPressedTheButtonOfStartingCalculating) {

        ImGui::SetNextWindowPos({ 331,141 });
        ImGui::SetNextWindowSize({ 330,90 });

        ImGui::Begin("Input count of N and const H", 0, flags_for_window);

        if (ImGui::Checkbox(u8"Использовать постоянный шаг", &isCalculatingWithConstH))
            isPressedTheButtonOfStartingCalculating = false;

        int N = rk.getMaxN();

        ImGui::Text(u8"Максимальное число шагов");
        if (ImGui::InputInt(" ", &N, 1, 10, 0))
            rk.set_count_of_max_N(N);

        ImGui::End();

    }

    void makeWindowForInputBorder(ImGuiWindowFlags& flags_for_window) {

        ImGui::SetNextWindowPos({ 331,0 }); // устанавливаем позицию для будущего окна
        ImGui::SetNextWindowSize({ 330,140 }); // устанвливаем размер для будущего окна = 330*140

        ImGui::Begin("Input Border", 0, flags_for_window); // создаем окно с выбранными настройками

        createInputBorderA(flags_for_window);
        createInputBorderB(flags_for_window);

        ImGui::End(); // удаляем окно
    }

    void createInputBorderA(ImGuiWindowFlags flags_for_window) {

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input A", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Левая граница участка интегрирования"); // дополнительный текст в окне

        double a = rk.get_border().first;
        double max_for_a = rk.get_border().second - 0.0001;
        static double min_for_a = -INFINITY;

        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &a, 0.0005f, &min_for_a, &max_for_a, "%.10f", ImGuiSliderFlags_AlwaysClamp))
            rk.set_left_border(a);

        ImGui::EndChild();
    }

    void createInputBorderB(ImGuiWindowFlags flags_for_window) {
        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input B", { 300,60 }, true, flags_for_window); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Правая граница участка интегрирования"); // дополнительный текст в окне

        double b = rk.get_border().second;
        static double max_for_b = +INFINITY;
        double min_for_b = rk.get_border().first + 0.0001;

        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &b, 0.0005f, &min_for_b, &max_for_b, "%.10f", ImGuiSliderFlags_AlwaysClamp))
            rk.set_right_border(b);

        ImGui::EndChild(); // удаляем дочернее окно
    }

    void makeWindowForInputEAndH(ImGuiWindowFlags& flags_for_window) {
        ImGui::SetNextWindowPos({ 0,41}); // устанавливаем позицию для будущего окна
        ImGui::SetNextWindowSize({ 330,220 }); // устанвливаем размер для будущего окна = 330*220

        ImGui::Begin("InPut E and H", 0, flags_for_window); // создаем окно с выбранными настройками

        createInputH(flags_for_window); // вызываем функцию для создания окна для ввода шага
        createInputE_local(flags_for_window); // вызыаем функцию для создания окна для ввода епсилон-границы, ограничивающая u сверху и снизу
        createInputE_right(flags_for_window); // вызываем функцию для создания онка для ввода епсилон-грницы, ограничивающая x справа

        ImGui::End(); // удаляем окно
    }

    void createInputE_local(ImGuiWindowFlags flags_for_window) { // функция для создания дочернего окна для ввода e_local

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка, чтобы не было заднего фона

        ImGui::BeginChild("Input E local", { 300,60 }, true, flags_for_window); // создаем дочернее окно, с заданными размерами 300*60

        ImGui::SeparatorText(u8"Введите епсилон-локальный"); // дополнительный текст Input E local

        static double input_e_local = rk.get_E_check_up(); // переменная input_e_local из класса RK
        static double max = 1.0; // max значение для input_e_local
        static double min = 0.000000001; // min значение для input_e_local
        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &input_e_local, 0.0005f, &min, &max, "%.10f", ImGuiSliderFlags_AlwaysClamp)) // создает поле для ввода данных и если были введены какие-то данные, то
            rk.set_e_check_up(input_e_local); // устанавливается значение для m_E_check_up в классе RK

        ImGui::EndChild(); // дочернее окно удаляется
    }

    void createInputE_right(ImGuiWindowFlags flags_for_window) { // функция для создания дочернего окна для ввода e_right - граница для x справа

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка, чтобы не было заднего фона

        ImGui::BeginChild(u8"Введите эпсилон-граничный", { 300,60 }, true, flags_for_window); // создаем дочернее окно, с заданными размерами 300*60

        ImGui::SeparatorText(u8"Введите эпсилон-граничный"); // дополнительный текст Input E right

        static double input_e_right = rk.get_E_check_right(); // переменная input_e_right из класса RK
        static double max = 1.0; // max значение для input_e_right
        static double min = 0.000000001; // min значение для input_e_right

        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &input_e_right, 0.0005f, &min, &max, "%.10f", ImGuiSliderFlags_AlwaysClamp)) // создает поле для ввода данных и если были введены какие-то данные, то
            rk.set_e_check_right(input_e_right); // устанавливается значение для m_E_check_right в классе RK

        ImGui::EndChild(); // дочернее окно удаляется
    }

    void createInputH(ImGuiWindowFlags flags_for_window) { // функция для создания дочернего окна для ввода h - шаг

        flags_for_window |= ImGuiWindowFlags_NoBackground; // дополнительная настройка, чтобы не было заднего фона

        ImGui::BeginChild("Input H", { 300,60 }, true, flags_for_window); // создаем дочернее окно, с заданными размерами 300*60

        ImGui::SeparatorText(u8"Введите начальный шаг численного интегрирования"); // дополнительный текст Input H

        static double input_h = rk.get_h_start(); // переменная input_h из класса RK
        static double max = 1.0; // max значение для input_h
        static double min = 0.0000001; // min значение для input_h

        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &input_h, 0.0005f, &min, &max, "%.10f", ImGuiSliderFlags_AlwaysClamp)) // создает поле для ввода данных и если были введены какие-то данные, то
            rk.set_h_start(input_h); // устанавливается значение для m_h в классе RK

        ImGui::EndChild(); // дочернее окно удаляется
    }

    void makeWindowNoticeAboutEndingCalculating(bool& isNotice) {

        ImGuiWindowFlags flags_for_window = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowSize({ 300,50 });
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f - 150, io.DisplaySize.y * 0.5f - 25));

        if (isNotice)
        {
            ImGui::OpenPopup(u8"Уведомление");

            if (ImGui::BeginPopupModal(u8"Уведомление", &isNotice, flags_for_window)) {
                ImGui::Text(u8"Вычисления были завершены");
                ImGui::EndPopup();
            }
        }
    }

    void createWindowForStartingCalculating(ImGuiWindowFlags& flags_for_window, double& t0, std::vector<double>& u0, double& a1, double& a2, double& w1, double& w2, double& b1, double& b2, double& g1, double& g2, bool& isPressedTheButtonOfStartingCalculating, bool& isNotice, bool& isCalculatingWithConstH, bool& isCheckLocalDown, static bool isFirst) {
        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::SetNextWindowSize({ 330, 40 });

        ImGui::Begin("Start calculating", 0, flags_for_window);

        if (ImGui::Button(u8"Старт", { 100,20 })) {
            rk.clear_data();
            if (isCalculatingWithConstH)
                rk.calculating_with_const_h(t0, u0, a1, a2, w1, w2, b1, b2, g1, g2, isCheckLocalDown);
            else
                rk.calculating_with_different_h(t0, u0, a1, a2, w1, w2, b1, b2, g1, g2, isCheckLocalDown);
            isNotice = true;
            isPressedTheButtonOfStartingCalculating = true;
        }
        ImGui::SameLine();
        ImGui::Text(u8"Нажмите \"Старт\" для начала");

        ImGui::End();
    }

    void createGraph(ImGuiWindowFlags& flags_for_window, bool& isFirst, const char* names[4], bool(&opened)[4]) {
        static std::vector<double> x_of_numerical_solution_for_first;
        static std::vector<double> y_of_numerical_solution_for_first;
        static std::vector<double> x_of_numerical_solution_for_second;
        static std::vector<double> y_of_numerical_solution_for_second;
        static std::vector<double> x_of_numerical_solution_for_third;
        static std::vector<double> y_of_numerical_solution_for_third;

        std::vector<std::pair<double, double>> data_for_first = rk.get_coords_for_first();
        x_of_numerical_solution_for_first.resize(data_for_first.size());
        y_of_numerical_solution_for_first.resize(data_for_first.size());
        for (size_t count = 0; count < data_for_first.size(); ++count) {
            x_of_numerical_solution_for_first[count] = data_for_first[count].first;
            y_of_numerical_solution_for_first[count] = data_for_first[count].second;
        }

        std::vector<std::pair<double, double>> data_for_second = rk.get_coords_for_second();
        x_of_numerical_solution_for_second.resize(data_for_second.size());
        y_of_numerical_solution_for_second.resize(data_for_second.size());
        for (size_t count = 0; count < data_for_second.size(); ++count) {
            x_of_numerical_solution_for_second[count] = data_for_second[count].first;
            y_of_numerical_solution_for_second[count] = data_for_second[count].second;
        }

        std::vector<std::pair<double, double>> data_for_third = rk.get_coords_for_third();
        x_of_numerical_solution_for_third.resize(data_for_third.size());
        y_of_numerical_solution_for_third.resize(data_for_third.size());
        for (size_t count = 0; count < data_for_third.size(); ++count) {
            x_of_numerical_solution_for_third[count] = data_for_third[count].first;
            y_of_numerical_solution_for_third[count] = data_for_third[count].second;
        }

        ImGui::SetNextWindowSize({ io.DisplaySize.x / 2, io.DisplaySize.y / 2 * 3 });
        ImGui::SetNextWindowPos({ 960, 220 });

        ImGui::Begin("Graph", 0, flags_for_window);

        if (opened[0]) {
            if (ImPlot::BeginPlot(u8"График", "t", "u", { 940,780 })) {
                if (opened[1])
                    ImPlot::PlotLine(u8"u1(t)", x_of_numerical_solution_for_first.data(), y_of_numerical_solution_for_first.data(), x_of_numerical_solution_for_first.size());
                if (opened[2])
                    ImPlot::PlotLine(u8"u2(t)", x_of_numerical_solution_for_second.data(), y_of_numerical_solution_for_second.data(), x_of_numerical_solution_for_second.size());
                if (opened[3])
                    ImPlot::PlotLine(u8"u3(t)", x_of_numerical_solution_for_third.data(), y_of_numerical_solution_for_third.data(), x_of_numerical_solution_for_third.size());
            }
            ImPlot::EndPlot();
        }
        else if (opened[1] && opened[2]) {
            if (ImPlot::BeginPlot(u8"График", "u1", "u2", { 940,780 })) {
                ImPlot::PlotLine(u8" ", y_of_numerical_solution_for_first.data(), y_of_numerical_solution_for_second.data(), y_of_numerical_solution_for_second.size());
            }
            ImPlot::EndPlot();
        }
        else if (opened[2] && opened[3]) {
            if (ImPlot::BeginPlot(u8"График", "u2", "u3", { 940,780 })) {
                ImPlot::PlotLine(u8" ", y_of_numerical_solution_for_second.data(), y_of_numerical_solution_for_third.data(), y_of_numerical_solution_for_third.size());
            }
            ImPlot::EndPlot();
        }
        else if (opened[1] && opened[3]) {
            if (ImPlot::BeginPlot(u8"График", "u1", "u3", { 940,780 })) {
                ImPlot::PlotLine(u8" ", y_of_numerical_solution_for_first.data(), y_of_numerical_solution_for_third.data(), y_of_numerical_solution_for_third.size());
            }
            ImPlot::EndPlot();
        }

        ImGui::End();
    }
    
    void createTable(ImGuiWindowFlags& flagsForWindows, bool& isCalculatingWithConstH) {

        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable; // настройки для таблицы

        std::vector<std::pair<double, double>> data_for_numerical_solution_for_first = rk.get_coords_for_first();
        std::vector<std::pair<double, double>> data_for_numerical_solution_for_second = rk.get_coords_for_second();
        std::vector<std::pair<double, double>> data_for_numerical_solution_for_third = rk.get_coords_for_third();

        std::vector<double> coords_with_twice_step_for_first = rk.get_coords_with_twice_step_for_first();
        std::vector<double> coords_with_twice_step_for_second = rk.get_coords_with_twice_step_for_second();
        std::vector<double> coords_with_twice_step_for_third = rk.get_coords_with_twice_step_for_third();

        std::vector<double> data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first = rk.get_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first();
        std::vector<double> data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second = rk.get_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second();
        std::vector<double> data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third = rk.get_data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third();
        
        std::vector<double> data_for_steps = rk.get_vector_of_h();
        std::vector<double> data_for_S = rk.get_vector_of_S();

        std::vector<int> data_of_divisions = rk.get_vector_of_C1();
        std::vector<int> data_of_doublings = rk.get_vector_of_C2();

        /*
        std::cout << data_for_numerical_solution_for_first.size() << std::endl;
        std::cout << data_for_numerical_solution_for_second.size() << std::endl;
        std::cout << data_for_numerical_solution_for_third.size() << std::endl;
        std::cout << coords_with_twice_step_for_first.size() << std::endl;
        std::cout << coords_with_twice_step_for_second.size() << std::endl;
        std::cout << coords_with_twice_step_for_third.size() << std::endl;
        std::cout << data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first.size() << std::endl;
        std::cout << data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second.size() << std::endl;
        std::cout << data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third.size() << std::endl;
        std::cout << data_for_steps.size() << std::endl;
        std::cout << data_for_S.size() << std::endl;
        std::cout << data_of_divisions.size() << std::endl;
        std::cout << data_of_doublings.size() << std::endl;
        std::cout << "++++++++++++++++" << std::endl;
        */

        ImGui::SetNextWindowPos({ 0,301 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 960,720 }); // устанавливаем размер для создаваемого окна для таблицы
        
        ImGui::Begin("Table", 0, flagsForWindows); // создаем окно с заданными настройками

        static int size = 15;

        if (isCalculatingWithConstH)
            size = 13;
        else size = 15;

        if (ImGui::BeginTable("table1", size, flags)) // создаем таблицу с заданными настройками
        {
            if (true)
            {
                ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed);

                ImGui::TableSetupColumn("Xi");

                ImGui::TableSetupColumn("Vi_1");
                ImGui::TableSetupColumn("Vi_2");
                ImGui::TableSetupColumn("Vi_3");

                ImGui::TableSetupColumn("Hi");

                ImGui::TableSetupColumn(u8"Норма ОЛП");

                ImGui::TableSetupColumn("V^i_1");
                ImGui::TableSetupColumn("V^i_2");
                ImGui::TableSetupColumn("V^i_3");

                ImGui::TableSetupColumn("Vi_1-V^i_1");
                ImGui::TableSetupColumn("Vi_2-V^i_2");
                ImGui::TableSetupColumn("Vi_3-V^i_3");

                if (!isCalculatingWithConstH) {
                    ImGui::TableSetupColumn(u8"Ум. шага", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn(u8"Ув. шага", ImGuiTableColumnFlags_WidthFixed);
                }

                ImGui::TableHeadersRow();
            }

            for (int row = 0; row < data_for_numerical_solution_for_first.size(); row++)
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", row);
                
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.12lf", data_for_numerical_solution_for_first[row].first);
                
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.24lf", data_for_numerical_solution_for_first[row].second);
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.24lf", data_for_numerical_solution_for_second[row].second);
                ImGui::TableSetColumnIndex(4);
                ImGui::Text("%.24f", data_for_numerical_solution_for_third[row].second);

                ImGui::TableSetColumnIndex(5);
                ImGui::Text("%.24lf", data_for_steps[row]);

                ImGui::TableSetColumnIndex(6);
                ImGui::Text("%.24lf", data_for_S[row]);
                
                ImGui::TableSetColumnIndex(7);
                ImGui::Text("%.24lf", coords_with_twice_step_for_first[row]);
                ImGui::TableSetColumnIndex(8);
                ImGui::Text("%.24lf", coords_with_twice_step_for_second[row]);
                ImGui::TableSetColumnIndex(9);
                ImGui::Text("%.24lf", coords_with_twice_step_for_third[row]);

                ImGui::TableSetColumnIndex(10);
                ImGui::Text("%.24lf", data_of_difference_of_coords_with_step_and_with_twice_half_step_for_first[row]);
                ImGui::TableSetColumnIndex(11);
                ImGui::Text("%.24lf", data_of_difference_of_coords_with_step_and_with_twice_half_step_for_second[row]);
                ImGui::TableSetColumnIndex(12);
                ImGui::Text("%.24lf", data_of_difference_of_coords_with_step_and_with_twice_half_step_for_third[row]);

                if (!isCalculatingWithConstH) {
                    ImGui::TableSetColumnIndex(13);
                    ImGui::Text("%d", data_of_divisions[row]);
                    ImGui::TableSetColumnIndex(14);
                    ImGui::Text("%d", data_of_doublings[row]);
                }
            }

            ImGui::EndTable();
        
        }

        ImGui::End();

    }

    void render() {

        ImGui::SFML::Render(m_window);
        m_window.display();
    }
};