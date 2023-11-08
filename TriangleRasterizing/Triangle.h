//
// Created by Kaleido on 3/31/23.
//

#pragma once
#include <iostream>
#include <eigen3/Eigen/Eigen>

class Triangle {
	const Eigen::Vector2f vertex[3];
	const Eigen::Vector3f color;
	const int xmin, ymin, xmax, ymax;

public:
	Triangle(float x1, float y1, float x2, float y2, float x3, float y3, Eigen::Vector3f color)
		:Triangle({ x1, y1 }, { x2, y2 }, { x3, y3 }, color) {}
	Triangle(Eigen::Vector2f(&vertices)[3], Eigen::Vector3f color)
		:Triangle(vertices[0], vertices[1], vertices[2], color) {}
	Triangle(Eigen::Vector2f v1, Eigen::Vector2f v2, Eigen::Vector2f v3, Eigen::Vector3f color) :
		color(color), vertex{ v1,v2,v3 },
		xmin((int)std::floor(std::min({ vertex[0].x(), vertex[1].x(), vertex[2].x() }))),
		ymin((int)std::floor(std::min({ vertex[0].y(), vertex[1].y(), vertex[2].y() }))),
		xmax((int)std::ceil (std::max({ vertex[0].x(), vertex[1].x(), vertex[2].x() }))),
		ymax((int)std::ceil (std::max({ vertex[0].y(), vertex[1].y(), vertex[2].y() }))),
		line_coefficients{
			Eigen::Vector3f(v1.y() - v2.y(), v2.x() - v1.x(), v1.x() * v2.y() - v2.x() * v1.y()),
			Eigen::Vector3f(v2.y() - v3.y(), v3.x() - v2.x(), v2.x() * v3.y() - v3.x() * v2.y()),
			Eigen::Vector3f(v3.y() - v1.y(), v1.x() - v3.x(), v3.x() * v1.y() - v1.x() * v3.y())
	}{}

	const Eigen::Vector3f line_coefficients[3];
	float inline edge_equation(const size_t idx, const float x, const float y)const
	{
		auto& L = line_coefficients[idx];
		return L[0] * x + L[1] * y + L[2];
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns>xmin, ymin, xmax, ymax</returns>
	std::tuple<const int&, const int&, const int&, const int&> bounding_box() const { return { xmin, ymin, xmax, ymax }; }
	const Eigen::Vector3f& get_color() const { return color; }
};