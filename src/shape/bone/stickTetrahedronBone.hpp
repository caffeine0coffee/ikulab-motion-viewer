#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "./bone.hpp"
#include "../cube/cube.hpp"

class StickTetrahedronBone : public Bone {
	std::unique_ptr<Cube> cube;
public:
	StickTetrahedronBone(float length, JointID id);

	const std::vector<Vertex>& getVertices() const override;
	const std::vector<uint32_t>& getIndices() const override;

	void setBaseIndex(uint32_t baseIndex) override;
};