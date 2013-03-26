
// Param3View.h : CParam3View ��Ľӿ�
//

#pragma once

#include <afxtempl.h>
#include <gl/GL.h>
#include <gl/glut.h>

class CParam3View : public CView
{
protected: // �������л�����
	CParam3View();
	DECLARE_DYNCREATE(CParam3View)

// ����
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
	bool m_IsNew;			//���ݸ��¹���
	bool m_IsPara;			//�������㷨���й�?
	bool m_IsMesh;			//������ʾ����?
	bool m_IsRender;		//������Ⱦ��ô?
	bool m_IsShown;			//���ݼ���ʾ��ô?
	bool m_IsText;			//��������txt��ô?
	bool m_IsMSG;			//ϵͳ��Ϣ���ڴ�����ô?
	bool m_bLButton;		//��������ʾ
	CPoint ptLastMousePosit;		//�������ʱ�ϴ�����
	CPoint ptCurrentMousePosit;		//��ǰ���ָ�ŵ�����
	bool m_IsParaming;
	bool m_IsDivid;					//��ʾ���ֽ��

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CParam3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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
	
	void DrawText(const char* text, float x, float y); // ��ʾ��Ļ��ʾ
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
	// ���ڲ���������
	
};

#ifndef _DEBUG  // Param3View.cpp �еĵ��԰汾
inline CParam3Doc* CParam3View::GetDocument() const
   { return reinterpret_cast<CParam3Doc*>(m_pDocument); }
#endif

