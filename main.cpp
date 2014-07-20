
#include "ace.h"
#include "cassert"
#include <memory>
#include <array>
#include <iostream>
#include <random>
#ifdef _DEBUG

#pragma comment(lib, "Debug/ace_engine.lib")

#else
#pragma comment(lib, "Release/ace_engine.lib")
#endif

// * DLLがなかった場合に感知できないのは厳しい
using namespace ace;

template<typename T> using sp = std::shared_ptr<T>;

static const int fieldWidth = 20;
static const int fieldHeight = 20;
static const int mineNum = 40;

namespace ImgManager {
	void setTexture2D(sp<Texture2D>& tex, char const* file) {
		tex = Engine::GetGraphics()->CreateTexture2D(ToAString(file).c_str());
	}
	sp<Texture2D> closedCell, minedCell, obstacleCell, player, digTarget;
	std::array<sp<Texture2D>, 9> freeCells;
	void init() {
		setTexture2D(closedCell, "img/closedCell.png");
		setTexture2D(minedCell, "img/minedCell.png");
		setTexture2D(obstacleCell, "img/obstacleCell.png");
		setTexture2D(freeCells[0], "img/freeCell.png");
		setTexture2D(freeCells[1], "img/numCell1.png");
		setTexture2D(freeCells[2], "img/numCell2.png");
		setTexture2D(freeCells[3], "img/numCell3.png");
		setTexture2D(freeCells[4], "img/numCell4.png");
		setTexture2D(freeCells[5], "img/numCell5.png");
		setTexture2D(freeCells[6], "img/numCell6.png");
		setTexture2D(freeCells[7], "img/numCell7.png");
		setTexture2D(freeCells[8], "img/numCell8.png");
		setTexture2D(player, "img/player.png");
		setTexture2D(digTarget, "img/digTarget.png");
	}
}



class Cell: public TextureObject2D {
private:
	int neighborMineNum = 0;
public:
	enum class Status {
		free,
		mined, // mined、obstacle、explodingは全マップで同期させる
		obstacle,
		exploding // mined→exploding→ダメージ処理→free
	};
private:
	Status status = Status::free;

	void changeTexture() {
		switch(status) {
		case Status::mined:
			SetTexture(ImgManager::closedCell); // TODO: for debug
			break;
		case Status::obstacle:
			SetTexture(ImgManager::obstacleCell);
			break;
		case Status::exploding:
			SetTexture(ImgManager::minedCell);
			break;
		case Status::free:
			if(!isOpenedByFriend) {
				SetTexture(ImgManager::closedCell);
				return;
			}
			if(neighborMineNum == 0) {
				SetTexture(ImgManager::freeCells[0]);
			} else {
				SetTexture(ImgManager::freeCells.at(neighborMineNum));
			}
			break;
		default:
			break;
		}
	}
public:
	bool isOpenedByFriend = false;
	bool isOpenedByEnemy = false;

	// 外部利用
	bool isToOpenByFriend = false;
	bool isToOpenByEnemy = false;
	
	/// lay a mine on this cell.
	/// @return true iff succeeded
	bool layMine() {
		if(status != Status::free) { return false; }
		isOpenedByEnemy = isOpenedByFriend = false;
		status = Status::mined;
		changeTexture();
		
		return true;
		
	}

	int getNeighborMineNum() const {return neighborMineNum;}
	void incNeighborMineNum() { neighborMineNum++; }
	void decNeighborMineNum() { neighborMineNum--; }

	Status getStatus() const {return status;}


	void explode() {
		if(status != Status::mined) {return;}
		status = Status::exploding;
		changeTexture();
	}
	void openByFriend() {
		if(status == Status::mined){
			explode();
			return;
		}
		if(status == Status::free) {
			isOpenedByFriend = true;
			changeTexture();
			return;
		}
	}

	void openByEnemy() {
		if(status == Status::mined) {
			explode();
			return;
		}
		if(status == Status::free) {
			isOpenedByEnemy = true;
			return;
		}
	}
	void endExplosion() {
		if(status != Status::exploding) { return; }
		status = Status::free;
		isOpenedByEnemy = isOpenedByFriend = true;
		changeTexture();
	}
	void OnStart() override {
		SetScale(Vector2DF(0.25f, 0.25f));
		changeTexture();
		
	}
	
};

class Field {

	using CellArray = std::array<std::array<sp<Cell>, fieldWidth>, fieldHeight>;
	CellArray cellArray;
	sp<Layer2D> parentLayer;

public:

	Field(sp<Layer2D> parent) : parentLayer(parent){
		

		for(int iy = 0; iy < fieldHeight; iy++) for(int ix = 0; ix < fieldWidth; ix++) {
			auto& e = cellArray.at(iy).at(ix);
			e = std::make_shared<Cell>();
			e->SetPosition(Vector2DF(ix * 246.0f / 4.0f, iy * 246.0f / 4.0f));
			parent->AddObject(e);
		}

		// 地雷を埋める
		
		std::random_device rnd;
		std::vector<unsigned int> v = {rnd(), rnd(), rnd()};
		std::mt19937 eng(std::seed_seq(v.begin(), v.end()));
		std::uniform_int_distribution<int> distX(0, fieldWidth - 1), distY(0, fieldHeight - 1);
		for(int i = 0; i < mineNum;) {
			if(layMine(distX(eng), distY(eng))) { i++; }
		}
		
		// openCell(3, 3, true);

	}

	sp<Cell> getCell(int const x, int const y) const { return cellArray.at(y).at(x); }
	/// lay a mine
	/// @return true iff succeeded to mine.
	bool layMine(int const x, int const y) {
		
		if(x < 0 || x > fieldWidth || y < 0 || y > fieldHeight || !cellArray.at(y).at(x)->layMine()) { return false; }
		
		for(int iy = -1; iy < 2; iy++) for(int ix = -1; ix < 2; ix++) {
			if((x != ix || y != iy) && x + ix < fieldWidth && x + ix >= 0 && y + iy < fieldHeight && y + iy >= 0 ) {
				cellArray.at(y + iy).at(x + ix)->incNeighborMineNum();
			}
		}
		return true;

	}

	// 地雷を起爆させる
	void explodeMine(int const x, int const y) {
		if(x < 0 || x > fieldWidth || y < 0 || y > fieldHeight) { return; }
		if(cellArray.at(y).at(x)->getStatus() == Cell::Status::mined) {
			cellArray.at(y).at(x)->explode();
			for(int iy = -1; iy < 2; iy++) for(int ix = -1; ix < 2; ix++) {
				if((x != ix || y != iy) && x + ix < fieldWidth && x + ix >= 0 && y + iy < fieldHeight && y + iy >= 0) {
					cellArray.at(y + iy).at(x + ix)->decNeighborMineNum();
				}
			}
		}
	}

	/// open the cell and set the neighbors' status to be nextOpen 
	/// @return true iff the cell is mined
	bool openCell(int const x, int const y, bool const isFriend) {
		if(x < 0 || x >= fieldWidth || y < 0 || y >= fieldHeight) { return false; }
		if(cellArray.at(y).at(x)->getStatus() == Cell::Status::mined) {
			explodeMine(x, y);
			return true;
		}

		if(cellArray.at(y).at(x)->getStatus() != Cell::Status::free) { return false; }
		if(isFriend) {
			cellArray.at(y).at(x)->openByFriend();
		} else {
			cellArray.at(y).at(x)->openByEnemy();
		}

		// 空セルならば周囲のセルを連鎖的に開ける
		if(cellArray.at(y).at(x)->getNeighborMineNum() > 0) {return false;}
		for(int iy = -1; iy < 2; iy++) for(int ix = -1; ix < 2; ix++) {
			if((x != ix || y != iy) && x + ix < fieldWidth && x + ix >= 0 && y + iy < fieldHeight && y + iy >= 0) {
				if(isFriend) {
					cellArray.at(y + iy).at(x + ix)->isToOpenByFriend = true;
				} else {
					cellArray.at(y + iy).at(x + ix)->isToOpenByEnemy = true;
				}
			}
		}

		return false;
	}



};

class EngineProvider {
public:
	EngineProvider() {
		auto opt = ace::EngineOption();
		// opt.GraphicsType = ace::eGraphicsType::GRAPHICS_TYPE_GL;
		opt.GraphicsDevice = ace::GraphicsDeviceType::DirectX11;
		opt.IsFullScreen = false;
		opt.IsMultithreadingMode = true;
		// * initの文字列にconst char*とastringがあった方がいい
		if(!ace::Engine::Initialize(ace::ToAString("MinePanzer").c_str(), 800, 600, opt)) assert(!"Err in init");
		Engine::SetTargetFPS(60);
	}

	virtual ~EngineProvider() {
		ace::Engine::Terminate();
	}

};

class Player: public TextureObject2D {
private:
	Keyboard *input;
	float prevExpectedDirection = 0.0f, expectedDirection = 0.0f;
	bool isRotating = false;
	double angle = 0.0;
	float speed = 0.0f;
protected:
	void move() {
		speed = 1.0f;
		prevExpectedDirection = expectedDirection;
		if(!((int)input->GetKeyState(Keys::Left) & 1)) {
			if(!((int)input->GetKeyState(Keys::Up) & 1)) {
				expectedDirection = 5.0f;
			} else if(!((int)input->GetKeyState(Keys::Down) & 1)) {
				expectedDirection = 3.0f;
			} else {
				expectedDirection = 4.0f;
			}

		} else if(!((int)input->GetKeyState(Keys::Right) & 1)) {
			if(!((int)input->GetKeyState(Keys::Up) & 1)) {
				expectedDirection = 7.0f;
			} else if(!((int)input->GetKeyState(Keys::Down) & 1)) {
				expectedDirection = 1.0f;
			} else {
				expectedDirection = 0.0f;
			}

		} else {
			if(!((int)input->GetKeyState(Keys::Up) & 1)) {
				expectedDirection = 6.0f;
			} else if(!((int)input->GetKeyState(Keys::Down) & 1)) {
				expectedDirection = 2.0f;
			} else {
				speed = 0.0f;
			}
		}

		if(prevExpectedDirection != expectedDirection) {isRotating = true;}

		const double expectedAngle = expectedDirection * 45.0;
		if(isRotating && abs(expectedAngle - angle) > 4.0f) {
			if(sin(((expectedAngle - angle) * 2.0 * PI) / 360.0) > 0) {
				angle += 4.0;
			} else {
				angle -= 4.0;
			}
		} else {
			angle = expectedAngle;
			isRotating = false;
		}
		SetAngle((float)angle);
		const auto pos = GetPosition();
		SetPosition(Vector2DF((float)(pos.X + cos(angle * 2.0 * PI / 360.0) * speed), (float)(pos.Y + sin(angle * 2.0 * PI / 360.0) * speed)));
	}
public:
	virtual void OnStart() override {
		input = Engine::GetKeyboard();
		SetTexture(ImgManager::player);
		SetCenterPosition(Vector2DF(256.0f, 256.0f));
		SetPosition(Vector2DF(0.0f, 0.0f));
	}
	
	virtual void OnUpdate() override{
		SetScale(Vector2DF(1.0f, 1.0f));
		move();
		SetScale(Vector2DF(0.25f, 0.25f));

	}
};


class GameScene: public Scene {
	sp<Layer2D> fieldLayer = sp<Layer2D>(new Layer2D()), objectLayer = sp<Layer2D>(new Layer2D()), effectLayer = sp<Layer2D>(new Layer2D());
	sp<CameraObject2D> cameraf = sp<CameraObject2D>(new CameraObject2D()), camerao = sp<CameraObject2D>(new CameraObject2D());;
	sp<Field> field;
	sp<Player> player = sp<Player>(new Player());
	Keyboard *input;

public:
	GameScene(): Scene() {
		ImgManager::init();
		input = Engine::GetKeyboard();
		AddLayer(fieldLayer);
		AddLayer(objectLayer);
		AddLayer(effectLayer);
		fieldLayer->SetDrawingPriority(0);
		objectLayer->SetDrawingPriority(1);
		effectLayer->SetDrawingPriority(2);
		
		auto lightBloom = std::make_shared<PostEffectLightBloom>();
		lightBloom->SetThreshold(0.5f);
		lightBloom->SetIntensity(16.0f);
		lightBloom->SetPower(0.7f);
		fieldLayer->AddPostEffect(lightBloom);
		objectLayer->AddPostEffect(lightBloom);
		effectLayer->AddPostEffect(lightBloom);
		cameraf->SetSrc(RectI(0, 0, 800, 600));

		cameraf->SetDst(RectI(0, 0, 800, 600));
		camerao->SetSrc(RectI(0, 0, 800, 600));
		camerao->SetDst(RectI(0, 0, 800, 600));

		objectLayer->AddObject(camerao);
		fieldLayer->AddObject(cameraf);
		field = std::make_shared<Field>(fieldLayer);

		objectLayer->AddObject(player);

	}

	void OnUpdating() override {
		for(int iy = 0; iy < fieldHeight; iy++) for(int ix = 0; ix < fieldWidth; ix++) {
			auto& e = field->getCell(ix, iy);
			if(e->isToOpenByFriend) { e->isToOpenByFriend = false; field->openCell(ix, iy, true); }
			if(e->isToOpenByEnemy) { e->isToOpenByEnemy = false; field->openCell(ix, iy, false); }

		}
		auto pPos = player->GetPosition();

		cameraf->SetSrc(RectI((int)(pPos.X + 0.5f) - 400, (int)(pPos.Y + 0.5f) - 300, 800, 600));
		camerao->SetSrc(RectI((int)(pPos.X + 0.5f) - 400, (int)(pPos.Y + 0.5f) - 300, 800, 600));


	}


};


int main() {
	EngineProvider engineProvider;
	sp<Scene> gameScene = sp<Scene>(new GameScene());
	Engine::ChangeScene(gameScene);
	while(Engine::DoEvents()) {
		//std::cout << Engine::GetCurrentFPS() << "\n";
		Engine::Update();
	}

}
