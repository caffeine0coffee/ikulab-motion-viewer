#pragma once

#include <memory>
#include <array>

#include "../shape.hpp"
#include "../cube/singleColorCube.hpp"

class DirectionDebugObject : public Shape {
	float size;
	/// x, y, z
	std::array<std::unique_ptr<SingleColorCube>, 3> axes;
public:
	DirectionDebugObject(float size, JointID id);
};