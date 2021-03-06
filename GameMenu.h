#ifndef GameMenu_H
#define GameMenu_H
#pragma warning (disable:4786)

#include <windows.h>
#include <vector>

#define WM_SAVE_SETTINGS   777
#define ID_SAVE_SETTINGS   770
#define WM_CANCEL_SETTINGS   666
#define ID_CANCEL_SETTINGS 660
#define ID_LEADER_COUNT    110
#define ID_AGENT_COUNT     120
#define ID_OBSTACLE_COUNT  130
#define ID_OFFSET_X  140
#define ID_OFFSET_Y  150

class GameMenu
{
private:

	int NumLeaders;
	int	NumAgents;
	int	NumObstacles;
	float OffsetX;
	float OffsetY;
	HWND mainWindow;
	HINSTANCE mainInstance;
	HWND group [13] ;

public:

	GameMenu(HWND window, HINSTANCE instance);

	~GameMenu();

	void  HandleInput(WPARAM wParam, HWND hwnd);

};



#endif