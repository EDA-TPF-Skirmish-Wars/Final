#include "./Game.h"

Game::Game() {
	Graphics screen;
	Player player;
	player.setPlayer(screen.getMap().getTeam(), &screen.getMap());
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
				string code = getUnitCode(unit);
				net.sendMessage(PURCHASE, code.c_str()[0], code.c_str()[1], action.positionFrom.row, action.positionFrom.column);
				player.updateInventory();
				screen.updateGraphics(*player.getMap());
				break;
			case A_MOVE:
				player.getMap()->move(action.positionTo, player.getMap()->getUnit(action.positionFrom));
				net.sendMessage(MOVE, action.positionFrom.row, action.positionFrom.column, action.positionTo.row, action.positionTo.column);
				screen.updateGraphics(*player.getMap());

			}
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
	if (move != ATTACK)
		return false;
	else {
		Position pos(data1, data2);
		Position pos2(data3, data4);
		answer = player.getMap()->enemyAttack(player.getMap()->getUnit(pos), pos2, data5);
	}
	screen.showDices(myDice, data5);
	return answer;
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