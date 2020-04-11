#pragma once
#include"EnumIDs.h"

//�V�[���e���v���[�g
class Scene {
public:
	virtual ~Scene() {}

	virtual void start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw()const = 0;
	virtual void lateDraw() = 0;
	//isEnd��true�ɂȂ�����̏���
	virtual void end() = 0;

	virtual bool isEnd() const { return isEnd_; }

	virtual SceneType nextScene()const { return next_; }

	//���ʂ̏�����
	void baseInit() {
		isEnd_ = false;
	}

protected:
	bool isEnd_{ false };
	SceneType next_{ SceneType::SCENE_GAMEPLAY };
};