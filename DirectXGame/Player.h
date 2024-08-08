#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Easing.h"

class Player {
public:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};


	void Initialize(Model*model, const Vector3& position, ViewProjection* viewProjection);

	void Update();

	void Draw();

private:
	
	WorldTransform worldTransform_;	
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.01f;
	static inline const float kLimitRunSpeed = 2.0f;

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;

	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;
	bool landing = false;

	static inline const float kGravityAcceleration = 0.05f;
	
	static inline const float kLimitFallSpeed = 0.2f;
	
	static inline const float kJumpAcceleration = 0.7f;
};