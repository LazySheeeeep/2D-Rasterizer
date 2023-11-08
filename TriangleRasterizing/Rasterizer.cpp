//
// Created by Kaleido on 4/1/23.
//

#include "Rasterizer.h"

void inline rst::Rasterizer::shade_pixel(const Eigen::Vector2i& point, const Eigen::Vector3f& color)
{
	if (point.x() < 0 || point.x() >= width ||
		point.y() < 0 || point.y() >= height) return;
	frame_buf[get_index(point)] = color;
}

void inline rst::Rasterizer::shade(const std::vector<Eigen::Vector2i>& points, const Eigen::Vector3f& color)
{
	for (const auto& point : points) {
		shade_pixel(point + bias, color);
	}
}

void rst::Rasterizer::rasterize_triangle(const Triangle& t)
{
	std::vector<Eigen::Vector2i> points = points_in_triangle(t);
	shade(points, t.get_color());
}

void rst::Rasterizer::print(const std::string& name)
{
	cv::Mat image(height, width, CV_32FC3, frame_buf.data());
	image.convertTo(image, CV_8UC3, 1.0f);
	cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
	cv::imshow(name, image);
}

std::vector<Eigen::Vector2i> rst::Rasterizer::points_in_triangle_1(const Triangle& t) {
	std::vector<Eigen::Vector2i> points;
	const auto& [x_min, y_min, x_max, y_max] = t.bounding_box();
	for (int y = y_min; y <= y_max; y++) {
		for (int x = x_min; x <= x_max; x++) {
			float e1 = t.edge_equation(0, x, y);
			float e2 = t.edge_equation(1, x, y);
			float e3 = t.edge_equation(2, x, y);
			if (e1 >= 0 && e2 >= 0 && e3 >= 0 || e1 <= 0 && e2 <= 0 && e3 <= 0)
				points.push_back({ x, y });
		}
	}
	return points;
}

std::vector<Eigen::Vector2i> rst::Rasterizer::points_in_triangle(const Triangle& t) {
	std::vector<Eigen::Vector2i> points;
	const auto& [x_min, y_min, x_max, y_max] = t.bounding_box();
	const float a1 = t.line_coefficients[0][0], a2 = t.line_coefficients[1][0], a3 = t.line_coefficients[2][0];
	for (int y = y_min; y <= y_max; y++) {
		int x = x_min;
		float e1 = t.edge_equation(0, x, y);
		float e2 = t.edge_equation(1, x, y);
		float e3 = t.edge_equation(2, x, y);
		for (x++; x <= x_max; x++,e1 += a1,e2 += a2,e3 += a3) 
			if (e1 >= 0 && e2 >= 0 && e3 >= 0 || e1 <= 0 && e2 <= 0 && e3 <= 0)
				points.push_back({ x, y });
		
	}
	return points;
}