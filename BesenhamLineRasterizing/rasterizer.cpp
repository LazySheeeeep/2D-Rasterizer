//
// Created by Kaleido on 3/20/23.
//

#include "rasterizer.h"
#include <math.h>

void rst::Rasterizer::shade_pixel(const Vector2i& point, const Vector3f& color)
{
	if (point.x() < 0 || point.x() >= width ||
		point.y() < 0 || point.y() >= height) return;
	frame_buf[get_index(point)] = color;
}

void rst::Rasterizer::shade(const std::vector<Vector2i>& points, const Vector3f& color)
{
    for (const auto& point : points) {
        shade_pixel(point, color);
    }
}

std::vector<Vector2i> rst::Rasterizer::line_points(Vector2f begin, Vector2f end)
{
    std::vector<Vector2i> points;

    auto x1 = begin.x();
    auto y1 = begin.y();
    auto x2 = end.x();
    auto y2 = end.y();

    int x, y, dx, dy, _dx_, _dy_, nex, ney, xe, ye;

    dx = x2 - x1;
    dy = y2 - y1;
    _dx_ = fabs(dx);
    _dy_ = fabs(dy);
    nex = 2 * _dy_ - _dx_; //NErrorX=2*|dy|-|dx|
    ney = 2 * _dx_ - _dy_;

    if (_dy_ <= _dx_) //line's length in x-direction is longer than (or equal to) that of in y. so that abs(k)<=1.
    {
        if (dx >= 0) //x1 is left to x2
        {
            x = x1;
            y = y1;
            xe = x2;
        }
        else //x2 is on the left
        {
            x = x2;
            y = y2;
            xe = x1;
        }
        points.push_back(Vector2i(x, y));
        for (x = x + 1; x <= xe; x++) //let x be the raster unit
        {
            // find out y and calculate nex
            if (nex < 0)
            {
                nex = nex + 2 * _dy_;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) // k>0
                {
                    y = y + 1;
                }
                else // k <= 0
                {
                    y = y - 1;
                }
                nex = nex + 2 * (_dy_ - _dx_);//nex=nex+2*|dy|;nex=nex-2|dx|;
            }
            points.push_back(Vector2i(x, y));
        }
    }
    else //y-direction length is longer, let y be the raster unit
    {
        if (dy >= 0) //y1 is under y2
        {
            x = x1;
            y = y1;
            ye = y2;
        }
        else
        {
            x = x2;
            y = y2;
            ye = y1;
        }
        points.push_back(Vector2i(x, y));
        for (y = y + 1; y <= ye; y++)
        {
            if (ney <= 0)
            {
                ney = ney + 2 * _dx_;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    x = x + 1;
                }
                else
                {
                    x = x - 1;
                }
                ney = ney + 2 * (_dx_ - _dy_);
            }
            points.push_back(Vector2i(x, y));
        }
    }
    return points;
}

std::vector<Vector2i> rst::Rasterizer::circle_points(Vector2f origin, float r)
{
    std::vector<Vector2i> points;
    int x = 0, y = (int)r, d = 3 - 2 * r;  // 当前d=d0
    //上下左右顶点
    points.push_back(Vector2i(origin.x(), origin.y() + y));
    points.push_back(Vector2i(origin.x(), origin.y() - y));
    points.push_back(Vector2i(origin.x() + y, origin.y()));
    points.push_back(Vector2i(origin.x() - y, origin.y()));

    for (x++; x < r / sqrt(2); x++) {
        //d指向下一位，并判断是否需要更新y
        if (d > 0)  d = d + 4 * (x - 1 - y--) + 10;
        else d = d + 4 * (x - 1) + 6;
        points.push_back(Vector2i(origin.x() + x, origin.y() + y));
        points.push_back(Vector2i(origin.x() + x, origin.y() - y));
        points.push_back(Vector2i(origin.x() - x, origin.y() + y));
        points.push_back(Vector2i(origin.x() - x, origin.y() - y));
        if (x != y) {
            points.push_back(Vector2i(origin.x() + y, origin.y() + x));
            points.push_back(Vector2i(origin.x() + y, origin.y() - x));
            points.push_back(Vector2i(origin.x() - y, origin.y() + x));
            points.push_back(Vector2i(origin.x() - y, origin.y() - x));
        }
    }
    return points;
}

void rst::Rasterizer::draw_line(Vector2f begin, Vector2f end, Vector3f color = { 255, 255, 255 })
{
    std::vector<Vector2i> points = line_points(begin, end);
    shade(points, color);
}

void rst::Rasterizer::draw_circle(Vector2f origin, float r, Vector3f color)
{
    auto points = circle_points(origin, r);
    shade(points, color);
}

std::vector<Vector2i> rst::Rasterizer::grid_square_points(int line_num, int interval)
{
    std::vector<Vector2i> points;
    Vector2f origin(width / 2.f, height / 2.f);
    int d, half = interval * (line_num - 1) / 2;
    if (line_num % 2 == 0)d = -(interval / 2.f);
    else d = -interval;
    for (d += interval; d < half+2; d += interval) {
        //x左右
        for (int y = origin.y() - half; y <= origin.y() + half; y++) {
            points.push_back(Vector2i(origin.x() + d, y));
            points.push_back(Vector2i(origin.x() - d, y));
        }
        //y上下
        for (int x = origin.x() - half; x <= origin.x() + half; x++) {
            points.push_back(Vector2i(x, origin.y() + d));
            points.push_back(Vector2i(x, origin.y() - d));
        }
    }

    return points;
}

void rst::Rasterizer::mission1(const Vector2f& origin, const int length)
{
    //浅灰，蓝色，绿色，青色，红色，粉色，黄色，白色
    std::vector<Vector3f> color_set{
        {128,128,128},
        {0,0,255},
        {0, 255, 0},
        {0,255,255},
        {255,0,0},
        {255,0,255},
        {255,255,0},
        {255,255,255}
    };
    int a = -5; //度数

    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 9; i++) {
            a += 5;
            Vector2f end(origin.x() + cos(a * PI / 180) * length, origin.y() + sin(a * PI / 180) * length);
            draw_line(origin, end, color_set[j]);
        }
    }
}

void rst::Rasterizer::mission2(const int a, const int interval)
{
    Vector2f origin(width / 2.f, height / 2.f);

    //网格
    std::vector<Vector2i> points = grid_square_points(2 * a + 1, interval);
    shade(points, { 100,100,100 });

    //黄{255,255,0},青{0,255,255},蓝{0,0,255},绿{0, 255, 0},红{255,0,0},从外到里递减5格
    std::vector<Vector3f> color_set{
        {255,255,0},
        {0,255,255},
        {0,0,255},
        {0, 255, 0},
        {255,0,0}
    };
    for (int i = 0; i < 5; i++) {
        //大圆环
        draw_circle(origin, a * (1 - 0.2 * i) * interval, color_set[i]);

        //小圆圈
        points = circle_points({ 0,0 }, a * (1 - 0.2 * i));  //小圆圈中心点的网格坐标
        for (auto& point : points) {
            //对于每个小圆圈中心点都画一个半径为0.25单位的小圆，与大圆同色
            draw_circle({ origin.x() + point.x() * interval, origin.y() + point.y() * interval }, interval * .25, color_set[i]);
        }
    }
}
