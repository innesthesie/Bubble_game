#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#pragma comment(lib, "glut32.lib")
#define pi 3.14159265

struct b {
	int radius, stop, st;
	float x, y, dx, dy, ConstDx, ConstDy;
	float R, G, B;
	float acceleration;
};

int width, height;

class game {
private:
	int sectCnt, grad; //number of sectors, sector angle in degrees
	int botCnt;
	b bot[16];

public:
	void init();
	void draw();
	void drawCircle(int Ind);
	void strikeBoard(int Ind);
} g; //экземпляр класса



void game::init() {
	botCnt = 5;

	bot[0].x = 140;
	bot[0].y = 150;
	bot[0].radius = 45;
	bot[0].R = 0.9;
	bot[0].G = 0.1;
	bot[0].B = 0.2;
	bot[0].dx = 0.4;
	bot[0].dy = -0.6;
	bot[0].st = 0;
	bot[0].stop = 0;
	bot[0].acceleration = 0.00000008;

	bot[1].x = 200;
	bot[1].y = 200;
	bot[1].radius = 50;
	bot[1].R = 0.1;
	bot[1].G = 0.7;
	bot[1].B = 0.3;
	bot[1].dx = -0.6;
	bot[1].dy = -0.6;
	bot[1].st = 0;
	bot[1].stop = 0;
	bot[1].acceleration = 0.00000010;

	bot[2].x = 300;
	bot[2].y = 500;
	bot[2].radius = 65;
	bot[2].R = 0.1;
	bot[2].G = 0.2;
	bot[2].B = 0.7;
	bot[2].dx = -0.2;
	bot[2].dy = 0.7;
	bot[2].st = 0;
	bot[2].stop = 0;
	bot[2].acceleration = 0.00000016;

	bot[3].x = 500;
	bot[3].y = 300;
	bot[3].radius = 40;
	bot[3].R = 0.9;
	bot[3].G = 0.9;
	bot[3].B = 0.0;
	bot[3].dx = -1.1;
	bot[3].dy = -0.2;
	bot[3].st = 0;
	bot[3].stop = 0;
	bot[3].acceleration = 0.00000013;

	bot[4].x = 140;
	bot[4].y = 150;
	bot[4].radius = 70;
	bot[4].R = 0.6;
	bot[4].G = 0.0;
	bot[4].B = 0.6;
	bot[4].dx = 0.9;
	bot[4].dy = -1.4;
	bot[4].st = 0;
	bot[4].stop = 0;
	bot[4].acceleration = 0.0000008;

	for (int bo = 0; bo < botCnt; bo++) {
		bot[bo].ConstDx = bot[bo].dx;
		bot[bo].ConstDy = bot[bo].dy;
	}

	sectCnt = 32;
	grad = 360.f / sectCnt;
}


void game::draw() {
	int right, left, top, bottom;
	float delX, delY, s, p;
	right = left = top = bottom = 0;

	for (int i = 0; i < botCnt; i++) {
		drawCircle(i);
		if (bot[i].st == bot[i].stop) {
			for (int j = i + 1; j < botCnt; j++) {
				delX = bot[j].x - bot[i].x;
				delY = bot[j].y - bot[i].y;
				s = sqrt((float)delX * delX + delY * delY);
				p = bot[i].radius + bot[j].radius;

				if (s <= p) //расстояние между центрами окружностей<=сумме радиусов -> столкновение
				{			//расстояние между центром окр i и точкой центра накладки окружностей,a+b=s
					float a = fabs(bot[j].radius * bot[j].radius - bot[i].radius * bot[i].radius - s * s) / (2 * s);
					float b = fabs(s - a);

					float c1 = fabs(bot[i].radius - a) / 2; // расстояние c1=c2=radius-(расст от центра до границы 2ой окр)
					float c2 = fabs(bot[j].radius - b) / 2;


					if (bot[i].x > bot[j].x) {
						if (bot[i].x + c1 + bot[i].radius < width)
							bot[i].x += c1;
						if (bot[j].x - c2 - bot[j].radius > 0)
							bot[j].x -= c2;
					}
					else {
						if (bot[i].x - c1 - bot[i].radius > 0)
							bot[i].x -= c1;
						if (bot[j].x + c2 + bot[i].radius < width)
							bot[j].x += c2;
					}

					if (bot[i].y > bot[j].y) {
						if (bot[i].y + c1 + bot[i].radius < height)
							bot[i].y += c1;
						if (bot[j].y - c2 - bot[j].radius > 0)
							bot[j].y -= c2;
					}
					else {
						if (bot[i].y - c1 - bot[i].radius > 0)
							bot[i].y -= c1;
						if (bot[j].y + c2 + bot[j].radius < height)
							bot[j].y += c2;
					}

					float v1 = sqrt(bot[i].dx * bot[i].dx + bot[i].dy * bot[i].dy); //скорость вектора направления=длина
					float v2 = sqrt(bot[j].dx * bot[j].dx + bot[j].dy * bot[j].dy);
					float fi1 = atan2(bot[i].dy, bot[i].dx); //угол наклона вектора
					float fi2 = atan2(bot[j].dy, bot[j].dx); //Получаем углы наклона скоростей к оси OX
					float lambda1 = atan2(delY, delX);		 //Получаем углы наклона между линией соединяющей центры шаров и осью OX
					float lambda2 = atan2(-delY, -delX);
					float pv1 = v1 * cos(lambda1 - fi1); //Получаем проекции скоростей на соединяющую линию между шарами
					float pv2 = v2 * cos(lambda2 - fi2);

					bot[i].dx = bot[i].dx - pv1 * cos(lambda1) + pv2 * cos(lambda2);
					bot[i].dy = bot[i].dy - pv1 * sin(lambda1) + pv2 * sin(lambda2);

					bot[j].dx = bot[j].dx - pv2 * cos(lambda2) + pv1 * cos(lambda1);
					bot[j].dy = bot[j].dy - pv2 * sin(lambda2) + pv1 * sin(lambda1);
				}

				float cs = sqrt(bot[i].ConstDx * bot[i].ConstDx + bot[i].ConstDy * bot[i].ConstDy);
				float s = sqrt(bot[i].dx * bot[i].dx + bot[i].dy * bot[i].dy);
				if (s < cs) {
					if (s == 0) {
						bot[i].dx += bot[i].ConstDx / 100;
						bot[i].dy += bot[i].ConstDy / 100;
					}

					if (cs != 0) {
						bot[i].dx += bot[i].dx / 100;
						bot[i].dy += bot[i].dy / 100;
					}
				}
				else {
					bot[i].dx -= bot[i].dx / 100;
					bot[i].dy -= bot[i].dy / 100;
				}
			}

			strikeBoard(i);
			// смещение бота
			bot[i].x += bot[i].dx;
			bot[i].y += bot[i].dy;
			bot[i].st = 0;

			if (bot[i].dx >= 0)
				bot[i].dx += bot[i].acceleration;
			else
				bot[i].dx -= bot[i].acceleration;

			if (bot[i].dy >= 0)
				bot[i].dy += bot[i].acceleration;
			else
				bot[i].dy -= bot[i].acceleration;
		}
		else
			bot[i].st++;
	}
}


void game::strikeBoard(int Ind) {
	int right, left, top, bottom;
	right = left = top = bottom = 0;

	if (bot[Ind].x + bot[Ind].radius + fabs(bot[Ind].dx) >= width)
		right = 1;
	if (bot[Ind].x - bot[Ind].radius - fabs(bot[Ind].dx) <= 0)
		left = 1;
	if (bot[Ind].y + bot[Ind].radius + fabs(bot[Ind].dy) >= height)
		bottom = 1;
	if (bot[Ind].y - bot[Ind].radius - fabs(bot[Ind].dy) <= 0)
		top = 1;
	if (right) {
		bot[Ind].dx = -bot[Ind].dx;
		right = 0;
		if (bot[Ind].dx >= 0)
			bot[Ind].dx -= 10 * bot[Ind].acceleration;
		else
			bot[Ind].dx += 10 * bot[Ind].acceleration;
	}

	if (left) {
		bot[Ind].dx = -bot[Ind].dx;
		left = 0;
		if (bot[Ind].dx >= 0)
			bot[Ind].dx -= 10 * bot[Ind].acceleration;
		else
			bot[Ind].dx += 10 * bot[Ind].acceleration;
	}

	if (top) {
		bot[Ind].dy = -bot[Ind].dy;
		top = 0;
		if (bot[Ind].dy >= 0)
			bot[Ind].dy -= 10 * bot[Ind].acceleration;
		else
			bot[Ind].dy += 10 * bot[Ind].acceleration;
	}

	if (bottom) {
		bot[Ind].dy = -bot[Ind].dy;
		bottom = 0;
		if (bot[Ind].dy >= 0)
			bot[Ind].dy -= 10 * bot[Ind].acceleration;
		else
			bot[Ind].dy += 10 * bot[Ind].acceleration;
	}
}


void game::drawCircle(int Ind) {
	float x, y, radian;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(bot[Ind].R, bot[Ind].G, bot[Ind].B);
	glVertex2i(bot[Ind].x, bot[Ind].y);
	for (int i = 0; i <= sectCnt + 1; i++) {
		radian = i * grad * pi / 180.0;
		x = cos(radian) * bot[Ind].radius;
		y = sin(radian) * bot[Ind].radius;
		glVertex2f(x + bot[Ind].x, y + bot[Ind].y);
	};
	glEnd();
}


void reshape(int w, int h) {
	glViewport(0, 0, w, h); //область вывода изображения размером со все окно
	width = w;
	height = h;
	glMatrixMode(GL_PROJECTION); //загрузим матрицу проекции
	glLoadIdentity();			 //замена ее единичной - Сброс просмотра
	gluOrtho2D(0, w, 0, h);		 //установка вектор проекци

	glMatrixMode(GL_MODELVIEW); //загрузим модельно-видовую матрицу
	glLoadIdentity();			//замена ее единичной - Сброс просмотра
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT); //очистка буфера цвета
	glClearColor(1, 1, 1, 0);	  //цвет фона
	g.draw();
	glutSwapBuffers(); //смена буфера при двойной буферизации
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("OpenGL Game");

	g.init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutMainLoop();
	return 0;
}