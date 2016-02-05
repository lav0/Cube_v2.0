
#include "RubicsCubeManager.h"

#include "RubicsCubeBuilder.h"
#include "TimeMeasureDecorator.h"

#ifdef TIME_MEASURE
#define RubicsCubeFactory TimeMeasureDirectCubeFactoryDecorator
#endif

//=============================================================================
bool RubicsCubeManager::Initialize(std::unique_ptr<IFactory>&& renderer_factory,
                                   Dimention a_cube_dimention, 
                                   size_t a_tesselation
                                   )
{
  auto view_projection = renderer_factory->CreateViewProjection(eyePosition);
  
  setUpRubicsMouseHandler(view_projection.get());

  auto factory = std::make_unique<RubicsCubeFactory>(
    std::move(renderer_factory),
    std::move(view_projection),
    a_cube_dimention, 
    a_tesselation
  );

  RubicsCubeBuilder builder(a_cube_dimention, std::move(factory));

  m_cube = builder.BuildCube();

  return true;
}

//=============================================================================
void RubicsCubeManager::setUpRubicsMouseHandler(
  IViewProjectionWrapper* view_projection
)
{
  using namespace SharedFunctions;

  auto xv = castToMatrix(view_projection->GetView());
  auto xp = castToMatrix(view_projection->GetProjection());
  
  auto centre = rcbVector3D(0.f, 0.f, 0.f);
  auto eye_to_centre = eyeToCentre(xv);
  auto eye_direction = eyeDirectionOfView(xv);
  auto eye_up_of_view = eyeUpOfView(xv);
  auto scale_x = scaleX(xp);
  auto scale_y = scaleY(xp);

  m_mouse_handler = std::make_unique<RubicsMouseHandler>(
    centre,
    eye_direction,
    eye_up_of_view,
    eye_to_centre,
    1.f, 
    m_cube->OuterSphereRadius(),
    scale_x,
    scale_y
  );
}

//=============================================================================
void RubicsCubeManager::Render(float tima_lapsed)
//
//
//
{
  auto quat_rotate = m_mouse_handler->Transformation();
  
  m_cube->Update(tima_lapsed);
  m_cube->Rotate(quat_rotate);
  
  if (!m_cube->IsTurning() && !m_commands_sequence.empty())
  {
    m_cube->TurnFace(m_commands_sequence.front());
    m_commands_sequence.pop();
  }
        
  m_cube->Draw();
}

//--------------------------------------------------------------------------------------
void RubicsCubeManager::ListenToMouse(bool a_pressed, float a_x, float a_y)
//
//
//
{
  m_mouse_handler->Listen(a_pressed, a_x, a_y);
}

//=============================================================================
void RubicsCubeManager::ListenToKeyboard(short a_buttonPressed)
//
//
//
{  
  Dimention dim = m_cube->GetDimention();
  auto base = 3 * dim + 1;

  if (a_buttonPressed % base == 0)
    return;
  
  TurnCommand command;
  
  command.angle = A_90;
  
  // shift key handling  
  if (a_buttonPressed > base) {
    command.angle = A_270;
    a_buttonPressed -= base;
  }
  
  --a_buttonPressed;
  
  short axisId = a_buttonPressed / dim;

  command.axis = TA_X;

  if (axisId >= 1)
    command.axis = TA_Y;
  if (axisId >= 2)
    command.axis = TA_Z;

  command.face = a_buttonPressed - dim*axisId;

  m_commands_sequence.push(command);
}