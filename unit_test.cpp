#include "../succession2/mainwindow.h"
#include <catch.hpp>

TEST_CASE("MainWindow::MainWindow()")
{
    MainWindow.number_of_simulation_years = 0;
    REQUIRE(MainWindow.number_of_simulation_years == 0);
}
