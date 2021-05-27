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
class QLineEdit;
class QLabel;

class Skin9GridImage
{
protected:
    QImage m_img;
    QRect m_arrayImageGrid[9];
    //
    bool clear();
public:
    static bool splitRect(const QRect& rcSrc, QPoint ptTopLeft, QRect* parrayRect, int nArrayCount);
//    bool setImage(const CString& strImageFileName, QPoint ptTopLeft);
    bool setImage(const QImage& image, QPoint ptTopLeft);
//    //
//    void draw(QPainter* p, QRect rc, int nAlpha) const;
    void drawBorder(QPainter* p, QRect rc) const;
//    bool valid() const;
//    //
//    QSize actualSize() const { return m_img.size(); }
};

class ShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowWidget(QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *e);

    void setShadowSize(int shadowSize);
    int shadowSize();
    void hideShadow();
    void showShadow();

private:
    Skin9GridImage* m_shadow;
    int m_shadowSize;

};

//class ShadowBaseWidget : public QWidget
//{
//public:
//    explicit ShadowBaseWidget(int shadowSize, QWidget *parent = nullptr);

//    QWidget* clientWidget()
//    {
//        return shadowClientWidget;
//    }
//private:
//    QWidget* shadowClientWidget;
//};

//class ShadowBaseDialog : public QDialog
//{
//public:
//    explicit ShadowBaseDialog(int shadowSize, QWidget *parent = nullptr);

//    QWidget* clientWidget()
//    {
//        return shadowClientWidget;
//    }
//private:
//    QWidget* shadowClientWidget;
//};

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

class WinAPIShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WinAPIShadowWidget(QWidget *parent = nullptr);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

public slots:
};

class WinAPIShadowDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WinAPIShadowDialog(QWidget *parent = nullptr);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

public slots:
};

#endif

#endif // MAKEPIXMAPWIDGET_H
