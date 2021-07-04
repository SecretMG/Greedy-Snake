#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>

/* todo: 
		score(with border)
		buff(lives'L', speeddown'D')
		debuff(confuse'C', speedup'U')
		����
*/
const int height = 20;
const int width = 30;
int mp[height][width] = { 0 };				// 0���յأ�-1���߿� 1����ͷ������������
int direction;								// �ߵ��ƶ�����1, 2, 3, 4���ϣ��£�����
int food_x, food_y;							// ʳ���λ��
int t_cool;									// Խ���ƶ��ٶ�Խ��
											
// ������ƶ���(x,y)λ�ã����system("cls")������������ֹ������˸
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
 ���ƶ�С��
 ����һ��ɨ������mp������Ԫ�أ��ҵ�����Ԫ�ض���1
 ���ҵ����Ԫ�أ�����β�ͣ��������Ϊ0
 ���ҵ�����2��Ԫ�أ�����ͷ��������������������ҷ���Ѷ�Ӧ����һ������ֵ��Ϊ1������ͷ��
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
	if (direction == 1)			// �����ƶ�
	{
		newHead_i = oldHead_i - 1;
		newHead_j = oldHead_j;
	}
	if (direction == 2)			// �����ƶ�
	{
		newHead_i = oldHead_i + 1;
		newHead_j = oldHead_j;
	}
	if (direction == 3)			// �����ƶ�
	{
		newHead_i = oldHead_i;
		newHead_j = oldHead_j - 1;
	}
	if (direction == 4)			// �����ƶ�
	{
		newHead_i = oldHead_i;
		newHead_j = oldHead_j + 1;
	}

	// �������ͷ�Ե�ʳ��
	if (mp[newHead_i][newHead_j] == -2)
	{
		mp[food_x][food_y] = 0;
		// ����һ���µ�ʳ��
		food_x = rand() % (height - 5) + 2;
		food_y = rand() % (width - 5) + 2;
		mp[food_x][food_y] = -2;

		// ԭ���ľ���β���ţ������Զ���1
	}
	else								// ����ԭ���ľ���β���������ֳ��Ȳ���
		mp[oldTail_i][oldTail_j] = 0;

	// С���Ƿ��������ײ���ߺͱ߿�ײ����Ϸʧ��
	if (mp[newHead_i][newHead_j] > 0 || mp[newHead_i][newHead_j] == -1)
	{
		printf("��Ϸʧ�ܣ�\n");
		exit(EXIT_FAILURE);
	}
	else
		mp[newHead_i][newHead_j] = 1;
}

// ���ó�ʼֵ
void startUp()
{
	// �߿�
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

	// ��ͷ
	mp[height / 2][width / 2] = 1;
	// ����
	for (int i = 1; i <= 4; i++)
	{
		mp[height / 2][width / 2 - i] = i + 1;
	}

	// �ƶ�����
	direction = 4;

	// �ƶ��ٶ�
	t_cool = 100;

	// ʳ��
	food_x = rand() % (height - 5) + 2;
	food_y = rand() % (width - 5) + 2;
	mp[food_x][food_y] = -2;
	return;
}

// ������Ļ
void show()
{
	moveCursor(0, 0);	// ���ÿ�����

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{				
			if (mp[i][j] == -2)
				putchar('$');				// ʳ��
			else if (mp[i][j] == -1)
				putchar('#');				// �߿�
			else if (mp[i][j] == 0)
				putchar(' ');				// �ո�
			else if (mp[i][j] == 1)
				putchar('@');				// ��ͷ
			else if (mp[i][j] > 1)
				putchar('*');				// ����
		}
		putchar('\n');
	}
	Sleep(t_cool);
}

// �����Ƿ��д�������Ҫ������
void updateWithoutInput()
{
	moveSnake();
}

// ��⵽������Ҫ������
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
	startUp();	// ��ʼ��
	while (1)
	{
		show();					// ������Ļ
		updateWithInput();		// �����¼�
		updateWithoutInput();	// ��Ȼ�¼�
	}
	return 0;
}