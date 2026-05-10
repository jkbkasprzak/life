#ifndef _GOL_CELL_DATA_H
#define _GOL_CELL_DATA_H

#include "gol/simulation.h"

typedef struct _gol_cell_data {
    gol_grid_position position;
    gol_bool is_alive;
} gol_cell_data;

#endif
