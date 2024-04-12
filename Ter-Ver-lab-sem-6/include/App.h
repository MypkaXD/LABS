#include "imgui.h"
#include "imgui-SFML.h"

#include "implot.h"
#include "implot_internal.h"

#include "imconfig.h"
#include "imconfig-SFML.h"

#include "SFML/Graphics.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <vector>
#include <map>

#pragma once

class App {
private:

    sf::RenderWindow m_window;
    ImGuiIO io;

    double sample_mean = 0; // выборочное среднее
    double sample_variance = 0; // выборочная дисперсия
    double expected_value = 0; // ожидаемое значение (математическое ожидание)
    double dispersion = 0; // дисперсия
    double sample_median = 0; // выборочная медиана
    double sample_range = 0; // размах выборки
    
    double p = 0.3; // вероятность правильного ответа на билет

    bool isOpened = false; // открыта ли программа
    bool isUpdated = false; // обновить ли данные

    int count_of_students = 10; // кол-во попыток

    std::vector<int> count_of_answered_tickets;

    std::vector<std::pair<int, int>> different_answered_tickets;

    ImGuiWindowFlags flagsForWindows = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    bool isDebug = false;

public:
    App()
    {
    }
    ~App() {
        ImGui::SFML::Shutdown();
    }

    void init() {

        m_window.create(sf::VideoMode(1920, 1080, 32), "TerVer");
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
        plotStyle.LineWeight = 3;

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

            input_count_of_students();
            input_p();

            create_button_for_start_calc();

            if (isOpened) {
                createTable();
                createGraph();
            }

            m_window.clear({ 255,255,255,100 });

            render();
        }
    }

    void create_button_for_start_calc() {

        ImGui::SetNextWindowPos({ 0,0 }); // устанавливаем позицию для создаваемого окна
        ImGui::SetNextWindowSize({ 350,80 }); // устанавливаем размер для создаваемого окна

        ImGui::Begin("Button", 0, flagsForWindows); // Создаем окно с заданными параметрами

        if (ImGui::Button("START", { 100,20 })) { // создаем кнопку с размерами 100*20
            get_data();
            get_different_answered_ticets();
            get_sample_mean();
            get_sample_variance();
            get_expected_value();
            get_dispersion();
            get_sample_median();
            get_sample_range();
        }

        ImGui::SameLine();
        ImGui::Text("Start Calc");

        ImGui::End();

    }

    void createTable() {

        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg; // настройки для таблицы

        ImGui::SetNextWindowPos({ 0,81 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 500,500}); // устанавливаем размер для создаваемого окна для таблицы

        ImGui::Begin("Table", 0, flagsForWindows); // создаем окно с заданными настройками

        if (ImGui::BeginTable("table1", different_answered_tickets.size(), flags)) // создаем таблицу с заданными настройками
        {
            for (size_t count = 0; count < different_answered_tickets.size(); ++count)
                ImGui::TableSetupColumn(std::to_string(different_answered_tickets[count].first).c_str(), ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            size_t i = 0;
            for (const auto& [count_ansewered, count] : different_answered_tickets) {
                ImGui::TableSetColumnIndex(i);
                ImGui::Text("%d", count);
                ++i;
            }
            i = 0;
            ImGui::TableNextRow();
            for (const auto& [count_ansewered, count] : different_answered_tickets) {
                ImGui::TableSetColumnIndex(i);
                ImGui::Text("%lf", (double)count / count_of_students);
                ++i;
            }
            ImGui::EndTable();
        }

        if (ImGui::BeginTable("table2", 8, flags)) // создаем таблицу с заданными настройками
        {
            ImGui::TableSetupColumn("x_bar", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("S^2", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("M", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("|M - x_bar|", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("D", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("|D - S^2|", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Me", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("R", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text((std::to_string(sample_mean)).c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text((std::to_string(sample_variance)).c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text((std::to_string(expected_value)).c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text((std::to_string(abs(expected_value - sample_mean))).c_str());
            ImGui::TableSetColumnIndex(4);
            ImGui::Text((std::to_string(dispersion).c_str()));
            ImGui::TableSetColumnIndex(5);
            ImGui::Text((std::to_string(abs(dispersion - sample_variance)).c_str()));
            ImGui::TableSetColumnIndex(6);
            ImGui::Text((std::to_string(sample_median).c_str()));
            ImGui::TableSetColumnIndex(7);
            ImGui::Text((std::to_string(sample_range).c_str()));


            ImGui::EndTable();
        }

        ImGui::End();
    }
    void createGraph() {
        
        ImGui::SetNextWindowPos({ 961, 0 });
        ImGui::SetNextWindowSize({ 940,1080 });

        ImGui::Begin("Graph", 0, flagsForWindows);

        if (ImPlot::BeginPlot(u8"График", "x", "y", { 930,1000 })) { // Отрисовываем график

            double x_data[] = { 1, 2, 3, 4, 5 };
            double y_data[] = { 2, 3, 4, 5, 6 };
            ImPlot::PlotStairs("Stairs", x_data, y_data, 5);
            ImPlot::EndPlot(); // Заканчиваем отрисовку графика
        }

        ImGui::End(); // Удаляем окно
    }


    void render() {
        ImGui::SFML::Render(m_window);
        m_window.display();
    }

private:

    void get_dispersion() {

        double q = 1 - p;

        dispersion = q / (p * p);

    }
    void get_sample_mean() { // функция для получения выборочного среднего

        sample_mean = 0; // обнуляем переменную

        for (size_t count = 0; count < count_of_answered_tickets.size(); ++count)
            sample_mean += (double)count_of_answered_tickets[count];

        sample_mean /= count_of_students;
        
    }
    void get_sample_variance() { // функция для получения выборочной дисперсии

        sample_variance = 0; // обнуляем переменную

        for (size_t count = 0; count < count_of_answered_tickets.size(); ++count)
            sample_variance += (count_of_answered_tickets[count] - sample_mean) * (count_of_answered_tickets[count] - sample_mean);

        sample_variance /= count_of_students;

    }
    void get_expected_value() { // функция для получения мат. ожидания
        
        expected_value = (double)1 / p;

    }
    void get_sample_median() {

        sample_median = 0;

        if (count_of_students % 2 == 0) {
            size_t k = count_of_students / 2;

            sample_median = (count_of_answered_tickets[k - 1] + count_of_answered_tickets[k]) / 2;

        }
        else {
            size_t k = count_of_students / 2 + 1;

            sample_median = count_of_answered_tickets[k - 1];
        }

    }
    void get_sample_range() {

        sample_range = 0;
        
        sample_range = count_of_answered_tickets.front() + count_of_answered_tickets.back();
    }

    void get_different_answered_ticets() { // функция для получения мапы с различным числом правильно отвеченных билетов

        different_answered_tickets.clear();

        std::vector<std::pair<int, int>> temp(count_of_students, { -1, 0 });

        if (!isDebug) { // если ведем отладку
            for (size_t count = 0; count < count_of_answered_tickets.size(); ++count) {
                std::cout << "Student " << count + 1 << " anserewed on " << count_of_answered_tickets[count] << " tickets!" << std::endl;
            }
        }

        for (size_t count = 0; count < count_of_answered_tickets.size(); ++count) { // проходим по всем попыткам студента (данный вектор содержит кол-во правильно отвченных билетов в каждой попытке)

            temp[count_of_answered_tickets[count]].first = count_of_answered_tickets[count];
            temp[count_of_answered_tickets[count]].second += 1;

        }

        for (size_t count = 0; count < temp.size(); ++count) {
            if (temp[count].first != -1)
                different_answered_tickets.push_back(temp[count]);
        }

    }
    void get_data() { // функция для получения числа правильно отвеченных билетов

        count_of_answered_tickets.clear(); // очищаем вектор с числом отвеченных билетов

        isOpened = true; // открыты ли данные = да
        isUpdated = true; // обновлены ли данные = да

        double t = 0.0; // рандомное число

        for (size_t count = 0; count < count_of_students; ++count) { // проходим циклом по кол-ву попыток

            int i = 0; // число билетов на который дали правильный ответ (для текущей попытки)
           
            while (true) {

                t = ((double)rand()) / RAND_MAX; // получаем рандомное число

                if (t <= p) // если это число меньше вероятности правильного ответа
                    ++i; // то увеличиваем счетчик на +1
                else { // иначе
                    count_of_answered_tickets.push_back(i); // сохраняем кол-во верно отвеченных билетов
                    break; // выходим из цикла
                }
            }
        }

        bubble_sort();
    }

    void bubble_sort() {

        for (size_t i = 0; i < count_of_answered_tickets.size(); ++i) {

            bool flag = true;

            for (size_t j = 0; j < count_of_answered_tickets.size() - (i + 1); ++j) {
                if (count_of_answered_tickets[j] > count_of_answered_tickets[j + 1]) {
                    
                    flag = false;
                    
                    std::swap(count_of_answered_tickets[j], count_of_answered_tickets[j + 1]);
                
                }
            }

            if (flag)
                break;

        }

    }

    void input_p() { // функция для ввода вероятности

        ImGui::SetNextWindowPos({ 351,0 }); // устанавливаем позицию для создаваемого окна
        ImGui::SetNextWindowSize({ 350,80 }); // устанавливаем размер для создаваемого окна

        ImGui::Begin("input_p", 0, flagsForWindows);

        ImGui::SeparatorText("Input p"); // дополнительный текст в окне

        if (ImGui::InputDouble(" ", &p, 0.01f, 1.0f, "%.8f")) { // ввод вероятности

            if (p < 0 || p > 1) // если вероятность больше 1 или меньше 0, то изменяем на 0.3 (дефолтное значение)
                p = 0.3;

            isOpened = false; // булева переменная на закрыть данные
        
        }

        ImGui::End();
    }
    void input_count_of_students() { // функция для ввода кол-ва попыток

        ImGui::SetNextWindowPos({ 702,0 }); // устанавливаем позицию для создаваемого окна
        ImGui::SetNextWindowSize({ 350,80 }); // устанавливаем размер для создаваемого окна

        ImGui::Begin("input_count_of_stidents", 0, flagsForWindows); // создаем окно

        ImGui::SeparatorText("Input count of students"); // дополнительный текст в окне

        if (ImGui::InputInt(" ", &count_of_students, 1, 100)) { // ввод данных
            if (count_of_students <= 0) // если число попыток меньше или равно 0, то заменяем значение на 1
                count_of_students = 1;

            count_of_answered_tickets.clear(); // очищаем вектор с кол-вом правильных ответов
            count_of_answered_tickets.resize(count_of_students); // изменяем размер на кол-во попыток

            isOpened = false; // булева переменная на закрыть данные
        }

        ImGui::End();
    }

};