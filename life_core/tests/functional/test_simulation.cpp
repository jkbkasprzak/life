#include <gol/simulation.h>
#include <gtest/gtest.h>

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
// WHEN simulation advances to next frame
// THEN simulation frame counter is updated
TEST_F(SimulationTest, CanCountFrames) {
    gol_result result = GOL_RESULT_ERROR;
    gol_size frame = 1000;
    result = gol_simulation_get_frame_count(simulation, &frame);
    ASSERT_EQ(result, GOL_RESULT_SUCCESS);
    EXPECT_EQ(frame, 0);
    result = gol_simulation_next_frame(simulation);
    ASSERT_EQ(result, GOL_RESULT_SUCCESS);
    result = gol_simulation_get_frame_count(simulation, &frame);
    ASSERT_EQ(result, GOL_RESULT_SUCCESS);
    EXPECT_EQ(frame, 1);
}

// GIVEN valid simulation
// WHEN setting simulation bounds
// THEN simulation bounds are updated
TEST_F(SimulationTest, CanSetBounds) {
    ASSERT_TRUE(false); // TODO
}

// GIVEN valid simulation with defined bounds
// WHEN cell operations are performed
// THEN out of bounds operation fail and do not affect valid operations
TEST_F(SimulationTest, FailOnOutOfBoundsOperations) {
    ASSERT_TRUE(false); // TODO
}

// GIVEN valid simulation with defined bounds
// WHEN simulation advances to next frame
// THEN simulation does not process out of bound cells
TEST_F(SimulationTest, SkipsOutOfBoundCells) {
    ASSERT_TRUE(false); // TODO
}

// GIVEN valid simulation
// WHEN querying cells at specific positions
// THEN all queried cells are reported correctly
TEST_F(SimulationTest, CanQueryPositions) {
    ASSERT_TRUE(false); // TODO
}

// GIVEN valid simulation
// WHEN setting cells at specific positions
// THEN all cells are updated correctly
TEST_F(SimulationTest, CanSetCells) {
    ASSERT_TRUE(false); // TODO
}

// GIVEN valid simulation
// WHEN querying all alive cells
// THEN all alive cells are reported correctly
TEST_F(SimulationTest, CanQueryAliveCells) {
    ASSERT_TRUE(false); // TODO
}

// GIVEN valid simulation
// WHEN querying all updated cells
// THEN all cells that were updated by the simulation in the last frame are
// reported correctly
TEST_F(SimulationTest, CanQueryUpdatedCells) {
    ASSERT_TRUE(false); // TODO
}

// GIVEN valid simulation
// WHEN simulation advances to next frame
// THEN cells are updated according to Conway's 23/3 rules
TEST_F(SimulationTest, FollowsConwaysRules) {
    ASSERT_TRUE(false); // TODO
}
