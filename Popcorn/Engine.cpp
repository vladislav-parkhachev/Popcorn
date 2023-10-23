#include "Engine.h"

#define _USE_MATH_DEFINES
#include <math.h>

const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Circle_Size = 7;
int Inner_Width = 21;

enum EBrick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

HPEN Highlight_Pen, Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush;

char Level_01[14][12] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//---------------------------------------------------------------------------------------------------
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}
//---------------------------------------------------------------------------------------------------
void Init()
{// Game settings at startup
   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

   Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
   Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
   Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
   Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}
//---------------------------------------------------------------------------------------------------
void Drow_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{// Brick output to the screen

   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_None: 
      return;
   case EBT_Red:
      pen = Brick_Red_Pen;
      brush = Brick_Red_Brush;
      break;
   case EBT_Blue: 
      pen = Brick_Blue_Pen;
      brush = Brick_Blue_Brush;
      break;
   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   RoundRect(hdc, x * Global_Scale, y * Global_Scale, 
      (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 2 * Global_Scale, 2 * Global_Scale);
}
//---------------------------------------------------------------------------------------------------
void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
{
   if (is_switch_color)
   {
      front_pen = Brick_Red_Pen;
      front_brush = Brick_Red_Brush;

      back_pen = Brick_Blue_Pen;
      back_brush = Brick_Blue_Brush;
   }
   else
   {
      front_pen = Brick_Blue_Pen;
      front_brush = Brick_Blue_Brush;

      back_pen = Brick_Red_Pen;
      back_brush = Brick_Red_Brush;
   }
}
//---------------------------------------------------------------------------------------------------
void Drow_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, int rotation_step)
{// Drawing a falling letter
   bool switch_color;
   double offset;
   double rotation_angle; // Converting a step to a rotation angle
   int brick_half_height = Brick_Height * Global_Scale / 2;
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;

   if (!(brick_type == EBT_Blue || brick_type == EBT_Red))
      return; // Falling letters can only be from bricks of this type

   // Adjust the rotation step and the rotation angle
   rotation_step = rotation_step % 16;

   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16 * (double)rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16 * (double)(8L - (long long)rotation_step);

   if (rotation_step > 4 && rotation_step <= 12)
   {
      if (brick_type == EBT_Blue)
         switch_color = true;
      else
         switch_color = false;
   }
   else
   {
      if (brick_type == EBT_Red)
         switch_color = true;
      else
         switch_color = false;;
   }

   Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

   if (rotation_step == 4 || rotation_step == 12)
   {
      // Drow the background
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, x, y + brick_half_height - Global_Scale, x + Brick_Width * Global_Scale, y + brick_half_height);

      // Draw foreground
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * Global_Scale, y + brick_half_height + Global_Scale - 1);
   }
   else
   {
      SetGraphicsMode(hdc, GM_ADVANCED);

      // Setting up the matrix of the "rotation" of the letter
      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx = (float)x;
      xform.eDy = (float)y + (float)(brick_half_height);
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // Drow the background
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Global_Scale;
      back_part_offset = (int)round(offset);
      Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * Global_Scale, brick_half_height - back_part_offset);

      // Draw foreground
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, -brick_half_height, Brick_Width * Global_Scale, brick_half_height);

      SetWorldTransform(hdc, &old_xform);
   }
}

void Drow_Level(HDC hdc)
{// Output of all bricks of the level
   int i, j;

   for (i = 0; i < 14; i++)
      for (j = 0; j < 12; j++)
         Drow_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}
//---------------------------------------------------------------------------------------------------
void Drow_Platform(HDC hdc, int x, int y)
{// Draw a platform

   // 1. Draw the side balls
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);

   Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Circle_Size + Inner_Width) * Global_Scale, (y + Circle_Size) * Global_Scale);
   
   // 2. Draw highlight
   SelectObject(hdc, Highlight_Pen);

   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale,
      (x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale, (x + 1) * Global_Scale, (y + 1 + 2) * Global_Scale);

   // 3. Draw the inner part
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);

   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, 
      (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 3 * Global_Scale, 3 * Global_Scale);
}
//---------------------------------------------------------------------------------------------------
void Drow_Frame(HDC hdc)
{// Rendering the game screen

   //Drow_Level(hdc);
   
   //Drow_Platform(hdc, 50, 100);

   int i;

   for (i = 0; i < 16; i++)
   {
      Drow_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, i);
      Drow_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Red, i);
   }   
}
//---------------------------------------------------------------------------------------------------