#pragma once

#include <Windows.h>

enum EKey_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};

const int Timer_ID = WM_USER + 1;

void Init_Engine(HWND hwnd);
void Drow_Frame(HDC hdc, RECT &paint_area);
int On_Key_Down(EKey_Type key_type);
int On_Timer();