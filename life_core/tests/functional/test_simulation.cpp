#include <gol/simulation.h>
#include <gtest/gtest.h>
#include <vector>

inline bool check_pos_equality(const gol_grid_position &left,
                               const gol_grid_position &right) {
    return left.x == right.x && left.y == right.y;
}

inline bool check_cell_equality(const gol_cell &left, const gol_cell &right) {
    return check_pos_equality(left.pos, right.pos) &&
           left.is_alive == right.is_alive;
}

class SimulationTest : public ::testing::Test {
protected:
    gol_simulation simulation = nullptr;
    gol_grid_scalar simulation_size = 10;

    void SetUp() override {
        gol_result result = gol_simulation_create(&simulation, simulation_size);
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

// WHEN creating simulation with maximum size
// THEN simulation is created successfully
TEST_F(SimulationTest, CanCreateBigSimulation) {
    gol_grid_scalar size = INT32_MAX;
    gol_simulation big_simulation = nullptr;
    gol_result result = gol_simulation_create(&big_simulation, size);
    ASSERT_EQ(result, GOL_RESULT_SUCCESS);
    ASSERT_NE(big_simulation, nullptr);
    result = gol_simulation_destroy(big_simulation);
    ASSERT_EQ(result, GOL_RESULT_SUCCESS);
}

// WHEN creating simulation with incorrect size
// THEN simulation is not created
TEST_F(SimulationTest, RejectsIncorrectlySizedSimulation) {
    gol_grid_scalar size = 0;
    gol_simulation new_simulation = nullptr;
    gol_result result = gol_simulation_create(&new_simulation, size);
    ASSERT_EQ(result, GOL_RESULT_ERROR);
    size = -1;
    result = gol_simulation_create(&new_simulation, size);
    ASSERT_EQ(result, GOL_RESULT_ERROR);
}

// GIVEN valid simulation
// WHEN cell operations are performed
// THEN out of bounds operations fail and do not affect valid operations
TEST_F(SimulationTest, CanQueryAndSetOutOfBoundPositions) {
    constexpr gol_size operations = 8;
    constexpr gol_size failing_ops = 4;
    gol_grid_position positions[operations] = {
        gol_grid_position{0, simulation_size},
        gol_grid_position{0, -simulation_size},
        gol_grid_position{simulation_size, 0},
        gol_grid_position{-simulation_size, 0},
        gol_grid_position{0, simulation_size - 1},
        gol_grid_position{0, -simulation_size + 1},
        gol_grid_position{simulation_size - 1, 0},
        gol_grid_position{-simulation_size + 1, 0},
    };
    gol_cell cells[operations] = {};
    gol_result results[operations];
    for (int i = 0; i < operations; i++) {
        results[i] = GOL_RESULT_ERROR;
        cells[i].pos = positions[i];
        cells[i].is_alive = GOL_TRUE;
    }
    gol_result result =
        gol_simulation_set_cells(simulation, operations, cells, results);
    ASSERT_EQ(result, GOL_RESULT_SUCCESS);
    gol_cell query[operations] = {};
    ASSERT_EQ(
        gol_simulation_query_cells(simulation, operations, positions, query),
        GOL_RESULT_SUCCESS);
    for (int i = 0; i < failing_ops; i++) {
        EXPECT_EQ(results[i], GOL_RESULT_OUT_OF_GRID_BOUNDS);
        EXPECT_TRUE(check_pos_equality(query[i].pos, positions[i]));
        EXPECT_EQ(query[i].is_alive, GOL_FALSE);
    }
    for (int i = failing_ops; i < operations; i++) {
        EXPECT_EQ(results[i], GOL_RESULT_SUCCESS);
        EXPECT_TRUE(check_pos_equality(query[i].pos, positions[i]));
        EXPECT_EQ(query[i].is_alive, GOL_TRUE);
    }
}

// GIVEN valid simulation
// WHEN simulation advances to next frame
// THEN simulation does not spawn out of bound cells
TEST_F(SimulationTest, DoesNotSpawnOutOfBoundCells) {
    constexpr gol_size op_count = 12;
    gol_grid_scalar border_pos = simulation_size - 1;
    gol_cell cells[op_count] = {
        gol_cell{gol_grid_position{1, border_pos}, GOL_TRUE},
        gol_cell{gol_grid_position{0, border_pos}, GOL_TRUE},
        gol_cell{gol_grid_position{-1, border_pos}, GOL_TRUE},
        gol_cell{gol_grid_position{1, -border_pos}, GOL_TRUE},
        gol_cell{gol_grid_position{0, -border_pos}, GOL_TRUE},
        gol_cell{gol_grid_position{-1, -border_pos}, GOL_TRUE},
        gol_cell{gol_grid_position{border_pos, 1}, GOL_TRUE},
        gol_cell{gol_grid_position{border_pos, 0}, GOL_TRUE},
        gol_cell{gol_grid_position{border_pos, -1}, GOL_TRUE},
        gol_cell{gol_grid_position{-border_pos, 1}, GOL_TRUE},
        gol_cell{gol_grid_position{-border_pos, 0}, GOL_TRUE},
        gol_cell{gol_grid_position{-border_pos, -1}, GOL_TRUE},

    };
    constexpr gol_size possible_spawn_count = 4;
    gol_grid_position possible_spawn_positions[possible_spawn_count] = {
        gol_grid_position{0, simulation_size},
        gol_grid_position{0, -simulation_size},
        gol_grid_position{simulation_size, 0},
        gol_grid_position{-simulation_size, 0}};
    ASSERT_EQ(gol_simulation_set_cells(simulation, op_count, cells, nullptr),
              GOL_RESULT_SUCCESS);
    ASSERT_EQ(gol_simulation_next_frame(simulation), GOL_RESULT_SUCCESS);

    // Check possible out of bound spawns
    gol_cell possible_spawns[possible_spawn_count] = {};
    gol_grid_position query_pos = gol_grid_position{0, simulation_size};
    ASSERT_EQ(gol_simulation_query_cells(simulation, possible_spawn_count,
                                         possible_spawn_positions,
                                         possible_spawns),
              GOL_RESULT_SUCCESS);
    for (int i = 0; i < possible_spawn_count; i++) {
        EXPECT_EQ(possible_spawns[i].is_alive, GOL_FALSE);
    }

    // Check alive cells
    gol_size alive_count = 0;
    ASSERT_EQ(gol_simulation_get_alive_cells(simulation, &alive_count, nullptr),
              GOL_RESULT_SUCCESS);
    std::vector<gol_cell> alive_cells(alive_count);
    ASSERT_EQ(gol_simulation_get_alive_cells(simulation, &alive_count,
                                             alive_cells.data()),
              GOL_RESULT_SUCCESS);
    for (auto &e : alive_cells) {
        for (int i = 0; i < possible_spawn_count; i++) {
            EXPECT_FALSE(
                check_pos_equality(e.pos, possible_spawn_positions[i]));
        }
    }

    // Check updated cells
    gol_size updated_count = 0;
    ASSERT_EQ(
        gol_simulation_get_updated_cells(simulation, &updated_count, nullptr),
        GOL_RESULT_SUCCESS);
    std::vector<gol_cell> updated_cells(updated_count);
    ASSERT_EQ(gol_simulation_get_updated_cells(simulation, &updated_count,
                                               updated_cells.data()),
              GOL_RESULT_SUCCESS);
    for (auto &e : updated_cells) {
        for (int i = 0; i < possible_spawn_count; i++) {
            EXPECT_FALSE(
                check_pos_equality(e.pos, possible_spawn_positions[i]));
        }
    }
}

// GIVEN valid simulation
// WHEN setting and querying cells at specific positions
// THEN all operations are processed correctly
TEST_F(SimulationTest, CanSetAndQueryPositions) {
    constexpr gol_size op_count = 3;
    gol_grid_position positions[op_count] = {gol_grid_position{1, 0},
                                             gol_grid_position{0, 0},
                                             gol_grid_position{-1, 0}};
    gol_cell cells[op_count] = {};
    gol_result results[op_count];
    for (int i = 0; i < op_count; i++) {
        results[i] = GOL_RESULT_ERROR;
        cells[i].pos = positions[i];
        cells[i].is_alive = GOL_TRUE;
    }
    ASSERT_EQ(gol_simulation_set_cells(simulation, op_count, cells, results),
              GOL_RESULT_SUCCESS);
    for (int i = 0; i < op_count; i++) {
        EXPECT_EQ(results[i], GOL_RESULT_SUCCESS);
    }
    cells[0].is_alive = GOL_FALSE;
    ASSERT_EQ(gol_simulation_set_cells(simulation, 2, cells, nullptr),
              GOL_RESULT_SUCCESS);
    gol_cell query[op_count] = {};
    ASSERT_EQ(
        gol_simulation_query_cells(simulation, op_count, positions, query),
        GOL_RESULT_SUCCESS);
    for (int i = 0; i < op_count; i++) {
        EXPECT_TRUE(check_cell_equality(query[i], cells[i]));
    }
}

// GIVEN valid simulation
// WHEN querying all alive cells
// THEN all alive cells are reported correctly
TEST_F(SimulationTest, CanQueryAliveCells) {
    // No alive cells at the beginning
    gol_size alive_count = 0;
    ASSERT_EQ(gol_simulation_get_alive_cells(simulation, &alive_count, nullptr),
              GOL_RESULT_SUCCESS);
    EXPECT_EQ(alive_count, 0);

    // Check alive cells after set operation
    constexpr gol_size op_count = 3;
    gol_cell cells[op_count] = {
        gol_cell{gol_grid_position{0, 0}, GOL_TRUE},
        gol_cell{gol_grid_position{1, 0}, GOL_TRUE},
        gol_cell{gol_grid_position{2, 0}, GOL_TRUE},
    };
    ASSERT_EQ(gol_simulation_set_cells(simulation, op_count, cells, nullptr),
              GOL_RESULT_SUCCESS);
    ASSERT_EQ(gol_simulation_get_alive_cells(simulation, &alive_count, nullptr),
              GOL_RESULT_SUCCESS);
    EXPECT_EQ(alive_count, op_count);
    std::vector<gol_cell> alive_cells(alive_count);
    ASSERT_EQ(gol_simulation_get_alive_cells(simulation, &alive_count,
                                             alive_cells.data()),
              GOL_RESULT_SUCCESS);
    EXPECT_EQ(alive_count, op_count);
    int equals[op_count] = {};
    for (int i = 0; i < alive_count; i++) {
        EXPECT_EQ(alive_cells[i].is_alive, GOL_TRUE);
        for (int j = 0; j < op_count; j++) {
            if (check_cell_equality(alive_cells[i], cells[j])) {
                equals[j]++;
            }
        }
    }
    for (int i = 0; i < op_count; i++) {
        EXPECT_EQ(equals[i], 1);
    }

    // Check alive cells after frame update
    ASSERT_EQ(gol_simulation_next_frame(simulation), GOL_RESULT_SUCCESS);
    ASSERT_EQ(gol_simulation_get_alive_cells(simulation, &alive_count, nullptr),
              GOL_RESULT_SUCCESS);
    constexpr gol_size expected_count = 3;
    std::vector<gol_cell> new_alive_cells(alive_count);
    EXPECT_EQ(alive_count, expected_count);
    ASSERT_EQ(gol_simulation_get_alive_cells(simulation, &alive_count,
                                             new_alive_cells.data()),
              GOL_RESULT_SUCCESS);
    gol_grid_position expected_positions[expected_count] = {
        gol_grid_position{1, 1}, gol_grid_position{1, 0},
        gol_grid_position{1, -1}};
    int matches[expected_count] = {};
    for (int i = 0; i < alive_count; i++) {
        EXPECT_EQ(new_alive_cells[i].is_alive, GOL_TRUE);
        for (int j = 0; j < expected_count; j++) {
            if (check_pos_equality(new_alive_cells[i].pos,
                                   expected_positions[j])) {
                matches[j]++;
            }
        }
    }
    for (int i = 0; i < expected_count; i++) {
        EXPECT_EQ(matches[i], 1);
    }
}

// GIVEN valid simulation
// WHEN querying all updated cells
// THEN all cells that were updated by the simulation in the last frame are
// reported correctly
TEST_F(SimulationTest, CanQueryUpdatedCells) {
    // No updated cells at the beinning
    gol_size updated_count = 0;
    ASSERT_EQ(
        gol_simulation_get_updated_cells(simulation, &updated_count, nullptr),
        GOL_RESULT_SUCCESS);
    EXPECT_EQ(updated_count, 0);

    // Check updated cells after set operation
    constexpr gol_size op_count = 3;
    gol_cell cells[op_count] = {
        gol_cell{gol_grid_position{0, 0}, GOL_TRUE},
        gol_cell{gol_grid_position{1, 0}, GOL_TRUE},
        gol_cell{gol_grid_position{2, 0}, GOL_TRUE},
    };
    ASSERT_EQ(gol_simulation_set_cells(simulation, op_count, cells, nullptr),
              GOL_RESULT_SUCCESS);
    ASSERT_EQ(
        gol_simulation_get_updated_cells(simulation, &updated_count, nullptr),
        GOL_RESULT_SUCCESS);
    EXPECT_EQ(updated_count, 0);

    // Check updated cells after frame update
    ASSERT_EQ(gol_simulation_next_frame(simulation), GOL_RESULT_SUCCESS);
    ASSERT_EQ(
        gol_simulation_get_updated_cells(simulation, &updated_count, nullptr),
        GOL_RESULT_SUCCESS);
    constexpr gol_size expected_update_count = 4;
    EXPECT_EQ(updated_count, expected_update_count);
    std::vector<gol_cell> updated_cells(updated_count);
    ASSERT_EQ(gol_simulation_get_updated_cells(simulation, &updated_count,
                                               updated_cells.data()),
              GOL_RESULT_SUCCESS);
    gol_cell expected_updates[expected_update_count] = {
        gol_cell{gol_grid_position{1, 1}, GOL_TRUE},
        gol_cell{gol_grid_position{1, -1}, GOL_TRUE},
        gol_cell{gol_grid_position{0, 0}, GOL_FALSE},
        gol_cell{gol_grid_position{2, 0}, GOL_FALSE}};
    int matches[expected_update_count] = {};
    for (auto &e : updated_cells) {
        for (int i = 0; i < expected_update_count; i++) {
            if (check_cell_equality(e, expected_updates[i])) {
                matches[i]++;
            }
        }
    }
    for (int i = 0; i < expected_update_count; i++) {
        EXPECT_EQ(matches[i], 1);
    }
}

// GIVEN valid simulation
// WHEN grid access profiling is queried
// THEN profiling data is reported without error
TEST_F(SimulationTest, CanQueryGridAccessProfiling) {
    gol_simulation_property_grid_access_profiling profiling{100, 100};
    ASSERT_EQ(gol_simulation_query_property(
                  simulation,
                  GOL_SIMULATION_PROPERTY_TYPE_GRID_ACCESS_PROFILING,
                  &profiling),
              GOL_RESULT_SUCCESS);
    EXPECT_EQ(profiling.hit_count, 0);
    EXPECT_EQ(profiling.miss_count, 0);
}
