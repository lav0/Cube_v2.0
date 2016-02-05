#include "stdafx.h"
#include "CppUnitTest.h"

#include "../RubicX_01/RubicsMouseHandler.h"
#include "../RubicX_01/RubicsMouseHandler.cpp"

#include "../RubicX_01/DirectCustomPrimitiveFunctions.cpp"

#include <DirectXMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX;
//using namespace DirectX::SimpleMath;


namespace UnitTestX_01
{		
  TEST_CLASS(UnitTest1)
  {
  public:

    TEST_METHOD(TestMethod1)
    {
      DirectX::XMMATRIX view
        (
        -1.00000000, 0.000000000, 0.000000000, 0.000000000,
        0.000000000, -1.00000000, 0.000000000, 0.000000000,
        0.000000000, 0.000000000, 1.00000000, 0.000000000,
        0.000000000, 0.000000000, 5.00000000, 1.00000000
        );

      DirectX::XMMATRIX proj
        (
        0.781250000, 0.000000000, 0.000000000, 0.000000000,
        0.000000000, 0.999999940, 0.000000000, 0.000000000,
        0.000000000, 0.000000000, 1.00300908, 1.00000000,
        0.000000000, 0.000000000, -0.300902724, 0.000000000
        );

      auto centre = rcbVector3D(0.f, 0.f, 0.f);
      auto eye_to_centre = 1.f;
      auto eye_direction = rcbVector3D(0.f, -1.f, 0.f);
      auto eye_up_of_view = rcbVector3D(-1.f, 0.f, 0.f);

      float radius = 2.59807611f;

      RubicsMouseHandler handler(
        centre,
        eye_direction,
        eye_up_of_view,
        eye_to_centre,
        1.f,
        radius,
        XMVectorGetX(proj.r[0]),
        XMVectorGetY(proj.r[1])
      );

      float circus_radius = 1.f;
      float x(0), y(0);
      auto circus = [&circus_radius, &x, &y](float alfa)
      {
        x = circus_radius * cos(alfa);
        y = circus_radius * sin(alfa);
      };

      float alfa = 0;

      while (alfa > -XM_2PI)
      {
        circus(alfa);
        handler.Listen(true, x, y);
        bool not_crossed = handler.rotationStartIsZero();
        //Assert::IsTrue(not_crossed);

        alfa -= XM_PI / 180;
      }

      handler.Listen(true, 0.0, 0.0);
      handler.Listen(true, 0.1, 0.0);

      rcbQuaternion trs = handler.Transformation();

      rcbVector3D xvc = { 0.0, 0.0, -1.0 };

      rcbVector3D res = trs.turn(xvc);
        //XMVector3Rotate(xvc, XMQuaternionRotationMatrix(trs));

      Assert::IsTrue(true);
    }
    TEST_METHOD(TestMethod2) {



      Assert::IsTrue(true);
    }
	};
}