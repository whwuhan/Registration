#include "pch.h"
#include "registration.h"
using namespace std;

Registration::Registration()
{

}

//��ʼ��
void Registration::init()
{
	//����������Properties�ṹ��
	PROPERTIES properties; //��ǰ����
	if (!properties.read_data())
	{
		std::cerr << "registration.properties�ļ������������󣬻���û�ж�ȡ���ļ�..." << std::endl;
	}
	else
	{
		prop = properties;
		properties.show_properties();
	}

	//�洢��ʽmap
	std::unordered_map<std::string, int> format =
	{
		{"ply",1},
		{"obj",2},
		{"pcd",3},
		{"stl",4},
		{"pts",5}
	};
	this->format = format;
}

//��ʼ�㷨
void Registration::run()
{
	std::cout << "��ʼʱ�䣺";
	show_time();
	//ɨ�����
	pcl::PointCloud<pcl::PointXYZ>::Ptr
		point_cloud_scan(new pcl::PointCloud<pcl::PointXYZ>);

	//ԭʼ����
	pcl::PointCloud<pcl::PointXYZ>::Ptr
		point_cloud_origin(new pcl::PointCloud<pcl::PointXYZ>);


	//��ȡ��������
	switch (format[prop.input_format])
	{
	case 1:
		if (pcl::io::loadPLYFile(prop.input_file_scan, *point_cloud_scan) == -1) 
		{
			std::cerr << "��ȡɨ�����ʧ��." << std::endl;
			return ;
		}
		if (pcl::io::loadPLYFile(prop.input_file_origin, *point_cloud_origin) == -1) 
		{
			std::cerr << "��ȡԭʼ����ʧ��." << std::endl;
			return ;
		}
		//std::cerr << point_cloud_scan->size() << " " <<point_cloud_origin->size() << std::endl;
		break;  //��Ҫ����break!!!!!!!!
	default:
		std::cerr << "�޷����������ʽ" << prop.input_format << "��ʽ������������ļ�" << std::endl;
		return ;
	}

	//========================================================
	//ȥ��
	if (prop.outlier_remove)
	{	
		std::cout << "=========================��ʼ����...===============================" << std::endl;
		std::cout << "ʱ�䣺";
		show_time();
		pcl::PointCloud<pcl::PointXYZ>::Ptr
			point_cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

		pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;
		outrem.setInputCloud(point_cloud_scan);
		outrem.setRadiusSearch(prop.outlier_radius);
		outrem.setMinNeighborsInRadius(prop.outlier_min_neighbors);
		outrem.setKeepOrganized(false); //�����Ϊtrue�������Ϊoutlier�ĵ�ᱻ��Ϊnan������ı�ԭʼ���ƵĴ�С
		outrem.filter(*point_cloud_filtered);
			
		unsigned int pre_size = point_cloud_scan->size();
		std::cout << "����ǰ���ƴ�С��" << pre_size << std::endl;
		point_cloud_scan = point_cloud_filtered;
		unsigned int pro_size = point_cloud_scan->size();
		std::cout << "�������ƴ�С: " << pro_size << std::endl;
		std::cout << "ȥ����������" << pre_size - pro_size << std::endl;

		std::cout << "ʱ�䣺";
		show_time();
		std::cout << "=========================�������===============================" << std::endl;
	}
	//ȥ�����
	//========================================================


	
	//===============================================================
	//ICP��׼
	std::cout << "=========================����ICP��׼...=================================" << std::endl;
	std::cout << "ʱ�䣺";
	show_time();
	//����ICP����
	pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
	//���ò���
	icp.setInputSource(point_cloud_scan);      //��Ҫ��׼�ĵ���
	icp.setInputTarget(point_cloud_origin);    //Ŀ����ƣ����ǲ������Ǹ�
	icp.setMaximumIterations(prop.iter);
	//������
	pcl::PointCloud<pcl::PointXYZ>::Ptr
		point_cloud_icp_res(new pcl::PointCloud<pcl::PointXYZ>);
	icp.align(*point_cloud_icp_res);
	//hasConverged()�����Ƿ�����
	std::cout << "has converged:" << icp.hasConverged() << std::endl;
	std::cout << "score: " << icp.getFitnessScore() << std::endl;
	std::cout << icp.getFinalTransformation() << std::endl;

	StatData stat_data;	//ͳ������
	//����icpͳ������
	stat_data.point_cloud_icp_res_ptr = point_cloud_icp_res;
	stat_data.point_cloud_origin_ptr = point_cloud_origin;
	stat_data.has_converged = icp.hasConverged();
	stat_data.fitness_score = icp.getFitnessScore();
	stat_data.trans_mat = icp.getFinalTransformation();
	switch (format[prop.output_format])
	{
	case 1:
		pcl::io::savePLYFile(prop.output_file_icp, *point_cloud_icp_res);
		break;
	default:
		std::cout << "�޷����������ʽ" << prop.output_format << "��ʽ������������ļ�" << std::endl;
		return ;
	}
	std::cout << "ʱ�䣺";
	show_time();
	std::cout << "=========================icp��׼����=================================" << std::endl;
	//icp��׼����
	//============================================================


	//=========================================================
	//ɸѡ������ֵ�ĵ�
	//����һ��kd_tree
	std::cout << "=========================��ʼɸѡ...==============================" << std::endl;
	std::cout << "ʱ�䣺";
	show_time();
	pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
	//�����������
	kdtree.setInputCloud(point_cloud_origin);
	//k���ھ�
	int k = 1;

	std::vector<int> point_knn_index(k);						//ĳһ�����k���ھ�����
	std::vector<float> point_knn_sqr_dis(k);					//ĳ����k���ھӵľ����ƽ��
	std::vector<std::vector<int> > point_cloud_knn_index;		//��׼�������е��k���ھ�����
	std::vector<std::vector<float> > point_cloud_knn_sqr_dis;	//��׼�������е��k���ھӾ����ƽ��
	for (int i = 0; i < point_cloud_icp_res->points.size(); i++)
	{
		if (kdtree.nearestKSearch(point_cloud_icp_res->points[i], k, point_knn_index, point_knn_sqr_dis) > 0)
		{
			point_cloud_knn_index.push_back(point_knn_index);
			point_cloud_knn_sqr_dis.push_back(point_knn_sqr_dis);
			point_knn_index.clear();
			point_knn_sqr_dis.clear();
		}
	}

	std::vector<int> flags(point_cloud_knn_sqr_dis.size(), 0);		//��־
	int size_count = 0;												//������ֵ�����
	for (int i = 0; i < point_cloud_knn_sqr_dis.size(); i++)
	{
		//�ж��Ƿ���ھ�����ֵ
		if (point_cloud_knn_sqr_dis[i][0] > prop.dis_threshold * prop.dis_threshold)
		{
			flags[i] = 1;
			size_count++;
		}
	}

	
	stat_data.total_amount = point_cloud_scan->points.size();					//�ܵĵ���
	stat_data.exc_amount = size_count;											//������ֵ�ĵ���
	stat_data.nn_index = trans_knn_index_to_nn_index(point_cloud_knn_index);	//����ھӵ�index

	//�������е��ƫ��
	stat_data.total_bias = cal_total_bias(point_cloud_knn_sqr_dis);
	//���㳬����ֵ���ƫ�� ע���һ������
	stat_data.exc_bias = cal_exc_bias(stat_data.total_bias, flags);
	//��Ӧ���Ƿ񳬹���ֵ
	stat_data.flags = flags;

	pcl::PointCloud<pcl::PointXYZ>::Ptr
		point_cloud_compare_res(new pcl::PointCloud<pcl::PointXYZ>); //�ԱȽ��
	point_cloud_compare_res->height = 1;
	point_cloud_compare_res->width = size_count;
	point_cloud_compare_res->resize(point_cloud_compare_res->height * point_cloud_compare_res->width);
	size_count = 0;
	for (int i = 0; i < point_cloud_knn_sqr_dis.size(); i++)
	{
		if (flags[i])
		{
			point_cloud_compare_res->points[size_count++] = point_cloud_icp_res->points[i];
		}
	}
	std::cout << "ʱ�䣺";
	show_time();
	std::cout << "=========================ɸѡ����==============================" << std::endl;
	//ɸѡ����
	//==================================================

	//==================================================
	//�����ɫ����
	std::cout << "=========================�����ɫ����...================================" << std::endl;
	std::cout << "ʱ�䣺";
	show_time();
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr
		point_cloud_xyzrgb_icp_res(new pcl::PointCloud<pcl::PointXYZRGB>);
	point_cloud_xyzrgb_icp_res->width = point_cloud_icp_res->width;
	point_cloud_xyzrgb_icp_res->height = point_cloud_icp_res->height;
	point_cloud_xyzrgb_icp_res->resize(point_cloud_xyzrgb_icp_res->width * point_cloud_xyzrgb_icp_res->height);
	for (int i = 0; i < point_cloud_xyzrgb_icp_res->size(); i++)
	{
		point_cloud_xyzrgb_icp_res->points[i].x = point_cloud_icp_res->points[i].x;
		point_cloud_xyzrgb_icp_res->points[i].y = point_cloud_icp_res->points[i].y;
		point_cloud_xyzrgb_icp_res->points[i].z = point_cloud_icp_res->points[i].z;
		/*if (flags[i])
		{
			point_cloud_xyzrgb_icp_res->points[i].r = 255;
			point_cloud_xyzrgb_icp_res->points[i].g = 0;
			point_cloud_xyzrgb_icp_res->points[i].b = 0;
		}
		else
		{
			point_cloud_xyzrgb_icp_res->points[i].r = 0;
			point_cloud_xyzrgb_icp_res->points[i].g = 0;
			point_cloud_xyzrgb_icp_res->points[i].b = 255;
		}*/
		Color color = get_color(prop, stat_data.total_bias[i]);
		point_cloud_xyzrgb_icp_res->points[i].r = color.R;
		point_cloud_xyzrgb_icp_res->points[i].g = color.G;
		point_cloud_xyzrgb_icp_res->points[i].b = color.B;
	}
	std::cout << "ʱ�䣺";
	show_time();
	std::cout << "=========================�����ɫ���ֽ���========================" << std::endl;
	//�����ɫ���ֽ���
	//==================================================
	
	stat_data.cal_ave_bais();//����ƽ��ƫ��
	stat_data.show_data();
	//������
	std::cout << "=========================��ʼ������========================" << std::endl;
	std::cout << "ʱ�䣺";
	show_time();
	switch (format[prop.output_format]) 
	{
	case 1:
		pcl::io::savePLYFile(prop.output_file_outlier, *point_cloud_compare_res);
		pcl::io::savePLYFile(prop.output_file_icp_color, *point_cloud_xyzrgb_icp_res);
		break;
	default:
		std::cout << "�޷����������ʽ" << prop.output_format << "��ʽ������������ļ�" << std::endl;
		return ;
	}
	stat_data.save_data(prop);
	std::cout << "ʱ�䣺";
	show_time();
	std::cout << "=========================����������================================" << std::endl;
	std::cout << "����ʱ�䣺";
	show_time();
	
}



//�������е��ƫ��
std::vector<float> Registration::cal_total_bias(std::vector<std::vector<float> >& sqr_dis)
{
	std::vector<float> res(sqr_dis.size(), 0);
	for (int i = 0; i < sqr_dis.size(); i++)
	{
		res[i] = sqrt(sqr_dis[i][0]);
	}
	return res;
}

//���㳬����ֵ��ƫ��
std::vector<float> Registration::cal_exc_bias
(
	std::vector<float> &dis,
	std::vector<int> &flags
)
{
	std::vector<float> res;
	int count = 0;
	for (int i = 0; i < flags.size(); i++)
	{
		if (flags[i])
		{
			res.push_back(dis[i]);
		}
	}
	return res;
}

//��vector<vector<int> >ת��Ϊvector<int>
std::vector<int> Registration::trans_knn_index_to_nn_index
(
	std::vector<std::vector<int> > &knn_index
)
{
	vector<int> res(knn_index.size());
	for (int i = 0; i < knn_index.size(); i++)
	{
		res[i] = knn_index[i][0];
	}
	return res;
}

//��������
Registration::~Registration()
{
}

//����ŷʽ����
float Registration::get_euclidean_distance(const pcl::PointXYZ& pointXYZ_1, const pcl::PointXYZ& pointXYZ_2)
{
	float euclidean_distance =
		(pointXYZ_1.x - pointXYZ_2.x) * (pointXYZ_1.x - pointXYZ_2.x) +
		(pointXYZ_1.y - pointXYZ_2.y) * (pointXYZ_1.y - pointXYZ_2.y) +
		(pointXYZ_1.z - pointXYZ_2.z) * (pointXYZ_1.z - pointXYZ_2.z);

	euclidean_distance = sqrt(euclidean_distance);
	return euclidean_distance;
}

//��ʾʱ��
void Registration::show_time()
{
	time_t now = time(0);			//��ȡ��ǰʱ�� 1970�����ڵ�����
	tm *loc_time = localtime(&now);	//ת���ɵ���ʱ��
	std::cout << 1900 + loc_time->tm_year << "��" << 1 + loc_time->tm_mon << "��" << loc_time->tm_mday << "��";
	std::cout << std::setw(2) << std::setfill('0') << loc_time->tm_hour << ":";
	std::cout << std::setw(2) << std::setfill('0') << loc_time->tm_min << ":";
	std::cout << std::setw(2) << std::setfill('0') << loc_time->tm_sec << std::endl;
}

