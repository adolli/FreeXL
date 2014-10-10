#pragma once

//================================================================================
class Dlg3DGrid : public CDialog
{
public:
    Dlg3DGrid(CWnd* pParent=NULL) : CDialog(IDD_EFF_3DGRID, pParent)
    {
        m_nSize=0 ; m_nDepth=0 ;
    }

    int     m_nSize ;
    int     m_nDepth ;

private:
    virtual void DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        DDX_CBIndex(pDX, IDC_3DGRID_SIZE, m_nSize) ;
        DDX_CBIndex(pDX, IDC_3DGRID_DEPTH, m_nDepth) ;
    }

    virtual void OnOK()
    {
        CDialog::OnOK() ;
        switch (m_nSize)
        {
            case 0 : m_nSize = 64 ; break ;
            case 1 : m_nSize = 32 ; break ;
            case 2 : m_nSize = 16 ; break ;
        }
        switch (m_nDepth)
        {
            case 0 : m_nDepth = 92 ; break ;
            case 1 : m_nDepth = 60 ; break ;
            case 2 : m_nDepth = 28 ; break ;
        }
    }
};
