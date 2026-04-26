#include "gol_debug.h"
#include "simulation_data.h"

#include <stdlib.h>

gol_result gol_simulation_data_create(gol_simulation_data **simulation) {
    GOL_ASSERT(simulation != NULL, "Expected valid simulation pointer but got NULL");
    gol_simulation_data *data =
        (gol_simulation_data *)malloc(sizeof(gol_simulation_data));
    if (data == NULL) {
        return GOL_RESULT_OUT_OF_MEMORY;
    }
    gol_simulation_data_reset(data);
    *simulation = data;
    return GOL_RESULT_SUCCESS;
}

void gol_simulation_data_reset(gol_simulation_data *simulation) {
    GOL_ASSERT(simulation != NULL, "Expected valid simulation but got NULL");
    simulation->frame_count = 0;
    simulation->grid_access.hit_count = 0;
    simulation->grid_access.miss_count = 0;
}

void gol_simulation_data_destroy(gol_simulation_data *simulation) {
    GOL_ASSERT(simulation != NULL, "Expected valid simulation but got NULL");
    free(simulation);
}
