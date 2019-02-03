#include "./Game.h"
#include "Callbacks.h"

void Game::initGame() {
	//al_init();
	//Graphics screen;
	//Player player;
	srand(time(NULL));
	net.establishConnection();
	srand(time(NULL));
}

void Game::run() {
	string name = screen.getName();
	net.setName(name.c_str(), name.size());
	int wildcard;
	bool isMyTurn;
	if (!net.amIServer()) {
		isMyTurn = net.initGame(&(callbackClient),0,0,NULL);
	}
	else {
		string mapName = screen.chooseMap();
		isMyTurn = net.initGame(NULL, mapName.size(), 0, mapName.c_str());
	}
	player.setPlayer(screen.getMap().getTeam(), screen.getMap());
	screen.setEnemyName(net.getOpponentName());
	bool end = false;
	bool change = true;
	player.getMap()->removeFogStart();
	while (!end) {
		if (change) {
			screen.updateGraphics(*player.getMap());
			change = false;
		}
		if (isMyTurn) {
			if (player.finish()) {
				end = true;
			}
			Unit * unit;
			action_s action = screen.getUserAction(player.getMoney());
			if (action.act == A_ATTACK && player.getMap()->getUnitPtr(action.positionFrom) != nullptr &&!player.getMap()->IsValidAttack(player.getMap()->getUnitPtr(action.positionFrom),action.positionTo)) {
				action.act = A_NO_ACTION;
			}
			else if (action.act == A_MOVE && player.getMap()->getUnitPtr(action.positionFrom) != nullptr && !player.getMap()->IsValidMove(player.getMap()->getUnitPtr(action.positionFrom), action.positionTo)) {
				action.act = A_NO_ACTION;
			}
			string code;
			switch (action.act) {
			case A_ATTACK:
				wildcard = rand() % 6 + 1;
				myDice = wildcard;
				player.getMap()->attack(player.getMap()->getUnitPtr(action.positionFrom), action.positionTo, myDice);
				myDice = wildcard;
				net.sendMessage(ATTACK, action.positionFrom.row, action.positionFrom.column,
					action.positionTo.row, action.positionTo.column, myDice, &callback);
				player.updateInventory();
				change = true;
				break;
			case A_PURCHASE:
				unit = player.buyUnit(screen.chooseUnitToBuy(player.getUnitsAvailableToBuy()), action.positionFrom, player.getMap()->getTeam());
				code = getUnitCode(unit->getUnitClass());

				net.sendMessage(PURCHASE, code.c_str()[0], code.c_str()[1], unit->getPosition().row, unit->getPosition().column);
				player.updateInventory();
				change = true;
				break;
			case A_MOVE:
				player.getMap()->move(action.positionTo, player.getMap()->getUnitPtr(action.positionFrom));
				net.sendMessage(MOVE, action.positionFrom.row, action.positionFrom.column, action.positionTo.row, action.positionTo.column);
				change = true;
				break;
			case A_PASS:
				isMyTurn = !isMyTurn;
				player.endTurn();
				net.sendMessage(PASS);
				player.endTurn();
				screen.showTransition();
				change = true;
				break;
			case A_NO_ACTION:
				break;
			case A_CLOSE_GAME:
				end = true;
				net.sendMessage(QUIT);
				break;
			default:
				break;
			}
		}
		else {
			int temp = net.waitForMyTurn(&callback, &callbackResponseAttack);
			int quit = screen.checkIfUserClose();
			if (quit) {
				net.sendMessage(QUIT);
				end = true;
			}
			/*if (temp != -1) {
				change = true;
			}*/
			if (temp == PASS) {
				isMyTurn = true;
				player.startTurn();
				change = true;
				screen.showTransition();
			}
			else if (temp == QUIT) {
				end = true;
			}
		}
	}

}

string Game::getUnitCode(units_d unitClass)
{
	string ans;
	switch (unitClass)
	{
	case INFANTRY:
	{
		ans = "in";
	}break;
	case MECH:
	{
		ans = "me";
	}break;
	case ROCKET:
	{
		ans = "ro";
	}break;
	case RECON:
	{
		ans = "re";
	}break;
	case APC:
	{
		ans = "ap";
	}break;
	case ANTIAIR:
	{
		ans = "aa";
	}break;
	case ARTILLERY:
	{
		ans = "ar";
	}break;
	case TANK:
	{
		ans = "ta";
	}break;
	case MEDTANK:
	{
		ans = "mt";
	}break;
	default:
	{
		ans = "";
	}break;
	}
	return ans;
}

units_d Game::getUnitFromCode(string code)
{
	units_d ans;

	if (code == "in")
		ans = INFANTRY;
	else if (code == "me")
		ans = MECH;
	else if (code == "ro")
		ans = ROCKET;
	else if (code == "re")
		ans = RECON;
	else if (code == "ap")
		ans = APC;
	else if (code == "aa")
		ans = ANTIAIR;
	else if (code == "ta")
		ans = TANK;
	else if (code == "mt")
		ans = MEDTANK;
	else
		ans = ERRORCLASS;

	return ans;
}