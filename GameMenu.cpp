#include "GameMenu.h"
#include "time/PrecisionTimer.h"
#include "ParamLoader.h"

#include <list>
using std::list;


//------------------------------- ctor -----------------------------------
//------------------------------------------------------------------------
GameMenu::GameMenu(HWND window, HINSTANCE instance) :

	mainWindow(window),
	mainInstance(instance)

{
	NumAgents = Prm.NumAgents;
	NumObstacles = Prm.NumObstacles;
	NumLeaders = Prm.NumLeaders;
	OffsetX = Prm.AgentOffsetX;
	OffsetY = Prm.AgentOffsetY;
	int ret;
	char agents[5];
	_itoa(NumAgents, agents, 10);
	char leaders[5];
	_itoa(NumLeaders, leaders, 10);
	char obstacles[5];
	_itoa(NumObstacles, obstacles, 10);
	char xoffset[5];
	snprintf(xoffset, sizeof xoffset, "%f", OffsetX);
	char yoffset[5];
	snprintf(yoffset, sizeof yoffset, "%f", OffsetY);

	group[0] = CreateWindow(
		"BUTTON",
		"Settings",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 10,
		220, 170,
		mainWindow,
		NULL,
		mainInstance,
		NULL);

	group[1] = CreateWindow(
		"STATIC",
		"Nb de leaders : (1-5)",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		20, 30,
		175, 20,
		mainWindow,
		NULL,
		instance,
		NULL);

	group[2] = CreateWindow(
		"EDIT",
		leaders,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER,
		180, 30,
		40, 20,
		mainWindow,
		(HMENU)ID_LEADER_COUNT,
		instance,
		NULL);

	group[3] = CreateWindow(
		"STATIC",
		"Nb de followers : (10-50)",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		20, 60,
		175, 20,
		mainWindow,
		NULL,
		instance,
		NULL);

	group[4] = CreateWindow(
		"EDIT",
		agents,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER,
		180, 60,
		40, 20,
		mainWindow,
		(HMENU)ID_AGENT_COUNT,
		instance,
		NULL);

	group[5] = CreateWindow(
		"STATIC",
		"Nb d'obstacles : (5-30)",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		20, 90,
		175, 20,
		mainWindow,
		NULL,
		instance,
		NULL);

	group[6] = CreateWindow(
		"EDIT",
		obstacles,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER,
		180, 90,
		40, 20,
		mainWindow,
		(HMENU)ID_OBSTACLE_COUNT,
		instance,
		NULL);

	group[7] = CreateWindow(
		"STATIC",
		"Offset X : (-40-40)",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		20, 120,
		175, 20,
		mainWindow,
		NULL,
		instance,
		NULL);

	group[8] = CreateWindow(
		"EDIT",
		xoffset,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
		180, 120,
		40, 20,
		mainWindow,
		(HMENU)ID_OFFSET_X,
		instance,
		NULL);

	group[9] = CreateWindow(
		"STATIC",
		"Offset Y : (-40-40)",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		20, 150,
		175, 20,
		mainWindow,
		NULL,
		instance,
		NULL);

	group[10] = CreateWindow(
		"EDIT",
		yoffset,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
		180, 150,
		40, 20,
		mainWindow,
		(HMENU)ID_OFFSET_Y,
		instance,
		NULL);

	group[11] = CreateWindow(
		"BUTTON",
		"SAVE",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 180,
		60, 20,
		mainWindow,
		(HMENU)ID_SAVE_SETTINGS,
		instance,
		NULL);

	group[12] = CreateWindow(
		"BUTTON",
		"CANCEL",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 180,
		60, 20,
		mainWindow,
		(HMENU)ID_CANCEL_SETTINGS,
		instance,
		NULL);
}


//-------------------------------- dtor ----------------------------------
//------------------------------------------------------------------------
GameMenu::~GameMenu()
{
	for (int i = 0; i < sizeof(group) / sizeof(*group); i++)
	{
		DestroyWindow(group[i]);
	}
}


//-------------------------- HandleMenuItems -----------------------------
void GameMenu::HandleInput(WPARAM wParam, HWND hwnd)
{
	switch (wParam)
	{
	case ID_SAVE_SETTINGS:
		TCHAR buff[1024];
		//Check Leader Count
		GetWindowText(group[2], buff, 1024);
		NumLeaders = atoi(buff);
		if (NumLeaders >= 1 && NumLeaders <= 5) {
			Prm.NumLeaders = NumLeaders;
		}
		else {
			MessageBox(hwnd, "Entrez un nombre de leader valide", "", MB_OK);
			break;
		}

		//Check Follower Count
		GetWindowText(group[4], buff, 1024);
		NumAgents = atoi(buff);
		if (NumAgents >= 10 && NumAgents <= 50) {
			Prm.NumAgents = NumAgents;
		}
		else {
			MessageBox(hwnd, "Entrez un nombre de follower valide", "", MB_OK);
			break;
		}

		//Check Obstacle Count
		GetWindowText(group[6], buff, 1024);
		NumObstacles = atoi(buff);
		if (NumObstacles >= 5 && NumObstacles <= 30) {
			Prm.NumObstacles = NumObstacles;
		}
		else {
			MessageBox(hwnd, "Entrez un nombre d'obstacles valide", "", MB_OK);
			break;
		}

		//Check Offset X
		GetWindowText(group[8], buff, 1024);
		OffsetX = atof(buff);
		if (OffsetX >= -40 && OffsetX <= 40 && OffsetX != 0) {
			Prm.AgentOffsetX = OffsetX;
		}
		else {
			MessageBox(hwnd, "Entrez un offset valide", "", MB_OK);
			break;
		}

		//Check Offset Y
		GetWindowText(group[10], buff, 1024);
		OffsetY = atof(buff);
		if (OffsetY >= -40 && OffsetY <= 40 && OffsetY != 0) {
			Prm.AgentOffsetY = OffsetY;
		}
		else {
			MessageBox(hwnd, "Entrez un offset valide", "", MB_OK);
			break;
		}

		SendMessage(hwnd, WM_COMMAND, WM_SAVE_SETTINGS, NULL);
		break;

	case ID_CANCEL_SETTINGS:
		SendMessage(hwnd, WM_COMMAND, WM_CANCEL_SETTINGS, NULL);
		break;
	}
}


