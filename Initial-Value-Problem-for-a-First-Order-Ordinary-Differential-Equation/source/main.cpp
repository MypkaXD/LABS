#include "imgui.h"
#include "imgui-SFML.h"
#include "implot.h"

#include "imconfig.h"
#include "imconfig-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>

#include "App.h"

int main() {
    
    App app;
    app.init();
    app.run();

    return 0;
}