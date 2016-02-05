
#include "CustomPrimitiveCubeInfo.h"

std::map<int, eCubeSide> CustomPrimitiveCubeInfo::composeFacesMap()
{
  std::map<int, eCubeSide> faces_map;

  faces_map[0] = CS_UP;
  faces_map[1] = CS_DOWN;
  faces_map[2] = CS_FRONT;
  faces_map[3] = CS_BACK;
  faces_map[4] = CS_RIGHT;
  faces_map[5] = CS_LEFT;

  return faces_map;
}