//
// Created by Kaleido on 3/31/23.
//

#include<Windows.h>
#include "Rasterizer.h"

int main() {
	int width = 700, height = 700;  //������Ⱥ͸߶�
	rst::Rasterizer r(width, height); //��ʼ����դ����r
	Triangle triangles[5]{ //��ʼ�����������飬�������5��������
		Triangle(-179.4, 59.9, -196.1, 221.8, 341.2, -63.5, {23, 120, 86}),
		Triangle(-331.0, -337.6, -227.9, 199.7, 299.2, -117.5, {234, 98, 37}),
		Triangle(90.5, -56.6, 214.3, -264.3, 103.9, -10.6, {16, 109, 176}),
		Triangle(-277.2, -248.5, -70.7, 322.8, 93.0, 262.1, {232, 190, 21}),
		Triangle(-41.4, -330.1, -229.4, -267.4, 55.7, 228.1, {150, 31, 101})
	};

	long compute_time = .0, start;
	
	for (auto& t : triangles) {
		start = GetTickCount64(); //��ʱ
		auto points = r.points_in_triangle_1(t); //ִ��δ�Ż����㷨
		compute_time += GetTickCount64() - start;
		r.shade(points, t.get_color());
	}
	
	r.print("Algorithm 1");
	std::cout << "Algorithm 1 computation time:" << compute_time << "ms" << std::endl;

	r.clear_buffer(); //���֡����
	compute_time = .0;

	for (auto& t : triangles) {
		start = GetTickCount64(); //��ʱ
		auto points = r.points_in_triangle(t); //ִ���Ż��˵��㷨
		compute_time += GetTickCount64() - start;
		r.shade(points, t.get_color());
	}

	r.print("Algorithm 2");
	std::cout << "Algorithm 2 computation time:" << compute_time << "ms" << std::endl;

	cv::waitKey(0);
}