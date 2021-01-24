#pragma once
#include "properties.h"
#include "stat_data.h"
#include "color.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <pcl/io/ply_io.h>              //ply�ļ���ȡ
#include <pcl/point_types.h>            //������
#include <pcl/point_cloud.h>            //��������
#include <pcl/registration/icp.h>       //ICP�㷨
#include <boost/algorithm/string.hpp>   //split
#include <pcl/kdtree/kdtree_flann.h>    //kd������
#include <pcl/filters/radius_outlier_removal.h>//ȥ��

class Registration
{
public:
	PROPERTIES prop;							//����
	std::unordered_map<std::string, int> format;//��ѡ���Ƶ�����
	

	Registration();								//���캯��
	void init();								//��ʼ��
	void run();									//��ʼ�㷨
	~Registration();							//��������
private:
	//���������ŷʽ����
	float get_euclidean_distance(const pcl::PointXYZ& pointXYZ_1, const pcl::PointXYZ& pointXYZ_2);

	// ��ʾ��ǰʱ��
	void show_time();

	//�������е��ƫ��
	std::vector<float> cal_total_bias(std::vector<std::vector<float> >& sqr_dis);

	//���㳬����ֵ��ƫ�� ע�������Ĳ������Ͳ�ͬ
	std::vector<float> cal_exc_bias
	(
		std::vector<float> &dis,
		std::vector<int> &flags
	);

	//��vector<vector<int> >ת��Ϊvector<int>
	std::vector<int> trans_knn_index_to_nn_index
	(
		std::vector<std::vector<int> > &knn_index
	);
};




