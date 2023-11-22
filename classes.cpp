#include "common.h"


class background {
private:
	int window_height;
	int pic_height;
	int move_stride;
	int ypos;
	IMAGE& img;
public:
	background(IMAGE& img1, int height, int move_stride) :img(img1), window_height(height), move_stride(move_stride) {
		this->pic_height = img1.getheight();
		this->ypos = height - img1.getheight();
	};
	void SHOW() {
		//背景图下移
		this->ypos = this->ypos + this->move_stride;
		if (this->ypos > 0)
			this->ypos = this->window_height - img.getheight();
		putimage(0, this->ypos, &img);
	}
};

class Image_collect {
public:
	vector<IMAGE> normal;
	vector<IMAGE> normal_in;
	vector<IMAGE> crash;
	vector<IMAGE> crash_in;
	LPCTSTR StringToLPCTSTR(const std::string& str)
	{
		static std::wstring wstr;
		wstr.clear();
		wstr.assign(str.begin(), str.end());
		return wstr.c_str();
	}
	Image_collect(vector<string> normal_address, vector<string> crash_address) {
		for (string address : normal_address) {
			IMAGE img;
			loadimage(&img, StringToLPCTSTR(address), 0, 0);
			normal.push_back(img);
			loadimage(&img, StringToLPCTSTR(address.substr(0, address.size() - 4) + "_in.png"), 0, 0);
			normal_in.push_back(img);
		}
		for (string address : crash_address) {
			IMAGE img;
			loadimage(&img, StringToLPCTSTR(address), 0, 0);
			crash.push_back(img);
			loadimage(&img, StringToLPCTSTR(address.substr(0, address.size() - 4) + "_in.png"), 0, 0);
			crash_in.push_back(img);
		}
	}
};

template<class T>
bool detect_is_overlap(RECT r1, T& eimg, RECT r2, T& img) {
	//取消矩形检测碰撞，采用opencv
	//合并矩形，并比较是否有非0像素点损失
	/*int left = min(r1.left, r2.left);
	int right = max(r1.right, r2.right);
	int top = min(r1.top, r2.top);
	int bottom = max(r1.bottom, r2.bottom);
	cv::Mat mat(img.normal[0].getheight(), img.normal[0].getwidth(), CV_8UC4, img.normal[0].getdata());
	cv::Mat extendedImage(bottom-top, right-left, mat.type());
	cv::Mat roi(extendedImage, cv::Rect(0, newHeight - mat.rows, mat.cols, mat.rows));
	mat.copyTo(roi);
	cv::imshow("Original Image", originalImage);
    cv::imshow("Extended Image", extendedImage);
    cv::waitKey(0);*/
	vector<int> x_store = { r1.left,r1.right,r2.left,r2.right };
	vector<int> y_store = { r1.top,r1.bottom,r2.top,r2.bottom };
	sort(x_store.begin(), x_store.end());
	sort(y_store.begin(), y_store.end());
	if ((x_store[0] == r1.left && x_store[1] == r2.left) || (x_store[0] == r2.left && x_store[1] == r1.left))
		if ((y_store[0] == r1.top && y_store[1] == r2.top) || (y_store[0] == r2.top && y_store[1] == r1.top))
			return true;
	return false;
}

class bullet {
private:
	IMAGE& img;
	IMAGE& img_in;
	int direction;
	int stride;
	int window_height;
public:
	RECT pos;
	bullet(int x, int y, IMAGE& img, IMAGE& img_in, int direction, int window_height,int stride = 20) :img(img),img_in(img_in), direction(direction), stride(stride), window_height(window_height) {
		pos.left = x - img.getwidth() * 0.5;
		pos.right = x + img.getwidth() * 0.5;
		pos.top = y - img.getheight() * 0.5;
		pos.bottom = y + img.getheight() * 0.5;
	}
	bool SHOW() {
		pos.top = pos.top + direction * stride;
		pos.bottom = pos.bottom + direction * stride;
		if (pos.top >= window_height)
			return false;
		putimage(pos.left,pos.top,&img_in, SRCAND);
		putimage(pos.left, pos.top, &img, SRCPAINT);
		return true;
	}
};

template<class T>
class airplane {
private:
	int PH;
	RECT pos;
	Image_collect& images;
	IMAGE bullet_img;
	IMAGE bullet_img_in;
	int window_height;
	int bullet_span;
	int crash_time;
public:
	vector<T*> biu;
	int score;
	airplane(int window_width, int window_height, Image_collect& images, IMAGE bullet_img, IMAGE bullet_img_in, int PH = 10) :images(images),bullet_img(bullet_img),bullet_img_in(bullet_img_in), PH(PH), window_height(window_height) {
		pos.left = window_width / 2 - images.normal[0].getwidth() / 2;
		pos.right = window_width / 2 + images.normal[0].getwidth() / 2;
		pos.top = window_height - images.normal[0].getheight();
		pos.bottom = window_height;
		SHOW(window_width / 2, window_height - images.normal[0].getheight() / 2);
		bullet_span = 0;
		crash_time = 0;
		//cout << "飞机实例化成功！" << endl;
	}
	void SHOW(int x, int y) {
		if (PH <= 0) {
			putimage(pos.left, pos.top, &this->images.crash_in[crash_time], SRCAND);
			putimage(pos.left, pos.top, &this->images.crash[crash_time], SRCPAINT);
			std::this_thread::sleep_for(std::chrono::milliseconds(70));
			crash_time++;
			return;
		}
		BULLET_SHOW(x,y);
		if (rand() % 2 >= 1) {
			putimage(x - this->images.normal[0].getwidth() / 2, y - this->images.normal[0].getheight() / 2, &this->images.normal_in[0], SRCAND);
			putimage(x - this->images.normal[0].getwidth() / 2, y - this->images.normal[0].getheight() / 2, &this->images.normal[0], SRCPAINT);
		}
		else {
			putimage(x - this->images.normal[0].getwidth() / 2, y - this->images.normal[0].getheight() / 2, &this->images.normal_in[1], SRCAND);
			putimage(x - this->images.normal[0].getwidth() / 2, y - this->images.normal[0].getheight() / 2, &this->images.normal[1], SRCPAINT);
		}
		pos.left = x - this->images.normal[0].getwidth() / 2;
		pos.top = y - this->images.normal[0].getheight() / 2;
		pos.right = pos.left + images.normal[0].getwidth();
		pos.bottom = pos.top + images.normal[0].getheight();
	}
	void SHOW() {
		if (PH <= 0) {
			putimage(pos.left, pos.top, &this->images.crash_in[crash_time], SRCAND);
			putimage(pos.left, pos.top, &this->images.crash[crash_time], SRCPAINT);
			std::this_thread::sleep_for(std::chrono::milliseconds(70));
			crash_time++;
			return;
		}
		BULLET_SHOW((pos.left + pos.right) / 2, pos.top);
		if (rand() % 2 >= 1) {
			putimage(pos.left, pos.top, &this->images.normal_in[0], SRCAND);
			putimage(pos.left, pos.top, &this->images.normal[0], SRCPAINT);
		}
		else {
			putimage(pos.left, pos.top, &this->images.normal_in[1], SRCAND);
			putimage(pos.left, pos.top, &this->images.normal[1], SRCPAINT);
		}
	}
	void BULLET_SHOW(int x,int y) {
		bullet_span++;
		if (bullet_span % 4 == 0) {
			T* new_bullet=new bullet(x, y, bullet_img, bullet_img_in, -1, window_height);
			biu.insert(biu.begin(),new_bullet);
		}
		int n = biu.size();
		for (int i = 0; i < n; i++) {
			if (i >= biu.size())
				break;
			if (biu[i]->pos.top > window_height)
			{
				delete biu[i];
				biu.erase(biu.begin() + i);
			}
		}
		n = biu.size();
		for (int i = 0; i < n; i++)
			biu[i]->SHOW();
	}
	void MOUSE_MOVE(vector<RECT>& epos,Image_collect& eimg, vector<T*>& ebiu,vector<int>& bullet_span) {
		if (PH < 0)
			PH = 0;
		string PH_line = "PH=" + to_string(PH);
		std::wstring wstr(PH_line.begin(), PH_line.end());
		LPCTSTR  lpcstr = wstr.c_str();
		outtextxy(20, 20, lpcstr);
		string SCORE_line = "SCORE=" + to_string(score);
		std::wstring wstr2(SCORE_line.begin(), SCORE_line.end());
		LPCTSTR  lpcstr2 = wstr2.c_str();
		outtextxy(20, 80, lpcstr2);
		ExMessage msg;
		if (peekmessage(&msg, EM_MOUSE)) {
			SHOW(msg.x, msg.y);
		}
		else { SHOW(); }
		//碰撞检测
		int n = epos.size();
		RECT smaller_pos = { pos.left+10 , pos.top+10 ,pos.right-10,pos.bottom-10 };

		for (int i = 0; i < n;i++) {
			if (detect_is_overlap<Image_collect>(epos[i], eimg, smaller_pos,images)) {
				epos.erase(epos.begin()+i);
				bullet_span.erase(bullet_span.begin() + i);
				cout << "发生碰撞！" << endl;
				PH--;
				//加入碰撞动画
				break;
			}
		}
		int m = ebiu.size();
		cout << m << endl;
		for (int j = 0; j < m; j++) {
			if (detect_is_overlap(ebiu[j]->pos, smaller_pos)) {
				ebiu.erase(ebiu.begin() + j);
				cout << "受到子弹攻击" << endl;
				PH--;
				break;
			}
		}
	}
	bool islive() {
		if (PH <= 0 and crash_time>3) {
			return false;
		}
		return true;
	}
};

template<class T>
class enemy {
private:
	int window_width;
	int window_height;
	int move_stride;
	int start_height;
	int enemy_num;
	IMAGE bullet_img;
	IMAGE bullet_img_in;

public:
	Image_collect& img;
	vector<RECT> pos;
	vector<int> bullet_span;
	vector<T*> biu;
	enemy(int enemy_num, int enemy_start_height, Image_collect& img,IMAGE bullet_img,IMAGE bullet_img_in, int window_width, int window_height, int move_stride)
		:img(img), bullet_img(bullet_img),bullet_img_in(bullet_img_in),start_height(enemy_start_height), enemy_num(enemy_num), window_width(window_width), window_height(window_height), move_stride(move_stride) {
		product_enemy();
		bullet_span ={};
		pos = {};
		vector<T*> empty_bullet;
		SHOW(empty_bullet);
	}
	void add_enemy(int add) {
		enemy_num+=add;
	}
	void product_enemy(int sign = 0) {
		if (sign == 1) {
			RECT a;
			a.top = start_height - img.normal[0].getheight();
			a.bottom = start_height;
			a.left = rand() % (window_width - img.normal[0].getwidth());
			a.right = a.left + img.normal[0].getwidth();
			pos.push_back(a);
			bullet_span.push_back(0);
			//检测碰撞
			int n = pos.size();
			if (n >= 2) {
				for (int i = 0; i < n - 1; i++) {
					if (detect_is_overlap(pos[i], pos[n - 1])) {
						pos.pop_back();
						bullet_span.pop_back();
						product_enemy(sign);
					}
				}
			}
			return;
		}
		if (pos.size() < enemy_num and rand() % 100 <= 1) {
			//根据随机数生成敌机
			RECT a;
			a.top = start_height;
			a.bottom = start_height + img.normal[0].getheight();
			a.left = rand() % (window_width - img.normal[0].getwidth());
			a.right = a.left + img.normal[0].getwidth();
			pos.push_back(a);
			bullet_span.push_back(0);
			//检测碰撞
			int n = pos.size();
			if (n >= 2) {
				for (int i = 0; i < n - 1; i++) {
					if (detect_is_overlap(pos[i], pos[n - 1])) {
						pos.pop_back();
						bullet_span.pop_back();
						product_enemy(sign);
					}
				}
			}
		}
		return;
	}
	int SHOW(vector<T*>& my_bullet) {
		product_enemy();
		int n = pos.size();
		for (int i = 0; i < n; i++) {
			pos[i].top = pos[i].top + move_stride;
			pos[i].bottom = pos[i].bottom + move_stride;
			if (pos[i].top >= window_height) {
				pos.erase(pos.begin() + i);
				bullet_span.erase(bullet_span.begin() + i);
				product_enemy(1);
			}
		}
		n = pos.size();
		int m = my_bullet.size();
		int flag = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (detect_is_overlap(my_bullet[j]->pos, pos[i])) {
					my_bullet.erase(my_bullet.begin() + j);
					if(pos.size()>i)
						pos.erase(pos.begin() + i);
					cout << "子弹攻击成功" << endl;
					flag++;
					break;
				}
			}
			BULLET_MAKE((pos[i].left + pos[i].right) / 2, pos[i].bottom,i);
			putimage(pos[i].left, pos[i].top, &img.normal_in[0], SRCAND);
			putimage(pos[i].left, pos[i].top, &img.normal[0], SRCPAINT);
		}

		BULLET_SHOW();
		return flag;
	}
	void BULLET_MAKE(int x, int y,int i) {
		bullet_span[i]++;
		if (bullet_span[i] % 50 == 0) {
			T* new_bullet=new bullet(x, y, bullet_img,bullet_img_in, 1, window_height,10);
			biu.insert(biu.begin(), new_bullet);
		}
	}
	void BULLET_SHOW() {
		int n = biu.size();
		for (int i = 0; i < n; i++) {
			if (i >= biu.size())
				break;
			if (biu[i]->pos.top > window_height) {
				delete biu[i];
				biu.erase(biu.begin() + i);
			}
		}
		n = biu.size();
		for (int i = 0; i < n; i++)
			biu[i]->SHOW();
	}
};
