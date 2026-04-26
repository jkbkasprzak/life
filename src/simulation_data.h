#ifndef _GOL_SIMULATION_DATA_H
#define _GOL_SIMULATION_DATA_H

#include "gol/simulation.h"

typedef struct _gol_simulation_data {
    gol_simulation_property_frame_count frame_count;
    gol_simulation_property_grid_access_profiling grid_access;
} gol_simulation_data;

gol_result gol_simulation_data_create(gol_simulation_data **simulation);
void gol_simulation_data_reset(gol_simulation_data *simulation);
void gol_simulation_data_destroy(gol_simulation_data *simulation);

#endif
