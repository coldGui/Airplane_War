#pragma once
#include <graphics.h>		// 引用图形库头文件
#include <conio.h>
#include <iostream>
#include <vector>
#include <algorithm>
//#include "opencv2/core.hpp"
//#include "opencv2/cudaarithm.hpp"
//#include<opencv2/cudaimgproc.hpp>
#include<opencv2/opencv.hpp>
#include <tchar.h>
#include <fstream>
//using namespace cv;
//using namespace cv::cuda;
using namespace std;
//画圆角矩形
RECT draw_rectangle(LPCTSTR c, int left, int top, int span) {
	RECT result;
	result.left = left - span;
	result.top = top - span;
	result.right = left + textwidth(c) + span;
	result.bottom = top + textheight(c) + span;
	roundrect(result.left, result.top, result.right, result.bottom, 5, 5);
	return result;
}

RECT return_rectangle(IMAGE& c, int midx, int midy, int span) {
	RECT result;
	result.left = midx - 0.5 * c.getwidth() - span;
	result.top = midy - 0.5 * c.getheight()- span;
	result.right = midx + 0.5*c.getwidth() + span;
	result.bottom = midy + 0.5*c.getheight() + span;
	//roundrect(result.left, result.top, result.right, result.bottom, 5, 5);
	return result;
}

bool judge_in_rect(RECT rect, int x, int y) {
	if (x >= rect.left && y >= rect.top && x <= rect.right && y <= rect.bottom) {
		return true;
	}
	return false;
}

//矩形检测碰撞
bool detect_is_overlap(RECT r1, RECT r2) {
	vector<int> x_store = { r1.left,r1.right,r2.left,r2.right };
	vector<int> y_store = { r1.top,r1.bottom,r2.top,r2.bottom };
	sort(x_store.begin(), x_store.end());
	sort(y_store.begin(), y_store.end());
	if ((x_store[0] == r1.left && x_store[1] == r2.left) || (x_store[0] == r2.left && x_store[1] == r1.left))
		if ((y_store[0] == r1.top && y_store[1] == r2.top) || (y_store[0] == r2.top && y_store[1] == r1.top))
			return true;
	return false;
}

//设置mask,黑白遮罩，
//string airplane_img_root = "images//resume_pressed.png";
//loadPNGWithAlpha(airplane_img_root);
//string loadPNGWithAlpha(string& fileName)
//{
//	cv::Mat image = cv::imread(fileName, cv::IMREAD_UNCHANGED);
//	if (image.empty())
//	{
//		std::cout << "Could not open or find the image" << std::endl;
//	}
//	// 分离RGBA通道
//	std::vector<cv::Mat> channels;
//	cv::split(image, channels);
//
//	// 创建遮罩图像，将透明部分设为黑色，不透明部分设为白色
//	cv::Mat mask1 = cv::Mat::zeros(image.size(), CV_8UC1);
//	cv::threshold(channels[3], mask1, 0, 255, cv::THRESH_BINARY);
//	cv::Mat mask2 = cv::Mat::zeros(mask1.size(), mask1.type());
//	cv::bitwise_not(mask1, mask2);
//	// 保存遮罩图像
//	int size = fileName.size();
//	string newname=fileName.substr(0, size - 4);
//	fileName = fileName.substr(0, size - 4) + "_out.png";
//	newname += "_in.png";
//	cv::imwrite(fileName, mask1);
//	cv::imwrite(newname, mask2);
//}

LPCTSTR StringToLPCTSTR(const std::string& str)
{
	static std::wstring wstr;
	wstr.clear();
	wstr.assign(str.begin(), str.end());
	return wstr.c_str();
}

//存储成绩
void store(string& filepath,int score) {
	// 获取当前时间
	std::time_t currentTime = std::time(nullptr);

	// 将时间转换为本地时间
	std::tm localTimeInfo;
	localtime_s(&localTimeInfo, &currentTime);

	// 获取年、月、日、时、分、秒
	int year = localTimeInfo.tm_year + 1900;      // 年份需要加上1900
	int month = localTimeInfo.tm_mon + 1;         // 月份从0开始，所以需要加1
	int day = localTimeInfo.tm_mday;
	int hour = localTimeInfo.tm_hour;
	int minute = localTimeInfo.tm_min;
	int second = localTimeInfo.tm_sec;

	// 构造日期和时间字符串
	std::string datetime = std::to_string(year) + "-" +
		std::to_string(month) + "-" +
		std::to_string(day) + " " +
		std::to_string(hour) + ":" +
		std::to_string(minute) + ":" +
		std::to_string(second);

	std::ifstream file(filepath);
	vector<string> play_name;
	vector<int> scores;
	int count = 0;
	if (file.is_open()) { // 检查文件是否成功打开
		std::string line;
		while (std::getline(file, line)) { // 逐行读取文件内容
			if (count % 2 == 0)
				play_name.push_back(line);
			else
				scores.push_back(stoi(line));
			count++;
		}
		file.close(); // 关闭文件
	}
	else {
		std::cout << "无法打开文件" << std::endl;
	}
	int n = scores.size();
	if (n == 0) {
		scores.push_back(score);
		play_name.push_back(datetime);
	}
	for (int i = 0; i < n; i++) {
		if (scores[i] < score) {
			scores.insert(scores.begin() + i,score);
			play_name.insert(play_name.begin() + i, datetime);
			break;
		}
	}
	if (scores.size() > 5) {
		scores.pop_back();
		play_name.pop_back();
	}
	std::cout << "数据存在" << scores.size()<<std::endl;
	std::ofstream file2(filepath, std::ios::out | std::ios::trunc);
	if (file2.is_open()) {
		std::cout << "a" << std::endl;
		int len = play_name.size();
		std::cout << "b" << std::endl;
		for (int i = 0; i < len; i++) {
			std::cout << "c" << std::endl;
			file2 << play_name[i] << std::endl; // 向文件中写入数据
			std::cout << "d" << std::endl;
			file2 << to_string(scores[i]) << std::endl;
			std::cout << "e" << std::endl;
		}
		std::cout << "f" << std::endl;
		file.close(); // 关闭文件
		std::cout << "数据已成功写入文件" << std::endl;
	}
	else {
		std::cout << "打开失败！" << endl;
		std::perror("Error while opening file: ");
	}
}

void record_SHOW(string filepath,int window_width,int window_height) {
	cleardevice();
	outtextxy(window_width / 2 - 0.5 * textwidth(_T("排行榜")), 100, _T("排行榜"));
	outtextxy(window_width / 2 - 0.5 * textwidth(_T("返回菜单")), window_height- 100, _T("返回菜单"));
	RECT quit_bottom = draw_rectangle(_T("返回菜单"), window_width / 2 - 0.5 * textwidth(_T("返回菜单")), window_height - 100, 10);
	std::ifstream file(filepath); 

	int start = 200;
	if (file.is_open()) { // 检查文件是否成功打开
		while (true) { // 逐行读取文件内容
			std::string line1;
			std::string line2;
			if (!std::getline(file, line1))
				break;
			std::getline(file, line2);
			LPCTSTR s = StringToLPCTSTR(line1+"   :   " + line2);
			outtextxy(window_width / 2 - 0.5 * textwidth(s), start, s);
			start += 100;
		}
		file.close(); // 关闭文件
	}
	else {
		std::cout << "无法打开文件" << std::endl;
	}
	while (true) {
		ExMessage a;
		getmessage(&a);
		cout << a.message << endl;
		if (a.message == WM_LBUTTONDOWN) {
			//鼠标单击
			//判断是否在矩形区域内
			if (judge_in_rect(quit_bottom, a.x, a.y))
				return;
		}

	}
}