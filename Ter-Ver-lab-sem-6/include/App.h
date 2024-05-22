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
    double max_deviation = 0; // максимальное отклонение

    double p = 0.3; // вероятность правильного ответа на билет

    bool isOpened = false; // открыта ли программа
    bool isUpdated = false; // обновить ли данные

    int count_of_students = 10; // кол-во попыток

    std::vector<int> count_of_answered_tickets;

    std::vector<std::pair<int, int>> different_answered_tickets;

    std::vector<double> x_coords_of_sample_function;
    std::vector<double> y_coords_of_sample_function;
    
    std::vector<double> x_coords_of_teor_function;
    std::vector<double> y_coords_of_teor_function;

    int k = 10;
    std::vector<double> border_of_intervals;

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
                createInfo();
                createInputIntervals();
            }

            m_window.clear({ 255,255,255,100 });

            render();
        }
    }

    void createInputIntervals() {

        ImGui::SetNextWindowPos({ 0,500 }); // устанавливаем позицию для создаваемого окна
        ImGui::SetNextWindowSize({ 318,80 }); // устанавливаем размер для создаваемого окна

        ImGui::Begin("InputK", 0, flagsForWindows); // Создаем окно с заданными параметрами


        if (ImGui::InputInt(" ", &k, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal)) { // ввод данных
            if (k <= 0) // если число попыток меньше или равно 0, то заменяем значение на 1
                k = 1;

            isOpened = false; // булева переменная на закрыть данные
        }

        ImGui::End();

    }

    void create_button_for_start_calc() {

        ImGui::SetNextWindowPos({ 0,0 }); // устанавливаем позицию для создаваемого окна
        ImGui::SetNextWindowSize({ 318,80 }); // устанавливаем размер для создаваемого окна

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

            get_data_of_sample_function_for_graph();
            get_data_of_teor_function_for_graph();

            get_max_deviation();
        }

        ImGui::SameLine();
        ImGui::Text(u8"Вычислить");

        ImGui::End();

    }

    void createTable() {

        ImGuiTableFlags flags = ImGuiTableFlags_Borders |ImGuiTableFlags_NoHostExtendX; // настройки для таблицы

        ImGui::SetNextWindowPos({ 0,222 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 960,235 }); // устанавливаем размер для создаваемого окна для таблицы

        ImGui::Begin("Table", 0, flagsForWindows); // создаем окно с заданными настройками

        ImGui::BeginChild("firstTable", { 960, 70 });

        if (ImGui::BeginTable("table1", different_answered_tickets.size() + 1, ImGuiTableFlags_Borders | (different_answered_tickets.size() > 18 ? ImGuiTableFlags_ScrollX : 0) | ImGuiTableFlags_NoHostExtendX)) // создаем таблицу с заданными настройками
        {
            ImGui::TableSetupColumn(u8"x_i", ImGuiTableColumnFlags_WidthFixed);
            for (size_t count = 0; count < different_answered_tickets.size(); ++count)
                ImGui::TableSetupColumn(std::to_string(different_answered_tickets[count].first).c_str(), ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            
            size_t i = 0;
            
            ImGui::TableSetColumnIndex(i++);
            ImGui::Text(u8"n_i");

            for (const auto& [count_ansewered, count] : different_answered_tickets) {
                ImGui::TableSetColumnIndex(i);
                ImGui::Text("%d", count);
                ++i;
            }
            i = 0;
            
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(i++);
            ImGui::Text(u8"n_i / n");

            for (const auto& [count_ansewered, count] : different_answered_tickets) {
                ImGui::TableSetColumnIndex(i);
                ImGui::Text("%lf", (double)count / count_of_students);
                ++i;
            }
            ImGui::EndTable();
        }

        ImGui::EndChild();

        ImGui::BeginChild("secondTable", {960, 60});

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

        ImGui::EndChild();
        ImGui::BeginChild("ThirdTable", { 960, 80 });

        if (ImGui::BeginTable("table3", different_answered_tickets.size(), ImGuiTableFlags_Borders | (different_answered_tickets.size() > 17 ? ImGuiTableFlags_ScrollX : 0) | ImGuiTableFlags_NoHostExtendX)) // создаем таблицу с заданными настройками
        {
            for (size_t count = 0; count < different_answered_tickets.size(); ++count)
                ImGui::TableSetupColumn(std::to_string(different_answered_tickets[count].first).c_str(), ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();

            for (size_t count = 0; count < different_answered_tickets.size(); ++count) {
                ImGui::TableSetColumnIndex(count);
                ImGui::Text(std::to_string(y_coords_of_teor_function[different_answered_tickets[count].first + 1]).c_str());
            }

            ImGui::TableNextRow();

            for (size_t count = 0; count < different_answered_tickets.size(); ++count) {
                ImGui::TableSetColumnIndex(count);
                ImGui::Text("%.12lf", (double)different_answered_tickets[count].second / count_of_students);
            }

            ImGui::EndTable();
        }
        ImGui::EndChild();

        ImGui::End();
    }
    void createGraph() {

        ImGui::SetNextWindowPos({ 961, 0 });
        ImGui::SetNextWindowSize({ 940,1080 });

        ImGui::Begin("Graph", 0, flagsForWindows);

        if (ImPlot::BeginPlot(u8"График", "x", "y", { 930,1000 })) { // Отрисовываем график

            ImPlot::PlotStairs(u8"Выборочная Fn(x)", x_coords_of_sample_function.data(), y_coords_of_sample_function.data(), x_coords_of_sample_function.size());
            ImPlot::PlotStairs(u8"Теоретическая Fn(x)", x_coords_of_teor_function.data(), y_coords_of_teor_function.data(), x_coords_of_teor_function.size());
            ImPlot::EndPlot(); // Заканчиваем отрисовку графика
        }

        ImGui::End(); // Удаляем окно
    }
    void createInfo() {

        ImGui::SetNextWindowPos({ 0, 81 });
        ImGui::SetNextWindowSize({ 960,140 });

        ImGui::Begin("Info", 0, flagsForWindows);

        ImGui::Text(u8"Вариант 14");
        ImGui::Text(u8"Студенту на зачёте задаются вопросы, которые прекращаются, если студент на заданный вопрос не ответит. Вероятность ответа на каждый вопрос\nнезависимо от других равна p. C.в. x - число полученных ответов.");
        ImGui::Text(u8"Геометрическое распределение");
        ImGui::Text(u8"Вероятность правильного ответа p = %lf", p);
        ImGui::Text(u8"Вероятность неправильного ответа q = 1 - p = %lf", 1 - p);
        ImGui::Text(u8"Максимальное отклонение: %lf", max_deviation);

        ImGui::End();

    }

    void render() {
        ImGui::SFML::Render(m_window);
        m_window.display();
    }

private:

    void get_dispersion() {

        double q = 1 - p;

        dispersion = p / (q * q);

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

        double q = 1 - p;

        expected_value = p / q;

    }
    void get_sample_median() {

        sample_median = 0;

        if (count_of_students % 2 == 0) {
            size_t k = count_of_students / 2;

            sample_median = (double)(count_of_answered_tickets[k - 1] + count_of_answered_tickets[k]) / 2;

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
    void get_max_deviation() {

        max_deviation = 0;

       
        for (size_t count = 1; count < x_coords_of_sample_function.size() - 1; ++count) {

            double current_deviation_first = 0;
            double current_deviation_second = 0;

            current_deviation_first = abs(y_coords_of_teor_function[count] - y_coords_of_sample_function[count]);
            current_deviation_second = abs(y_coords_of_teor_function[count] - y_coords_of_sample_function[count]);

            if (current_deviation_first >= max_deviation || current_deviation_second >= max_deviation)
                max_deviation = std::max(current_deviation_first, current_deviation_second);

        }

    }
    /*
    int babunbubunbubz()
    {
        int salo = 8.9;
        return salo;
    }
    // squazimabzabza
    */

    void get_different_answered_ticets() { // функция для получения мапы с различным числом правильно отвеченных билетов

        different_answered_tickets.clear();

        std::vector<std::pair<int, int>> temp(*std::max_element(count_of_answered_tickets.begin(), count_of_answered_tickets.end()) + 1, {-1, 0});

        if (isDebug) { // если ведем отладку
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
    void get_data_of_sample_function_for_graph() {

        size_t size = different_answered_tickets.back().first + 3;
        
        x_coords_of_sample_function.clear();
        x_coords_of_sample_function.resize(size);

        y_coords_of_sample_function.clear();
        y_coords_of_sample_function.resize(size);

        x_coords_of_sample_function[0] = 0;
        y_coords_of_sample_function[0] = 0;

        x_coords_of_sample_function[size - 1] = different_answered_tickets.back().first + 2;
        y_coords_of_sample_function[size - 1] = 1;

        size_t i = 1;

        for (size_t count = 1; count < size - 1; ++count) {
            
            x_coords_of_sample_function[count] = count - 1;

            if (different_answered_tickets[i - 1].first == count - 1) {
                y_coords_of_sample_function[count] = (double)different_answered_tickets[i-1].second / count_of_students + y_coords_of_sample_function[count - 1];
                ++i;
            }
            else    
                y_coords_of_sample_function[count] = y_coords_of_sample_function[count - 1];

        }
    }
    void get_data_of_teor_function_for_graph() {

        int n = log(0.005) / log(p);

        if (n <= different_answered_tickets.back().first) {
            n = different_answered_tickets.back().first;
        }

        x_coords_of_teor_function.clear();
        x_coords_of_teor_function.resize(n + 3);

        y_coords_of_teor_function.clear();
        y_coords_of_teor_function.resize(n + 3);

        x_coords_of_teor_function[0] = 0;
        y_coords_of_teor_function[0] = 0;

        for (size_t count = 1; count < y_coords_of_teor_function.size() - 1; ++count) {
            x_coords_of_teor_function[count] = count-1;
            y_coords_of_teor_function[count] = std::pow(p, count - 1) * (1 - p) + y_coords_of_teor_function[count-1];
        }

        x_coords_of_teor_function[x_coords_of_teor_function.size() - 1] = x_coords_of_teor_function[x_coords_of_teor_function.size() - 2] + 1;
        y_coords_of_teor_function[x_coords_of_teor_function.size() - 1] = y_coords_of_teor_function[x_coords_of_teor_function.size() - 2];
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

        ImGui::SetNextWindowPos({ 319,0 }); // устанавливаем позицию для создаваемого окна
        ImGui::SetNextWindowSize({ 320,80 }); // устанавливаем размер для создаваемого окна

        ImGui::Begin("input_p", 0, flagsForWindows);

        ImGui::SeparatorText(u8"Введите вероятность"); // дополнительный текст в окне

        if (ImGui::InputDouble(" ", &p, 0.01f, 1.0f, "%.8f")) { // ввод вероятности

            if (p < 0 || p > 1) // если вероятность больше 1 или меньше 0, то изменяем на 0.3 (дефолтное значение)
                p = 0.3;

            isOpened = false; // булева переменная на закрыть данные
        
        }

        ImGui::End();
    }
    void input_count_of_students() { // функция для ввода кол-ва попыток

        ImGui::SetNextWindowPos({ 640,0 }); // устанавливаем позицию для создаваемого окна
        ImGui::SetNextWindowSize({ 320,80 }); // устанавливаем размер для создаваемого окна

        ImGui::Begin("input_count_of_stidents", 0, flagsForWindows); // создаем окно

        ImGui::SeparatorText(u8"Кол-во попыток"); // дополнительный текст в окне

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