#ifndef _LIFE_SIMULATION_H
#define _LIFE_SIMULATION_H

#include <stdint.h>

typedef void* gol_handle;
typedef gol_handle gol_simulation;
typedef int32_t gol_grid_scalar;
typedef int32_t gol_bool;
typedef uintptr_t gol_size;

#define GOL_TRUE 1
#define GOL_FALSE 0

typedef enum _gol_result {
  GOL_RESULT_SUCCESS = 0,  
  GOL_RESULT_ERROR = 1,
  GOL_RESULT_OUT_OF_GRID_BOUNDS = 2,
} gol_result;

typedef struct _gol_grid_position {
  gol_grid_scalar x;
  gol_grid_scalar y;
} gol_grid_position;

typedef struct _gol_grid_bounds {
  gol_grid_scalar top;
  gol_grid_scalar left;
  gol_grid_scalar bottom;
  gol_grid_scalar right;
} gol_grid_bounds;

typedef struct _gol_cell {
  gol_grid_position pos;
  gol_bool is_alive;
} gol_cell;

// simulation api
gol_result gol_create_simulation(gol_simulation *simulation);
gol_result gol_simulation_step(gol_simulation simulation);
gol_result gol_simulation_get_step(gol_simulation simulation, gol_size *step);
gol_result gol_simulation_set_bounds(gol_simulation simulation, const gol_grid_bounds *bounds);
gol_result gol_simulation_get_cell(gol_simulation simulation, const gol_grid_position *position, gol_cell *cell);
gol_result gol_simulation_set_cells(gol_simulation simulation, gol_size size, const gol_cell *cells);
gol_result gol_simulation_get_alive_cells(gol_simulation simulation, gol_size *size, gol_cell *cells);
gol_result gol_simulation_get_updated_cells(gol_simulation simulation, gol_size *size, gol_cell *cells);
gol_result gol_simulation_destroy(gol_simulation simulation);

// move this out of public api
gol_result gol_check_grid_position_in_bounds(gol_grid_position *position, gol_grid_bounds *bounds);

#endif
