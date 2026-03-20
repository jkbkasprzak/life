#include <cassert>
#include <gtest/gtest.h>

#include <gol/simulation.h>


class SimulationTest : public ::testing::Test {
protected:
  gol_simulation simulation = nullptr;

  void SetUp() override {
    gol_result result = gol_simulation_create(&simulation);
    ASSERT_EQ(result, GOL_RESULT_SUCCESS);
    ASSERT_NE(simulation, nullptr);
  }

  void TearDown() override {
    gol_result result = gol_simulation_destroy(simulation);
    ASSERT_EQ(result, GOL_RESULT_SUCCESS);
  }
};

// GIVEN valid simulation
// WHEN trying to proceed to next simulation step
// THEN simulation step is updated correctly
TEST_F(SimulationTest, CanMakeStep) {
  gol_result result = GOL_RESULT_ERROR;
  gol_size step = 1000;
  result = gol_simulation_get_step(simulation, &step);
  ASSERT_EQ(result, GOL_RESULT_SUCCESS);
  EXPECT_EQ(step, 0);
  result = gol_simulation_step(simulation);
  ASSERT_EQ(result, GOL_RESULT_SUCCESS);
  result = gol_simulation_get_step(simulation, &step);
  ASSERT_EQ(result, GOL_RESULT_SUCCESS);
  EXPECT_EQ(step, 1);
}
