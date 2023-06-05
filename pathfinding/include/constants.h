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

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cmath>
#include <string>

namespace pt = boost::property_tree;
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
    Constants() {
        pt::ptree tree;
        pt::read_json(config_filename, tree);
        coutDEBUG = tree.get<bool>("coutDEBUG");
        manual = tree.get<bool>("manual");
        reverse = tree.get<bool>("reverse");
        dubinsShot = tree.get<bool>("reverse");
        dubins = tree.get<bool>("dubins");
        dubinsLookup = false && dubins;
        twoD = tree.get<bool>("twoD");
        iterations = tree.get<int>("iterations");
        bloating = tree.get<double>("bloating");
        width = tree.get<double>("width") + 2 * bloating;
        length = tree.get<double>("length") + 2 * bloating;
        r = tree.get<float>("r");
        headings = tree.get<int>("headings");
        deltaHeadingDeg = 360 / (float)headings;
        deltaHeadingRad = 2 * M_PI / (float)headings;
        deltaHeadingNegRad = 2 * M_PI - deltaHeadingRad;
        tieBreaker = tree.get<float>("tieBreaker");
        penaltyTurning = tree.get<float>("penaltyTurning");
        penaltyReversing = tree.get<float>("penaltyReversing");
        penaltyCOD = tree.get<float>("penaltyCOD");
        dubinsShotDistance = tree.get<float>("dubinsShotDistance");
        dubinsStepSize = tree.get<float>("dubinsStepSize");
        dubinsWidth = tree.get<int>("dubinsWidth");
        dubinsArea = dubinsWidth * dubinsWidth;
        positionResolution = tree.get<int>("positionResolution");
        positions = positionResolution * positionResolution;
        minRoadWidth = tree.get<float>("minRoadWidth");
    }
    // _________________
    // CONFIG FLAGS

    /// A flag for additional debugging output via `std::cout`
    bool coutDEBUG;
    /// A flag for the mode (true = manual; false = dynamic). Manual for static map or dynamic for dynamic map.
    bool manual;
    /// A flag to toggle reversing (true = on; false = off)
    bool reverse;
    /// A flag to toggle the connection of the path via Dubin's shot (true = on; false = off)
    bool dubinsShot;
    /// A flag to toggle the Dubin's heuristic, this should be false, if reversing is enabled (true = on; false = off)
    bool dubins;
    /*!
      \var bool dubinsLookup
      \brief A flag to toggle the Dubin's heuristic via lookup, potentially speeding up the search by a lot
      \todo not yet functional
    */
    bool dubinsLookup;
    /// A flag to toggle the 2D heuristic (true = on; false = off)
    bool twoD;

    // _________________
    // GENERAL CONSTANTS

    /// [#] --- Limits the maximum search depth of the algorithm, possibly terminating without the solution
    int iterations;
    /// [m] --- Uniformly adds a padding around the vehicle
    double bloating;
    /// [m] --- The width of the vehicle
    double width;
    /// [m] --- The length of the vehicle
    double length;
    /// [m] --- The minimum turning radius of the vehicle
    float r;
    /// [m] --- The number of discretizations in heading
    int headings;
    /// [°] --- The discretization value of the heading (goal condition)
    float deltaHeadingDeg;
    /// [c*M_PI] --- The discretization value of heading (goal condition)
    float deltaHeadingRad;
    /// [c*M_PI] --- The heading part of the goal condition
    float deltaHeadingNegRad;
    /*!
      \brief [m] --- The tie breaker breaks ties between nodes expanded in the same cell


      As the cost-so-far are bigger than the cost-to-come it is reasonbale to believe that the algorithm would prefer the predecessor rather than the successor.
      This would lead to the fact that the successor would never be placed and the the one cell could only expand one node. The tieBreaker artificially increases the cost of the predecessor
      to allow the successor being placed in the same cell.
    */
    float tieBreaker;

    // ___________________
    // HEURISTIC CONSTANTS

    /// [#] --- A factor to ensure admissibility of the holonomic with obstacles heuristic
    static constexpr float factor2D = std::sqrt(5) / std::sqrt(2) + 1;
    /// [#] --- A movement cost penalty for turning (choosing non straight motion primitives)
    float penaltyTurning;
    /// [#] --- A movement cost penalty for reversing (choosing motion primitives > 2)
    float penaltyReversing;
    /// [#] --- A movement cost penalty for change of direction (changing from primitives < 3 to primitives > 2)
    float penaltyCOD;
    /// [m] --- The distance to the goal when the analytical solution (Dubin's shot) first triggers
    float dubinsShotDistance;
    /// [m] --- The step size for the analytical solution (Dubin's shot) primarily relevant for collision checking
    float dubinsStepSize;

    // ______________________
    // DUBINS LOOKUP SPECIFIC

    /// [m] --- The width of the dubinsArea / 2 for the analytical solution (Dubin's shot)
    int dubinsWidth;
    /// [m] --- The area of the lookup for the analytical solution (Dubin's shot)
    int dubinsArea;

    // _________________________
    // COLLISION LOOKUP SPECIFIC

    /// [#] --- The sqrt of the number of discrete positions per cell
    int positionResolution;
    /// [#] --- The number of discrete positions per cell
    int positions;

    // _________________
    // SMOOTHER SPECIFIC
    /// [m] --- The minimum width of a safe road for the vehicle at hand
    float minRoadWidth;
    inline static bool isLoad;
    inline static std::string config_filename;

   public:
    Constants(const Constants&) = delete;
    Constants& operator=(const Constants&) = delete;
    Constants(Constants&&) = delete;
    Constants& operator=(Constants&&) = delete;

    static Constants& GetInstance() {
        if (isLoad) {
            static Constants obj;
            return obj;
        } else
            throw std::runtime_error(config_filename + " not found");
    }

    static void SetConstatsFromFile(std::string filename) {
        config_filename = filename;
        isLoad = true;
    }

    inline auto COUT_DEBUG() const { return coutDEBUG; }
    inline auto MANUAL() const { return manual; }
    inline auto REVERSE() const { return reverse; }
    inline auto DUBINS_SHOOT() const { return dubinsShot; }
    inline auto DUBINS() const { return dubins; }
    inline auto DUBINS_LOOKUP() const { return dubinsLookup; }
    inline auto TWO_D() const { return twoD; }
    inline auto ITERATIONS() const { return iterations; }
    inline auto BLOATING() const { return bloating; }
    inline auto WIDTH() const { return width; }
    inline auto LENGTH() const { return length; }
    inline auto R() const { return r; }
    inline constexpr auto HEADINGS() const { return headings; }
    inline auto DELTA_HEADING_DEG() const { return deltaHeadingDeg; }
    inline auto DELTA_HEADING_RAD() const { return deltaHeadingRad; }
    inline auto DELTA_HEADING_NEG_RAD() const { return deltaHeadingNegRad; }
    inline auto TIE_BREAKER() const { return tieBreaker; }
    inline auto FACTOR_2D() const { return factor2D; }
    inline auto PENALTY_TURNING() const { return penaltyTurning; }
    inline auto PENALTY_REVERSING() const { return penaltyReversing; }
    inline auto PENALTY_COD() const { return penaltyCOD; }
    inline auto DUBINS_SHOOT_DISTANCE() const { return dubinsShotDistance; }
    inline auto DUBINS_STEP_SIZE() const { return dubinsStepSize; }
    inline auto DUBINS_WIDTH() const { return dubinsWidth; }
    inline auto DUBINS_AREA() const { return dubinsArea; }
    inline auto POSITION_RESOLUTION() const { return positionResolution; }
    inline constexpr auto POSITIONS() const { return positions; }
    inline auto MIN_ROAD_WIDTH() const { return minRoadWidth; }
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
}  // namespace HybridAStar