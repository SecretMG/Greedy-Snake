#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>

/* todo: 
		score(with border)
		buff(lives'L', speeddown'D')
		debuff(confuse'C', speedup'U')
		美化
*/
const int height = 20;
const int width = 30;
int mp[height][width] = { 0 };				// 0：空地，-1：边框， 1：蛇头，正数：蛇身
int direction;								// 蛇的移动方向1, 2, 3, 4：上，下，左，右
int food_x, food_y;							// 食物的位置
int t_cool;									// 越大移动速度越慢
											
// 将光标移动到(x,y)位置，替代system("cls")清屏函数，防止发生闪烁
void moveCursor(int x, int y)					
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
	return;
}

 /*
 ·移动小蛇
 ·第一步扫描数组mp的所有元素，找到正数元素都加1
 ·找到最大元素（即蛇尾巴），把其变为0
 ·找到等于2的元素（即蛇头），根据输出的上下左右方向把对应的另一个像素值设为1（新蛇头）
 */
void moveSnake()
{
	int i, j;
	for (int i = 1; i < height - 1; i++)
		for (int j = 1; j < width - 1; j++)
			if (mp[i][j] > 0)
				mp[i][j]++;	//here

	int oldTail_i, oldTail_j, oldHead_i, oldHead_j;
	int max = 0;
	for (i = 1; i < height - 1; i++)
		for (j = 1; j < width - 1; j++)
			if (mp[i][j] > 0)
			{
				if (max < mp[i][j])
				{
					max = mp[i][j];
					oldTail_i = i;
					oldTail_j = j;
				}

				if (mp[i][j] == 2)
				{
					oldHead_i = i;
					oldHead_j = j;
				}
			}

	int newHead_i, newHead_j;
	if (direction == 1)			// 向上移动
	{
		newHead_i = oldHead_i - 1;
		newHead_j = oldHead_j;
	}
	if (direction == 2)			// 向下移动
	{
		newHead_i = oldHead_i + 1;
		newHead_j = oldHead_j;
	}
	if (direction == 3)			// 向左移动
	{
		newHead_i = oldHead_i;
		newHead_j = oldHead_j - 1;
	}
	if (direction == 4)			// 向右移动
	{
		newHead_i = oldHead_i;
		newHead_j = oldHead_j + 1;
	}

	// 如果新蛇头吃到食物
	if (mp[newHead_i][newHead_j] == -2)
	{
		mp[food_x][food_y] = 0;
		// 产生一个新的食物
		food_x = rand() % (height - 5) + 2;
		food_y = rand() % (width - 5) + 2;
		mp[food_x][food_y] = -2;

		// 原来的旧蛇尾留着，长度自动加1
	}
	else								// 否则，原来的旧蛇尾减掉，保持长度不变
		mp[oldTail_i][oldTail_j] = 0;

	// 小蛇是否和自身碰撞或者和边框撞，游戏失败
	if (mp[newHead_i][newHead_j] > 0 || mp[newHead_i][newHead_j] == -1)
	{
		printf("游戏失败！\n");
		exit(EXIT_FAILURE);
	}
	else
		mp[newHead_i][newHead_j] = 1;
}

// 设置初始值
void startUp()
{
	// 边框
	for (int i = 0; i < height; i++)
	{
		mp[i][0] = -1;
		mp[i][width - 1] = -1;
	}
	for (int j = 0; j < width; j++)
	{
		mp[0][j] = -1;
		mp[height - 1][j] = -1;
	}

	// 蛇头
	mp[height / 2][width / 2] = 1;
	// 蛇身
	for (int i = 1; i <= 4; i++)
	{
		mp[height / 2][width / 2 - i] = i + 1;
	}

	// 移动方向
	direction = 4;

	// 移动速度
	t_cool = 100;

	// 食物
	food_x = rand() % (height - 5) + 2;
	food_y = rand() % (width - 5) + 2;
	mp[food_x][food_y] = -2;
	return;
}

// 绘制屏幕
void show()
{
	moveCursor(0, 0);	// 光标每秒归零

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{				
			if (mp[i][j] == -2)
				putchar('$');				// 食物
			else if (mp[i][j] == -1)
				putchar('#');				// 边框
			else if (mp[i][j] == 0)
				putchar(' ');				// 空格
			else if (mp[i][j] == 1)
				putchar('@');				// 蛇头
			else if (mp[i][j] > 1)
				putchar('*');				// 蛇身
		}
		putchar('\n');
	}
	Sleep(t_cool);
}

// 无论是否有触发都需要做的事
void updateWithoutInput()
{
	moveSnake();
}

// 检测到触发需要做的事
void updateWithInput()
{
	if (_kbhit())
	{
		char c = _getch();
		switch (c) {
		case 'w':
			direction = 1;
			break;
		case 's':
			direction = 2;
			break;
		case 'a':
			direction = 3;
			break;
		case 'd':
			direction = 4;
			break;
		}
	}
	return;
}

int main(void)
{
	startUp();	// 初始化
	while (1)
	{
		show();					// 绘制屏幕
		updateWithInput();		// 激发事件
		updateWithoutInput();	// 必然事件
	}
	return 0;
}