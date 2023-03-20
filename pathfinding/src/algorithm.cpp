#include "algorithm.h"
#include "helper.h"
#include <boost/heap/binomial_heap.hpp>
#include <ompl/base/spaces/ReedsSheppStateSpace.h>
#include <ompl/base/spaces/DubinsStateSpace.h>
#include <ompl/base/spaces/SE2StateSpace.h>
#include <ompl/base/State.h>
typedef ompl::base::SE2StateSpace::StateType State;

using namespace HybridAStar;

float aStar(Node2D& start, Node2D& goal, Node2D* nodes2D, int width, int height, CollisionDetection& configurationSpace);
void updateH(Node3D& start, const Node3D& goal, Node2D* nodes2D, float* dubinsLookup, int width, int height, CollisionDetection& configurationSpace);
std::pair<Node3D*,Node3D*> dubinsShot(Node3D& start, const Node3D& goal, CollisionDetection& configurationSpace);

//###################################################
//                                    NODE COMPARISON
//###################################################
/*!
   \brief A structure to sort nodes in a heap structure
*/
struct CompareNodes {
  /// Sorting 3D nodes by increasing C value - the total estimated cost
  bool operator()(const Node3D* lhs, const Node3D* rhs) const {
    return lhs->getC() > rhs->getC();
  }
  /// Sorting 2D nodes by increasing C value - the total estimated cost
  bool operator()(const Node2D* lhs, const Node2D* rhs) const {
    return lhs->getC() > rhs->getC();
  }
};

//###################################################
//                                        3D A*
//###################################################
Node3D* Algorithm::hybridAStar(Node3D& start,
                               const Node3D& goal,
                               Node3D* nodes3D,
                               Node2D* nodes2D,
                               int width,
                               int height,
                               CollisionDetection& configurationSpace,
                               float* dubinsLookup) {

  // PREDECESSOR AND SUCCESSOR INDEX
  int iPred, iSucc;
  float newG;
  // Number of possible directions, 3 for forward driving and an additional 3 for reversing
  int dir = Constants::GetInstance().REVERSE() ? 6 : 3;
  // Number of iterations the algorithm has run for stopping based on Constants::GetInstance().iterations
  int iterations = 0;

  // OPEN LIST AS BOOST IMPLEMENTATION
  typedef boost::heap::binomial_heap<Node3D*,
          boost::heap::compare<CompareNodes>
          > priorityQueue;
  priorityQueue O;

  // update h value
  updateH(start, goal, nodes2D, dubinsLookup, width, height, configurationSpace);
  // mark start as open
  start.open();
  // push on priority queue aka open list
  O.push(&start);
  iPred = start.setIdx(width, height);
  nodes3D[iPred] = start;

  // NODE POINTER
  Node3D* nPred;
  Node3D* nSucc;

  // float max = 0.f;

  // continue until O empty
  while (!O.empty()) {

    //    // DEBUG
    //    Node3D* pre = nullptr;
    //    Node3D* succ = nullptr;

    //    std::cout << "\t--->>>" << std::endl;

    //    for (priorityQueue::ordered_iterator it = O.ordered_begin(); it != O.ordered_end(); ++it) {
    //      succ = (*it);
    //      std::cout << "VAL"
    //                << " | C:" << succ->getC()
    //                << " | x:" << succ->getX()
    //                << " | y:" << succ->getY()
    //                << " | t:" << Helper::toDeg(succ->getT())
    //                << " | i:" << succ->getIdx()
    //                << " | O:" << succ->isOpen()
    //                << " | pred:" << succ->getPred()
    //                << std::endl;

    //      if (pre != nullptr) {

    //        if (pre->getC() > succ->getC()) {
    //          std::cout << "PRE"
    //                    << " | C:" << pre->getC()
    //                    << " | x:" << pre->getX()
    //                    << " | y:" << pre->getY()
    //                    << " | t:" << Helper::toDeg(pre->getT())
    //                    << " | i:" << pre->getIdx()
    //                    << " | O:" << pre->isOpen()
    //                    << " | pred:" << pre->getPred()
    //                    << std::endl;
    //          std::cout << "SCC"
    //                    << " | C:" << succ->getC()
    //                    << " | x:" << succ->getX()
    //                    << " | y:" << succ->getY()
    //                    << " | t:" << Helper::toDeg(succ->getT())
    //                    << " | i:" << succ->getIdx()
    //                    << " | O:" << succ->isOpen()
    //                    << " | pred:" << succ->getPred()
    //                    << std::endl;

    //          if (pre->getC() - succ->getC() > max) {
    //            max = pre->getC() - succ->getC();
    //          }
    //        }
    //      }

    //      pre = succ;
    //    }

    // pop node with lowest cost from priority queue
    nPred = O.top();
    // set index
    iPred = nPred->setIdx(width, height);
    iterations++;

    // _____________________________
    // LAZY DELETION of rewired node
    // if there exists a pointer this node has already been expanded
    if (nodes3D[iPred].isClosed()) {
      // pop node from the open list and start with a fresh node
      O.pop();
      continue;
    }
    // _________________
    // EXPANSION OF NODE
    else if (nodes3D[iPred].isOpen()) {
      // add node to closed list
      nodes3D[iPred].close();
      // remove node from open list
      O.pop();

      // _________
      // GOAL TEST
      if (*nPred == goal || iterations > Constants::GetInstance().ITERATIONS()) {
        // DEBUG
        return nPred;
      }

      // ____________________
      // CONTINUE WITH SEARCH
      else {
        // _______________________
        // SEARCH WITH DUBINS SHOT
        if (Constants::GetInstance().DUBINS_SHOOT() && nPred->isInRange(goal) && nPred->getPrim() < 3) {
          auto shoot=dubinsShot(*nPred, goal, configurationSpace);
          nSucc=shoot.second;
          if (nSucc != nullptr && *nSucc == goal) {
            //DEBUG
            // std::cout << "max diff " << max << std::endl;
            shoot.first->setPred(nPred);
            return nSucc;
          }
          nSucc=nullptr;

          delete [] shoot.first;

        }

        // ______________________________
        // SEARCH WITH FORWARD SIMULATION
        for (int i = 0; i < dir; i++) {
          // create possible successor
          nSucc = nPred->createSuccessor(i);
          // set index of the successor
          iSucc = nSucc->setIdx(width, height);

          // ensure successor is on grid and traversable
          if (nSucc->isOnGrid(width, height) && configurationSpace.isTraversable(nSucc)) {

            // ensure successor is not on closed list or it has the same index as the predecessor
            if (!nodes3D[iSucc].isClosed() || iPred == iSucc) {

              // calculate new G value
              nSucc->updateG();
              newG = nSucc->getG();

              // if successor not on open list or found a shorter way to the cell
              if (!nodes3D[iSucc].isOpen() || newG < nodes3D[iSucc].getG() || iPred == iSucc) {

                // calculate H value
                updateH(*nSucc, goal, nodes2D, dubinsLookup, width, height, configurationSpace);

                // if the successor is in the same cell but the C value is larger
                if (iPred == iSucc && nSucc->getC() > nPred->getC() + Constants::GetInstance().TIE_BREAKER()) {
                  delete nSucc;
                  continue;
                }
                // if successor is in the same cell and the C value is lower, set predecessor to predecessor of predecessor
                else if (iPred == iSucc && nSucc->getC() <= nPred->getC() + Constants::GetInstance().TIE_BREAKER()) {
                  nSucc->setPred(nPred->getPred());
                }

                if (nSucc->getPred() == nSucc) {
                  std::cout << "looping";
                }

                // put successor on open list
                nSucc->open();
                nodes3D[iSucc] = *nSucc;
                O.push(&nodes3D[iSucc]);
                delete nSucc;
              } else { delete nSucc; }
            } else { delete nSucc; }
          } else { delete nSucc; }
        }
      }
    }
  }

  if (O.empty()) {
    return nullptr;
  }

  return nullptr;
}

//###################################################
//                                        2D A*
//###################################################
float aStar(Node2D& start,
            Node2D& goal,
            Node2D* nodes2D,
            int width,
            int height,
            CollisionDetection& configurationSpace) {

  // PREDECESSOR AND SUCCESSOR INDEX
  int iPred, iSucc;
  float newG;

  // reset the open and closed list
  for (int i = 0; i < width * height; ++i) {
    nodes2D[i].reset();
  }

  boost::heap::binomial_heap<Node2D*,
        boost::heap::compare<CompareNodes>> O;
  // update h value
  start.updateH(goal);
  // mark start as open
  start.open();
  // push on priority queue
  O.push(&start);
  iPred = start.setIdx(width);
  nodes2D[iPred] = start;

  // NODE POINTER
  Node2D* nPred;
  Node2D* nSucc;

  // continue until O empty
  while (!O.empty()) {
    // pop node with lowest cost from priority queue
    nPred = O.top();
    // set index
    iPred = nPred->setIdx(width);

    // _____________________________
    // LAZY DELETION of rewired node
    // if there exists a pointer this node has already been expanded
    if (nodes2D[iPred].isClosed()) {
      // pop node from the open list and start with a fresh node
      O.pop();
      continue;
    }
    // _________________
    // EXPANSION OF NODE
    else if (nodes2D[iPred].isOpen()) {
      // add node to closed list
      nodes2D[iPred].close();
      nodes2D[iPred].discover();

      // remove node from open list
      O.pop();

      // _________
      // GOAL TEST
      if (*nPred == goal) {
        return nPred->getG();
      }
      // ____________________
      // CONTINUE WITH SEARCH
      else {
        // _______________________________
        // CREATE POSSIBLE SUCCESSOR NODES
        for (int i = 0; i < Node2D::dir; i++) {
          // create possible successor
          nSucc = nPred->createSuccessor(i);
          // set index of the successor
          iSucc = nSucc->setIdx(width);

          // ensure successor is on grid ROW MAJOR
          // ensure successor is not blocked by obstacle
          // ensure successor is not on closed list
          if (nSucc->isOnGrid(width, height) &&  configurationSpace.isTraversable(nSucc) && !nodes2D[iSucc].isClosed()) {
            // calculate new G value
            nSucc->updateG();
            newG = nSucc->getG();

            // if successor not on open list or g value lower than before put it on open list
            if (!nodes2D[iSucc].isOpen() || newG < nodes2D[iSucc].getG()) {
              // calculate the H value
              nSucc->updateH(goal);
              // put successor on open list
              nSucc->open();
              nodes2D[iSucc] = *nSucc;
              O.push(&nodes2D[iSucc]);
              delete nSucc;
            } else { delete nSucc; }
          } else { delete nSucc; }
        }
      }
    }
  }

  // return large number to guide search away
  return 1000;
}

//###################################################
//                                         COST TO GO
//###################################################
void updateH(Node3D& start, const Node3D& goal, Node2D* nodes2D, float* dubinsLookup, int width, int height, CollisionDetection& configurationSpace) {
  float dubinsCost = 0;
  float reedsSheppCost = 0;
  float twoDCost = 0;
  float twoDoffset = 0;

  // if dubins heuristic is activated calculate the shortest path
  // constrained without obstacles
  if (Constants::GetInstance().DUBINS()) {

    // //ONLY FOR dubinsLookup
    //    int uX = std::abs((int)goal.getX() - (int)start.getX());
    //    int uY = std::abs((int)goal.getY() - (int)start.getY());
    //    // if the lookup table flag is set and the vehicle is in the lookup area
    //    if (Constants::GetInstance().DUBINS_LOOKUP() && uX < Constants::GetInstance().DUBINS_WIDTH() - 1 && uY < Constants::GetInstance().DUBINS_WIDTH() - 1) {
    //      int X = (int)goal.getX() - (int)start.getX();
    //      int Y = (int)goal.getY() - (int)start.getY();
    //      int h0;
    //      int h1;

    //      // mirror on x axis
    //      if (X >= 0 && Y <= 0) {
    //        h0 = (int)(Helper::normalizeHeadingRad(M_PI_2 - t) / Constants::GetInstance().DELTA_HEADING_RAD());
    //        h1 = (int)(Helper::normalizeHeadingRad(M_PI_2 - goal.getT()) / Constants::GetInstance().DELTA_HEADING_RAD());
    //      }
    //      // mirror on y axis
    //      else if (X <= 0 && Y >= 0) {
    //        h0 = (int)(Helper::normalizeHeadingRad(M_PI_2 - t) / Constants::GetInstance().DELTA_HEADING_RAD());
    //        h1 = (int)(Helper::normalizeHeadingRad(M_PI_2 - goal.getT()) / Constants::GetInstance().DELTA_HEADING_RAD());

    //      }
    //      // mirror on xy axis
    //      else if (X <= 0 && Y <= 0) {
    //        h0 = (int)(Helper::normalizeHeadingRad(M_PI - t) / Constants::GetInstance().DELTA_HEADING_RAD());
    //        h1 = (int)(Helper::normalizeHeadingRad(M_PI - goal.getT()) / Constants::GetInstance().DELTA_HEADING_RAD());

    //      } else {
    //        h0 = (int)(t / Constants::GetInstance().DELTA_HEADING_RAD());
    //        h1 = (int)(goal.getT() / Constants::GetInstance().DELTA_HEADING_RAD());
    //      }

    //      dubinsCost = dubinsLookup[uX * Constants::GetInstance().DUBINS_WIDTH() * Constants::GetInstance().HEADINGS() * Constants::GetInstance().HEADINGS()
    //                                + uY *  Constants::GetInstance().HEADINGS() * Constants::GetInstance().HEADINGS()
    //                                + h0 * Constants::GetInstance().HEADINGS()
    //                                + h1];
    //    } else {

    // if (Constants::GetInstance().DUBINS_SHOOT() && std::abs(start.getX() - goal.getX()) >= 10 && std::abs(start.getY() - goal.getY()) >= 10)
    //      // start
    //      double q0[] = { start.getX(), start.getY(), start.getT()};
    //      // goal
    //      double q1[] = { goal.getX(), goal.getY(), goal.getT()};
    //      DubinsPath dubinsPath;
    //      dubins_init(q0, q1, Constants::GetInstance().R(), &dubinsPath);
    //      dubinsCost = dubins_path_length(&dubinsPath);

    ompl::base::DubinsStateSpace dubinsPath(Constants::GetInstance().R());
    State* dbStart = (State*)dubinsPath.allocState();
    State* dbEnd = (State*)dubinsPath.allocState();
    dbStart->setXY(start.getX(), start.getY());
    dbStart->setYaw(start.getT());
    dbEnd->setXY(goal.getX(), goal.getY());
    dbEnd->setYaw(goal.getT());
    dubinsCost = dubinsPath.distance(dbStart, dbEnd);
    dubinsPath.freeState(dbStart);
    dubinsPath.freeState(dbEnd);
  }

  // if reversing is active use a
  if (Constants::GetInstance().REVERSE() && !Constants::GetInstance().DUBINS()) {
    //    ros::Time t0 = ros::Time::now();
    ompl::base::ReedsSheppStateSpace reedsSheppPath(Constants::GetInstance().R());
    State* rsStart = (State*)reedsSheppPath.allocState();
    State* rsEnd = (State*)reedsSheppPath.allocState();
    rsStart->setXY(start.getX(), start.getY());
    rsStart->setYaw(start.getT());
    rsEnd->setXY(goal.getX(), goal.getY());
    rsEnd->setYaw(goal.getT());
    reedsSheppCost = reedsSheppPath.distance(rsStart, rsEnd);
    reedsSheppPath.freeState(rsStart);
    reedsSheppPath.freeState(rsEnd);
    //    ros::Time t1 = ros::Time::now();
    //    ros::Duration d(t1 - t0);
    //    std::cout << "calculated Reed-Sheep Heuristic in ms: " << d * 1000 << std::endl;
  }

  // if twoD heuristic is activated determine shortest path
  // unconstrained with obstacles
  if (Constants::GetInstance().TWO_D() && !nodes2D[(int)start.getY() * width + (int)start.getX()].isDiscovered()) {
    //    ros::Time t0 = ros::Time::now();
    // create a 2d start node
    Node2D start2d(start.getX(), start.getY(), 0, 0, nullptr);
    // create a 2d goal node
    Node2D goal2d(goal.getX(), goal.getY(), 0, 0, nullptr);
    // run 2d astar and return the cost of the cheapest path for that node
    nodes2D[(int)start.getY() * width + (int)start.getX()].setG(aStar(goal2d, start2d, nodes2D, width, height, configurationSpace));
    //    ros::Time t1 = ros::Time::now();
    //    ros::Duration d(t1 - t0);
    //    std::cout << "calculated 2D Heuristic in ms: " << d * 1000 << std::endl;
  }

  if (Constants::GetInstance().TWO_D()) {
    // offset for same node in cell
    twoDoffset = sqrt(((start.getX() - (long)start.getX()) - (goal.getX() - (long)goal.getX())) * ((start.getX() - (long)start.getX()) - (goal.getX() - (long)goal.getX())) +
                      ((start.getY() - (long)start.getY()) - (goal.getY() - (long)goal.getY())) * ((start.getY() - (long)start.getY()) - (goal.getY() - (long)goal.getY())));
    twoDCost = nodes2D[(int)start.getY() * width + (int)start.getX()].getG() - twoDoffset;

  }

  // return the maximum of the heuristics, making the heuristic admissable
  start.setH(std::max(reedsSheppCost, std::max(dubinsCost, twoDCost)));
}

//###################################################
//                                        DUBINS SHOT
//###################################################
std::pair<Node3D*, Node3D*> dubinsShot(Node3D& start, const Node3D& goal, CollisionDetection& configurationSpace) {
  // start
  double q0[] = { start.getX(), start.getY(), start.getT() };
  // goal
  double q1[] = { goal.getX(), goal.getY(), goal.getT() };
  // initialize the path
  DubinsPath path;
  // calculate the path
  dubins_init(q0, q1, Constants::GetInstance().R(), &path);

  int i = 0;
  float x = 0.f;
  float length = dubins_path_length(&path);

  Node3D* dubinsNodes = new Node3D [(int)(length / Constants::GetInstance().DUBINS_STEP_SIZE()) + 1];

  // avoid duplicate waypoint
  x += Constants::GetInstance().DUBINS_STEP_SIZE();
  while (x <  length) {
    double q[3];
    dubins_path_sample(&path, x, q);
    dubinsNodes[i].setX(q[0]);
    dubinsNodes[i].setY(q[1]);
    dubinsNodes[i].setT(Helper::normalizeHeadingRad(q[2]));

    // collision check
    if (configurationSpace.isTraversable(&dubinsNodes[i])) {

      // set the predecessor to the previous step
      if (i > 0) {
        dubinsNodes[i].setPred(&dubinsNodes[i - 1]);
      } else {
        dubinsNodes[i].setPred(nullptr);
      }

      if (&dubinsNodes[i] == dubinsNodes[i].getPred()) {
        std::cout << "looping shot";
      }

      x += Constants::GetInstance().DUBINS_STEP_SIZE();
      i++;
    } else {
      //      std::cout << "Dubins shot collided, discarding the path" << "\n";
      // delete all nodes
      delete [] dubinsNodes;
      return {nullptr,nullptr};
    }
  }
  auto* ptr=&dubinsNodes[i - 1];
  //  std::cout << "Dubins shot connected, returning the path" << "\n";
  return {dubinsNodes, ptr};
}
