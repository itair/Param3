
// Param3Doc.cpp : CParam3Doc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Param3.h"
#endif

#include "Param3Doc.h"
#include "Para.h"
//#include <string>
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CParam3Doc

IMPLEMENT_DYNCREATE(CParam3Doc, CDocument)

BEGIN_MESSAGE_MAP(CParam3Doc, CDocument)
END_MESSAGE_MAP()


// CParam3Doc ����/����

CParam3Doc::CParam3Doc()
{
	// TODO: �ڴ����һ���Թ������

}

CParam3Doc::~CParam3Doc()
{
}

BOOL CParam3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CParam3Doc ���л�

void CParam3Doc::Serialize(CArchive& ar)
{
	CFile *fp;
	CString name, ext;
	char* namechar;
	fp = ar.GetFile();
	
	//get the extention name of the file
	name = fp->GetFileName();
	name.MakeReverse();
	ext = name.SpanExcluding(L".");
	ext.MakeReverse();
	name.MakeReverse();
	//string tp; 
	//wchar_t   *s; 
	//char   *d; 
	//tp=name; 
	//namechar=tp.c_str(); 
	
	//�������⣬������������ĸ�ʽ 
	if (ar.IsStoring())
	{
		// TODO: add storing code here
// 		if (ext=="txt")
// 		{
// 			FILE *stream;
// 			char* namechar;
// 			name=name.SpanExcluding(L".");
// 			name.Append(L"_results.txt");    // add "_results" at end of filename.
// 			namechar=(LPSTR)(LPCTSTR)name;
// 			stream=fopen(namechar,"w");		
// 			if (!stream==NULL)	{ fseek(stream,0,SEEK_SET); }
// 			//	fwrite("woleigequtesttest",strlen("woleigequtesttest"),1,stream);
// 		//	int data=221;
// 		//	fprintf(stream,"%d\t %d\n",data,data);		
// 		//	fflush(stream);
// //////////////////���ո�ʽ��ȡ����
// 			SaveData(stream);
// 		    fclose(stream);
// 		    MessageBox(NULL,L"Data Saved",L"SaveData",MB_OK);
// 			}
	}
	else
	{
		// TODO: add loading code here
// 		if (ext=="txt")		{
// 		FILE *stream;
// 			 if(fp->Open(name,CFile::modeRead,NULL)==FALSE) {
// 				 MessageBox(NULL,L"cannot open this file",L"FAULT",MB_OK);
// 			 }
// 			 else {
// 				 if (this->m_IsUpdata==true) {
// 					 this->m_pntSamplePnt.RemoveAll();
// 					 this->m_egEdgeAry.RemoveAll();
// 					 this->m_fcFaceAry.RemoveAll();
// 					 this->m_iColOfSamplePnt=0;
// 					 this->m_iRowOfSamplePnt=0;
// 					 this->m_IsUpdata=false;
// 				 }
// 				 else {
// 				//	 stream=fopen(namechar,"r");
// 				//	 fseek(stream,0,SEEK_SET);
// 					 
// 
// 
// 					 ReadData(stream);
// 					 fclose(stream);
// 					 MessageBox(NULL,L"Date Read",L"ReadData",MB_OK);
// 					 this->m_IsUpdata=true;// data update
// 				 }
// 			 }			
// 		}
 	}
// 	CPara pa(this);// ��ʼ���ڴ�����
 }

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CParam3Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;


	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CParam3Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CParam3Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CParam3Doc ���

#ifdef _DEBUG
void CParam3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CParam3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CParam3Doc ����
//void CParam3Doc::ReadData(FILE *stream)
//{
//	//FILE *str;
//	int i,v,e,f,row,col;
//	//double *data;
//	fscanf(stream,"%d %d %d %d %d\n",&v,&e,&f,&row,&col);
//			//getchar();
//
//	/////////read Sample Points
//	m_iRowOfSamplePnt = row; //�߽���
//	m_iColOfSamplePnt = col;// �߽���
//	
//	PNT pt;
//	for ( i=0 ; i<v ; i++)
//	{
//		fscanf(stream,"%lf\t%lf\t%lf\n", &pt.x,&pt.y,&pt.z);
//		m_pntSamplePnt.Add(pt);
//	}
//	////////////////read FreeEdges
//	int v_1=0;
//	int v_2=0;
//	EDGE ed;
//
//	for (i=1;i<=e;i++)
//	{	
//		fscanf(stream,"%d\t%d\n",&v_1,&v_2);
//		ed.vn[0]=v_1-1;
//		ed.vn[1]=v_2-1;
//		m_egEdgeAry.Add(ed);
//	}
//	//////////////read FreeFaces
//	 v_1=0;
//	 v_2=0;
//	int	v_3=0;
//	FACE fe;
//	for (i=1 ; i<=f ; i++)
//	{		
//		fscanf(stream,"%d\t%d\t%d\n",&v_1,&v_2,&v_3);
//		fe.vn[0]=v_1-1;
//		fe.vn[1]=v_2-1;
//		fe.vn[2]=v_3-1;
//		m_fcFaceAry.Add(fe);
//	}
//}


//void CParam3Doc::SaveData(FILE *stream)
//{
//	int i,v,e,f;
//	v=m_pntSamplePnt.GetSize();
//	f=m_fcFaceAry.GetSize();
//	e=m_egEdgeAry.GetSize();
//	
//	fprintf(stream,"%d\t%d\t%d\t%d\t%d\n",v,f,e,m_iRowOfSamplePnt,m_iColOfSamplePnt);//��һ�� 
//	
//	for (i=0;i<v;i++)  //��
//	{ 
//		fprintf(stream,"%lf\t%lf\t%lf\n",m_pntSamplePnt.ElementAt(i).x,m_pntSamplePnt.ElementAt(i).y,m_pntSamplePnt.ElementAt(i).z);
//	}
//
//	for (i=0;i<e;i++)//��
//	{
//		fprintf(stream,"%d\t%d\n",m_egEdgeAry.ElementAt(i).vn[0],m_egEdgeAry.ElementAt(i).vn[1]);
//	}
//
//	for (i=0;i<f;i++)//��
//	{ 
//		fprintf(stream,"%d\t%d\t%d\n",m_fcFaceAry.ElementAt(i).vn[0],m_fcFaceAry.ElementAt(i).vn[1],m_fcFaceAry.ElementAt(i).vn[2]); 
//	}
//	
// 
// 	fprintf(stream,"%d\t%d\t%d\n",		m_ptVxlArySize.x,		m_ptVxlArySize.y,		m_ptVxlArySize.z,		);//��һ�� 
// 	fprintf(stream,"%d\n",		m_vxlVoxel.GetSize(),				);//��2��
// 	fprintf(stream,"%d\t%d\t%d\t%d\n",		m_isoIsoUParaBtoT.GetSize()	,
// 									m_isoIsoUParaTtoB.GetSize(),
// 									m_isoIsoVParaLtoR.GetSize(),
// 									m_isoIsoVParaRtoL.GetSize());
// 
// 
// 	for (i=1;i<this->m_isoIsoUParaBtoT.GetSize();i++)
// 	{
// 
// 	}
// 		
//	getchar();
//	
//}


PNT PNT::operator +(PNT p)
{
	PNT r;
	r.x = x + p.x;
	r.y = y + p.y;
	r.z = z + p.z;
	return r;
	
}

PNT PNT::operator -(PNT p)
{
	PNT r;
	r.x = x - p.x;
	r.y = y - p.y;
	r.z = z - p.z;
	return r;
	
}

double PNT::operator *(PNT p)
{
	return(x*p.x + y*p.y + z*p.z);
	
}

PNT PNT::operator /(double d)
{
	PNT r;
	r.x = x / d;
	r.y = y / d;
	r.z = z / d;
	return r;
	
}

bool PNT::operator ==(PNT p)
{
	if(x == p.x && y == p.y && z == p.z)
		return 1;
	else return 0;
	
}

PNT PNT::operator *(double m)
{
	PNT r;
	r.x = m*x; r.y = m*y; r.z = m*z;
	return r;
	
}



void CParam3Doc::ParamRun(void)
{
	CPara pa(this);
	pa.Run();
	//MessageBox("�������");
	return;
}


void CParam3Doc::SaveDivid(FILE *stream)
{
	int i,j;
	double x,y,z;
	fprintf(stream,"ptVxlArySize\t %lf\t%lf\t%lf\n",
		m_ptVxlArySize.x,m_ptVxlArySize.y,m_ptVxlArySize.z);//��һ�� 
	fprintf(stream,"vxlVoxel\t%d\n",m_vxlVoxel.GetSize());//��2��
	
	fprintf(stream,"m_isoIsoUParaBtoT\t%d\t\n",m_U_BtoT.GetSize());
	for (j=0;j<m_U_BtoT.GetSize();j++)	{
		fprintf(stream,"%lf\t%lf\t%lf\n",m_U_BtoT[j].x,m_U_BtoT[j].y,m_U_BtoT[j].z);		
	}
	fprintf(stream,"\n");

	fprintf(stream,"m_isoIsoUParaTtoB\t%d\t\n",m_U_TtoB.GetSize());
	for (j=0;j<m_U_TtoB.GetSize();j++)	{
		fprintf(stream,"%lf\t%lf\t%lf\n",m_U_TtoB[j].x,m_U_TtoB[j].y,m_U_TtoB[j].z);		
	}
	fprintf(stream,"\n");

	fprintf(stream,"m_isoIsoVParaLtoR\t%d\t\n",m_V_LtoR.GetSize());
	for (j=0;j<m_V_LtoR.GetSize();j++)	{
		fprintf(stream,"%lf\t%lf\t%lf\n",m_V_LtoR[j].x,m_V_LtoR[j].y,m_V_LtoR[j].z);		
	}
	fprintf(stream,"\n");

	fprintf(stream,"m_m_isoIsoVParaRtoL\t%d\t\n",m_V_RtoL.GetSize());
	for (j=0;j<m_V_RtoL.GetSize();j++)	{
		fprintf(stream,"%lf\t%lf\t%lf\n",m_V_RtoL[j].x,m_V_RtoL[j].y,m_V_RtoL[j].z);		
	}
	fprintf(stream,"\n");
	
	fclose(stream);
}
