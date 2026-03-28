#include "gol/simulation.h"

gol_result gol_simulation_create(gol_simulation *simulation) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_step(gol_simulation simulation) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_get_step(gol_simulation simulation, gol_size *step) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_set_bounds(gol_simulation simulation,
                                     const gol_grid_bounds *bounds) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_get_bounds(gol_simulation simulation,
                                     gol_grid_bounds *bounds) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_get_cell(gol_simulation simulation,
                                   const gol_grid_position *position,
                                   gol_cell *cell) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_set_cells(gol_simulation simulation, gol_size size,
                                    const gol_cell *cells,
                                    gol_result *results) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_get_alive_cells(gol_simulation simulation,
                                          gol_size *size, gol_cell *cells) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_get_updated_cells(gol_simulation simulation,
                                            gol_size *size, gol_cell *cells) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_destroy(gol_simulation simulation) {
    return GOL_RESULT_ERROR;
}
