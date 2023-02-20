#include "collisiondetection.h"

using namespace HybridAStar;

CollisionDetection::CollisionDetection():collisionLookup(Constants::GetInstance().HEADINGS() * Constants::GetInstance().POSITIONS()) {
  Lookup::collisionLookup(collisionLookup.data());
}

bool CollisionDetection::configurationTest(float x, float y, float t) const {
  int X = (int)x;
  int Y = (int)y;
  int iX = (int)((x - (long)x) * Constants::GetInstance().POSITION_RESOLUTION());
  iX = iX > 0 ? iX : 0;
  int iY = (int)((y - (long)y) * Constants::GetInstance().POSITION_RESOLUTION());
  iY = iY > 0 ? iY : 0;
  int iT = (int)(t / Constants::GetInstance().DELTA_HEADING_RAD());
  int idx = iY * Constants::GetInstance().POSITION_RESOLUTION() * Constants::GetInstance().HEADINGS() + iX * Constants::GetInstance().HEADINGS() + iT;
  int cX;
  int cY;

  for (int i = 0; i < collisionLookup[idx].length; ++i) {
    cX = (X + collisionLookup[idx].pos[i].x);
    cY = (Y + collisionLookup[idx].pos[i].y);

    // make sure the configuration coordinates are actually on the grid
    if (cX >= 0 && (unsigned int)cX < grid.width && cY >= 0 && (unsigned int)cY < grid.height) {
      if (grid.data[cY * grid.width + cX]) {
        return false;
      }
    }
  }

  return true;
}