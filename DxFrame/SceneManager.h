#pragma once
#include<map>
#include<memory>
#include"EnumIDs.h"
class Scene;

//シーンの管理を行う、Game及びその派生クラスで利用する
class SceneManager {
public:
	SceneManager();
	void start();
	void Update(float deltaTime);
	void Draw();

	void End();
	//シーンが終了しているかを確認し、必要であれば次のシーンに遷移する
	void checkIsEnd();
	//次のシーンへ遷移する
	void next();


private:
	//シーン一覧
	std::map<SceneType, std::shared_ptr<Scene>> scenes_;
	//現在のシーン
	SceneType currentScene_;
};