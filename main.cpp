#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#include "common.h"
#include "classes.cpp"

//��Ϸ����

void Play(int window_width, int window_height) {
	cleardevice();
	//����ͼƬ
	Image_collect airplane_imags({ "images//me1.png", "images//me2.png" }, { "images//me_destroy_1.png" ,"images//me_destroy_2.png" ,"images//me_destroy_3.png","images//me_destroy_4.png" });
	Image_collect enemy_images({ "images//enemy1.png", "images//enemy2.png"}, { "images//enemy1_down1.png" ,"images//enemy1_down2.png" ,"images//enemy1_down3.png","images//enemy1_down4.png","images//enemy2_down1.png" ,"images//enemy2_down2.png" ,"images//enemy2_down3.png","images//enemy2_down4.png" });
	IMAGE bullet_my[2];
	loadimage(&bullet_my[0], _T("images//bullet2.png"), 0, 0);
	loadimage(&bullet_my[1], _T("images//bullet2_in.png"), 0, 0);
	IMAGE enemy_bullet[2];
	loadimage(&enemy_bullet[0], _T("images//bullet1.png"), 0, 0);
	loadimage(&enemy_bullet[1], _T("images//bullet1_in.png"), 0, 0);
	IMAGE bk_img;
	loadimage(&bk_img, _T("images//bk2.png"), 0, 0);
	//ʵ����������
	background bk(bk_img, window_height, 10);
	airplane<bullet> myself(window_width, window_height, airplane_imags, bullet_my[0], bullet_my[1]);
	enemy<bullet> ee(5, 0, enemy_images, enemy_bullet[0], enemy_bullet[1], window_width, window_height, 3);
	int count = 0;
	while (myself.islive()) {
		if (count % 3000 == 0)
			ee.add_enemy(1);
		BeginBatchDraw();
		bk.SHOW();
		Sleep(1);
		//��ײ���
		myself.MOUSE_MOVE(ee.pos,ee.img,ee.biu,ee.bullet_span);
		myself.score+=ee.SHOW(myself.biu);
		EndBatchDraw();
		count++;
	}
	string store_place = "images//record.txt";
	store(store_place, myself.score);
	outtextxy(20, 20, _T("PH=0"));
	IMAGE rebegin,drop;
	loadimage(&rebegin, _T("images//again.png"), 0, 0);
	loadimage(&drop, _T("images//gameover.png"), 0, 0);
	IMAGE rebegin_in, drop_in;
	loadimage(&rebegin_in, _T("images//again_in.png"), 0, 0);
	loadimage(&drop_in, _T("images//gameover_in.png"), 0, 0);
	putimage(0.5 * window_width - rebegin.getwidth() * 0.5, 0.4 * window_height - rebegin.getheight() * 0.5, &rebegin_in, SRCAND);
	putimage(0.5 * window_width - rebegin.getwidth() * 0.5, 0.4 * window_height - rebegin.getheight() * 0.5, &rebegin, SRCPAINT);
	putimage(0.5 * window_width - drop.getwidth() * 0.5, 0.6 * window_height - drop.getheight() * 0.5, &drop_in, SRCAND);
	putimage(0.5 * window_width - drop.getwidth() * 0.5, 0.6 * window_height - drop.getheight() * 0.5, &drop, SRCPAINT);
	RECT rect_rebegin = return_rectangle(rebegin, 0.5 * window_width, 0.4 * window_height,0);
	RECT rect_drop = return_rectangle(drop, 0.5 * window_width, 0.6 * window_height,0);
	while (true) {
		ExMessage a;
		getmessage(&a);
		cout << a.message << endl;
		if (a.message == WM_LBUTTONDOWN) {
			//��굥��
			//�ж��Ƿ��ھ���������
			if (judge_in_rect(rect_rebegin, a.x, a.y))
				Play(window_width,window_height);
			else if (judge_in_rect(rect_drop, a.x, a.y)) {
				cout << "������Ϸ";
				return;
			}
		}

	}
	Sleep(1000);
}

//��ӭ����
bool Welcome(int window_width,int window_height) {
	cleardevice();
	LOGFONT f;
	gettextstyle(&f);						// ��ȡ��ǰ��������
	f.lfHeight = 56;
	settextstyle(&f);// �޸������С
	outtextxy(window_width / 2 - 0.5 * textwidth(_T("�ɻ���ս")), 250, _T("�ɻ���ս"));
	f.lfHeight = 40;
	settextstyle(&f);
	outtextxy(window_width / 2 - 0.5 * textwidth(_T("��ʼ��Ϸ")), 500, _T("��ʼ��Ϸ"));
	outtextxy(window_width / 2 - 0.5 * textwidth(_T("���а�")), 600, _T("���а�"));
	outtextxy(window_width / 2 - 0.5 * textwidth(_T("�˳�")), 700, _T("�˳�"));
	RECT start_game_bottom = draw_rectangle(_T("��ʼ��Ϸ"), window_width / 2 - 0.5 * textwidth(_T("��ʼ��Ϸ")), 500, 10);
	RECT score_show = draw_rectangle(_T("���а�"), window_width / 2 - 0.5 * textwidth(_T("���а�")), 600, 10);
	RECT quit_bottom = draw_rectangle(_T("�˳�"), window_width / 2 - 0.5 * textwidth(_T("�˳�")), 700, 10);
	while (true) {
		ExMessage a;
		getmessage(&a);
		cout << a.message << endl;
		if (a.message == WM_LBUTTONDOWN) {
			//��굥��
			//�ж��Ƿ��ھ���������
			if (judge_in_rect(start_game_bottom, a.x, a.y))
				return true;
			else if (judge_in_rect(score_show, a.x, a.y)) {
				return false;
			}
			else if (judge_in_rect(quit_bottom, a.x, a.y)) {
				closegraph();
				exit(0);
			}
		}

	}
	return false;
}
int main()
{
	int window_width = 500;
	int window_height = 800;
	initgraph(window_width, window_height);	// ������ͼ���ڣ���СΪ 500x800 ����
	//cleardevice();
	int fetch_state = Welcome(window_width, window_height);
	while (true) {
		if(fetch_state==true)
			Play(window_width, window_height);
		else if(fetch_state==false)
			record_SHOW("images//record.txt", window_width, window_height);
		//�ص���ʼ����
		fetch_state=Welcome(window_width, window_height);
	}
	closegraph();
	return 0;
}