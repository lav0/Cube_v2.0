
#include "../RubicX_01/CustomPrimitiveFunctions.h"
#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>

#ifdef RENDER_DRX

#include "../RubicX_01/DirectCustomPrimitiveFunctions.cpp"

#endif

using namespace CustomPrimitiveFunctions;

bool compare(const CALC_VECTOR4_DFN& v1, const CALC_VECTOR4_DFN& v2)
{
  if (fabs(GetX(v1) - GetX(v2)) > 1e-5)
    return false;

  if (fabs(GetY(v1) - GetY(v2)) > 1e-5)
    return false;

  if (fabs(GetZ(v1) - GetZ(v2)) > 1e-5)
    return false;

  if (fabs(GetW(v1) - GetW(v2)) > 1e-5)
    return false;

  return true;
}

static float PI = 3.14159265f;
static CALC_VECTOR4_DFN x = { 1.f, 0.f, 0.f, 0.f };
static CALC_VECTOR4_DFN y = { 0.f, 1.f, 0.f, 0.f };
static CALC_VECTOR4_DFN z = { 0.f, 0.f, 1.f, 0.f };
static CALC_VECTOR4_DFN q = { 0.f, 0.f, 0.707106769f, 0.707106769f };

void checkNorm(std::vector<CALC_VECTOR4_DFN>& vectors)
{
  std::vector<CALC_VECTOR4_DFN> expNorm = {
    { 1.f, 0.f, 0.f, 0.f },
    { 0.0f, 1.f, 0.f, 0.f },
    { 0.0f, 0.f, -1.f, 0.f },
    { 0.2873478f, 0.957826f, 0.f, 0.f },
    { 0.371390700f, 0.f, 0.928476691f, 0.f },
    { 0.0f, -0.894427180f, 0.447213590f, 0.f }
  };

  for (size_t i = 0; i < vectors.size(); ++i)
  {
    auto& v = vectors[i];

    auto result = Normalize(v);

    assert(compare(expNorm[i], result));
  }
}

void checkDot(std::vector<CALC_VECTOR4_DFN>& vectors)
{
  std::vector<CALC_VECTOR4_DFN> expDotX = {
    { 0.6f, 0.6f, 0.6f, 0.6f },
    { 0.0f, 0.f, 0.f, 0.f },
    { 0.0f, 0.f, 0.f, 0.f },
    { 0.6f, 0.6f, 0.6f, 0.6f },
    { 0.6f, 0.6f, 0.6f, 0.6f },
    { 0.0f, 0.f, 0.f, 0.f }
  };
  std::vector<CALC_VECTOR4_DFN> expDotY = {
    { 0.f, 0.f,  0.f, 0.f },
    { 0.7f, 0.7f, 0.7f, 0.7f },
    { 0.f, 0.f,  0.f, 0.f },
    { 2.f, 2.f, 2.f, 2.f },
    { 0.f, 0.f,  0.f, 0.f },
    { -1.f, -1.f, -1.f, -1.f }
  };
  std::vector<CALC_VECTOR4_DFN> expDotZ = {
    { 0.f, 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f, 0.f },
    { -0.8f, -0.8f, -0.8f, -0.8f },
    { 0.f, 0.f, 0.f, 0.f },
    { 1.5f, 1.5f, 1.5f, 1.5f },
    { 0.5f, 0.5f, 0.5f, 0.5f }
  };

  for (size_t i = 0; i < vectors.size(); ++i)
  {
    auto& v = vectors[i];

    auto resultX = DotProduct(v, x);
    if (!compare(expDotX[i], resultX))
      assert(false);

    auto resultY = DotProduct(v, y);
    if (!compare(expDotY[i], resultY))
      assert(false);

    auto resultZ = DotProduct(v, z);
    if (!compare(expDotZ[i], resultZ))
      assert(false);
  }
}

void checkCross(std::vector<CALC_VECTOR4_DFN>& vectors)
{
  std::vector<CALC_VECTOR4_DFN> expCrossX = {
    { 0.0f, 0.f, 0.f, 0.f },
    { 0.0f, 0.f, -0.7f, 0.f },
    { 0.0f, -0.8f, 0.f, 0.f },
    { 0.0f, 0.0f, -2.f, 0.0f },
    { 0.0f, 1.5f, 0.0f, 0.0f },
    { 0.0f, 0.5f, 1.f, 0.f }
  };
  std::vector<CALC_VECTOR4_DFN> expCrossY = {
    { 0.0f, 0.f, 0.6f, 0.f },
    { 0.0f, 0.f, 0.0f, 0.f },
    { 0.8f, 0.f, 0.f, 0.f },
    { 0.0f, 0.0f, 0.6f, 0.0f },
    { -1.5f, 0.0f, 0.6f, 0.0f },
    { -0.5f, 0.0f, 0.f, 0.f }
  };
  std::vector<CALC_VECTOR4_DFN> expCrossZ = {
    { 0.0f, -0.6f, 0.f, 0.f },
    { 0.7f, 0.f, 0.0f, 0.f },
    { 0.0f, 0.f, 0.f, 0.f },
    { 2.0f, -0.6f, 0.0f, 0.0f },
    { 0.f, -0.6f, 0.0f, 0.0f },
    { -1.0f, 0.0f, 0.f, 0.f }
  };
  
  for (size_t i = 0; i < vectors.size(); ++i)
  {
    auto& v = vectors[i];

    auto resultX = CrossProduct(v, x);
    if (!compare(expCrossX[i], resultX))
      assert(false);

    auto resultY = CrossProduct(v, y);
    if (!compare(expCrossY[i], resultY))
      assert(false);

    auto resultZ = CrossProduct(v, z);
    if (!compare(expCrossZ[i], resultZ))
      assert(false);
  }
}

void checkScale(std::vector<CALC_VECTOR4_DFN>& vectors)
{
  std::vector<CALC_VECTOR4_DFN> expScaleHalf = {
    { 0.3f, 0.f, 0.f, 0.f },
    { 0.0f, 0.35f, 0.f, 0.f },
    { 0.0f, 0.f, -0.4f, 0.f },
    { 0.3f, 1.f, 0.f, 0.f },
    { 0.3f, 0.f, 0.75f, 0.f },
    { 0.0f, -0.5f, 0.25f, 0.f }
  };
  std::vector<CALC_VECTOR4_DFN> expScaleMinTwo = {
    { -1.2f, 0.f, 0.f, 0.f },
    { 0.0f, -1.4f, 0.f, 0.f },
    { 0.0f, 0.f, 1.6f, 0.f },
    { -1.2f, -4.f, 0.f, 0.f },
    { -1.2f, 0.f, -3.f, 0.f },
    { 0.0f, 2.f, -1.0f, 0.f }
  };

  for (size_t i = 0; i < vectors.size(); ++i)
  {
    auto& v = vectors[i];

    auto resultH = VectorScale(v, 0.5f);
    if (!compare(expScaleHalf[i], resultH))
      assert(false);

    auto resultT = VectorScale(v, -2.f);
    if (!compare(expScaleMinTwo[i], resultT))
      assert(false);
  }
}

void checkQuaternion(std::vector<CALC_VECTOR4_DFN>& vectors)
{
  std::vector<CALC_VECTOR4_DFN> expQuaternion = {
    { 0.707106769f, 0.f, 0.f, 0.707106769f },
    { 0.0f, 0.707106769f, 0.f, 0.707106769f },
    { 0.0f, 0.f, -0.707106769f, 0.707106769f },
    { 0.203185633f, 0.677285433f, 0.f, 0.707106709f },
    { 0.262612879f, 0.f, 0.656532168f, 0.707106709f },
    { 0.0f, -0.632455528f, 0.316227764f, 0.707106709f }
  };

  for (size_t i = 0; i < vectors.size(); ++i)
  {
    auto& v = vectors[i];

    auto result = QuaternionRotationAxis(v, PI / 2);
    if (!compare(expQuaternion[i], result))
      assert(false);
  }
}

void checkRotation(std::vector<CALC_VECTOR4_DFN>& vectors)
{
  std::vector<CALC_VECTOR4_DFN> expVector = {
    { 0.0f, 0.6f, 0.f, 0.f },
    { -0.7f, 0.0f, 0.f, 0.f },
    { 0.0f, 0.f, -0.8f, 0.f },
    { -2.0f, 0.6f, 0.f, 0.f },
    { 0.f, 0.6f, 1.5f, 0.f },
    { 1.0f, 0.f, 0.5f, 0.f }
  };

  for (size_t i = 0; i < vectors.size(); ++i)
  {
    auto& v = vectors[i];

    auto result = RotateWithQuaternion(v, q);
    assert(compare(expVector[i], result));
  }
}

void checkAngles(std::vector<CALC_VECTOR4_DFN>& vectors)
{
  std::vector<CALC_VECTOR4_DFN> expAnglesX = {
    { 0.f, 0.f, 0.f, 0.f },
    { PI / 2, PI / 2, PI / 2, PI / 2 },
    { PI / 2, PI / 2, PI / 2, PI / 2 },
    { 1.27933943, 1.27933943, 1.27933943, 1.27933943 },
    { 1.19028986f, 1.19028986f, 1.19028986f, 1.19028986f },
    { PI / 2, PI / 2, PI / 2, PI / 2 }
  };
  std::vector<CALC_VECTOR4_DFN> expAnglesY = {
    { PI / 2, PI / 2, PI / 2, PI / 2 },
    { 0.f, 0.f, 0.f, 0.f },
    { PI / 2, PI / 2, PI / 2, PI / 2 },
    { 0.291456878, 0.291456878, 0.291456878, 0.291456878 },
    { PI / 2, PI / 2, PI / 2, PI / 2 },
    { 2.67794514, 2.67794514, 2.67794514, 2.67794514 }
  };


  for (size_t i = 0; i < vectors.size(); ++i)
  {
    auto& v = vectors[i];

    auto resultX = AngleBetweenNormals(Normalize(v), x);
    assert(compare(expAnglesX[i], resultX));

    auto resultY = AngleBetweenNormals(Normalize(v), y);
    assert(compare(expAnglesY[i], resultY));
  }
}

int main()
{
  std::vector<CALC_VECTOR4_DFN> vectors = {
    { 0.6f, 0.f, 0.f, 0.f },
    { 0.0f, 0.7f, 0.f, 0.f },
    { 0.0f, 0.f, -0.8f, 0.f },
    { 0.6f, 2.f, 0.f, 0.f },
    { 0.6f, 0.f, 1.5f, 0.f },
    { 0.0f, -1.f, 0.5f, 0.f }
  };

  checkNorm(vectors);
  checkDot(vectors);
  checkCross(vectors);
  checkScale(vectors);
  checkQuaternion(vectors);
  checkRotation(vectors);
  checkAngles(vectors);

  std::cout << "SUCCESS\n";

  return 0;
}
