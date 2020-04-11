#include "SceneManager.h"
#include"Scenes.h"

//ローディングシーンから開始
SceneManager::SceneManager() :currentScene_(SceneType::SCENE_GAMEPLAY) {
	//利用するシーンを作成し、シーンリストに追加する
	scenes_[SceneType::SCENE_GAMEPLAY] = std::make_shared<GamePlayScene>();
}

void SceneManager::start() {
	scenes_[currentScene_]->start();
}

void SceneManager::Update(float deltaTime) {
	scenes_[currentScene_]->Update(deltaTime);

}

void SceneManager::Draw() {
	scenes_[currentScene_]->Draw();
	scenes_[currentScene_]->lateDraw();
}

void SceneManager::End()
{
	scenes_[currentScene_]->end();
}

//シーンが終了しているかを確認し、必要であれば次のシーンに遷移する

void SceneManager::checkIsEnd() {
	if (scenes_.at(currentScene_)->isEnd()) next();

}

//次のシーンへ遷移する

void SceneManager::next() {
	//シーンを終了して
	scenes_[currentScene_]->end();
	SceneType nextScene = scenes_[currentScene_]->nextScene();
	currentScene_ = nextScene;
	//次のシーンを開始する
	scenes_[currentScene_]->baseInit();
	scenes_[currentScene_]->start();
}
