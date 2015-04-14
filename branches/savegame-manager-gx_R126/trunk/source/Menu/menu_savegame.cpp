#include "SaveGameMenu.hpp"
#include "menus.h"

/****************************************************************************
 * MenuSaveGame
 ***************************************************************************/
int MenuSaveGame()
{
	int retMenu = MENU_NONE;

	SaveGameMenu * Menu = new SaveGameMenu();
	mainWindow->Append(Menu);

	retMenu = Menu->Show();

	delete Menu;
	Menu = NULL;

	return retMenu;
}
