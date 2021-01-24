#pragma once
/*
	ͳ������
*/
#include "properties.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <pcl/io/ply_io.h>              //ply�ļ���ȡ
#include <pcl/point_types.h>            //������
#include <pcl/point_cloud.h>            //��������
#include <pcl/registration/icp.h>       //ICP�㷨
#include <boost/algorithm/string.hpp>   //split
#include <pcl/kdtree/kdtree_flann.h>    //kd������
#include <pcl/filters/radius_outlier_removal.h>//ȥ��


typedef struct StatData
{
	//��Ҫ���������
	pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_icp_res_ptr;
	pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_origin_ptr;

	unsigned int total_amount;				//�ܵĵ���
	unsigned int exc_amount;				//������ֵ�ĵ���
	std::vector<float> total_bias;			//���е��ƫ��ֵ
	std::vector<float> exc_bias;			//������ֵ��ƫ��ֵ
	double total_ave_bias;					//���е��ƽ�����
	double exc_ave_bias;					//������ֵ���ƽ�����
	std::vector<int> nn_index;				//����ھӵ�index
	std::vector<int> flags;					//�Ƿ񳬹���ֵ,1��ʾ������0��ʾδ����
	int has_converged;						//�Ƿ�����
	double fitness_score;					//ƥ�����
	Eigen::Matrix4f trans_mat;				//�任����
	
	//�ṹ�巽��
	void cal_ave_bais();					//����ƽ�����
	void show_data();						//��ʾͳ����Ϣ
	void save_data(Properties &prop);		//����ͳ������
} STATDATA;