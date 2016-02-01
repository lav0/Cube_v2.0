#include "stdafx.h"
#include "CppUnitTest.h"

#include "../RubicX_01/MouseHandler.h"
#include "../RubicX_01/MouseHandler.cpp"

#include "../RubicX_01/DirectCustomPrimitiveFunctions.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX;
using namespace DirectX::SimpleMath;


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

      float radius = 2.59807611f;

      MouseHandler handler(view, proj, 1.f, XMVectorZero(), radius);

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
        Assert::IsTrue(not_crossed);

        alfa -= XM_PI / 180;
      }

      handler.Listen(true, 0.0, 0.0);
      handler.Listen(true, 0.1, 0.0);

      XMMATRIX trs = handler.Transformation();

      XMVECTOR xvc = { 0.0, 0.0, -1.0 };

      XMVECTOR res = XMVector3Rotate(xvc, XMQuaternionRotationMatrix(trs));

      Assert::IsTrue(true);
    }
    TEST_METHOD(TestMethod2) {



      Assert::IsTrue(true);
    }
	};
}