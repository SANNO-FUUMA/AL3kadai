#pragma once
#include "Vector3.h"
#include <cstdint>
#include <string>
#include <vector>

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;

};

class MapChipField {
public:
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filepath);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndenx, uint32_t yIndex);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndenx, uint32_t yIndex);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }

	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	private:

		static inline const uint32_t kNumBlockVirtical = 20;

		static inline const uint32_t kNumBlockHorizontal = 100;

		MapChipData mapChipData_;
};