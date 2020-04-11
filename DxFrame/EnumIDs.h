#pragma once

//�S�̂Ŏg����enum class���`

//���f��ID
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
	FONT_COUNT,//�J�E���g�p�t�H���g
};
//�Q�[�����ŗ��p����V�[����񋓂���
enum class SceneType {
	SCENE_TITLE,
	SCENE_LOADING,
	SCENE_GAMEPLAY,
	SCENE_GAMEOVER,
	SCENE_CLEAR,
};

//���b�Z�[�W�ɂ��Actor�Ԃ̑���
enum class EventMessage {
	HitPlayer,
	HitEnemy,
};

//Actor�̃O���[�v����
enum class ActorGroup {
	ETCETERA_ACTOR,//�Q�[���Ɋ֗^���Ȃ��A�N�^�[
	PLAYER_ACTOR,//�v���C���[
	ENEMY_ACTOR,//�G

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