#pragma once

struct IKeyboardListener
{
  virtual ~IKeyboardListener() {}

  virtual void ListenToKeyboard(short a_buttonPressed) = 0;
};

struct IMouseListener
{
  virtual ~IMouseListener() {}

  virtual void ListenToMouse(bool a_pressed, float a_x, float a_y) = 0;
};