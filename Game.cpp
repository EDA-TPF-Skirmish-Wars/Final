#include "./Game.h"

Game::Game() {
	Graphics screen;
	Player player;
	player.setPlayer(screen.getMap().getTeam(), &screen.getMap());
	srand(time(NULL));
}

void Game::run() {
	if (!al_init())
	{
		cout << "Allegro no pudo inicializarse" << endl;
	}
	srand(time(NULL));
	Connections net;
	net.establishConnection();
	//CARGAR NOMBRE POR GRAPHIC
	//Graphics screen;
	//string name = screen.getName();
	string name = "martina";
	net.setName(name.c_str(), name.size());
	bool isMyTurn;
	if (!net.amIServer()) {
		isMyTurn = net.initGame(&this->callbackClient);
	}
	else {
		string mapName = screen.chooseMap();
		isMyTurn = net.initGame(NULL, mapName.size(), 0, mapName.c_str());
	}
	bool end = false;
	while (!end) {
		if (isMyTurn) {
			player.collectIncome();
			action_s action = screen.getUserAction();
			string code;
			switch (action.act) {
			case A_ATTACK:
				myDice = rand() % 6 + 1;
				player.getMap()->attack(player.getMap()->getUnit(action.positionFrom), action.positionTo, myDice);
				net.sendMessage(ATTACK, action.positionFrom.row, action.positionFrom.column,
					action.positionTo.row, action.positionTo.column, myDice, &this->callback);
				player.updateInventory();
				screen.updateGraphics(*player.getMap());
				break;
			case A_PURCHASE:
				Unit * unit = player.buyUnit(screen.chooseUnitToBuy(), action.positionTo);
				code = getUnitCode(unit->getUnitClass());
				net.sendMessage(PURCHASE, code.c_str()[0], code.c_str()[1], action.positionFrom.row, action.positionFrom.column);
				player.updateInventory();
				screen.updateGraphics(*player.getMap());
				break;
			case A_MOVE:
				player.getMap()->move(action.positionTo, player.getMap()->getUnit(action.positionFrom));
				net.sendMessage(MOVE, action.positionFrom.row, action.positionFrom.column, action.positionTo.row, action.positionTo.column);
				screen.updateGraphics(*player.getMap());
				break;
			case A_PASS:
				isMyTurn = !isMyTurn;
				net.sendMessage(PASS);
				screen.updateGraphics(*player.getMap());
				break;
			case A_NO_ACTION:
				break;
			case A_CLOSE_GAME:
				end = true;
			default:
				end = true;
			}
		}
		else {
			net.waitForMyTurn(&this->callback, &this->callbackResponseAttack);
		}
	}

}

void * Game::callbackClient(const char* mapName, unsigned int mapNameSize, int checksum) {
	string name = mapName;
	screen.selectMap(mapName, checksum);
	return NULL;
}

bool Game::callback(move_s move, int data1, int data2, int data3, int data4, int data5) {
	bool answer = false;
	if (move == ATTACK){
		Position pos(data1, data2);
		Position pos2(data3, data4);
		answer = player.getMap()->enemyAttack(player.getMap()->getUnit(pos), pos2, data5);
		screen.showDices(myDice, data5);
	}
	else if (move == PURCHASE) {
		string code;
		code.push_back(data1);
		code.push_back(data2);
		units_d unit = getUnitFromCode(code);
		Position pos(data3, data4);
		player.getMap()->addUnit(unit, pos, player.getMap()->getEnemyTeam());
		answer = true;
	}
	return answer;
}

int Game::callbackResponseAttack(void) {
	return rand() % 6 + 1;
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