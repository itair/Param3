// Para.cpp: implementation of the CPara class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Param3.h"
#include "Para.h"

#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////
//ISO
/////////////////////////
ISOPARA::ISOPARA(ISOPARA& p)
{
	int i;
	for(i = 0; i < p.iso.GetSize(); i++)
		iso.Add(p.iso[i]);					//翻倍??

}

ISOPARA::~ISOPARA()
{
	iso.RemoveAll();
}

ISOPARA::ISOPARA()
{
}

ISOPARA ISOPARA::operator = (ISOPARA& p)
{
	ISOPARA ip;
	ip.iso.Copy(p.iso);
	return(ip);
}





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPara::CPara()
{

}

CPara::~CPara()
{

}


CPara::CPara(CParam3Doc *pdoc)
{
	m_pDoc = pdoc;
//	m_pView = pview;
	/*m_IsUpdata=false;*/
// 	m_IsRun=false;

}


//--------------------------------------------------------------------------
//Function: Get the four corner points and the bounding box of the surface
void CPara::CalcCrnrBndbx()
{
	//Get the four corner points of the surface
	int row, col;
	row = m_pDoc->m_iRowOfSamplePnt;
	col = m_pDoc->m_iColOfSamplePnt;
	m_iCornerPnts[0] = 0; //serial: (0,0)
	m_iCornerPnts[1] = row - 1; //serial: (0,1)
	m_iCornerPnts[2] = (col - 1) * row + row - 1; //serial: (1,1)
	m_iCornerPnts[3] = (col - 1) * row + 1 - 1; //serial: (1,0);

	//Get the bounding box of the surface
	int nelm,i;
	double x, y, z;
	nelm = m_pDoc->m_pntSamplePnt.GetSize();
	m_bbBndBox.xmin = MAXV; m_bbBndBox.xmax = -MAXV;
	m_bbBndBox.ymin = MAXV; m_bbBndBox.ymax = -MAXV;
	m_bbBndBox.zmin = MAXV; m_bbBndBox.zmax = -MAXV;
	for(i = 1; i < nelm; i++)
	{
		x = m_pDoc->m_pntSamplePnt[i].x;
		y = m_pDoc->m_pntSamplePnt[i].y;
		z = m_pDoc->m_pntSamplePnt[i].z;
		if(m_bbBndBox.xmin > x) m_bbBndBox.xmin = x; 
		if(m_bbBndBox.xmax < x) m_bbBndBox.xmax = x;
		if(m_bbBndBox.ymin > y) m_bbBndBox.ymin = y;
		if(m_bbBndBox.ymax < y) m_bbBndBox.ymax = y;
		if(m_bbBndBox.zmin > z) m_bbBndBox.zmin = z;
		if(m_bbBndBox.zmax < z) m_bbBndBox.zmax = z;
	}
}


//------------------------------------------------------------------
//Function: Calculate the size of the voxel divided
double CPara::CalcVoxSize()
{
	int nelm, sv1, sv2;
	PNT p1,p2,dp;
	double minlen,len;
	nelm = m_pDoc->m_egEdgeAry.GetSize();
    minlen = MAXV;
	for(int i = 0; i < nelm; i++)
	{
		sv1 = m_pDoc->m_egEdgeAry[i].vn[0];  
		sv2 = m_pDoc->m_egEdgeAry[i].vn[1];
		p1 = m_pDoc->m_pntSamplePnt[sv1];
		p2 = m_pDoc->m_pntSamplePnt[sv2];
		dp = p1 - p2;
		len = sqrt(dp*dp);
		if(minlen > len) minlen = len;
	}
	
	return(minlen /5);

}


//-----------------------------------------------------------
//Function: Adjust the bounding box
void CPara::AdjtBndbx()
{
	m_bbBndBox.xmin = m_bbBndBox.xmin - 2*m_dVoxSize;
	m_bbBndBox.xmax = m_bbBndBox.xmax + 2*m_dVoxSize;
	m_bbBndBox.ymin = m_bbBndBox.ymin - 2*m_dVoxSize;
	m_bbBndBox.ymax = m_bbBndBox.ymax + 2*m_dVoxSize;
	m_bbBndBox.zmin = m_bbBndBox.zmin - 2*m_dVoxSize;
	m_bbBndBox.zmax = m_bbBndBox.zmax + 2*m_dVoxSize;

}


//-------------------------------------------------------------------------
//Function: Divide the bounding box of the surface such that there is only
//          one point in each voxel
void CPara::DvdBndbx()
{

	m_dVoxSize = CalcVoxSize();
	AdjtBndbx();
	
	int cond = 1; //for test the validation of the dividing
	while(cond)
	{
		DvdBndbxByCurSize();
		    
		//Check whether there is only one point in each voxel
		cond = 0;
		int nump = m_pDoc->m_pntSamplePnt.GetSize();
		double sx, sy, sz;
		int ix, iy, iz;
		PNT pnt;
		for(int ii = 0; ii < nump; ii++)
		{
			pnt = m_pDoc->m_pntSamplePnt[ii];
			sx = (pnt.x - m_bbBndBox.xmin) / m_dVoxSize;
			sy = (pnt.y - m_bbBndBox.ymin) / m_dVoxSize;
			sz = (pnt.z - m_bbBndBox.zmin) / m_dVoxSize;
			ix = floor(sx); iy = floor(sy); iz = floor(sz);
			if (Index(ix,iy,iz)>0 && Index(ix,iy,iz)<m_vxlVoxel.GetSize())
			{			
				if(m_vxlVoxel[Index(ix,iy,iz)].prcd == 1)
				{
					cond = 1;
					m_dVoxSize = m_dVoxSize / 2;
					break; // for  //There are two points in one voxel.
				}
				else
				m_vxlVoxel[Index(ix,iy,iz)].prcd = 1; // this vox has been proccesed
			}
		}
	}
	
	//adjust the bounding box  对准
	int ii,jj,kk;
	ii = m_ptVxlArySize.x; jj = m_ptVxlArySize.y; kk = m_ptVxlArySize.z;
    m_bbBndBox.xmin = m_bbBndBox.xmin; m_bbBndBox.xmax = m_bbBndBox.xmin + ii*m_dVoxSize;
    m_bbBndBox.ymin = m_bbBndBox.ymin; m_bbBndBox.ymax = m_bbBndBox.ymin + jj*m_dVoxSize;
    m_bbBndBox.zmin = m_bbBndBox.zmin; m_bbBndBox.zmax = m_bbBndBox.zmin + kk*m_dVoxSize;

	
}


//-------------------------------------------------------------------
//Function: Get the index of the array m_vxlVoxel 序列序号
int CPara::Index(int i, int j, int k)
{
	int zn, yn, xn;
	zn = m_ptVxlArySize.z;
	yn = m_ptVxlArySize.y;
	xn = m_ptVxlArySize.x;

	return(k*(xn*yn) + j*xn + i);

}


//-------------------------------------------------------------------------
//Function: Calculate the tri-serial from the index s
void CPara::InvIndex(int s, int ind[])
{
	int i, j, k;
	int zn, yn, xn;

	zn = m_ptVxlArySize.z;
	yn = m_ptVxlArySize.y;
	xn = m_ptVxlArySize.x;

	k = s % (xn*yn);
	s = s - k;
	j = s % xn;
	i = s - j;

	ind[0] = i; ind[1] = j; ind[2] = k;

}


//-----------------------------------------------------------------
//Function: Divide the bounding box with the current voxel size
void CPara::DvdBndbxByCurSize()
{
	m_ptVxlArySize.x = m_ptVxlArySize.y = m_ptVxlArySize.z = 0;
	
	double hs, zprcd, yprcd, xprcd;
	int i, j, k;
	VOXEL nvxl;

	m_vxlVoxel.RemoveAll(); //clear the voxel array
	
	hs = m_dVoxSize / 2;
 	zprcd = m_bbBndBox.zmin;
	k = -1;
	while(zprcd < m_bbBndBox.zmax)
	{
		k = k + 1;
		zprcd = m_bbBndBox.zmin + k*m_dVoxSize;
		yprcd = m_bbBndBox.ymin;
		j = -1;
		while(yprcd < m_bbBndBox.ymax)
		{
			j = j + 1;
			yprcd = m_bbBndBox.ymin + j*m_dVoxSize;
			xprcd = m_bbBndBox.xmin;
			i = -1;
			while(xprcd < m_bbBndBox.xmax)
			{
				i = i + 1;
				xprcd = m_bbBndBox.xmin + i*m_dVoxSize;
				nvxl.centr.x = xprcd + hs; nvxl.centr.y = yprcd + hs;
				nvxl.centr.z = zprcd + hs;
				nvxl.sgn = 0;
				nvxl.prcd = 0;
				nvxl.upara[0] = nvxl.upara[1] = -1;
				nvxl.vpara[0] = nvxl.vpara[1] = -1;
				m_vxlVoxel.Add(nvxl);
			}
		}
	}

	//evaulate the number of the elements along x, y, z direction respectively to
	//m_ptVxlArySize
	m_ptVxlArySize.x = i+1; m_ptVxlArySize.y = j+1; m_ptVxlArySize.z = k+1;

}


//----------------------------------------------------------------------
//Function: Determine the feature voxel
void CPara::DetmFeatrVxl()
{
	int np, i;
	BOUNDBOX tribd, vxllst;
	np = m_pDoc->m_fcFaceAry.GetSize();

	for(i = 0; i < np; i++)
	{
		tribd = CalcTriBndbx(i);  //计算面包围盒
		vxllst = CalcVxlLst(tribd);  //包含三角面的体元的编号范围

		if(VxlLstHasOneVxl(vxllst)) //There is only one voxle in the voxle list
			m_vxlVoxel[Index(vxllst.xmin,vxllst.ymin,vxllst.zmin)].sgn = 1;
		else
			DetmFeatrVxlByCurTri(vxllst,i);
	}

}


//------------------------------------------------------------------------
//Function: Calculate the bounding box of the triangle whose serial is ser
BOUNDBOX CPara::CalcTriBndbx(int ser)
{
	BOUNDBOX bx;
	bx.xmin = bx.ymin = bx.zmin = MAXV;
	bx.xmax = bx.ymax = bx.zmax = -MAXV;
	
	int s[3];									//		三角面片的顶点编号
	s[0] = m_pDoc->m_fcFaceAry[ser].vn[0];
	s[1] = m_pDoc->m_fcFaceAry[ser].vn[1];
	s[2] = m_pDoc->m_fcFaceAry[ser].vn[2];

	PNT pt;
	for(int i = 0; i < 3; i++)
	{
		pt = m_pDoc->m_pntSamplePnt[s[i]];			//找到对应顶点的数据		
		if(bx.xmin > pt.x) bx.xmin = pt.x;
		if(bx.ymin > pt.y) bx.ymin = pt.y;
		if(bx.zmin > pt.z) bx.zmin = pt.z;
		if(bx.xmax < pt.x) bx.xmax = pt.x;
		if(bx.ymax < pt.y) bx.ymax = pt.y;
		if(bx.zmax < pt.z) bx.zmax = pt.z;
	}

	return bx;									//面包围盒
}


//-----------------------------------------------------------------------------
//Fucntion: Calculate voxles which contains the bounding box bx of the triangle
BOUNDBOX CPara::CalcVxlLst(BOUNDBOX bx)
{
	BOUNDBOX rg;
	
	rg.xmin = (bx.xmin - m_bbBndBox.xmin) / m_dVoxSize;				//
    rg.xmax = (bx.xmax - m_bbBndBox.xmin) / m_dVoxSize;
    rg.ymin = (bx.ymin - m_bbBndBox.ymin) / m_dVoxSize;
    rg.ymax = (bx.ymax - m_bbBndBox.ymin) / m_dVoxSize;
    rg.zmin = (bx.zmin - m_bbBndBox.zmin) / m_dVoxSize;
    rg.zmax = (bx.zmax - m_bbBndBox.zmin) / m_dVoxSize;
    
	rg.xmin = floor(rg.xmin); rg.xmax = floor(rg.xmax);			//返回的是体元的编号范围
    rg.ymin = floor(rg.ymin); rg.ymax = floor(rg.ymax);
    rg.zmin = floor(rg.zmin); rg.zmax = floor(rg.zmax);

	return rg;			
	
}


//---------------------------------------------------------------------------
//Function: Determine whether there is only one voxel in the voxel list vl
bool CPara::VxlLstHasOneVxl(BOUNDBOX vl)
{
	int i1, i2, j1, j2, k1, k2;

	i1 = vl.xmin; i2 = vl.xmax;
	j1 = vl.ymin; j2 = vl.ymax;
	k1 = vl.zmin; k2 = vl.zmax;

	if(i1 == i2 && j1 == j2 && k1 == k2) 
		return 1;
	else
		return 0;

}



//------------------------------------------------------------------------------
//Function: Determine the feature voxel within the voxel list bx by the current
//          triangle whose serial is ser
void CPara::DetmFeatrVxlByCurTri(BOUNDBOX vxllst, int ser)
{
	CArray<VXLPNT,VXLPNT> PntTbl;
	
	int i, j, k;
	PNT pt[8], cetr;				//包围盒8顶点和中心
	double hs = m_dVoxSize / 2;
	for(k = (int)vxllst.zmin; k <= (int)vxllst.zmax; k++)
		for(j = (int)vxllst.ymin; j <= (int)vxllst.ymax; j++)
			for(i = (int)vxllst.xmin; i <= (int)vxllst.xmax; i++)
			{
				cetr = m_vxlVoxel[Index(i,j,k)].centr; //关键点..

				CnstEightPntOfVxl(cetr,hs,pt);
				if(AllVertAtOneSide(pt,ser)) //non-feature point
					m_vxlVoxel[Index(i,j,k)].sgn = 0;
			    else //not all vertices at the same side 
				{
					if(AllPjtPntOutside(pt,ser,PntTbl)) //non-feature point
						m_vxlVoxel[Index(i,j,k)].sgn = 0;
					else  //feature point
						m_vxlVoxel[Index(i,j,k)].sgn = 1;
				}

			}
}



//-----------------------------------------------------------------------------
//Function: Construct the eight vertices of the voxle whose center is centr
void CPara::CnstEightPntOfVxl(PNT centr, double hs, PNT pt[])
{
	pt[0].x = centr.x - hs; pt[0].y = centr.y - hs; pt[0].z = centr.z - hs;
	pt[1].x = centr.x + hs; pt[1].y = centr.y - hs; pt[1].z = centr.z - hs;
	pt[2].x = centr.x + hs; pt[2].y = centr.y + hs; pt[2].z = centr.z - hs;
	pt[3].x = centr.x - hs; pt[3].y = centr.y + hs; pt[3].z = centr.z - hs;
	pt[4].x = centr.x - hs; pt[4].y = centr.y - hs; pt[4].z = centr.z + hs;
	pt[5].x = centr.x + hs; pt[5].y = centr.y - hs; pt[5].z = centr.z + hs;
	pt[6].x = centr.x + hs; pt[6].y = centr.y + hs; pt[6].z = centr.z + hs;
	pt[7].x = centr.x - hs; pt[7].y = centr.y + hs; pt[7].z = centr.z + hs;

}



//-----------------------------------------------------------------------------
//Function: Determine whether all the vertices of the voxel are at the same side 
//          of the triangle with serial ser 
bool CPara::AllVertAtOneSide(PNT pt[], int ser)
{
	//calculate the normal vector of the triangle
	PNT nrl;
	nrl = CalcTriNrml(ser); // 计算当前面片的归一化方向向量
	
	//calculate the localization for each vertices
	PNT p[3];
	int i;
	for(i = 0; i < 3; i++)
		p[i] = m_pDoc->m_pntSamplePnt[m_pDoc->m_fcFaceAry[ser].vn[i]];
	
	PNT cent = (p[0] + p[1] + p[2]) / 3;
	PNT dv;
	int sgold = -1, sgnew;
	for(i = 0; i < 8; i++)
	{
		dv = pt[i] - cent;  // 体元顶点指向三角面中心点的向量
		sgnew = Sign(dv*nrl);  // 向量与面片法向量方向的夹角
		if(sgnew != 0)			
		{
			if(sgold == -1) 
				sgold = sgnew;
			else
				if(sgnew != sgold) return 0;
		}
		// 方向一致否？
	}
	
	return 1;

}


//--------------------------------------------------------------------------
//Function: Calculate the normalized normal of the triangle with serial ser
PNT CPara::CalcTriNrml(int ser)
{
	int s1, s2, s3;
	s1 = m_pDoc->m_fcFaceAry[ser].vn[0];
	s2 = m_pDoc->m_fcFaceAry[ser].vn[1];
	s3 = m_pDoc->m_fcFaceAry[ser].vn[2];
	PNT p1, p2, p3, v1, v2, nrl;
	p1 = m_pDoc->m_pntSamplePnt[s1];
	p2 = m_pDoc->m_pntSamplePnt[s2];
	p3 = m_pDoc->m_pntSamplePnt[s3];
	v1 = p2 - p1; v2 = p3 - p1;
	nrl.x = v1.y*v2.z - v1.z*v2.y;
	nrl.y = v1.z*v2.x - v1.x*v2.z;
	nrl.z = v1.x*v2.y - v1.y*v2.x;
	double norm = sqrt(nrl*nrl);
	nrl = nrl / norm;
	return nrl;
}



//----------------------------------------------------------------------------
//Function: Determine whether the points in pt[] are all out of the triangle
bool CPara::AllPjtPntOutside(PNT pt[], int ser, CArray<VXLPNT,VXLPNT>& ptl)
{
	int i, s, pint;
	VXLPNT npt;
	PNT nrl, pjt;
	
	nrl = CalcTriNrml(ser);

	for(i = 0; i < 8; i++)
	{
		s = PntInTable(pt[i],ptl);
		if(s == -1) // the point pt[i] is not in the point table
		{
			pjt = CalcProjPnt(pt[i],nrl,ser);
			pint = PntInTri(pjt,ser);
			
			npt.pt = pt[i];
			npt.pjt = pjt;
			npt.pjsgn = pint;

			ptl.Add(npt);

			if(pint == 1) return 0;
		}
		else
			if(ptl[s].pjsgn == 1) return 0;
		
	}

	return 1;
}



//------------------------------------------------------------------
//Function: Determine whether the point pt is in the point table,
//          if so, return the serial; if not so, return -1;
int CPara::PntInTable(PNT pt, CArray<VXLPNT,VXLPNT>& ptl)
{
	int num, i;
	num = ptl.GetSize();
	for(i = 0; i < num; i++)
		if(ptl[i].pt == pt) return i;

	return -1;
	
}


//-----------------------------------------------------------------------------
//Function: Calculate the projecting point of point pt along direction nrl to
//          the plane which the triangle with serial ser lies in
PNT CPara::CalcProjPnt(PNT pt, PNT nrl, int ser)
{
	int s;
	s = m_pDoc->m_fcFaceAry[ser].vn[0];
	PNT p1 = m_pDoc->m_pntSamplePnt[s];
	PNT dv = p1 - pt;
	double k = (nrl * dv) / (nrl * nrl);

	PNT r;
	r = pt + nrl * k;

	return r;

}


//----------------------------------------------------------------------------
//Function: Determine whether the point pt is in the triangle with serial ser
bool CPara::PntInTri(PNT pt, int ser)
{
	PNT p[3];
	int i;
	for(i = 0; i < 3; i++)
		p[i] = m_pDoc->m_pntSamplePnt[m_pDoc->m_fcFaceAry[ser].vn[i]];
	
	PNT v1, v2, v;
	PNT n1, n2;
	for(i = 0; i < 3; i++)
	{
		v = pt - p[i];
		v1 = p[(i+1)%3] - p[i];
		v2 = p[(i+2)%3] - p[i];

		n1 = Cross(v,v1);
		n2 = Cross(v,v2);

		if(n1*n2 > 0) return 0;  //表征 cos <n1 , n2> >0 夹角为锐角
	}

	return 1;

}


//------------------------------------------------------------------------
//Function: Calculate the cross product of the vectors p1 and p2
PNT CPara::Cross(PNT p1, PNT p2)
{
	PNT r;

	r.x = p1.y*p2.z - p1.z*p2.y;
	r.y = p1.z*p2.x - p1.x*p2.z;
	r.z = p1.x*p2.y - p1.y*p2.x;

	return r;
}


//----------------------------------------------------------------
//function: Get the sign of m
int CPara::Sign(double m)
{
	if(m > 0)
		return 1;
	else if(m < 0)
		return -1;
	else
		return 0;

}


//--------------------------------------------------------------------
//Function: Check whether the voxels are validation  
bool CPara::CalcIsoParam()
{
	//Initialize the state of the .prcd bit of the m_vxlVoxel
	int i;
	for(i = 0; i < m_vxlVoxel.GetSize(); i++)
		m_vxlVoxel[i].prcd = 0;
	
	CnstUParaTopToBotm();
	CnstUParaBotmToTop();
	CnstVParaLftToRgt();
	CnstVParaRgtToLft();
	return 1;

}



bool CPara::CnstUParaTopToBotm()
{
	return 1;

}

bool CPara::CnstUParaBotmToTop()
{
	return 1;

}

bool CPara::CnstVParaLftToRgt()
{
	return 1;

}

bool CPara::CnstVParaRgtToLft()
{
	return 1;

}



//--------------------------------------------------------------
//Function: the main interface of the class
void CPara::Run()
{
	CalcCrnrBndbx();
	DvdBndbx();
	DetmFeatrVxl();
	DetmFourBorders();
	CalcIsoParam();// 开始参数化划分。。。
	SaveDivid();
	SavetoDoc();
// 
// 	DIVPARA div;
// 	div.m_isoIsoUParaBtoT=m_isoIsoUParaBtoT;
// 	div.m_isoIsoUParaTtoB=m_isoIsoUParaTtoB;
// 	div.m_isoIsoVParaLtoR=m_isoIsoVParaLtoR;
// 	div.m_isoIsoVParaRtoL=m_isoIsoVParaRtoL;
// 	return div;
}


//-------------------------------------------------------------------
//Function: Calculate the serial of the voxel the point pt lies in
void CPara::CalcVxlSerPntLieIn(PNT pt, int ser[])
{
	float i, j, k;
	i = (pt.x - m_bbBndBox.xmin) / m_dVoxSize;
	j = (pt.y - m_bbBndBox.ymin) / m_dVoxSize;
	k =(pt.z - m_bbBndBox.zmin) / m_dVoxSize;

	ser[0] = floor(i); ser[1] =floor(j); ser[2] =  floor(k);

}




//-----------------------------------------------------------------
//Function: Reverse the sequence of elements in the array
void CPara::ReverseSequence(CArray<int,int>& brd)
{
	CArray<int,int> rbrd;
	int i,num;
	num = brd.GetSize();

	for(i = 0; i < num; i++)
		rbrd.Add(brd[num-1-i]);

	brd.RemoveAll();

	for(i = 0; i < num; i++)
		brd.Add(rbrd[i]);

}


//-----------------------------------------------------------------------
//Function: Check wether the point has been in the existing border
bool CPara::NewElemInExistBorder(int ser, ISOPARA isop, int ends)
{
	if(ser == ends) return 0;

	int num, i;
	num = isop.iso.GetSize();
	for(i = 0; i < num; i++)
		if(ser == isop.iso[i]) return 1;

	return 0;

}


//------------------------------------------------------------------------
//Function: Look for the second voxel adjacent to the first voxel
/*int CPara::LkfrSecdBorderVxl(int s)
{
	PNT curcent, adjc;
	int ser, serc[3];
	curcent = m_vxlVoxel[s].centr;

	//check whether the voxels adjacent to the face of the current 
	//voxel are unprocessed border voxel
	int i, j, k;
	int bser, bnum = 0;
	for(k = -1; k <= 1; k++)
		for(j = -1; j <= 1; j++)
			for(i = -1; i <= 1; i++)
			{
				if((int)(fabs(i) + fabs(j) + fabs(k)) == 1)
				{
					adjc.x = curcent.x + i*m_dVoxSize;
					adjc.y = curcent.y + j*m_dVoxSize;
					adjc.z = curcent.z + k*m_dVoxSize;

					CalcVxlSerPntLieIn(adjc,serc);
					ser = Index(serc[0],serc[1],serc[2]);
					if(m_vxlVoxel[ser].sgn == 2 && m_vxlVoxel[ser].prcd == 0)
						return ser;
				}
			}
			

	//check whether the voxels adjacent to the edge of the current 
	//voxel are unprocessed border voxel
	bnum = 0;
	for(k = -1; k <= 1; k++)
		for(j = -1; j <= 1; j++)
			for(i = -1; i <= 1; i++)
			{
				if((int)(fabs(i) + fabs(j) + fabs(k)) == 2)
				{
					adjc.x = curcent.x + i*m_dVoxSize;
					adjc.y = curcent.y + j*m_dVoxSize;
					adjc.z = curcent.z + k*m_dVoxSize;

					CalcVxlSerPntLieIn(adjc,serc);
					ser = Index(serc[0],serc[1],serc[2]);
					if(m_vxlVoxel[ser].sgn == 2 && m_vxlVoxel[ser].prcd == 0)
						return ser;
				}
			}
	
	return -1;
}*/



//----------------------------------------------------------------------------
//Function: Determine the border voxel of the surface
bool CPara::DetmFourBorders()
{
	//clear the four borders
	m_isoIsoUParaTtoB.RemoveAll();
	m_isoIsoUParaBtoT.RemoveAll();
	m_isoIsoVParaLtoR.RemoveAll();
	m_isoIsoVParaRtoL.RemoveAll();

	//Construct the four borders
	int col, row, i, sr, br;
	row = m_pDoc->m_iRowOfSamplePnt;
	col = m_pDoc->m_iColOfSamplePnt;

	CArray<int,int> leftb, rightb, topb, botmb;
	for(i = 0; i < row; i++) //left border
		leftb.Add(i);
	
	sr = (col-1)*row;
	for(i = 0; i < row; i++) //right border
		rightb.Add(sr+i);
	
	for(i = 0; i < col; i++) //top border
		topb.Add(i*row);
	
	br = row-1;
	for(i = 0; i < col; i++) //bottom border
		botmb.Add(i*row + br);

	//construct the border composed by border voxels
//	int endv; 
		int vs, ser[3], ver;
//	PNT sp,ep;
	PNT cp;
	bool b;
	
	CArray<int,int> brd;//缓冲
	ISOPARA ip;//缓存

	//the left border
	
	ver = 0;
	cp = m_pDoc->m_pntSamplePnt[leftb[ver]];
	b = 1;
	while(b)
	{
		CalcVxlSerPntLieIn(cp,ser);
		vs = Index(ser[0],ser[1],ser[2]);
		m_vxlVoxel[vs].sgn = 2;
		m_vxlVoxel[vs].vpara[0] = 0;
		if(!NewElemInExistBorder(vs,brd))
			brd.Add(vs);
		b = CalcNxtPntInBorder(ver,cp,leftb);
	}
	ip.iso.Copy(brd);
	m_isoIsoVParaLtoR.Add(ip);
	m_isoIsoVParaLtoR[0].iso.Copy(brd);
	brd.RemoveAll();
	ip.iso.RemoveAll();


	//the right border
	
	ver = 0;
	cp = m_pDoc->m_pntSamplePnt[rightb[0]];
	b = 1;
	while(b)
	{
		CalcVxlSerPntLieIn(cp,ser);
		vs = Index(ser[0],ser[1],ser[2]);
		m_vxlVoxel[vs].sgn = 2;
		m_vxlVoxel[vs].vpara[1] = 0;
		if(!NewElemInExistBorder(vs,brd))
			brd.Add(vs);
		b = CalcNxtPntInBorder(ver,cp,rightb);
	}
	ip.iso.Copy(brd);
	m_isoIsoVParaRtoL.Add(ip);
	m_isoIsoVParaRtoL[0].iso.Copy(brd);
	brd.RemoveAll();
	ip.iso.RemoveAll();
	//the top border
	
	ver = 0;
	cp = m_pDoc->m_pntSamplePnt[topb[0]];
	b = 1;
	while(b)
	{
		CalcVxlSerPntLieIn(cp,ser);
		vs = Index(ser[0],ser[1],ser[2]);
		m_vxlVoxel[vs].sgn = 2;
		m_vxlVoxel[vs].upara[0] = 0;
		if(!NewElemInExistBorder(vs,brd))
			brd.Add(vs);
		b = CalcNxtPntInBorder(ver,cp,topb);
	}
	ip.iso.Copy(brd);
	m_isoIsoUParaTtoB.Add(ip);
	m_isoIsoUParaTtoB[0].iso.Copy(brd);
	brd.RemoveAll();
	ip.iso.RemoveAll();
	//the bottom border
	
	ver = 0;
	cp = m_pDoc->m_pntSamplePnt[botmb[0]];
	b = 1;
	while(b)
	{
		CalcVxlSerPntLieIn(cp,ser);
		vs = Index(ser[0],ser[1],ser[2]);
		m_vxlVoxel[vs].sgn = 2;
		m_vxlVoxel[vs].upara[1] = 0;
		if(!NewElemInExistBorder(vs,brd))
			brd.Add(vs);
		b = CalcNxtPntInBorder(ver,cp,botmb);
	}
	ip.iso.Copy(brd);
	m_isoIsoUParaBtoT.Add(ip);
	m_isoIsoUParaBtoT[0].iso.Copy(brd);
	brd.RemoveAll();
	ip.iso.RemoveAll();
	return 1;
}


//------------------------------------------------------------------------
//Function: Calculate the next point in the border. If the ser > the size 
//          of the array brd, return 0.
bool CPara::CalcNxtPntInBorder(int &ser, PNT &pt, CArray<int,int>& brd)
{

	int num = brd.GetSize();

	if(ser == num-1) return 0;

	double step = m_dVoxSize / 2;

	PNT nxp, dv, cp;
	double dis, norm;

	nxp = m_pDoc->m_pntSamplePnt[brd[ser+1]];
	dv = nxp - pt;
	dis = sqrt(dv*dv);
	dv = dv / dis;   //normalize

	if(dis > step)
		pt = pt + dv*step;
	else
	{
		if(ser + 1 == num -1){
			pt = m_pDoc->m_pntSamplePnt[brd[num-1]];
			ser = ser + 1;
		}
		else	{
			step = step - dis;
			ser = ser + 1;
			cp = m_pDoc->m_pntSamplePnt[brd[ser]];
			nxp = m_pDoc->m_pntSamplePnt[brd[ser+1]];
			dv = nxp - cp;
			norm = sqrt(dv*dv);
			dv = dv / norm;  //normalize
			pt = cp + dv*step;
		}
	}
	return 1;
}



//-----------------------------------------------------------------------
//Function: Check whether the element with serial ser is in the array brd
bool CPara::NewElemInExistBorder(int ser, CArray<int,int>& brd)
{
	int num = brd.GetSize();
	
	if(num == 0) return 0;

	int i;

	for(i = 0; i < num; i++)
	{
		if(ser == brd[i]) return 1;
	}

	return 0;
	

}

void CPara::SaveDivid(void)
{
	
	//CFile *fp;
	CString name, ext;
// 	//get the extention name of the file
// 	name = fp->GetFileName();
// 	name.MakeReverse();
// 	ext = name.SpanExcluding(".");
// 	ext.MakeReverse();
// 	name.MakeReverse();

	FILE *stream;
	stream=_tfopen(L"Result.txt",L"w+");
	if (!stream==NULL)	{ fseek(stream,0,SEEK_SET); }

	int j,cou;
	//double x,y,z;
	fprintf(stream,"ptVxlArySize\t %lf\t%lf\t%lf\n",m_ptVxlArySize.x,m_ptVxlArySize.y,m_ptVxlArySize.z);//第一行 
 	fprintf(stream,"vxlVoxel\t%d\n",m_vxlVoxel.GetSize());//第2行
//  	fprintf(stream,"%d\t%d\t%d\t%d\n",
//		m_isoIsoUParaBtoT[0].iso.GetSize(), 
// 		m_isoIsoUParaTtoB[0].iso.GetSize(),
// 		m_isoIsoVParaLtoR[0].iso.GetSize(), 
// 		m_isoIsoVParaRtoL[0].iso.GetSize());
	
		fprintf(stream,"m_isoIsoUParaBtoT\t%d\t\n",m_isoIsoUParaBtoT[0].iso.GetSize());
		for (j=0;j<m_isoIsoUParaBtoT[0].iso.GetSize();j++){
			cou=m_isoIsoUParaBtoT[0].iso.ElementAt(j);
			PNT vex;
			vex=m_vxlVoxel.ElementAt(cou).centr;
			fprintf(stream,"%lf\t%lf\t%lf\n",vex.x,vex.y,vex.z);
			m_pDoc->m_U_BtoT.Add(vex);
		}
		fprintf(stream,"\n");

		fprintf(stream,"m_isoIsoUParaTtoB\t%d\t\n",m_isoIsoUParaTtoB[0].iso.GetSize());
		for (j=0;j<m_isoIsoUParaTtoB[0].iso.GetSize();j++)	{
			cou=m_isoIsoUParaTtoB[0].iso.ElementAt(j);
			PNT vex;
			vex=m_vxlVoxel.ElementAt(cou).centr;
			fprintf(stream,"%lf\t%lf\t%lf\n",vex.x,vex.y,vex.z);
			m_pDoc->m_U_TtoB.Add(vex);
		}
		fprintf(stream,"\n");

	fprintf(stream,"m_isoIsoVParaLtoR\t%d\t\n",m_isoIsoVParaLtoR[0].iso.GetSize());
		for (j=0;j<m_isoIsoVParaLtoR[0].iso.GetSize();j++)	{
			cou=m_isoIsoVParaLtoR[0].iso.ElementAt(j);
			PNT vex;
			vex=m_vxlVoxel.ElementAt(cou).centr;
			fprintf(stream,"%lf\t%lf\t%lf\n",vex.x,vex.y,vex.z);
			m_pDoc->m_V_LtoR.Add(vex);
		}
		fprintf(stream,"\n");

		fprintf(stream,"m_isoIsoVParaRtoL\t%d\t\n",m_isoIsoVParaRtoL[0].iso.GetSize());
		for (j=0;j<m_isoIsoVParaRtoL[0].iso.GetSize();j++)	{
			cou=m_isoIsoVParaRtoL[0].iso.ElementAt(j);
			PNT vex;
			vex=m_vxlVoxel.ElementAt(cou).centr;
			fprintf(stream,"%lf\t%lf\t%lf\n",vex.x,vex.y,vex.z);
			this->m_pDoc->m_V_RtoL.Add(vex);
		}
		fprintf(stream,"\n");
	
	fclose(stream);
 	MessageBox(NULL,L"Data Saved",L"SaveData",MB_OK);
}

void CPara::SavetoDoc()
{
// 	m_pDoc->m_isoIsoUParaBtoT.Copy(m_isoIsoUParaBtoT);
// 	m_pDoc->m_isoIsoUParaTtoB.Copy(m_isoIsoUParaTtoB);
// 	m_pDoc->m_isoIsoVParaLtoR.Copy(m_isoIsoVParaLtoR);
// 	m_pDoc->m_isoIsoVParaRtoL.Copy(m_isoIsoVParaRtoL);
	m_pDoc->m_vxlVoxel.Copy(m_vxlVoxel);
	m_pDoc-> m_ptVxlArySize=m_ptVxlArySize;
	m_pDoc->m_dVoxSize=m_dVoxSize;	
}
