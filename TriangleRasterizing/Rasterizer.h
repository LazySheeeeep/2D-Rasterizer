//
// Created by Kaleido on 3/31/23.
//

#pragma once
#include<opencv2/opencv.hpp>
#include "Triangle.h"

namespace rst {
    class Rasterizer
    {
        const int width, height;
        const Eigen::Vector2i bias;
        std::vector<Eigen::Vector3f> frame_buf;

        int inline get_index(Eigen::Vector2i modified_point) { return (height - modified_point.y()) * width + modified_point.x(); }

        void inline shade_pixel(const Eigen::Vector2i& point, const Eigen::Vector3f& color);
    public:
        void inline shade(const std::vector<Eigen::Vector2i>& points, const Eigen::Vector3f& color);

        static std::vector<Eigen::Vector2i> points_in_triangle_1(const Triangle& t);
        static std::vector<Eigen::Vector2i> points_in_triangle(const Triangle& t);

        Rasterizer(const int w, const int h) :width(w), height(h), bias({ w * 0.5, h * 0.5 }) { frame_buf.resize(width * height); }

        const inline std::vector<Eigen::Vector3f>& frame_buffer() { return frame_buf; }
        void inline clear_buffer() { std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{ 0, 0, 0 }); }

        void rasterize_triangle(const Triangle& t);

        void print(const std::string& name);
    };

} // namespace rst
