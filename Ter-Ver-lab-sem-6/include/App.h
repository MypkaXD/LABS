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

public:
    App()
    {
    }
    ~App() {
        ImGui::SFML::Shutdown();
    }

    void init() {
        m_window.create(sf::VideoMode(1920, 1080, 32), "Te");
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
        //plotStyle.Colors[ImPlotCol_Line] = ImVec4(255, 0.0f, 0.0f, 1.0f);  // ������� ����
        plotStyle.LineWeight = 3;  // ������� �����

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

            static bool isOpened = false;
            static bool isUpdated = false;

            static int count_of_students = 10;

            static double p = 0.3;

            static std::vector<int> count_of_answered_tickets;

            static std::map<int, int> counter;

            input_count_of_students(flagsForWindows, count_of_students, count_of_answered_tickets, isOpened);
            input_p(flagsForWindows, p, isOpened);

            create_button_for_start_calc(flagsForWindows, count_of_students, count_of_answered_tickets, p, isOpened, isUpdated);

            if (isOpened) {
                if (isUpdated)
                    create_data_for_table(count_of_answered_tickets, counter, isUpdated);
                createTable(flagsForWindows, count_of_answered_tickets, count_of_students, counter);
            }

            m_window.clear({ 255,255,255,100 });

            render();
        }
    }

    void create_data_for_table(std::vector<int>& count_of_answered_tickets, std::map<int, int>& counter, bool& isUpdated) {

        counter.clear();

        for (size_t count = 0; count < count_of_answered_tickets.size(); ++count) {
            std::cout << "Student " << count + 1 << " anserewed on " << count_of_answered_tickets[count] << " tickets!" << std::endl;
        }

        for (size_t count = 0; count < count_of_answered_tickets.size(); ++count) {
            if (counter.find(count_of_answered_tickets[count]) != counter.end()) {
                counter[count_of_answered_tickets[count]] += 1;
            }
            else {
                counter.insert({count_of_answered_tickets[count],1});
            }
        }

        isUpdated = false;
    }

    void create_button_for_start_calc(ImGuiWindowFlags& flagsForWindows, int& count_of_students, std::vector<int>& count_of_answered_tickets, double& p, bool& isOpened
        , bool& isUpdated) {

        ImGui::SetNextWindowPos({ 0,0 }); // устанавливаем позицию для создаваемого окна
        ImGui::SetNextWindowSize({ 350,80 }); // устанавливаем размер для создаваемого окна

        ImGui::Begin("Button", 0, flagsForWindows); // Создаем окно с заданными параметрами

        if (ImGui::Button("START", { 100,20 })) { // создаем кнопку с размерами 100*20

            count_of_answered_tickets.clear();

            isOpened = true;
            isUpdated = true;

            double t = 0.0;

            for (size_t count = 0; count < count_of_students; ++count) {
                int i = 0;
                while (true) {

                    t = ((double)rand()) / RAND_MAX;
                    if (t <= p) {
                        ++i;
                    }
                    else {
                        count_of_answered_tickets.push_back(i);
                        break;
                    }
                }
            }
        }

        ImGui::SameLine();
        ImGui::Text("Start Calc");

        ImGui::End();

    }

    void input_p(ImGuiWindowFlags& flagsForWindows, double& p, bool& isOpened) {

        ImGui::SetNextWindowPos({ 351,0 }); // устанавливаем позицию для создаваемого окна
        ImGui::SetNextWindowSize({ 350,80 }); // устанавливаем размер для создаваемого окна

        ImGui::Begin("input_p", 0, flagsForWindows);

        ImGui::SeparatorText("Input p"); // дополнительный текст в окне

        if (ImGui::InputDouble(" ", &p, 0.01f, 1.0f, "%.8f")) {
            if (p < 0 || p > 1)
                p = 1;

            isOpened = false;
        }

        ImGui::End();
    }

    void input_count_of_students(ImGuiWindowFlags& flagsForWindows, int& count_of_students, std::vector<int> count_of_answered_tickets, bool& isOpened) {

        ImGui::SetNextWindowPos({ 702,0 }); // устанавливаем позицию для создаваемого окна
        ImGui::SetNextWindowSize({ 350,80}); // устанавливаем размер для создаваемого окна

        ImGui::Begin("input_count_of_stidents", 0, flagsForWindows);

        ImGui::SeparatorText("Input count of students"); // дополнительный текст в окне

        if (ImGui::InputInt(" ", &count_of_students, 1, 100)) {
            if (count_of_students <= 0)
                count_of_students = 1;

            count_of_answered_tickets.clear();
            count_of_answered_tickets.resize(count_of_students);

            isOpened = false;
        }

        ImGui::End();
    }

    void createTable(ImGuiWindowFlags& flagsForWindows, std::vector<int>& count_of_answered_tickets, int& count_of_students, std::map<int, int>& counter) {

        static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg; // настройки для таблицы

        ImGui::SetNextWindowPos({ 0,81 }); // устанавливаем позицию для создаваемого окна для таблицы
        ImGui::SetNextWindowSize({ 1920,1080 }); // устанавливаем размер для создаваемого окна для таблицы
        ImGui::Begin("Table", 0, flagsForWindows); // создаем окно с заданными настройками

        if (ImGui::BeginTable("table1", counter.size(), flags)) // создаем таблицу с заданными настройками
        {
            if (true)
            {
                for (const auto& [count_ansewered, count] : counter)
                    ImGui::TableSetupColumn(std::to_string(count_ansewered).c_str(), ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableHeadersRow();
            }

            ImGui::TableNextRow();
            size_t i = 0;
            for (const auto& [count_ansewered, count] : counter) {
                ImGui::TableSetColumnIndex(i);
                ImGui::Text("%d", count);
                ++i;
            }
            i = 0;
            ImGui::TableNextRow();
            for (const auto& [count_ansewered, count] : counter) {
                ImGui::TableSetColumnIndex(i);
                ImGui::Text("%lf", (double)count / count_of_students);
                ++i;
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