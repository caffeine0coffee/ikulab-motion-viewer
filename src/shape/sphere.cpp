#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "./sphere.hpp"

std::vector<Vertex> Sphere::getVertices() const {
	std::vector<Vertex> result;

	// Top vertex (n = 0)
	result.push_back({ {pos.x, pos.y, pos.z + r}, {0.0, 1.0, 0.0}, {0.0, 0.0} });

	for (uint32_t n = 1; n < numSplitV; n++) {
		for (uint32_t m = 0; m < numSplitH; m++) {
			float degXY = ((M_PI * 2) / numSplitH) * m;
			float degZ = M_PI / 2 - (M_PI / numSplitV) * n;

			float x = r * std::cos(degXY) * std::cos(degZ);
			float y = r * std::sin(degXY) * std::cos(degZ);
			float z = r * std::sin(degZ);

			result.push_back({
				{pos.x + x, pos.y + y, pos.z + z},
				{n / (float)numSplitV, 0.2, 0.2},
				{0.0, 0.0}
				});
		}
	}

	// Bottom vertex (n = numSplitV)
	result.push_back({ {pos.x, pos.y, pos.z - r}, {1.0, 0.0, 0.0}, {0.0, 0.0} });

	return result;
}

std::vector<uint32_t> Sphere::getIndices() const {
	std::vector<uint32_t> result;

	// Top index (n = 1)
	for (uint32_t m = 0; m < numSplitH; m++) {
		result.insert(result.end(), {
			0,
			m + 1,
			(m + 1) % numSplitH + 1
			});
	}

	for (uint32_t n = 2; n < numSplitV; n++) {
		for (uint32_t m = 0; m < numSplitH; m++) {
			result.insert(result.end(), {
				numSplitH * (n - 1) + m + 1,
				numSplitH * (n - 1) + (m + 1) % numSplitH + 1,
				numSplitH * (n - 2) + m + 1
				});
			result.insert(result.end(), {
				numSplitH * (n - 1) + (m + 1) % numSplitH + 1,
				numSplitH * (n - 2) + (m + 1) % numSplitH + 1,
				numSplitH * (n - 2) + m + 1
				});
		}
	}

	// Bottom index (n = numSplitV)
	for (uint32_t m = 0; m < numSplitH; m++) {
		result.insert(result.end(), {
			(numSplitV - 1) * numSplitH + 1,
			(numSplitV - 2) * numSplitH + (m + 1) % numSplitH + 1,
			(numSplitV - 2) * numSplitH + m + 1
			});
	}

	std::for_each(result.begin(), result.end(), [=](uint32_t& index) {
		index += baseIndex;
	});

	return result;
}