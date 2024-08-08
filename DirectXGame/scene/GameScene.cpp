#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "Math/MyMath/MatrixMath.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete modelBlock_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
}
<<<<<<< Updated upstream
	
=======

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む

	// 3Dモデルの生成
	modelBlock_ = Model::CreateFromOBJ("block");
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelEnemy_ = Model::CreateFromOBJ("enemy");
	modelSkydome_ = Model::CreateFromOBJ("sphere", true);
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);

	// 天球の生成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	player_->SetMapChipField(mapChipField_);

	GenerateBlocks();

	cameraController = new CameraController;
	cameraController->Initialize();
	cameraController->SetTarget(player_);
	cameraController->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController->SetMovableArea(cameraArea);

	// 敵の生成
	newEnemy_ = new Enemy();
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14, 18);
	newEnemy_->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

	enemies_.push_back(newEnemy_);
	
}
>>>>>>> Stashed changes

	void GameScene::Initialize() {

		dxCommon_ = DirectXCommon::GetInstance();
		input_ = Input::GetInstance();
		audio_ = Audio::GetInstance();

		//カメラの生成
		debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
		cameraMarix_ = MatrixMath::MakeAffineMatrix(
			{ 0.0f,0.0f,0.0f },
			debugCamera_->GetViewProjection().rotation_,
			debugCamera_->GetViewProjection().translation_
		);

		AxisIndicator::GetInstance()->SetVisible(true);
		AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

		//要素数
		const uint32_t kNumblockVirtical = 10;
		const uint32_t kNumBlockHorizonal = 20;
		//ブロック1個分の横幅
		const float kBlockWidth = 2.0f;
		const float kBlockHeight = 2.0f;
		//要素数を変更する
		worldTransformBlocks_.resize(kNumblockVirtical);
		for (uint32_t i = 0; i < kNumblockVirtical; i++) {
			worldTransformBlocks_[i].resize(kNumBlockHorizonal);
		}

		//	ブロックの生成
		for (uint32_t row = 0; row < kNumblockVirtical; row++) {
			for (uint32_t column = 0; column < kNumBlockHorizonal; column++) {
				
					worldTransformBlocks_[row][column] = new WorldTransform();
					worldTransformBlocks_[row][column]->Initialize();
					if ((row + column) % 2 == 0) {
						worldTransformBlocks_[row][column]->translation_.x = kBlockWidth * column;
						worldTransformBlocks_[row][column]->translation_.y = kBlockHeight * row;
					}
			}
		}

		viewProjection_.Initialize();
		modelBlock_ = Model::Create();
	}

	void GameScene::Update() {

		//ブロックの更新

<<<<<<< Updated upstream
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
					
				}
				worldTransformBlock->UpdateMatrix();
			}
=======
	//敵の更新
	newEnemy_->Update();

	// 縦横ブロック更新
	for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;

			// アフィン変換行列の作成
			//(MakeAffineMatrix：自分で作った数学系関数)
			worldTransformBlockYoko->matWorld_ = MakeAffineMatrix(
			    worldTransformBlockYoko->scale_, worldTransformBlockYoko->rotation_,
			    worldTransformBlockYoko->translation_);

			// 定数バッファに転送
			worldTransformBlockYoko->TransferMatrix();
>>>>>>> Stashed changes
		}
#ifdef _DEBUG
		
		if (input_->TriggerKey(DIK_SPACE)) {
			isDebugCameraActive_ = true;
		}		
#endif // _DEBUG

		//カメラの処理
		if (isDebugCameraActive_ == true) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		} else {
			//ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}
		

		
	}
<<<<<<< Updated upstream
=======
	CheckAllCollisions();
}

void GameScene::CheckAllCollisions() {

	// 判定対象１と２の座標
	AABB aabb1, aabb2;

#pragma region
	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾全ての当たり判定
	for (Enemy* enemy : enemies_) {

		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {

			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);

			enemy->OnCollision(player_);
		}
	}
#pragma endregion
}
>>>>>>> Stashed changes

	void GameScene::Draw() {

		// コマンドリストの取得
		ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
		// 背景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに背景スプライトの描画処理を追加できる
		/// </summary>

		// スプライト描画後処理
		Sprite::PostDraw();
		// 深度バッファクリア
		dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
		// 3Dオブジェクト描画前処理
		Model::PreDraw(commandList);

		/// <summary>
		/// ここに3Dオブジェクトの描画処理を追加できる
		/// </summary>
	
<<<<<<< Updated upstream
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				modelBlock_->Draw(*worldTransformBlock, viewProjection_);
			}
=======
	// 自キャラの描画
	player_->Draw();

	// 天球の描画
	skydome_->Draw();

	//敵の描画
	newEnemy_->Draw();

	// 縦横ブロック描画
	for (std::vector<WorldTransform*> worldTransformBlockTate : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlockYoko : worldTransformBlockTate) {
			if (!worldTransformBlockYoko)
				continue;

			modelBlock_->Draw(*worldTransformBlockYoko, viewProjection_);
>>>>>>> Stashed changes
		}

		// 3Dオブジェクト描画後処理
		Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>

		// スプライト描画後処理
		Sprite::PostDraw();

#pragma endregion
<<<<<<< Updated upstream
	}
=======
}
>>>>>>> Stashed changes
