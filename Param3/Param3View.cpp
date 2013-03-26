// Param3View.cpp : CParam3View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Param3.h"
#endif

#include "Param3Doc.h"
#include "Param3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParam3View

IMPLEMENT_DYNCREATE(CParam3View, CView)

BEGIN_MESSAGE_MAP(CParam3View, CView)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CHAR()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZING()
	ON_COMMAND(ID_TRIANGLE, &CParam3View::OnTriangle)
	ON_COMMAND(ID_PARAM, &CParam3View::OnParam)
	ON_COMMAND(ID_DIVID, &CParam3View::OnDivid)
	ON_COMMAND(ID_DIVID_SAVE, &CParam3View::OnDivsave)
	ON_COMMAND(ID_FITTING, &CParam3View::OnFitting)
	ON_COMMAND(ID_READDATA, &CParam3View::OnReaddata)
	//ON_UPDATE_COMMAND_UI(ID_DIVID_SAVE, &CParam3View::OnUpdateDivsave)
END_MESSAGE_MAP()

// CParam3View 构造/析构

CParam3View::CParam3View()
	: m_IsParaming(false)
{
	// TODO: 在此处添加构造代码
	m_xrot=0.0f;				// X Rotation
	m_yrot=0.0f;				// Y Rotation
	m_xspeed=0.0f;				// X Rotation Speed
	m_yspeed=0.0f;				// Y Rotation Speed
	m_zdeep=-3.0f;			// Depth Into The Screen
	m_scale=1.0f;
	m_IsMSG=false;
	m_IsNew=false;
	m_IsShown=false;
	m_IsPara=false;
	m_IsMesh=false;
	m_IsRender=false;
	m_IsText=false;
	m_bLButton=false;	
	m_IsParaming=false;
}

CParam3View::~CParam3View()
{
}

BOOL CParam3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style|=WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

// CParam3View 绘制

void CParam3View::OnDraw(CDC* /*pDC*/)
{
	CParam3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)return;
	// TODO: 在此处为本机数据添加绘制代码
	////////////////////////////////每次绘制之前旋转
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	if(m_IsMSG==false){
		glTranslatef(0.0f,0.0f,m_zdeep);
		glRotatef(m_xrot, 1.0f, 0.0f, 0.0f);
		glRotatef(m_yrot, 0.0f, 1.0f, 0.0f);
	}
	/////////////////////////////// 绘制 物体的主代码
	if (m_IsMSG)	{
		glPushMatrix();
	}
	if (m_IsShown==false){ // 空闲
		
		DrawNormalMeter();
		glColor3f(0.4f,0.2f,0.2f);
		glutWireCone(0.3,1.0,20.0 ,20.0 );
	}
	if ((m_IsMesh==true)&&(m_IsShown==true)) {
		glPushMatrix();
		DrawNormalMeter();
		OnTriangle();		
	}
	if ((m_IsParaming==true))	{
		glPushMatrix();
		DrawParaming();

	}
	if ((m_IsDivid==true)&&(m_IsPara==true)) 	{
		glPushMatrix();
		DrawNormalMeter();
		OnDivid();
	}
	glPopMatrix();
	glFlush();
	SwapBuffers(wglGetCurrentDC());
	////////////////////////////// 绘制 完毕
}


// CParam3View 诊断

#ifdef _DEBUG
void CParam3View::AssertValid() const
{
	CView::AssertValid();
}

void CParam3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CParam3Doc* CParam3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CParam3Doc)));
	return (CParam3Doc*)m_pDocument;
}
#endif //_DEBUG


// CParam3View 消息处理程序


void CParam3View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//初始化OpenGL
	//////////////////////////////////////////////////////
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
	//	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	//	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do//剪裁
	//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	//set material property1
	glNewList(0, GL_COMPILE);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat mat_ambient[]={0.15, 0.07, 0.04, 1.0};	 //原材料的环境颜色
	GLfloat mat_diffuse[]={0.15, 0.07, 0.04, 1.0};	 //原材料的散射颜色
	GLfloat mat_specular[]={0.15, 0.07, 0.04, 1.0};	 //原材料的反射颜色
	GLfloat mat_emission[]={0.15, 0.07, 0.04, 1.0};	 //原材料的发散颜色
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glEndList();
	
	//set light0
	glNewList(1, GL_COMPILE);
	glEnable(GL_LIGHTING);
	GLfloat light0_position[]={-1.0, 1.0, 2.0, 1.0};
	GLfloat light0_spot_direction[]={-1.0, -1.0, -1.0, 1.0};
	GLfloat light0_ambient[]={0.015, 0.007, 0.004, 1.0};
	GLfloat ligh0_diffuse[]={1.0, 1.0, 1.0, 1.0};
	GLfloat light0_specular[]={1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	//	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_spot_direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ligh0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glEnable(GL_LIGHT0);
	glEndList();
	/////////////////////////////////////////////////////
	
	glClearColor( 0.35f, 0.53f, 0.7f, 1.0f );//天蓝色背景
	glMatrixMode(GL_PROJECTION);	//
	glLoadIdentity();									// Reset The View
	gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	
	glCallList(0);
	glCallList(1);	
}


void CParam3View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	glCallList(1);
	
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0, (GLdouble)cx / (GLdouble)cy, 0.1, 100.0);
	// 	if (cx <= cy)  
	// 		gluOrtho2D (-1.0, 1.0,-1.0*(GLfloat)cy/(GLfloat)cx, 1.0*(GLfloat)cy/(GLfloat)cx);
	// 	else  
	// 		gluOrtho2D (-1.0*(GLfloat)cx/(GLfloat)cy, 1.0*(GLfloat)cx/(GLfloat)cy, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//m_IsMSG=false;
	//////////////////////////////////////////////////////
}


void CParam3View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (!m_IsShown)	return;
	CPaintDC pDC(this);
	//	m_xrot=m_yrot=0.0;
	m_bLButton=true;
	ptLastMousePosit = ptCurrentMousePosit = point;
	CView::OnLButtonDown(nFlags, point);
}


void CParam3View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (!m_IsShown)	return;
	m_bLButton=false;
	CView::OnLButtonUp(nFlags, point);
}


void CParam3View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (!m_IsShown)	return;
	CPaintDC pDC(this);
	ptCurrentMousePosit=point;
	if (m_bLButton==true)	{
		m_yrot+=(ptCurrentMousePosit.x-ptLastMousePosit.x);
		m_xrot+=(ptCurrentMousePosit.y-ptLastMousePosit.y);		
	}
	ptLastMousePosit=ptCurrentMousePosit;
	OnDraw(&pDC);
	CView::OnMouseMove(nFlags, point);
}


BOOL CParam3View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPaintDC pDC(this);
	m_zdeep+=(zDelta/120)*0.1f;
	m_scale+=(zDelta/120)*0.1f;
	OnDraw(&pDC);	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
	
}


void CParam3View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString ch;	
	CPaintDC pDC(this);
	switch(nChar)
	{
	case 0x41:
		ch='a';
		MessageBox(L"a");
		m_xspeed=0.0;
		m_yspeed=0.0;
		m_zdeep=0.0;
		m_scale=1.0;
		m_xrot=0.0;
		m_yrot=0.0;
		break;
	case VK_UP:
		m_xspeed-=0.02f;
		ch="up";
		//	MessageBox(ch,MB_OK);		
		break;
	case VK_DOWN:
		m_xspeed+=0.02f;
		ch="down";
		//		MessageBox(ch,MB_OK);
		
		break;
	case VK_LEFT:
		m_yspeed-=0.02f;
		ch="left";
		//MessageBox(ch,MB_OK);
		
		break;
	case VK_RIGHT:
		m_yspeed+=0.02f;
		ch="right";
		//	MessageBox(ch,MB_OK);
		
		break;
	case VK_PRIOR:
		m_zdeep+=0.1f;
		m_scale+=0.1f;		
		ch="PageUP";
		//	MessageBox(ch,MB_OK);
		break;
	case VK_NEXT:
		m_zdeep-=0.1f;
		m_scale-=0.1f;		
		ch="PageDown";
		//	MessageBox(ch,MB_OK);
		break;
	case VK_SPACE:
		m_xspeed=0.0;
		m_yspeed=0.0;
		//m_zdeep=0.0;
		//m_scale=1.0;
		break;
	default:
		m_xspeed=0.0;
		m_yspeed=0.0;
		//	m_zdeep=0.0;
		//m_scale=1.0;
		break;
	}
	m_xrot+=m_xspeed;
	m_yrot+=m_yspeed;
	OnDraw(&pDC);
	return;
	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CParam3View::OnDestroy()
{
	CView::OnDestroy();
	

	// TODO: 在此处添加消息处理程序代码
}


int CParam3View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO:  在此添加您专用的创建代码
	HDC hDC;
	HGLRC hRC;
	hDC=::GetDC(GetSafeHwnd());
	SetupPixelFormat(hDC);
	hRC=wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	
	return 0;
}


// 显示屏幕提示
void CParam3View::DrawText(const char* text, float x, float y)
{
	bool isDepthOpen = false;
	bool isStencilOpen = false;
	bool isLightOpen = false;
	bool isFogOpen = false;
	
	if(glIsEnabled(GL_DEPTH_TEST))  {
		isDepthOpen = true;
		glDisable(GL_DEPTH_TEST);
	}
	if(glIsEnabled(GL_STENCIL_TEST))  {
		isStencilOpen = true;
		glDisable(GL_STENCIL_TEST);
	}
	if(glIsEnabled(GL_LIGHTING))   {
		isLightOpen = true;
		glDisable(GL_LIGHTING);
	}
	if(glIsEnabled(GL_FOG))  {
		isFogOpen = true;
		glDisable(GL_FOG);
	}
	
	int font = (int)GLUT_BITMAP_8_BY_13;     
	glColor3f(1.0, 1.0, 0.0);  // 设置字体颜色
	glMatrixMode(GL_PROJECTION);	// 设置正投影 
	//获取当前绘制区尺寸
	CRect rect;
	GetClientRect(&rect);
	int g_WindowWidth = rect.Width(); 
	int g_WindowHeight = rect.Height();
	//  保存当前投影矩阵
	glPushMatrix();		
	glLoadIdentity();
	gluOrtho2D( 0, g_WindowWidth, 0, g_WindowHeight );
	// 反转Y轴（朝下为正方向）(与窗口坐标一致)
	glScalef(1, -1, 1);
	// 将原点移动到屏幕左上方(与窗口坐标一致)
	glTranslatef(0, -g_WindowHeight, 0);
	glMatrixMode(GL_MODELVIEW);         
	//  保存当前模型视图矩阵
	glPushMatrix();
	glLoadIdentity();             
	//  设置文字位置
	glRasterPos2f( x, y );             
	//  依次描绘所有字符(使用显示列表)
	for(const char* c = text; *c != '\0'; c++)        {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
	}	
	// 恢复之前保存的模型视图矩阵
	glPopMatrix();	
	glMatrixMode(GL_PROJECTION);	
	// 恢复之前保存的投影矩阵
	glPopMatrix();	
	// 返回模型视图矩阵状态
	glMatrixMode(GL_MODELVIEW);	
	// 恢复OpenGL状态
	if(isDepthOpen) 	glEnable(GL_DEPTH_TEST);
	if(isStencilOpen)	glEnable(GL_STENCIL_TEST);
	if(isLightOpen) 	glEnable(GL_LIGHTING);
	if(isFogOpen)		glEnable(GL_FOG);
}


bool CParam3View::SetupPixelFormat(HDC hDC)
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
			1, // version number
			PFD_DRAW_TO_WINDOW | // support window
			PFD_SUPPORT_OPENGL | // support OpenGL
			PFD_DOUBLEBUFFER, // double buffered
			PFD_TYPE_RGBA, // RGBA type
			24, // 24-bit color depth
			0, 0, 0, 0, 0, 0, // color bits ignored
			0, // no alpha buffer
			0, // shift bit ignored
			0, // no accumulation buffer
			0, 0, 0, 0, // accum bits ignored
			32, // 32-bit z-buffer
			0, // no stencil buffer
			0, // no auxiliary buffer
			PFD_MAIN_PLANE, // main layer
			0, // reserved
			0, 0, 0 // layer masks ignored
	};
	int pixelformat;
	
	//分配一个像素格式号
	if ( (pixelformat = ChoosePixelFormat(hDC, &pfd)) == 0 )	{
		MessageBox(L"ChoosePixelFormat failed",NULL,MB_OK);
		return FALSE;
	}	
	//设置为当前的像素格式
	if (SetPixelFormat(hDC, pixelformat, &pfd) == FALSE)	{
		MessageBox(L"SetPixelFormat failed");
		return FALSE;
	}
	return TRUE;
}


BOOL CParam3View::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString ClassName=AfxRegisterWndClass(CS_OWNDC);
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CParam3View::OnSizing(UINT fwSide, LPRECT pRect)
{
	CView::OnSizing(fwSide, pRect);

	// TODO:	 在此处添加消息处理程序代码
	m_IsMSG=false;
	}


void CParam3View::OnTriangle()
{
	// TODO:	 在此添加命令处理程序代码
	CParam3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	m_pDoc=pDoc;
	
	if (m_IsNew==false) {
		MessageBox(L"Data not changed.");
		return;
	}
	//------------------------------绘图---------------------------s-
	glPointSize(2.0f);
	glColor3f(1.0, 1.0, 1.0);					//绘制颜色大红

	glBegin(GL_POINTS);
	long cou=0;
	for(cou=0;cou<(m_pDoc->m_pntSamplePnt.GetSize());cou++)	{
		glVertex3d(	m_pDoc->m_pntSamplePnt[cou].x,
					m_pDoc->m_pntSamplePnt[cou].y,
					m_pDoc->m_pntSamplePnt[cou].z);		
	}	
	glEnd();	

	glColor3f(0.0f, 1.0f, 1.0f);	// 青色划线
	//glutWireTeapot(0.5);
	long count=0;
	CArray<PNT,PNT> CurPnt;
	int num=0;
	for(count=0;count<(m_pDoc->m_fcFaceAry.GetSize());count++)
	{	
		glPolygonMode( GL_FRONT_AND_BACK, GL_POINTS );
		glBegin(GL_LINE_LOOP);
		for (int i=0;i<3;i++){
			num=m_pDoc->m_fcFaceAry[count].vn[i];			
			if (num>m_pDoc->m_pntSamplePnt.GetSize() || num <0){			
				MessageBox(L"wrong PNT numbers");
				m_IsShown=false;
				m_IsMesh=false;
				CurPnt.RemoveAll();
				//Invalidate();
				return;
			}
			CurPnt.Add(m_pDoc->m_pntSamplePnt[num]);						
			glVertex3d(CurPnt[3*count+i].x,CurPnt[3*count+i].y,CurPnt[3*count+i].z);		
		}
		glEnd();
		m_IsShown=true;
		m_IsMesh=true;
	}
	CurPnt.RemoveAll();
	glFlush();	
	//-----------------------------绘图结束--------------------------
	   //刷新onDraw
}


void CParam3View::OnParam()
{
	// TODO: 在此添加命令处理程序代码
	if ( (m_IsPara==true)||(m_IsNew==false) ) return;
	CParam3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CPaintDC dc(this);	
	m_pDoc=pDoc;
	m_IsMesh=false;	
	m_IsParaming=true;
	//Invalidate();
	OnDraw(&dc);
	m_pDoc->ParamRun();
	AfxMessageBox(L"参数化完成.");
	m_IsPara=true;
	m_IsParaming=false;	
	m_IsMesh=true;
	OnDraw(&dc);
	return;
		 
}


void CParam3View::OnDivid()
{
	// TODO: 在此添加命令处理程序代码
	CParam3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	m_pDoc=pDoc;

	if (m_IsPara==false) {
		MessageBox(L"Data not changed.");
		return;
	}
	//------------------------------绘图---------------------------s-
	//------------绘制边框特征体元-------
	glPointSize(4.0f);
	glColor3f(1.0, 0.0, 0.0);					//绘制颜色大红

	glBegin(GL_POINTS);
	 for(long cou=0; cou<(m_pDoc->m_U_BtoT.GetSize()); cou++)	{
		glVertex3f(	m_pDoc->m_U_BtoT[cou].x,
				m_pDoc->m_U_BtoT[cou].y,
				m_pDoc->m_U_BtoT[cou].z);		
	}	
	for (long cou=0; cou<(m_pDoc->m_U_TtoB.GetSize()); cou++)	{
		glVertex3f(	m_pDoc->m_U_TtoB[cou].x,
			m_pDoc->m_U_TtoB[cou].y,
			m_pDoc->m_U_TtoB[cou].z);		
	}
	for (long cou=0; cou<(m_pDoc->m_V_LtoR.GetSize()); cou++)	{
		glVertex3f(	m_pDoc->m_V_LtoR[cou].x,
			m_pDoc->m_V_LtoR[cou].y,
			m_pDoc->m_V_LtoR[cou].z);		
	}
	for (long cou=0; cou<(m_pDoc->m_V_RtoL.GetSize()); cou++)	{
		glVertex3f(	m_pDoc->m_V_RtoL[cou].x,
			m_pDoc->m_V_RtoL[cou].y,
			m_pDoc->m_V_RtoL[cou].z);		
	}
	glEnd();	
//---------------------绘制内部特征体元-------------
	glColor3f(1.0, 0.0, 0.0);	//绘制颜色大红
	glBegin(GL_POINTS);
	for (long cou=0; cou <m_pDoc->m_vxlVoxel.GetSize(); cou++){
		if (m_pDoc->m_vxlVoxel[cou].sgn==1 )
			glVertex3f(m_pDoc->m_vxlVoxel[cou].centr.x,
						m_pDoc->m_vxlVoxel[cou].centr.y,
						m_pDoc->m_vxlVoxel[cou].centr.z	);
	}
	glEnd();


// 	glColor3f(1.0f, 1.0f, 0.0f);	// 青色划线
// 	//glutWireTeapot(0.5);
// 	long count=0;
// 	CArray<PNT,PNT> CurPnt;
// 	int num=0;
// 	for(count=0;count<(m_pDoc->m_fcFaceAry.GetSize());count++)
// 	{	
// 		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
// 		glBegin(GL_LINE_LOOP);
// 		for (int i=0;i<3;i++){
// 			num=m_pDoc->m_fcFaceAry[count].vn[i];			
// 			if (num>m_pDoc->m_pntSamplePnt.GetSize() || num <0){			
// 				MessageBox(L"wrong PNT numbers");
// 				m_IsShown=false;
// 				m_IsMesh=false;
// 				CurPnt.RemoveAll();
// 				//Invalidate();
// 				return;
// 			}
// 			CurPnt.Add(m_pDoc->m_pntSamplePnt[num]);						
// 			glVertex3d(CurPnt[3*count+i].x,CurPnt[3*count+i].y,CurPnt[3*count+i].z);		
// 		}
// 		glEnd();
// 	}
//	CurPnt.RemoveAll();
	m_IsDivid=true;
	glFlush();	
	//-----------------------------绘图结束--------------------------
	//刷新onDraw

}


void CParam3View::OnDivsave()
{
	// TODO: 在此添加命令处理程序代码
	//static char BASED_CODE szOpenFilter[] = " Chart Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CString sFileName; 
	CFileDialog FileDlg( FALSE ,
		NULL,
		sFileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L" TXT文件 (*.txt)|*.txt|All Files (*.*)|*.*||");
	if( FileDlg.DoModal()==IDOK )	{  
		sFileName=FileDlg.m_ofn.lpstrFile;  
		FILE* stream = _tfopen((LPCTSTR)sFileName,_T("wt"));		
		CParam3Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		m_pDoc=pDoc;
		m_pDoc->SaveDivid(stream);
		AfxMessageBox(sFileName+"保存");
		/////////////保存代码
// 		int i,v,e,f;
// 		v=m_pDoc->m_pntSamplePnt.GetSize();
// 		f=m_pDoc->m_fcFaceAry.GetSize();
// 		e=m_pDoc->m_egEdgeAry.GetSize();
// 
// 		fprintf(stream,"%d\t%d\t%d\t%d\t%d\n",
// 			v,f,e,m_pDoc->m_iRowOfSamplePnt,m_pDoc->m_iColOfSamplePnt); //第一行 
// 		//点
// 		for (i=0;i<v;i++) { 
// 			fprintf(stream,"%lf\t%lf\t%lf\n",
// 				m_pDoc->m_pntSamplePnt.ElementAt(i).x,
// 				m_pDoc->m_pntSamplePnt.ElementAt(i).y,
// 				m_pDoc->m_pntSamplePnt.ElementAt(i).z);
// 		}
// 		//边
// 		for (i=0;i<e;i++){
// 			fprintf(stream,"%d\t%d\n",
// 				m_pDoc->m_egEdgeAry.ElementAt(i).vn[0],
// 				m_pDoc->m_egEdgeAry.ElementAt(i).vn[1]);
// 		}
// 		//面
// 		for (i=0;i<f;i++){ 
// 			fprintf(stream,"%d\t%d\t%d\n",
// 				m_pDoc->m_fcFaceAry.ElementAt(i).vn[0],
// 				m_pDoc->m_fcFaceAry.ElementAt(i).vn[1],
// 				m_pDoc->m_fcFaceAry.ElementAt(i).vn[2]); 
		
		//m_IsNew=true;
		//Invalidate();
	}//if
}


void CParam3View::OnFitting()
{
	// TODO: 在此添加命令处理程序代码
}


void CParam3View::OnReaddata()  //读取对话框
{
	// TODO: 在此添加命令处理程序代码
	//static char BASED_CODE szOpenFilter[] = " Chart Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CString sFileName; 
	CFileDialog FileDlg( TRUE ,
						NULL,
						sFileName,
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						L"TXT文件数据集 (*.txt)|*.txt|All Files (*.*)|*.*||");
	if( FileDlg.DoModal()==IDOK )	{  
		sFileName=FileDlg.m_ofn.lpstrFile;  
		AfxMessageBox(sFileName+"读取");
		FILE* stream = _tfopen((LPCTSTR)sFileName,_T("rt"));		
		
		CParam3Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		m_pDoc=pDoc;
		int i,v,e,f,row,col;
		//读取数据集参数
		fscanf(stream,"%d %d %d %d %d\n",&v,&e,&f,&row,&col);
				/////////read Sample Points
		m_pDoc->m_iRowOfSamplePnt = row; //边界行
		m_pDoc->m_iColOfSamplePnt = col;// 边界列
		//读取点
		PNT pt;
		for ( i=0 ; i<v ; i++){
			fscanf(stream,"%lf\t%lf\t%lf\n", &pt.x,&pt.y,&pt.z);
			m_pDoc->m_pntSamplePnt.Add(pt);
		}
		////////////////read FreeEdges
		int v_1=0;
		int v_2=0;
		EDGE ed;

		for (i=1;i<=e;i++){	
			fscanf(stream,"%d\t%d\n",&v_1,&v_2);
			ed.vn[0]=v_1-1;
			ed.vn[1]=v_2-1;
			m_pDoc->m_egEdgeAry.Add(ed);
		}
		//////////////read FreeFaces
		v_1=0;
		v_2=0;
		int	v_3=0;
		FACE fe;
		for (i=1 ; i<=f ; i++){		
			fscanf(stream,"%d\t%d\t%d\n",&v_1,&v_2,&v_3);
			fe.vn[0]=v_1-1;
			fe.vn[1]=v_2-1;
			fe.vn[2]=v_3-1;
			m_pDoc->m_fcFaceAry.Add(fe);
		}
	fclose(stream);
	m_IsNew=true;
	//Invalidate();
	}//if
}


// 一般情况下的界面提示
void CParam3View::DrawNormalMeter(void)
{
	float x_tab = 70.0f;
	float y_tab = 20.0f;
	float x = 10.0f;
	float y = 20.0f;
	float x2 = x + x_tab;
	char buffer1[10];
	char buffer2[10];
	char buffer3[10];
	char buffer4[10]="true";
	char buffer5[10]="false";
	DrawText("I's", x, y); 	  
	//draw m_xrot
	y+=y_tab;
	DrawText("xrot",x,y);
	_gcvt_s(buffer1 ,m_xrot,4 ); 
	DrawText(buffer1,x2,y);
	//drwa m_yrot
	y+=y_tab;
	DrawText("yrot",x,y);
	_gcvt_s(buffer2 ,m_yrot,4 );
	DrawText(buffer2,x2,y);
	///// draw m_zdeep
	y+=y_tab;
	DrawText("zdeep",x,y);
	if (m_zdeep>0.1 || m_zdeep< -0.1){
		_gcvt_s( buffer3,m_zdeep,4);
	}else{
		buffer3[0]='0';buffer3[1]='.';buffer3[2]='0';
	}
	DrawText(buffer3,x2,y);
	////////////各种参数
	y+=y_tab;
 	DrawText("New",x,y);
	if (m_IsNew) {
		DrawText(buffer4,x2,y);
	}
	else{
		DrawText(buffer5,x2,y);
	}
	
	y+=y_tab;
	DrawText("Shown",x,y);
	if (m_IsShown) {
		DrawText(buffer4,x2,y);
	}
	else{
		DrawText(buffer5,x2,y);
	}

	y+=y_tab;
	DrawText("Mesh",x,y);
	if (m_IsMesh) {
		DrawText(buffer4,x2,y);
	}
	else{
		DrawText(buffer5,x2,y);
	}

	y+=y_tab;
	DrawText("Para",x,y);
	if (m_IsPara) {
		DrawText(buffer4,x2,y);
	}
	else{
		DrawText(buffer5,x2,y);
	}

	y+=y_tab;
	DrawText("Render",x,y);
	if (m_IsRender) {
		DrawText(buffer4,x2,y);
	}
	else{
		DrawText(buffer5,x2,y);
	}
	
}


void CParam3View::DrawParaming(void)
{
	float x = 10.0f;
	float y = 20.0f;
	DrawText("I's", x, y); 
	DrawText("Paraming...", x, y+20); 

}
