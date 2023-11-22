#pragma once
#include <graphics.h>		// ����ͼ�ο�ͷ�ļ�
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
//��Բ�Ǿ���
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

//���μ����ײ
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

//����mask,�ڰ����֣�
//string airplane_img_root = "images//resume_pressed.png";
//loadPNGWithAlpha(airplane_img_root);
//string loadPNGWithAlpha(string& fileName)
//{
//	cv::Mat image = cv::imread(fileName, cv::IMREAD_UNCHANGED);
//	if (image.empty())
//	{
//		std::cout << "Could not open or find the image" << std::endl;
//	}
//	// ����RGBAͨ��
//	std::vector<cv::Mat> channels;
//	cv::split(image, channels);
//
//	// ��������ͼ�񣬽�͸��������Ϊ��ɫ����͸��������Ϊ��ɫ
//	cv::Mat mask1 = cv::Mat::zeros(image.size(), CV_8UC1);
//	cv::threshold(channels[3], mask1, 0, 255, cv::THRESH_BINARY);
//	cv::Mat mask2 = cv::Mat::zeros(mask1.size(), mask1.type());
//	cv::bitwise_not(mask1, mask2);
//	// ��������ͼ��
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

//�洢�ɼ�
void store(string& filepath,int score) {
	// ��ȡ��ǰʱ��
	std::time_t currentTime = std::time(nullptr);

	// ��ʱ��ת��Ϊ����ʱ��
	std::tm localTimeInfo;
	localtime_s(&localTimeInfo, &currentTime);

	// ��ȡ�ꡢ�¡��ա�ʱ���֡���
	int year = localTimeInfo.tm_year + 1900;      // �����Ҫ����1900
	int month = localTimeInfo.tm_mon + 1;         // �·ݴ�0��ʼ��������Ҫ��1
	int day = localTimeInfo.tm_mday;
	int hour = localTimeInfo.tm_hour;
	int minute = localTimeInfo.tm_min;
	int second = localTimeInfo.tm_sec;

	// �������ں�ʱ���ַ���
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
	if (file.is_open()) { // ����ļ��Ƿ�ɹ���
		std::string line;
		while (std::getline(file, line)) { // ���ж�ȡ�ļ�����
			if (count % 2 == 0)
				play_name.push_back(line);
			else
				scores.push_back(stoi(line));
			count++;
		}
		file.close(); // �ر��ļ�
	}
	else {
		std::cout << "�޷����ļ�" << std::endl;
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
	std::cout << "���ݴ���" << scores.size()<<std::endl;
	std::ofstream file2(filepath, std::ios::out | std::ios::trunc);
	if (file2.is_open()) {
		std::cout << "a" << std::endl;
		int len = play_name.size();
		std::cout << "b" << std::endl;
		for (int i = 0; i < len; i++) {
			std::cout << "c" << std::endl;
			file2 << play_name[i] << std::endl; // ���ļ���д������
			std::cout << "d" << std::endl;
			file2 << to_string(scores[i]) << std::endl;
			std::cout << "e" << std::endl;
		}
		std::cout << "f" << std::endl;
		file.close(); // �ر��ļ�
		std::cout << "�����ѳɹ�д���ļ�" << std::endl;
	}
	else {
		std::cout << "��ʧ�ܣ�" << endl;
		std::perror("Error while opening file: ");
	}
}

void record_SHOW(string filepath,int window_width,int window_height) {
	cleardevice();
	outtextxy(window_width / 2 - 0.5 * textwidth(_T("���а�")), 100, _T("���а�"));
	outtextxy(window_width / 2 - 0.5 * textwidth(_T("���ز˵�")), window_height- 100, _T("���ز˵�"));
	RECT quit_bottom = draw_rectangle(_T("���ز˵�"), window_width / 2 - 0.5 * textwidth(_T("���ز˵�")), window_height - 100, 10);
	std::ifstream file(filepath); 

	int start = 200;
	if (file.is_open()) { // ����ļ��Ƿ�ɹ���
		while (true) { // ���ж�ȡ�ļ�����
			std::string line1;
			std::string line2;
			if (!std::getline(file, line1))
				break;
			std::getline(file, line2);
			LPCTSTR s = StringToLPCTSTR(line1+"   :   " + line2);
			outtextxy(window_width / 2 - 0.5 * textwidth(s), start, s);
			start += 100;
		}
		file.close(); // �ر��ļ�
	}
	else {
		std::cout << "�޷����ļ�" << std::endl;
	}
	while (true) {
		ExMessage a;
		getmessage(&a);
		cout << a.message << endl;
		if (a.message == WM_LBUTTONDOWN) {
			//��굥��
			//�ж��Ƿ��ھ���������
			if (judge_in_rect(quit_bottom, a.x, a.y))
				return;
		}

	}
}