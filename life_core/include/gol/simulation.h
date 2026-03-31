#ifndef _LIFE_SIMULATION_H
#define _LIFE_SIMULATION_H

#include <stdint.h>

typedef void *gol_handle;
typedef gol_handle gol_simulation;
typedef int32_t gol_grid_scalar;
typedef int32_t gol_bool;
typedef uintptr_t gol_size;

#define GOL_TRUE 1
#define GOL_FALSE 0

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _gol_result {
    GOL_RESULT_SUCCESS = 0,
    GOL_RESULT_ERROR = 1,
    GOL_RESULT_OUT_OF_GRID_BOUNDS = 2,
} gol_result;

typedef enum _gol_simulation_property_type {
    GOL_SIMULATION_PROPERTY_TYPE_GRID_ACCESS_PROFILING
} gol_simulation_property_type;

typedef struct _gol_simulation_property_grid_access_profiling {
    uint32_t hit_count;
    uint32_t miss_count;
} gol_simulation_property_grid_access_profiling;

typedef struct _gol_grid_position {
    gol_grid_scalar x;
    gol_grid_scalar y;
} gol_grid_position;

typedef struct _gol_cell {
    gol_grid_position pos;
    gol_bool is_alive;
} gol_cell;

gol_result gol_simulation_create(gol_simulation *simulation,
                                 gol_grid_scalar size);
gol_result gol_simulation_query_property(gol_simulation simulation,
                                         gol_simulation_property_type property,
                                         void *data);
gol_result gol_simulation_next_frame(gol_simulation simulation);
gol_result gol_simulation_get_frame_count(gol_simulation simulation,
                                          gol_size *step);
gol_result gol_simulation_query_cells(gol_simulation simulation, gol_size size,
                                      const gol_grid_position *positions,
                                      gol_cell *cells);
gol_result gol_simulation_set_cells(gol_simulation simulation, gol_size size,
                                    const gol_cell *cells, gol_result *results);
gol_result gol_simulation_get_alive_cells(gol_simulation simulation,
                                          gol_size *size, gol_cell *cells);
gol_result gol_simulation_get_updated_cells(gol_simulation simulation,
                                            gol_size *size, gol_cell *cells);
gol_result gol_simulation_destroy(gol_simulation simulation);

#ifdef __cplusplus
}
#endif
#endif
