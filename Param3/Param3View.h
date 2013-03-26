
// Param3View.h : CParam3View 类的接口
//

#pragma once

#include <afxtempl.h>
#include <gl/GL.h>
#include <gl/glut.h>

class CParam3View : public CView
{
protected: // 仅从序列化创建
	CParam3View();
	DECLARE_DYNCREATE(CParam3View)

// 特性
public:
	CParam3Doc* GetDocument() const;
	CParam3Doc* m_pDoc;
public:
		CPoint m_ptPrev;
	HGLRC m_hRC;
	GLfloat	m_xrot;				// X Rotation
	GLfloat	m_yrot;				// Y Rotation
	GLfloat m_xspeed;				// X Rotation Speed
	GLfloat m_yspeed;				// Y Rotation Speed
	GLfloat	m_zdeep;			// Depth Into The Screen
	GLfloat m_scale;
	bool m_IsNew;			//数据更新过否？
	bool m_IsPara;			//参数化算法运行过?
	bool m_IsMesh;			//网格显示了吗?
	bool m_IsRender;		//表面渲染了么?
	bool m_IsShown;			//数据集显示了么?
	bool m_IsText;			//结果保存成txt了么?
	bool m_IsMSG;			//系统消息正在传递中么?
	bool m_bLButton;		//鼠标左键标示
	CPoint ptLastMousePosit;		//左键按下时上次坐标
	CPoint ptCurrentMousePosit;		//当前鼠标指着的坐标
	bool m_IsParaming;
	bool m_IsDivid;					//显示划分结果

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CParam3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	void DrawText(const char* text, float x, float y); // 显示屏幕提示
	bool SetupPixelFormat(HDC hDC);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTriangle();
	afx_msg void OnParam();
	afx_msg void OnDivid();
	afx_msg void OnDivsave();
	afx_msg void OnFitting();
	afx_msg void OnReaddata();
//	afx_msg void OnUpdateDivsave(CCmdUI *pCmdUI);
	void DrawNormalMeter(void);
	void DrawParaming(void);
	// 正在参数化计算
	
};

#ifndef _DEBUG  // Param3View.cpp 中的调试版本
inline CParam3Doc* CParam3View::GetDocument() const
   { return reinterpret_cast<CParam3Doc*>(m_pDocument); }
#endif

