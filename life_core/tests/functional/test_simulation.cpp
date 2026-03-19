#include <gtest/gtest.h>

#include <gol/simulation.h>

// WHEN trying to create simulation
// THEN operation finishes successfully and simulation handle is returned
TEST(SimulationTest, CanCreateSimulation) {
  gol_simulation simulation = nullptr;
  gol_result result = gol_simulation_create(&simulation);
  EXPECT_EQ(result, GOL_RESULT_SUCCESS);
  EXPECT_NE(simulation, nullptr);
}
