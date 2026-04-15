#ifndef _LIFE_SIMULATION_H
#define _LIFE_SIMULATION_H

#include <stdint.h>

/// @file
/// @brief Defines simulation API

/// @defgroup simulation_api Simulation API
/// @brief API for creating and running game of life simulations

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

/// @brief Defines possible operation results
/// @ingroup simulation_api
typedef enum {
    GOL_RESULT_SUCCESS = 0, ///< Operation succeded
    GOL_RESULT_ERROR = 1,   ///< Operation failed
    GOL_RESULT_INVALID_ARGUMENT =
        2, ///< Operation failed because of invalid arguments
} gol_result;

/// @brief Defines simulation properties
/// @ingroup simulation_api
typedef enum {
    GOL_SIMULATION_PROPERTY_TYPE_GRID_ACCESS_PROFILING =
        1, ///< Grid access profiling
} gol_simulation_property_type;

/// @brief Grid access profiling data
/// @ingroup simulation_api
///
/// Statistics collected per frame about access to grid cells
typedef struct {
    /// @brief Count of accesses that returned requested cell
    uint32_t hit_count;
    /// @brief Count of accesses that returned cell other than the requested one
    uint32_t miss_count;
} gol_simulation_property_grid_access_profiling;

/// @brief Grid position
/// @ingroup simulation_api
///
/// Position in 2D grid space
typedef struct {
    /// @brief First component of 2D vector
    gol_grid_scalar x;
    /// @brief Second component of 2D vector
    gol_grid_scalar y;
} gol_grid_position;

/// @brief Cell data
/// @ingroup simulation_api
///
/// Cell in game of life has a position and can be either dead or alive
typedef struct {
    /// @brief Cell's position
    gol_grid_position pos;
    /// @brief Determines if cell is alive or dead
    gol_bool is_alive;
} gol_cell;

/// @brief Creates new simulation
/// @ingroup simulation_api
///
/// Creates new empty simulation where all cells are in dead state.
///
/// @param[out] simulation Opaque simulation handle
/// @retval GOL_RESULT_SUCCESS Operation succeeded
gol_result gol_simulation_create(gol_simulation *simulation);
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
                                    const gol_cell *cells);
gol_result gol_simulation_get_alive_cells(gol_simulation simulation,
                                          gol_size *size, gol_cell *cells);
gol_result gol_simulation_get_updated_cells(gol_simulation simulation,
                                            gol_size *size, gol_cell *cells);
gol_result gol_simulation_destroy(gol_simulation simulation);

#ifdef __cplusplus
}
#endif
#endif
