#ifndef _GOL_CHUNK_H
#define _GOL_CHUNK_H

#include "cell_data.h"
#include "gol/simulation.h"

#define GOL_CHUNK_SIZE 32

typedef struct _gol_chunk {
    gol_grid_position position;
    gol_cell_data chunk[GOL_CHUNK_SIZE][GOL_CHUNK_SIZE];
} gol_chunk;

gol_grid_position
gol_chunk_position_to_cell_position(gol_grid_position chunk_position);
gol_grid_position
gol_cell_position_to_chunk_position(gol_grid_position cell_position);
gol_cell_data gol_chunk_get_cell(gol_grid_position cell_position);

#endif
