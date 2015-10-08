#include "DirectCore.h"

class DirectCoreForPolygon : public DirectCore
{
public:

  DirectCoreForPolygon(HWND&);

private:

  virtual void buildUpPolygonAndIndices();
};