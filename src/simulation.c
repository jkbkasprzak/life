#include "gol/simulation.h"

gol_result gol_simulation_create(gol_simulation *simulation) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_query_property(gol_simulation simulation,
                                         gol_simulation_property_type property,
                                         gol_size *bytes, void *data) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_next_frame(gol_simulation simulation) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_query_cells(gol_simulation simulation, gol_size size,
                                      const gol_grid_position *positions,
                                      gol_cell *cells) {
    return GOL_RESULT_ERROR;
}

gol_result gol_simulation_set_cells(gol_simulation simulation, gol_size size,
                                    const gol_cell *cells) {
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
