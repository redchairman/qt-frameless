#ifndef SHADOWHELPER_H
#define SHADOWHELPER_H

#include <QWidget>
#include <QDialog>
class QPainter;


#ifdef Q_OS_WIN320
#include <windows.h>

typedef struct _MARGINS
{
    int cxLeftWidth;      // width of left border that retains its size
    int cxRightWidth;     // width of right border that retains its size
    int cyTopHeight;      // height of top border that retains its size
    int cyBottomHeight;   // height of bottom border that retains its size
} MARGINS, *PMARGINS;

class WinDwmapi
{
public:
    WinDwmapi();
    ~WinDwmapi();

    typedef HRESULT (WINAPI* DwmIsCompositionEnabledPtr)(BOOL* pfEnabled);
    typedef HRESULT (WINAPI* DwmExtendFrameIntoClientAreaPtr)(HWND hWnd, const MARGINS *pMarInset);

    HRESULT DwmIsCompositionEnabled(BOOL* pfEnabled) const;
    HRESULT DwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS *pMarInset) const;

    static const WinDwmapi* instance();

private:
      DwmIsCompositionEnabledPtr dwm_is_composition_enabled_;
      DwmExtendFrameIntoClientAreaPtr dwm_extendframe_into_client_area_;
      HMODULE dwmapi_dll_;
};

class DwmShadowHelper
{
public:
    explicit DwmShadowHelper(QWidget *w);
private:
    QWidget *m_widget;
};
#endif

class DrawShadowHelper : public QObject
{
    Q_OBJECT

    //Q_PROPERTY(QColor clientBgColor READ getClientBgColor WRITE setClientBgColor)

public:
    explicit DrawShadowHelper(QWidget* w, int shadowSize = 10, QObject *parent = 0);
    ~DrawShadowHelper();
    void setShadowSize(int shadowSize);
    void paint(QPainter* p);
    int shadowSize();

//    QColor getClientBgColor() const;
//    void setClientBgColor(const QColor &value);

    QColor getClientBorderColor() const;
    void setClientBorderColor(const QColor &value);

public slots:
    void hide();
    void show();
protected:
    QImage makeShadowImage(int shadowSize, bool activated);
    bool splitRect(const QRect &rcSrc, int shadowSize, QRect *parrayRect, int nArrayCount);
private:
    int m_shadowSize;
    QImage m_img;
    QRect m_arrayImageGrid[9];
    QColor m_clientBorderColor;
    QColor m_clientBgColor;
    QWidget *m_widget;
    bool m_show;
};

#endif // SHADOWHELPER_H
