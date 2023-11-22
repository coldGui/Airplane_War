#pragma once
#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>   // 包含延时函数所需的头文件
#include <thread>   // 包含延时函数所需的头文件

using namespace std;
#include <tchar.h>
RECT draw_rectangle(LPCTSTR c, int left, int top, int span);
RECT return_rectangle(IMAGE& c, int midx, int midy, int span);
bool judge_in_rect(RECT rect, int x, int y);
bool detect_is_overlap(RECT r1, RECT r2);

bool Welcome(int window_width, int window_height);
string loadPNGWithAlpha(string& fileName);
void store(string& filepath,int score);
void record_SHOW(string filepath, int window_width, int window_height);