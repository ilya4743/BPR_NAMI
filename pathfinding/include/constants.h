#pragma once

/*!
   \file constants.h
   \brief This is a collection of constants that are used throughout the project.
   \todo All constants need to be checked and documented
*/

////###################################################
////                                               INFO
////###################################################
//    HEADING => 0 - 359 degrees, 0 being north pointing towards positive Y
//    X-COORDINATE => designating the width of the grid
//    Y-COORDINATE => designating the height of the grid

#include <cmath>

/*!
    \brief The namespace that wraps the entire project
    \namespace HybridAStar
*/

namespace HybridAStar {
/*!
    \brief The namespace that wraps constants.h
    \namespace Constants
*/
class Constants {

private:
    // _________________
    // CONFIG FLAGS

    /// A flag for additional debugging output via `std::cout`
    bool coutDEBUG = false;
    /// A flag for the mode (true = manual; false = dynamic). Manual for static map or dynamic for dynamic map.
    bool manual = true;
    /// A flag to toggle reversing (true = on; false = off)
    bool reverse = true;
    /// A flag to toggle the connection of the path via Dubin's shot (true = on; false = off)
    bool dubinsShot = true;
    /// A flag to toggle the Dubin's heuristic, this should be false, if reversing is enabled (true = on; false = off)
    bool dubins = false;
    /*!
      \var bool dubinsLookup
      \brief A flag to toggle the Dubin's heuristic via lookup, potentially speeding up the search by a lot
      \todo not yet functional
    */
    bool dubinsLookup = false && dubins;
    /// A flag to toggle the 2D heuristic (true = on; false = off)
    bool twoD = false;

    // _________________
    // GENERAL CONSTANTS

    /// [#] --- Limits the maximum search depth of the algorithm, possibly terminating without the solution
    int iterations = 30000;
    /// [m] --- Uniformly adds a padding around the vehicle
    double bloating = 0;
    /// [m] --- The width of the vehicle
    double width = 1.75 + 2 * bloating;
    /// [m] --- The length of the vehicle
    double length = 2.65 + 2 * bloating;
    /// [m] --- The minimum turning radius of the vehicle
    float r = 6;
    /// [m] --- The number of discretizations in heading
    int headings = 72;
    /// [°] --- The discretization value of the heading (goal condition)
    float deltaHeadingDeg = 360 / (float)headings;
    /// [c*M_PI] --- The discretization value of heading (goal condition)
    float deltaHeadingRad = 2 * M_PI / (float)headings;
    /// [c*M_PI] --- The heading part of the goal condition
    float deltaHeadingNegRad = 2 * M_PI - deltaHeadingRad;
    /// [m] --- The cell size of the 2D grid of the world
    float cellSize = 2;
    /*!
      \brief [m] --- The tie breaker breaks ties between nodes expanded in the same cell


      As the cost-so-far are bigger than the cost-to-come it is reasonbale to believe that the algorithm would prefer the predecessor rather than the successor.
      This would lead to the fact that the successor would never be placed and the the one cell could only expand one node. The tieBreaker artificially increases the cost of the predecessor
      to allow the successor being placed in the same cell.
    */
    float tieBreaker = 0.01;

    // ___________________
    // HEURISTIC CONSTANTS

    /// [#] --- A factor to ensure admissibility of the holonomic with obstacles heuristic
    static constexpr float factor2D = std::sqrt(5) / std::sqrt(2) + 1;
    /// [#] --- A movement cost penalty for turning (choosing non straight motion primitives)
    float penaltyTurning = 1.05;
    /// [#] --- A movement cost penalty for reversing (choosing motion primitives > 2)
    float penaltyReversing = 2.0;
    /// [#] --- A movement cost penalty for change of direction (changing from primitives < 3 to primitives > 2)
    float penaltyCOD = 2.0;
    /// [m] --- The distance to the goal when the analytical solution (Dubin's shot) first triggers
    float dubinsShotDistance = 100;
    /// [m] --- The step size for the analytical solution (Dubin's shot) primarily relevant for collision checking
    float dubinsStepSize = 1;


    // ______________________
    // DUBINS LOOKUP SPECIFIC

    /// [m] --- The width of the dubinsArea / 2 for the analytical solution (Dubin's shot)
    int dubinsWidth = 15;
    /// [m] --- The area of the lookup for the analytical solution (Dubin's shot)
    int dubinsArea = dubinsWidth * dubinsWidth;


    // _________________________
    // COLLISION LOOKUP SPECIFIC

    /// [m] -- The bounding box size length and width to precompute all possible headings
    int bbSize = std::ceil((sqrt(width * width + length* length) + 4) / cellSize);
    /// [#] --- The sqrt of the number of discrete positions per cell
    int positionResolution = 10;
    /// [#] --- The number of discrete positions per cell
    int positions = positionResolution * positionResolution;

    // _________________
    // SMOOTHER SPECIFIC
    /// [m] --- The minimum width of a safe road for the vehicle at hand
    float minRoadWidth = 2;

    Constants()=default;
public:
    Constants(const Constants&) = delete;
    Constants& operator=(const Constants&) = delete;
    Constants(Constants&&) = delete;
    Constants& operator=(Constants&&) = delete;

    static Constants& GetInstance() {
        static Constants obj;
        return obj;
    }

    inline auto COUT_DEBUG() const {return coutDEBUG;}
    inline auto MANUAL() const {return manual;}
    inline auto REVERSE() const {return reverse;}
    inline auto DUBINS_SHOOT() const {return dubinsShot;}
    inline auto DUBINS() const {return dubins;}
    inline auto DUBINS_LOOKUP() const {return dubinsLookup;}
    inline auto TWO_D() const {return twoD;}
    inline auto ITERATIONS() const {return iterations;}
    inline auto BLOATING() const {return bloating;}
    inline auto WIDTH() const {return width;}
    inline auto LENGTH() const {return length;}
    inline auto R() const {return r;}
    inline constexpr auto HEADINGS() const {return headings;}
    inline auto DELTA_HEADING_DEG() const {return deltaHeadingDeg;}
    inline auto DELTA_HEADING_RAD() const {return deltaHeadingRad;}
    inline auto DELTA_HEADING_NEG_RAD() const {return deltaHeadingNegRad;}
    inline auto CELL_SIZE() const {return cellSize;}
    inline auto TIE_BREAKER() const {return tieBreaker;}
    inline auto FACTOR_2D() const {return factor2D;}
    inline auto PENALTY_TURNING() const {return penaltyTurning;}
    inline auto PENALTY_REVERSING() const {return penaltyReversing;}
    inline auto PENALTY_COD() const {return penaltyCOD;}
    inline auto DUBINS_SHOOT_DISTANCE() const {return dubinsShotDistance;}
    inline auto DUBINS_STEP_SIZE() const {return dubinsStepSize;}
    inline auto DUBINS_WIDTH() const {return dubinsWidth;}
    inline auto DUBINS_AREA() const {return dubinsArea;}
    inline auto BBSIZE() const {return bbSize;}
    inline auto POSITION_RESOLUTION() const {return positionResolution;}
    inline constexpr auto POSITIONS() const {return positions;}
    inline auto MIN_ROAD_WIDTH() const {return minRoadWidth;}  
};

        /// A structure describing the relative position of the occupied cell based on the center of the vehicle
    struct relPos {
      /// the x position relative to the center
      int x;
      /// the y position relative to the center
      int y;
    };
    /// A structure capturing the lookup for each theta configuration
    struct config {
      /// the number of cells occupied by this configuration of the vehicle
      int length;
      /*!
        \var relPos pos[64]
        \brief The maximum number of occupied cells
        \todo needs to be dynamic
      */
      relPos pos[64];
    };
}