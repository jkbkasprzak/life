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
// WHEN creating another simulation
// THEN operation succeeds and returns different object
TEST_F(SimulationTest, CanCreateMultipleSimulations) {
    gol_grid_scalar size = INT32_MAX;
    gol_simulation another_simulation = nullptr;
    gol_result result = gol_simulation_create(&another_simulation);
    ASSERT_EQ(result, GOL_RESULT_SUCCESS);
    ASSERT_NE(another_simulation, nullptr);
    ASSERT_NE(another_simulation, simulation);
    result = gol_simulation_destroy(another_simulation);
    ASSERT_EQ(result, GOL_RESULT_SUCCESS);
}

// GIVEN valid simulation
// WHEN setting and querying cells at specific positions
// THEN operations succeed and query returns expected cells
TEST_F(SimulationTest, CanSetAndQueryPositions) {
    constexpr gol_size op_count = 4;
    constexpr gol_size set_count = op_count - 1;
    gol_grid_position positions[op_count] = {
        gol_grid_position{0, 0}, gol_grid_position{1, 0},
        gol_grid_position{2, 0}, gol_grid_position{3, 0}};
    gol_cell cells[op_count] = {};
    for (int i = 0; i < op_count; i++) {
        cells[i].pos = positions[i];
        cells[i].is_alive = i < set_count ? GOL_TRUE : GOL_FALSE;
    }
    ASSERT_EQ(gol_simulation_set_cells(simulation, set_count, cells),
              GOL_RESULT_SUCCESS);
    cells[0].is_alive = GOL_FALSE;
    ASSERT_EQ(gol_simulation_set_cells(simulation, set_count, cells),
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
    ASSERT_EQ(gol_simulation_set_cells(simulation, op_count, cells),
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
    alive_cells.clear();
    alive_count = 0;
    ASSERT_EQ(gol_simulation_next_frame(simulation), GOL_RESULT_SUCCESS);
    ASSERT_EQ(gol_simulation_get_alive_cells(simulation, &alive_count, nullptr),
              GOL_RESULT_SUCCESS);
    constexpr gol_size expected_count = 3;
    EXPECT_EQ(alive_count, expected_count);
    alive_cells.resize(alive_count);
    ASSERT_EQ(gol_simulation_get_alive_cells(simulation, &alive_count,
                                             alive_cells.data()),
              GOL_RESULT_SUCCESS);
    gol_grid_position expected_positions[expected_count] = {
        gol_grid_position{1, 1}, gol_grid_position{1, 0},
        gol_grid_position{1, -1}};
    int matches[expected_count] = {};
    for (int i = 0; i < alive_count; i++) {
        EXPECT_EQ(alive_cells[i].is_alive, GOL_TRUE);
        for (int j = 0; j < expected_count; j++) {
            if (check_pos_equality(alive_cells[i].pos, expected_positions[j])) {
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
    ASSERT_EQ(gol_simulation_set_cells(simulation, op_count, cells),
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

// GIVEN valid simulation
// WHEN simulation advances to next frame
// THEN cells are processed according to Conway's rules
TEST_F(SimulationTest, FollowsConwaysRules) {
    // Cell has 8 neighbors so 2^8 possible combinations
    constexpr uint32_t combinations = 0b100000000;
    constexpr uint32_t neighbors = 8;
    constexpr gol_grid_position tested_pos = gol_grid_position{0, 0};
    gol_grid_position positions[neighbors] = {
        gol_grid_position{-1, -1}, gol_grid_position{-1, 0},
        gol_grid_position{-1, 1},  gol_grid_position{0, -1},
        gol_grid_position{0, 1},   gol_grid_position{1, -1},
        gol_grid_position{1, 0},   gol_grid_position{1, 1}};
    for (uint32_t starts_alive = 0; starts_alive < 2; starts_alive++) {
        for (uint32_t i = 0; i < combinations; i++) {
            gol_simulation tmp_sim = nullptr;
            std::vector<gol_cell> alive_cells;
            alive_cells.reserve(9);
            uint32_t alive_neighbors = 0;
            if (starts_alive == 1) {
                alive_cells.push_back(gol_cell{tested_pos, GOL_TRUE});
            }
            for (uint32_t j = 0; j < neighbors; j++) {
                uint32_t bit = 0b1 << j;
                if ((i & bit) != 0) {
                    alive_cells.push_back(gol_cell{positions[j], GOL_TRUE});
                    alive_neighbors++;
                }
            }
            ASSERT_EQ(gol_simulation_create(&tmp_sim), GOL_RESULT_SUCCESS);
            ASSERT_EQ(gol_simulation_set_cells(tmp_sim, alive_cells.size(),
                                               alive_cells.data()),
                      GOL_RESULT_SUCCESS);
            ASSERT_EQ(gol_simulation_next_frame(tmp_sim), GOL_RESULT_SUCCESS);
            gol_cell tested_cell;
            ASSERT_EQ(gol_simulation_query_cells(tmp_sim, 1, &tested_pos,
                                                 &tested_cell),
                      GOL_RESULT_SUCCESS);
            bool should_live = (alive_neighbors == 3);
            if (starts_alive == 1) {
                should_live = should_live || (alive_neighbors == 2);
            }
            EXPECT_EQ(tested_cell.is_alive, should_live ? GOL_TRUE : GOL_FALSE);
            ASSERT_EQ(gol_simulation_destroy(tmp_sim), GOL_RESULT_SUCCESS);
        }
    }
}
