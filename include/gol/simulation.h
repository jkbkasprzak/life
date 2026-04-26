#ifndef _GOL_SIMULATION_H
#define _GOL_SIMULATION_H

#include <stdint.h>

/// @file
/// @brief Defines simulation API.

/// @defgroup simulation_api Simulation API
/// @brief API for creating and running game of life simulations.

/// @defgroup simulation_api_func Simulation API Functions
/// @brief Functions for creating and running game of life simulation.

/// @addtogroup simulation_api
/// @{

/// @brief Generic opaque handle.
/// @since 1.0.0
typedef void *gol_handle;

/// @brief Handle for simulation resource.
/// @since 1.0.0
typedef gol_handle gol_simulation;

/// @brief Position in single grid dimension.
/// @since 1.0.0
typedef int32_t gol_grid_scalar;

/// @brief Boolean type.
/// @since 1.0.0
typedef int32_t gol_bool;

/// @brief Type representing nonnegative size of collections.
/// @since 1.0.0
typedef uintptr_t gol_size;

/// @brief True value
/// @since 1.0.0
#define GOL_TRUE 1

/// @brief False value
/// @since 1.0.0
#define GOL_FALSE 0

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Defines possible operation results.
/// @since 1.0.0
typedef enum {
    GOL_RESULT_SUCCESS = 0, ///< Operation succeded.
    GOL_RESULT_ERROR = 1,   ///< Operation failed.
    GOL_RESULT_UNKNOWN_SIMULATION_PROPERTY =
        2, ///< Operation failed because of unknown simulation property.
    GOL_RESULT_OUT_OF_MEMORY =
        3 ///< Operation failed because of insufficient memory.
} gol_result;

/// @brief Lists available simulation properties.
/// @since 1.0.0
typedef enum {
    GOL_SIMULATION_PROPERTY_TYPE_FRAME_COUNT =
        1, ///< Property type for @ref gol_simulation_property_frame_count
    GOL_SIMULATION_PROPERTY_TYPE_GRID_ACCESS_PROFILING =
        2, ///< Property type for
           ///< @ref gol_simulation_property_grid_access_profiling
} gol_simulation_property_type;

/// @brief Statistics collected per frame about access to grid cells.
/// @since 1.0.0
typedef struct {
    /// @brief Count of accesses that returned requested cell.
    uint32_t hit_count;
    /// @brief Count of accesses that returned cell other than the requested
    /// one.
    uint32_t miss_count;
} gol_simulation_property_grid_access_profiling;

/// @brief Count of time steps processed by the simulation.
/// @since 1.0.0
typedef uint32_t gol_simulation_property_frame_count;

/// @brief Position in 2D simulation grid.
/// @since 1.0.0
typedef struct {
    /// @brief Position in x dimension.
    gol_grid_scalar x;
    /// @brief Position in y dimension.
    gol_grid_scalar y;
} gol_grid_position;

/// @brief Cell at certain position that can be either dead or alive.
/// @since 1.0.0
typedef struct {
    /// @brief Cell's position.
    gol_grid_position pos;
    /// @brief Determines if cell is alive or dead.
    gol_bool is_alive;
} gol_cell;

/// @addtogroup simulation_api_func
/// @{

/// @brief Creates new simulation.
/// @since 1.0.0
///
/// @li Created simulation is empty - all cells are in dead state.
/// @li Application is responsible for destroying simulation with
/// gol_simulation_destroy().
/// @li Application may call this function from simultaneus threads.
///
/// @param[in,out] simulation Pointer for writing new simulation handle.
/// @retval GOL_RESULT_SUCCESS Operation succeeded.
/// @retval GOL_RESULT_OUT_OF_MEMORY
/// Operation failed due to insufficient memory.
gol_result gol_simulation_create(gol_simulation *simulation);

/// @brief Query simulation property.
/// @since 1.0.0
///
/// See @ref gol_simulation_property_type for available properties.
/// @li This operation does not mutate simulation state.
/// @li Simultaneus threads may call this function with different simulation
/// handles.
/// @li Simultaneus threads may call this and other functions that do not modify
/// simulation state with the same simulation handle.
///
/// @param[in] simulation Simulation to query.
/// @param[in] property Property to query.
/// @param[out] bytes Pointer for writing size of query results in bytes.
/// Ignored if NULL.
/// @param[out] data Pointer for writing query results. Ignored if NULL.
/// @retval GOL_RESULT_SUCCESS Operation succeeded.
/// @retval GOL_RESULT_UNKNOWN_SIMULATION_PROPERTY Queried unknown property.
/// @retval GOL_RESULT_OUT_OF_MEMORY
/// Operation failed due to insufficient memory.
gol_result gol_simulation_query_property(gol_simulation simulation,
                                         gol_simulation_property_type property,
                                         gol_size *bytes, void *data);

/// @brief Progress simulation to next time step.
/// @since 1.0.0
///
/// @li Simulation state is updated according to simulation rules.
/// @li This operation mutates simulation state.
/// @li Simultaneus threads may call this function only with different
/// simulation handles.
///
/// @param[in] simulation Simulation to progress.
/// @retval GOL_RESULT_SUCCESS Operation succeeded.
/// @retval GOL_RESULT_OUT_OF_MEMORY
/// Operation failed due to insufficient memory.
gol_result gol_simulation_next_frame(gol_simulation simulation);

/// @brief Query cells at multiple positions.
/// @since 1.0.0
///
/// @li This operation does not mutate simulation state.
/// @li Simultaneus threads may call this function with different simulation
/// handles.
/// @li Simultaneus threads may call this and other functions that do not modify
/// simulation state with the same simulation handle.
///
/// @param[in] simulation Simulation to query.
/// @param[in] size Number of positions to query.
/// @param[in] positions Pointer for reading queried positions.
/// @param[out] cells Pointer for writing queried cells. Order of written cells
/// corresponds to order of queried positions.
/// @retval GOL_RESULT_SUCCESS Operation succeeded.
/// @retval GOL_RESULT_OUT_OF_MEMORY
/// Operation failed due to insufficient memory.
gol_result gol_simulation_query_cells(gol_simulation simulation, gol_size size,
                                      const gol_grid_position *positions,
                                      gol_cell *cells);

/// @brief Update multiple simulation cells.
/// @since 1.0.0
///
/// @li Modyfing same cell multiple times within a single call is allowed.
/// @li All updates are applied in order of @a cells array.
/// @li This operation mutates simulation state.
/// @li Simultaneus threads may call this function only with different
/// simulation handles.
///
/// @param[in] simulation Simulation to update.
/// @param[in] size Number of cells to update.
/// @param[in] cells Pointer for reading cells to update.
/// @retval GOL_RESULT_SUCCESS Operation succeeded. All updates were applied.
/// @retval GOL_RESULT_OUT_OF_MEMORY
/// Operation failed due to insufficient memory.
gol_result gol_simulation_set_cells(gol_simulation simulation, gol_size size,
                                    const gol_cell *cells);

/// @brief Query all alive cells.
/// @since 1.0.0
///
/// @li This operation does not mutate simulation state.
/// @li Simultaneus threads may call this function with different simulation
/// handles.
/// @li Simultaneus threads may call this and other functions that do not modify
/// simulation state with the same simulation handle.
///
/// @param[in] simulation Simulation to query.
/// @param[out] size Pointer for writing number of alive cells. Ignored if NULL.
/// @param[out] cells Pointer for writing alive cells. Ignored if NULL.
/// @retval GOL_RESULT_SUCCESS Operation succeeded.
/// @retval GOL_RESULT_OUT_OF_MEMORY
/// Operation failed due to insufficient memory.
gol_result gol_simulation_get_alive_cells(gol_simulation simulation,
                                          gol_size *size, gol_cell *cells);

/// @brief Query cells updated by the simulation in last time step.
/// @since 1.0.0
///
/// @li This operation does not mutate simulation state.
/// @li Simultaneus threads may call this function with different simulation
/// handles.
/// @li Simultaneus threads may call this and other functions that do not modify
/// simulation state with the same simulation handle.
///
/// @param[in] simulation Simulation to query.
/// @param[out] size Pointer for writing number of updated cells. Ignored if
/// NULL.
/// @param[out] cells Pointer for writing updated cells. Ignored if NULL.
/// @retval GOL_RESULT_SUCCESS Operation succeeded.
/// @retval GOL_RESULT_OUT_OF_MEMORY
/// Operation failed due to insufficient memory.
gol_result gol_simulation_get_updated_cells(gol_simulation simulation,
                                            gol_size *size, gol_cell *cells);

/// @brief Destroys existing simulation.
/// @since 1.0.0
///
/// @li All resources owned by the simulation are immediately released.
/// @li Application must not use simulation handle or resources after it was
/// destroyed.
/// @li Simultaneus threads may call this function only with different
/// simulation handles.
///
/// @param[in] simulation Handle to destroy.
/// @retval GOL_RESULT_SUCCESS Operation succeeded.
/// @retval GOL_RESULT_OUT_OF_MEMORY
/// Operation failed due to insufficient memory.
gol_result gol_simulation_destroy(gol_simulation simulation);

/// @}

#ifdef __cplusplus
}
#endif
#endif
/// @}
