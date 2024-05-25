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

	srand(0);

    App app;
    app.init();
    app.run();

    //std::cout << (double)1/calc_gamma(1) << std::endl;
    
    return 0;
}