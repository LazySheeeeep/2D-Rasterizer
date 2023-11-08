//
// Created by Kaleido on 3/20/23.
//

#include <iostream>
#include <eigen3/Eigen/Eigen>
#include "global.h"
using namespace Eigen;

namespace rst {
    class Rasterizer
    {
    public:
        Rasterizer(int w, int h) :width(w), height(h) { frame_buf.resize(width * height); }

        std::vector<Vector3f>& frame_buffer() { return frame_buf; }
        void clear_buffer(){ std::fill(frame_buf.begin(), frame_buf.end(), Vector3f{ 0, 0, 0 }); }

        void draw_line(Vector2f begin, Vector2f end, Vector3f color);
        void draw_circle(Vector2f origin, float r, Vector3f color);

        void mission1(const Vector2f& origin, const int length);
        void mission2(const int a, const int interval);

    private:
        void shade_pixel(const Vector2i& point, const Vector3f& color);
        void shade(const std::vector<Vector2i>& points, const Vector3f& color);

        std::vector<Vector2i> grid_square_points(int line_num, int interval);
        std::vector<Vector2i> line_points(Vector2f begin, Vector2f end);
        std::vector<Vector2i> circle_points(Vector2f origin, float r);

        std::vector<Vector3f> frame_buf;

        int get_index(Vector2i point) { return (height - point.y()) * width + point.x(); }

        int width, height;
    };
} // namespace rst
