// Para.h: interface for the CPara class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_PARA_H__43D988C0_11EA_4B29_A6D7_429CB1A51B2A__INCLUDED_
#define AFX_PARA_H__43D988C0_11EA_4B29_A6D7_429CB1A51B2A__INCLUDED_
//#pragma once

#include "Param3Doc.h"	// Added by ClassView
//#include "Param2View.h"
#include <gl/glut.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#ifndef AFX_PARA_H_
//#define AFX_PARA_H_

class ISOPARA
{
public:
	CArray<int,int> iso;     //这是神马?
	ISOPARA();
	ISOPARA(ISOPARA& p);
	~ISOPARA();
	ISOPARA operator =(ISOPARA& p);
};

class VXLPNT{
public:
		PNT pt; // the coordinate of the point
		PNT pjt; // the projecting point of point pt to the triangle
		int pjsgn; //whether is the projecting point lie in the triangle, 1: in,
		           //0: out, the initial value is -1;
};


class CPara  
{
public:
	//VOXEL vxl[100][100][100];
	CArray<ISOPARA,ISOPARA> m_isoIsoUParaTtoB;		//最终划分的结果..U,V两个方向
	CArray<ISOPARA,ISOPARA> m_isoIsoUParaBtoT;
	CArray<ISOPARA,ISOPARA> m_isoIsoVParaLtoR;
	CArray<ISOPARA,ISOPARA> m_isoIsoVParaRtoL;
//	DIVPARA div;
	BOUNDBOX m_bbBndBox;						//包围盒
	int m_iCornerPnts[4]; //the four corner points of the surface, the sequence is
	                      //00, 01, 11, 10
	void CalcCrnrBndbx();
//	CParam2View* m_pView;
	CParam3Doc* m_pDoc;
	CArray<VOXEL,VOXEL> m_vxlVoxel;   //参数化体元?
	PNT m_ptVxlArySize; //m_ptVxlArySize.x: the dividing number along x direction
	                    //m_ptVxlArySize.y: the dividing number along y direction
	                    //m_ptVxlArySize.z: the dividing number along z direction
	double m_dVoxSize;		//划分最小单位??


	
public:
	bool NewElemInExistBorder(int ser, CArray<int,int>& brd);
	bool CalcNxtPntInBorder(int& ser, PNT& pt, CArray<int,int>& brd);
	bool DetmFourBorders();
	bool NewElemInExistBorder(int ser,ISOPARA isop,int ends);
	int m_iCornerVxl[4];
	void ReverseSequence(CArray<int,int>& brd);
	void CalcVxlSerPntLieIn(PNT pt, int ser[3]);
	void Run();
	void InvIndex(int s, int ind[]);
	bool CnstVParaRgtToLft();
	bool CnstVParaLftToRgt();
	bool CnstUParaBotmToTop();
	bool CnstUParaTopToBotm();
	void AdjtBndbx();
	bool CalcIsoParam();
	int Sign(double m);
	PNT Cross(PNT p1, PNT p2);
	bool PntInTri(PNT pt, int ser);
	PNT CalcProjPnt(PNT pt,PNT nrl,int ser);
	int PntInTable(PNT pt, CArray<VXLPNT,VXLPNT>& ptl);
	bool AllPjtPntOutside(PNT pt[],int ser,CArray<VXLPNT,VXLPNT>& ptl);
	PNT CalcTriNrml(int ser);
	bool AllVertAtOneSide(PNT pt[],int ser);
	void CnstEightPntOfVxl(PNT centr, double hs, PNT pt[]);
	void DetmFeatrVxlByCurTri(BOUNDBOX bx, int ser);
	bool VxlLstHasOneVxl(BOUNDBOX vl);
	BOUNDBOX CalcVxlLst(BOUNDBOX bx);
	BOUNDBOX CalcTriBndbx(int ser);
	void DetmFeatrVxl();
	void DvdBndbxByCurSize();
	int Index(int i, int j, int k);
	void DvdBndbx();
	double CalcVoxSize();
	void SaveDivid();

	
	CPara(CParam3Doc *pdoc);
	CPara();
	virtual ~CPara();

public:
	void SavetoDoc(void);
	
	bool m_IsRun;//测试 Para::run()是否运行过;

	
};




#endif // !defined(AFX_PARA_H__43D988C0_11EA_4B29_A6D7_429CB1A51B2A__INCLUDED_)
