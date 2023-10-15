#include "Engine.h"

const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;

enum EBrick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

HPEN Brick_Red_Pen, Brick_Blue_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

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
void Init()
{// Game settings at startup
   Brick_Red_Pen = CreatePen(PS_SOLID, 0, RGB(255, 85, 85));
   Brick_Red_Brush = CreateSolidBrush(RGB(255, 85, 85));

   Brick_Blue_Pen = CreatePen(PS_SOLID, 0, RGB(85, 255, 255));
   Brick_Blue_Brush = CreateSolidBrush(RGB(85, 255, 255));
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
void Drow_Level(HDC hdc)
{// Output of all bricks of the level
   int i, j;

   for (i = 0; i < 14; i++)
      for (j = 0; j < 12; j++)
         Drow_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}
//---------------------------------------------------------------------------------------------------
void Drow_Frame(HDC hdc)
{// Rendering the game screen
   //Drow_Level(hdc);

   int x = 50;
   int y = 100;
   int circle_size = 7;
   int inner_width = 21;

   SelectObject(hdc, Brick_Red_Pen);
   SelectObject(hdc, Brick_Red_Brush);

   Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + circle_size) * Global_Scale, (y + circle_size) * Global_Scale);
   Ellipse(hdc, (x + inner_width) * Global_Scale, y * Global_Scale, (x + circle_size + inner_width) * Global_Scale, (y + circle_size) * Global_Scale);

   SelectObject(hdc, Brick_Blue_Pen);
   SelectObject(hdc, Brick_Blue_Brush);

   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, 
      (x + 4 + inner_width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 3 * Global_Scale, 3 * Global_Scale);
}
//---------------------------------------------------------------------------------------------------

