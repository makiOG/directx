#pragma once

//全体で使えるenum classを定義

//モデルID
enum class MODEL_ID {
	MODEL_PLAYER,
	MODEL_PLAYER_SWORD,
	MODEL_SKYBOX,
	MODEL_STAGE,
};
enum class SPRITE_ID {
};
enum class BGM_ID {
	SOUND_TITLE,
	SOUND_GAME,
	SOUND_CLEAR
};
enum class SE_ID {
};
enum class FONT_ID {
	FONT_COUNT,//カウント用フォント
};
//ゲーム中で利用するシーンを列挙する
enum class SceneType {
	SCENE_TITLE,
	SCENE_LOADING,
	SCENE_GAMEPLAY,
	SCENE_GAMEOVER,
	SCENE_CLEAR,
};

//メッセージによるActor間の操作
enum class EventMessage {
	HitPlayer,
	HitEnemy,
};

//Actorのグループ分け
enum class ActorGroup {
	ETCETERA_ACTOR,//ゲームに関与しないアクター
	PLAYER_ACTOR,//プレイヤー
	ENEMY_ACTOR,//敵

};
enum class PlayerStateID
{
	Null = -1,
	Idle,
	Walk,
	Slash1,
	Slash2,
	Slash3,
	Jump,
	ShortJump,
};