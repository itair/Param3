
// Param3Doc.h : CParam3Doc 类的接口
//
#pragma once
#include <afxtempl.h>
#define MAXV 100000
//#include "Para.h"

class PNT{
public:
	bool operator ==(PNT p);
	PNT operator /(double d);
	double operator *(PNT p); //对应对应坐标相乘之和
	PNT operator - (PNT p);
	PNT operator + (PNT p);
	PNT operator *(double m); // 坐标线性乘法
	double x;
	double y;
	double z;
};

class FACE{
public:
	int vn[3];
	int en[3];
};

class EDGE{
public:
	int vn[2];
	int fn[2];
};

class BOUNDBOX{
public:
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double zmin;
	double zmax;
};

class VOXEL{
public:
	PNT centr; //the center of the voxel
	double upara[2]; //the parameter along u direction
	double vpara[2]; //the parameter along v direction
	int sgn;   //whether the voxel is feature voxel. 
	//2: border voxel, 1: feature, 0: non-feature
	int prcd;  //whether the voxel is processed
};

class CParam3Doc : public CDocument
{
protected: // 仅从序列化创建
	CParam3Doc();
	DECLARE_DYNCREATE(CParam3Doc)

// 特性
public:
	CArray<PNT,PNT> m_pntSamplePnt;
	CArray<EDGE,EDGE> m_egEdgeAry;
	CArray<FACE,FACE> m_fcFaceAry;
	int m_iRowOfSamplePnt;
	int m_iColOfSamplePnt;
	bool m_IsUpdata; // 测试数据是否更新;
	bool m_IsRun; //检测para是否运行过;
	CArray<PNT,PNT>	m_U_TtoB;
	CArray<PNT,PNT>	m_U_BtoT;
	CArray<PNT,PNT>	m_V_LtoR;
	CArray<PNT,PNT>	m_V_RtoL;
	CArray<VOXEL,VOXEL> m_vxlVoxel;   //参数化体元?
	PNT m_ptVxlArySize;
	double m_dVoxSize;	
// 	CArray<ISOPARA,ISOPARA> m_isoIsoUParaTtoB;		//最终划分的结果..U,V两个方向
// 	CArray<ISOPARA,ISOPARA> m_isoIsoUParaBtoT;
// 	CArray<ISOPARA,ISOPARA> m_isoIsoVParaLtoR;
// 	CArray<ISOPARA,ISOPARA> m_isoIsoVParaRtoL;
// 操作
public:
//	void SaveData(FILE *stream);
//	void ReadData(FILE *stream);
	void ParamRun(void);
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CParam3Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
 	
public:
	void SaveDivid(FILE* stream);
};
