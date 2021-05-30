/**
 * 去掉标题之后添加边框阴影
 *
 * MakePixmapWidget.h
 * 构造出边框阴影QImage方法的头文件。
 *
 * FlyWM_
 * GitHub: https://github.com/FlyWM
 * CSDN: https://blog.csdn.net/a844651990
 *
 */

#ifndef MAKEPIXMAPWIDGET_H
#define MAKEPIXMAPWIDGET_H

#include <QWidget>
#include <QDialog>
class QPainter;


#ifdef Q_OS_WIN32
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

class WinDwmShadow
{
public:
    explicit WinDwmShadow(QWidget *w);
private:
    QWidget *m_widget;
};
#endif

class ShadowImage : public QObject
{
    Q_OBJECT
public:
    explicit ShadowImage(QWidget* w, int shadowSize = 10, QObject *parent = 0);
    ~ShadowImage();
    void setShadowSize(int shadowSize);
    void paint(QPainter* p);
    int shadowSize();

public slots:
    void hide();
    void show();
private:
    int m_shadowSize;
    QImage m_img;
    QRect m_arrayImageGrid[9];
    QColor m_borderColor;
    QColor m_bgColor;
    QWidget *m_widget;
    bool m_show;
};

#endif // MAKEPIXMAPWIDGET_H
