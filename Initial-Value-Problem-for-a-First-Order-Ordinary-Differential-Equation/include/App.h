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
        m_window.create(sf::VideoMode(1920, 1080, 32), "Zabotin Cauchy problem");
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
        //plotStyle.Colors[ImPlotCol_Line] = ImVec4(255, 0.0f, 0.0f, 1.0f);  // Красный цвет
        plotStyle.LineWeight = 3;  // Толщина линии

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

            static ImGuiWindowFlags flagsForWindows = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
            
            static double x0 = 0;
            static double u0 = 1;
            static double a = 1;
            static double T = 1;
            static double u_end = 1;

            static bool isPressedTheButtonOfStartingCalculating = false;
            static bool isCalculatingWithConstH = false;
            static bool isDrawTable = true;
            static bool isDrawGraph = true;
            static bool isNotice = false;
            static bool isCheckLocalDown = true;
            static bool isMakeAnalSolut = true;
            static bool isMake_liquid_temperature = true;
            static bool isUseEDown = false;

            createWindowForStartingCalculating(flagsForWindows, x0, u0, a, T, isPressedTheButtonOfStartingCalculating, isNotice, isCalculatingWithConstH, isCheckLocalDown, isMakeAnalSolut, isMake_liquid_temperature, isUseEDown, u_end);
            createWindowForInputStartingParams(flagsForWindows, x0, u0, a, T);
            createWindowForInputE_and_H(flagsForWindows, u_end);
            createWindowForInputCountOfN(flagsForWindows, isCalculatingWithConstH, isPressedTheButtonOfStartingCalculating);
            createWindowForInputBorder(flagsForWindows);
            createWindowForDrawingGraphAndTable(flagsForWindows, isDrawTable, isDrawGraph, isCheckLocalDown, isPressedTheButtonOfStartingCalculating, isMakeAnalSolut, isMake_liquid_temperature, isUseEDown);
            makeWindowForInfo(flagsForWindows, isPressedTheButtonOfStartingCalculating, isCalculatingWithConstH, x0, u0);

            if (isPressedTheButtonOfStartingCalculating) {
                if (isDrawGraph)
                    createGraph(flagsForWindows, isMakeAnalSolut, isMake_liquid_temperature);
                else
                    clearGparh(flagsForWindows);
                if (isDrawTable)
                    createTable(flagsForWindows, isCalculatingWithConstH);
                else
                    clearTable(flagsForWindows);
            }
            else
                clearTableAndGraph(flagsForWindows);

            if (isNotice)
                makeWindowNoticeAboutCalculating(isNotice);

            m_window.clear({255,255,255,100});
            render();
        }
    }

    void makeWindowForInfo(ImGuiWindowFlags& flagsForWindows, bool& isPressedTheButtonOfStartingCalculating, bool& isCalculatingWithConstH, double& x0, double& u0) {

        ImGui::SetNextWindowPos({ 960,0 }); // устанавливаем позицию для будущего окна
        ImGui::SetNextWindowSize({ 956,220 });

        ImGui::Begin("Window for info", 0, flagsForWindows);

        if (isPressedTheButtonOfStartingCalculating) {

            ImGui::Text("du/dx = -a * (u - T)");

            ImGui::Text(u8"Метод Рунге Кутта порядка p = %d", rk.getP());

            ImGui::Text("t0 = %lf", x0);
            ImGui::SameLine();
            ImGui::Text("u0 = %lf", u0);
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

            ImGui::Text(u8"Результат расчёта:");

            ImGui::Text(u8"Количество шагов: %d", rk.get_coords().size() - 1);
            ImGui::SameLine();
            ImGui::Text(u8"(Должно быть N <= N_max)");

            ImGui::Text("b - x_N = %.12lf", rk.get_border().second - rk.get_coords()[rk.get_coords().size() - 1].first);
            ImGui::SameLine();
            ImGui::Text(u8"(При N < N_max должно быть 0 < b - x_N <= E_гр)");

            ImGui::Text("x_N = %.12lf", rk.get_coords()[rk.get_coords().size() - 1].first);
            ImGui::SameLine();
            ImGui::Text(u8"V_N_итог = %.12lf", rk.get_coords()[rk.get_coords().size() - 1].second);
            ImGui::Text(u8"(Последняя найденная точка численной траектории)");

            auto it_for_E = std::max_element(rk.get_difference_of_u_and_v().begin() + 1, rk.get_difference_of_u_and_v().end());
            int min_idx_of_E = std::distance(rk.get_difference_of_u_and_v().begin(), it_for_E);
            ImGui::Text("max|E| = %.12lf", *it_for_E);
            ImGui::SameLine();
            ImGui::Text(u8"при x_n = %.12lf", rk.get_coords()[min_idx_of_E].first);

            auto it_for_S = std::minmax_element(rk.getVectorOfS().begin() + 1, rk.getVectorOfS().end());
            int min_idx_of_S = std::distance(rk.getVectorOfS().begin(), it_for_S.first);
            int max_idx_of_S = std::distance(rk.getVectorOfS().begin(), it_for_S.second);
            ImGui::Text("max|S| = %.12lf", *it_for_S.second);
            ImGui::SameLine();
            ImGui::Text(u8"при x_n = %.12lf", rk.get_coords()[max_idx_of_S].first);
            ImGui::SameLine();
            ImGui::Text(u8"(Должно быть |S| < E_гр)");
            ImGui::Text("min|S| = %.12lf", *it_for_S.first);
            ImGui::SameLine();
            ImGui::Text(u8"при x_n = %.12lf", rk.get_coords()[min_idx_of_S].first);
            ImGui::SameLine();
            ImGui::Text(u8"(кроме x = x0)");

            if (!isCalculatingWithConstH) {

                int countOfDiv = rk.getCountOfDivisions();
                int countOfMult = rk.getCountOfDoublings();

                ImGui::Text(u8"Всего уменьшений шага: %d", countOfDiv);
                ImGui::Text(u8"Всего увеличений шага: %d", countOfMult);
            }

            auto it_for_H = std::minmax_element(rk.getVectorOfSteps().begin() + 1, rk.getVectorOfSteps().end());
            int min_idx_of_H = std::distance(rk.getVectorOfSteps().begin(), it_for_H.first);
            int max_idx_of_H = std::distance(rk.getVectorOfSteps().begin(), it_for_H.second);
            ImGui::Text("max h_n = %.12lf", *it_for_H.second);
            ImGui::SameLine();
            ImGui::Text(u8"при t_n+1 = %.12lf", rk.get_coords()[max_idx_of_H].first);
            ImGui::Text("min h_n = %.12lf", *it_for_H.first);
            ImGui::SameLine();
            ImGui::Text(u8"при t_n+1 = %.12lf", rk.get_coords()[min_idx_of_H].first);
        }

        ImGui::End();
    }

    void makeWindowNoticeAboutCalculating(bool& isNotice) {

        ImGuiWindowFlags flagsForWindows = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowSize({ 300,50 });
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f - 150, io.DisplaySize.y * 0.5f - 25));

        if (isNotice)
        {
            ImGui::OpenPopup(u8"Уведомление");

            if (ImGui::BeginPopupModal(u8"Уведомление", &isNotice, flagsForWindows)) {
                ImGui::Text(u8"Вычисления были завершены");
                ImGui::EndPopup();
            }
        }
    }

    void clearTable(ImGuiWindowFlags& flagsForWindows) {
        ImGui::SetNextWindowPos({ 0,312 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 960,768 }); // устанавливаем размер для создаваемого окна для таблицы
        ImGui::Begin("Clear table", 0, flagsForWindows); // создаем окно с заданными настройками

        ImGui::End(); // удаляем окно
    }

    void clearGparh(ImGuiWindowFlags& flagsForWindows) {
        ImGui::SetNextWindowSize({ 960,860 }); // устанавливаем размер для окна с графиком
        ImGui::SetNextWindowPos({ 960,220 }); // устанавливаем позицию для окна с графиком

        ImGui::Begin("Clear window", 0, flagsForWindows); // создаем окно с выбранными настройками

        ImGui::End(); // Удаляем окно
    }

    void clearTableAndGraph(ImGuiWindowFlags& flagsForWindows) {
        ImGui::SetNextWindowSize({ 960,860 }); // устанавливаем размер для окна с графиком
        ImGui::SetNextWindowPos({ 960,220 }); // устанавливаем позицию для окна с графиком

        ImGui::Begin("Clear window", 0, flagsForWindows); // создаем окно с выбранными настройками

        ImGui::End(); // Удаляем окно

        ImGui::SetNextWindowPos({ 0,312 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 960,768 }); // устанавливаем размер для создаваемого окна для таблицы
        ImGui::Begin("Clear table", 0, flagsForWindows); // создаем окно с заданными настройками

        ImGui::End(); // удаляем окно
    }

    void createWindowForDrawingGraphAndTable(ImGuiWindowFlags& flagsForWindows, bool& isDrawTable, bool& isDrawGraph, bool& isCheckLocalDown, bool& isPressedTheButtonOfStartingCalculating, bool& isMakeAnalSolut, bool& isMake_liquid_temperature, bool& isUseEDown) {
        
        ImGui::SetNextWindowPos({632,141});
        ImGui::SetNextWindowSize({327,170});

        ImGui::Begin("Window of checkboxs of table and graph", 0, flagsForWindows);

        ImGui::Checkbox(u8"Рисовать график", &isDrawGraph);
        ImGui::Checkbox(u8"Рисовать таблицу", &isDrawTable);
        if (ImGui::Checkbox(u8"Учитывать епсилон-нижнюю границу", &isCheckLocalDown))
            isPressedTheButtonOfStartingCalculating = false;
        if (ImGui::Checkbox(u8"Строить истинное решение", &isMakeAnalSolut))
            isPressedTheButtonOfStartingCalculating = false;
        if (ImGui::Checkbox(u8"Строить линию температуры жидкости", &isMake_liquid_temperature))
            isPressedTheButtonOfStartingCalculating = false;
        if (ImGui::Checkbox(u8"Счет с выходом на значение U \"сверху\"", &isUseEDown))
            isPressedTheButtonOfStartingCalculating = false;

        ImGui::End();
    }

    void createWindowForInputCountOfN(ImGuiWindowFlags& flagsForWindows, bool& isCalculatingWithConstH, bool& isPressedTheButtonOfStartingCalculating) {

        ImGui::SetNextWindowPos({301,221});
        ImGui::SetNextWindowSize({330,90});

        ImGui::Begin("Input count of N and const H", 0, flagsForWindows);

        if (ImGui::Checkbox(u8"Постоянный шаг", &isCalculatingWithConstH))
            isPressedTheButtonOfStartingCalculating = false;

        int N = rk.getMaxN();

        ImGui::Text(u8"Максимальное число шагов");
        if (ImGui::InputInt(u8" ", &N, 1, 10, 0))
            rk.set_count_of_max_N(N);

        ImGui::End();

    }

    void createWindowForInputE_and_H(ImGuiWindowFlags& flagsForWindows, double& u_end) { // функцию для создания окна для ввода шага и епсилон-границ

        ImGui::SetNextWindowPos({ 301,0 }); // устанавливаем позицию для будущего окна
        ImGui::SetNextWindowSize({ 330,220 }); // устанвливаем размер для будущего окна = 330*220

        ImGui::Begin("InPut E and H", 0, flagsForWindows); // создаем окно с выбранными настройками

        createInputH(flagsForWindows); // вызываем функцию для создания окна для ввода шага
        createInputE_local(flagsForWindows); // вызыаем функцию для создания окна для ввода епсилон-границы, ограничивающая u сверху и снизу
        createInputE_right(flagsForWindows); // вызываем функцию для создания онка для ввода епсилон-грницы, ограничивающая x справа
        createInputE_down(flagsForWindows);
        createInputU_End(flagsForWindows, u_end);

        ImGui::End(); // удаляем окно
    }

    void createInputU_End(ImGuiWindowFlags& flagsForWindows, double& u_end) {
        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка, чтобы не было заднего фона

        ImGui::BeginChild("Input E final", { 300,60 }, true, flagsForWindows); // создаем дочернее окно, с заданными размерами 300*60

        ImGui::SeparatorText(u8"Введите U"); // дополнительный текст Input E local

        ImGui::InputDouble(" ", &u_end, 0.01f, 1.0f, "%.8f"); // создает поле для ввода данных и если были введены какие-то данные, 

        ImGui::EndChild(); // дочернее окно удаляется
    }

    void createInputE_down(ImGuiWindowFlags& flagsForWindows) {
        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка, чтобы не было заднего фона

        ImGui::BeginChild("Input E up", { 300,60 }, true, flagsForWindows); // создаем дочернее окно, с заданными размерами 300*60

        ImGui::SeparatorText(u8"Введите епсилон-верхний"); // дополнительный текст Input E local

        static double input_e_down = rk.get_E_down(); // переменная input_e_local из класса RK
        static double max = 1.0; // max значение для input_e_local
        static double min = 0.000000001; // min значение для input_e_local
        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &input_e_down, 0.0005f, &min, &max, "%.10f", ImGuiSliderFlags_AlwaysClamp)) // создает поле для ввода данных и если были введены какие-то данные, то
            rk.set_e_down(input_e_down); // устанавливается значение для m_E_check_up в классе RK

        ImGui::EndChild(); // дочернее окно удаляется
    }

    void createInputE_local(ImGuiWindowFlags flagsForWindows) { // функция для создания дочернего окна для ввода e_local

        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка, чтобы не было заднего фона

        ImGui::BeginChild("Input E local", { 300,60 }, true, flagsForWindows); // создаем дочернее окно, с заданными размерами 300*60

        ImGui::SeparatorText(u8"Введите епсилон-локальный"); // дополнительный текст Input E local

        static double input_e_local = rk.get_E_check_up(); // переменная input_e_local из класса RK
        static double max = 1.0; // max значение для input_e_local
        static double min = 0.000000001; // min значение для input_e_local
        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &input_e_local, 0.0005f, &min, &max, "%.10f", ImGuiSliderFlags_AlwaysClamp)) // создает поле для ввода данных и если были введены какие-то данные, то
            rk.set_e_check_up(input_e_local); // устанавливается значение для m_E_check_up в классе RK

        ImGui::EndChild(); // дочернее окно удаляется
    }

    void createInputE_right(ImGuiWindowFlags flagsForWindows) { // функция для создания дочернего окна для ввода e_right - граница для x справа

        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка, чтобы не было заднего фона

        ImGui::BeginChild("Input E right", { 300,60 }, true, flagsForWindows); // создаем дочернее окно, с заданными размерами 300*60

        ImGui::SeparatorText(u8"Введите эпсилон-граничный"); // дополнительный текст Input E right

        static double input_e_right = rk.get_E_check_right(); // переменная input_e_right из класса RK
        static double max = 1.0; // max значение для input_e_right
        static double min = 0.000000001; // min значение для input_e_right

        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &input_e_right, 0.0005f, &min, &max, "%.10f", ImGuiSliderFlags_AlwaysClamp)) // создает поле для ввода данных и если были введены какие-то данные, то
            rk.set_e_check_right(input_e_right); // устанавливается значение для m_E_check_right в классе RK

        ImGui::EndChild(); // дочернее окно удаляется
    }

    void createInputH(ImGuiWindowFlags flagsForWindows) { // функция для создания дочернего окна для ввода h - шаг

        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка, чтобы не было заднего фона

        ImGui::BeginChild("Input H", { 300,60 }, true, flagsForWindows); // создаем дочернее окно, с заданными размерами 300*60

        ImGui::SeparatorText(u8"Введите начальный шаг"); // дополнительный текст Input H

        static double input_h = rk.get_h_start(); // переменная input_h из класса RK
        static double max = 1.0; // max значение для input_h
        static double min = 0.0000001; // min значение для input_h

        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &input_h, 0.0005f, &min, &max, "%.10f", ImGuiSliderFlags_AlwaysClamp)) // создает поле для ввода данных и если были введены какие-то данные, то
            rk.set_h_start(input_h); // устанавливается значение для m_h в классе RK

        ImGui::EndChild(); // дочернее окно удаляется
    }

    void createWindowForStartingCalculating(ImGuiWindowFlags& flagsForWindows, double& x0, double& u0, double& a, double& T, bool& isPressedTheButtonOfStartingCalculating, bool& isNotice, bool& isCalculatingWithConstH, bool& isCheckLocalDown, bool& isMakeAnalSolut, bool& isMake_liquid_temperature, bool& isUseEDown, double& u_end) {

        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::SetNextWindowSize({300, 40});

        ImGui::Begin("Start calculating", 0, flagsForWindows);

        if (ImGui::Button(u8"Старт", { 70,20 })) {
            rk.clear_data();
            if (isCalculatingWithConstH)
                rk.calculating_with_const_h(x0, u0, a, T, u_end, isUseEDown);
            else
                rk.calculating_with_different_h(x0, u0, a, T, isCheckLocalDown, isUseEDown, u_end);

            if (isMakeAnalSolut)
                rk.analytical_solution(u0, a, T);

            if (isMake_liquid_temperature)
                rk.liquid_temperature_coords(T);

            isNotice = true;
            isPressedTheButtonOfStartingCalculating = true;
        }
        ImGui::SameLine();
        ImGui::Text(u8"Нажмите \"Старт\" для начала");

        ImGui::End();

    }

    void createWindowForInputStartingParams(ImGuiWindowFlags& flagsForWindows, double& x0, double& u0, double& a, double& T) {

        ImGui::SetNextWindowPos({0,41});
        ImGui::SetNextWindowSize({ 300,270 });

        ImGui::Begin("Input Starting Params", 0, flagsForWindows);

        createWindowForInputX0(flagsForWindows, x0);
        createWindowForInputU0(flagsForWindows, u0);
        createWindowForInputA(flagsForWindows, a);
        createWindowForInputT(flagsForWindows, T);

        ImGui::End();
    }

    void createWindowForInputT(ImGuiWindowFlags flagsForWindows, double& T) {

        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input T", { 300,60 }, true, flagsForWindows); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите параметр T"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &T, 0.01f, 1.0f, "%.8f"); // поле для ввода double u

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputA(ImGuiWindowFlags flagsForWindows, double& a) {

        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input A", { 300,60 }, true, flagsForWindows); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите параметр a"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &a, 0.01f, 1.0f, "%.8f"); // поле для ввода double u

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputX0(ImGuiWindowFlags flagsForWindows, double& x0) {

        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input X0", { 300,60 }, true, flagsForWindows); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите x0"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &x0, 0.01f, 1.0f, "%.8f"); // поле для ввода double u

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputU0(ImGuiWindowFlags flagsForWindows, double& u0) {

        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input U0", { 300,60 }, true, flagsForWindows); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Введите u0"); // дополнительный текст в окне

        ImGui::InputDouble(" ", &u0, 0.01f, 1.0f, "%.8f"); // поле для ввода double u

        ImGui::EndChild();// удаляем дочернее окно

    }

    void createWindowForInputBorder(ImGuiWindowFlags& flagsForWindows) {

        ImGui::SetNextWindowPos({ 632,0 }); // устанавливаем позицию для будущего окна
        ImGui::SetNextWindowSize({ 327,140 }); // устанвливаем размер для будущего окна = 330*140

        ImGui::Begin("Input Border", 0, flagsForWindows); // создаем окно с выбранными настройками

        createInputBorderA(flagsForWindows);
        createInputBorderB(flagsForWindows);

        ImGui::End(); // удаляем окно
    }

    void createInputBorderA(ImGuiWindowFlags flagsForWindows) {

        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input A", { 300,60 }, true, flagsForWindows); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Левая граница"); // дополнительный текст в окне

        double a = rk.get_border().first;
        double max_for_a = rk.get_border().second - 0.0001;
        static double min_for_a = -INFINITY;

        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &a, 0.0005f, &min_for_a, &max_for_a, "%.10f", ImGuiSliderFlags_AlwaysClamp))
            rk.set_left_border(a);

        ImGui::EndChild();
    }

    void createInputBorderB(ImGuiWindowFlags flagsForWindows) {
        flagsForWindows |= ImGuiWindowFlags_NoBackground; // дополнительная настройка для окна, чтобы не было заднего фона

        ImGui::BeginChild("Input B", { 300,60 }, true, flagsForWindows); // Создаем дочернеё окно с размером 300*60 и настройками-flags

        ImGui::SeparatorText(u8"Правая граница"); // дополнительный текст в окне

        double b = rk.get_border().second;
        static double max_for_b = +INFINITY;
        double min_for_b = rk.get_border().first + 0.0001;

        if (ImGui::DragScalar(" ", ImGuiDataType_Double, &b, 0.0005f, &min_for_b, &max_for_b, "%.10f", ImGuiSliderFlags_AlwaysClamp))
            rk.set_right_border(b);

        ImGui::EndChild(); // удаляем дочернее окно
    }

    void createGraph(ImGuiWindowFlags& flagsForWindows, bool& isMakeAnalSolut, bool& isMake_liquid_temperature) { // функция для отрисовки графика
        
        std::vector<std::pair<double, double>> data_for_numerical_solution = rk.get_coords(); // создаем вектор точек для аналитического решения, если аналитического решения нет, то тут будет пустой ветктор
        std::vector<std::pair<double, double>> data_of_anal_solution = rk.get_anal_solut();
        std::vector<std::pair<double, double>> data_of_liquid_solution = rk.get_liquid_temperature_coords();

        const size_t size_of_numerical_solution = data_for_numerical_solution.size(); // получаем размер вектора точек для численного решения
        double* x_of_numerical_solution = new double[size_of_numerical_solution]; // создаем динамический массив x-точек численного решения. Размер этого массива равен размеру вектор точек для численного решения
        double* y_of_numerical_solution = new double[size_of_numerical_solution]; // создаем динамический массив y-точек численного решения. Размер этого массива равен размеру вектор точек для численного решения

        for (int count = 0; count < size_of_numerical_solution; ++count) { // цикл для копирования данных в массив из вектора точек для численного решения
            x_of_numerical_solution[count] = std::get<0>(data_for_numerical_solution[count]);
            y_of_numerical_solution[count] = std::get<1>(data_for_numerical_solution[count]);
        }

        const size_t size_of_anal_solution = data_of_anal_solution.size(); // получаем размер вектора точек для численного решения
        double* x_of_anal_solution = new double[size_of_anal_solution]; // создаем динамический массив x-точек численного решения. Размер этого массива равен размеру вектор точек для численного решения
        double* y_of_anal_solution = new double[size_of_anal_solution]; // создаем динамический массив y-точек численного решения. Размер этого массива равен размеру вектор точек для численного решения

        for (int count = 0; count < size_of_anal_solution; ++count) { // цикл для копирования данных в массив из вектора точек для численного решения
            x_of_anal_solution[count] = std::get<0>(data_of_anal_solution[count]);
            y_of_anal_solution[count] = std::get<1>(data_of_anal_solution[count]);
        }

        const size_t size_of_liquid_solution = data_of_liquid_solution.size(); // получаем размер вектора точек для численного решения
        double* x_of_liquid_sol = new double[size_of_liquid_solution]; // создаем динамический массив x-точек численного решения. Размер этого массива равен размеру вектор точек для численного решения
        double* y_of_liquid_sol = new double[size_of_liquid_solution]; // создаем динамический массив y-точек численного решения. Размер этого массива равен размеру вектор точек для численного решения

        for (int count = 0; count < size_of_liquid_solution; ++count) { // цикл для копирования данных в массив из вектора точек для численного решения
            x_of_liquid_sol[count] = std::get<0>(data_of_liquid_solution[count]);
            y_of_liquid_sol[count] = std::get<1>(data_of_liquid_solution[count]);
        }

        ImGui::SetNextWindowSize({ io.DisplaySize.x/2,io.DisplaySize.y/2*3 }); // устанавливаем размер для окна с графиком
        ImGui::SetNextWindowPos({ 960,220 }); // устанавливаем позицию для окна с графиком

        ImGui::Begin("Graph",0, flagsForWindows); // создаем окно с выбранными настройками

        if (ImPlot::BeginPlot(u8"График", "x", "u", { 940,780 })){
            ImPlot::PlotLine(u8"u(x)", x_of_numerical_solution, y_of_numerical_solution, size_of_numerical_solution); // отрисовываем линию
            if (isMakeAnalSolut)
                ImPlot::PlotLine(u8"Истинное решение", x_of_anal_solution, y_of_anal_solution, size_of_anal_solution); // отрисовываем линию
            if (isMake_liquid_temperature)
                ImPlot::PlotLine(u8"Линия температуры жидкости", x_of_liquid_sol, y_of_liquid_sol, size_of_liquid_solution); // отрисовываем линию
            ImPlot::EndPlot(); // заканчиваем отрисовку графика
        }

        ImGui::End(); // удаляем окно
    }

    void createTable(ImGuiWindowFlags& flagsForWindows, bool& isCalculatingWithConstH) {

        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable; // настройки для таблицы

        std::vector<std::pair<double, double>> data_for_numerical_solution = rk.get_coords();
        std::vector<double> data_for_steps = rk.getVectorOfSteps();
        std::vector<double> data_for_S = rk.getVectorOfS();
        std::vector<double> data_for_coords_getting_by_twice_half_step = rk.getVectorOfCoordsGettingByTwiceHalfStep();
        std::vector<double> data_of_difference_of_coords_with_step_and_with_twice_half_step = rk.getDifferenceOfCoordsWithStepAndWithTwiceHalfStep();
        std::vector<double> data_of_u = rk.get_vector_of_u();
        std::vector<double> data_of_diff_u_and_v = rk.get_difference_of_u_and_v();

        std::vector<int> data_of_divisions = rk.getVectorOfDivisions();
        std::vector<int> data_of_doublings = rk.getVectorOfDoublings();

        ImGui::SetNextWindowPos({ 0,312 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 960,700 }); // устанавливаем размер для создаваемого окна для таблицы
        ImGui::Begin("Table", 0, flagsForWindows); // создаем окно с заданными настройками

        static int size = 11;

        if (isCalculatingWithConstH)
            size = 9;
        else
            size = 11;

        if (ImGui::BeginTable("table1", size, flags)) // создаем таблицу с заданными настройками
        {
            if (true)
            {
                ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Xi");
                ImGui::TableSetupColumn("Vi");
                ImGui::TableSetupColumn("Hi");
                ImGui::TableSetupColumn(u8"ОЛП");
                ImGui::TableSetupColumn("V^i");
                ImGui::TableSetupColumn("Vi-V^i");
                ImGui::TableSetupColumn("Ui");
                ImGui::TableSetupColumn("|Ui-Vi|");

                if (!isCalculatingWithConstH) {
                    ImGui::TableSetupColumn(u8"Ум. шага", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn(u8"Ув. шага", ImGuiTableColumnFlags_WidthFixed);
                }
                ImGui::TableHeadersRow();
            }

            for (int row = 0; row < data_for_numerical_solution.size(); row++)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", row);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.12e", data_for_numerical_solution[row].first);
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.12e", data_for_numerical_solution[row].second);
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.12e", data_for_steps[row]);
                ImGui::TableSetColumnIndex(4);
                ImGui::Text("%.12e", data_for_S[row]);
                ImGui::TableSetColumnIndex(5);
                ImGui::Text("%.12e", data_for_coords_getting_by_twice_half_step[row]);
                ImGui::TableSetColumnIndex(6);
                ImGui::Text("%.12e", data_of_difference_of_coords_with_step_and_with_twice_half_step[row]);
                ImGui::TableSetColumnIndex(7);
                ImGui::Text("%.12e", data_of_u[row]);
                ImGui::TableSetColumnIndex(8);
                ImGui::Text("%.12e", data_of_diff_u_and_v[row]);
                if (!isCalculatingWithConstH) {
                    ImGui::TableSetColumnIndex(9);
                    ImGui::Text("%d", data_of_divisions[row]);
                    ImGui::TableSetColumnIndex(10);
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