#include "gol/simulation.h"

#include "simulation_data.h"

#include <stdlib.h>
#include <string.h>

gol_result gol_simulation_create(gol_simulation *simulation) {
    gol_simulation_data *data = NULL;
    gol_result result = gol_simulation_data_create(&data);
    if (result != GOL_RESULT_SUCCESS) {
        return result;
    }
    *simulation = data;
    return GOL_RESULT_SUCCESS;
}

gol_result gol_simulation_query_property(gol_simulation simulation,
                                         gol_simulation_property_type property,
                                         gol_size *bytes, void *data) {
    gol_size cp_bytes = 0;
    void *cp_src = NULL;
    switch (property) {
    case GOL_SIMULATION_PROPERTY_TYPE_FRAME_COUNT:
        cp_bytes = sizeof(gol_simulation_property_frame_count);
        cp_src = &simulation->frame_count;
        break;
    case GOL_SIMULATION_PROPERTY_TYPE_GRID_ACCESS_PROFILING:
        cp_bytes = sizeof(gol_simulation_property_grid_access_profiling);
        cp_src = &simulation->grid_access;
        break;
    default:
        return GOL_RESULT_UNKNOWN_SIMULATION_PROPERTY;
    }
    if (bytes != NULL) {
        *bytes = cp_bytes;
    }
    if (data != NULL) {
        memcpy(data, cp_src, cp_bytes);
    }
    return GOL_RESULT_SUCCESS;
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
    gol_simulation_data_destroy(simulation);
    return GOL_RESULT_SUCCESS;
}
