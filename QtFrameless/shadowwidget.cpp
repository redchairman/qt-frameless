/**
 * 去掉标题之后添加边框阴影
 *
 * MakePixmapWidget.cpp
 * 构造出边框阴影QImage方法的cpp文件。
 *
 * FlyWM_
 * GitHub: https://github.com/FlyWM
 * CSDN: https://blog.csdn.net/a844651990
 *
 */

#include <QVBoxLayout>
#include <QPainter>
#include <QMap>
#include "shadowwidget.h"

inline unsigned char MakeAlpha(int i, double f, int nSize)
{
    if (i == nSize)
        f *= 1.2;
    //

    double f2 = 1 - cos((double)i / nSize * 3.14 / 2);
    //
    return int(fabs((i * f) * f2));
}

QImage MakeShadowImage(int shadowSize, bool activated)
{
    int size = shadowSize * 2 + 10;
    QImage image(size, size, QImage::Format_ARGB32);
    image.fill(QColor(Qt::black));

    int borderR = 165;
    int borderG = 165;
    int borderB = 165;
    //
    if (activated) {
        borderR = 209;
        borderG = 209;
        borderB = 209;
    }

#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
    image.setColor(0, qRgba(borderR, borderG, borderB, 255));
    QMap<int, int> alphaColorMap;
#endif

    //
    double f = activated ? 2.0 : 1.0;
    //
    QSize szImage = image.size();
    //
    //left
    for (int y = shadowSize; y < szImage.height() - shadowSize; y++) {
        for (int x = 0; x < shadowSize; x++) {
            int i = x + 1;
            int alpha = MakeAlpha(i, f, shadowSize);

#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            int colorIndex = -1;
            QMap<int, int>::iterator it = alphaColorMap.find(alpha);
            if (it == alphaColorMap.end())
            {
                colorIndex = image.colorCount();
                image.setColor(colorIndex, qRgba(0, 0, 0, alpha));
				alphaColorMap[alpha] = colorIndex;
            }
            else{
                colorIndex = it.value();
            }
            image.setPixel(x, y, colorIndex);
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif

        }
    }
    //right
    for (int y = shadowSize; y < szImage.height() - shadowSize; y++) {
        for (int x = szImage.width() - shadowSize - 1; x < szImage.width(); x++) {
            int i = szImage.width() - x;
            int alpha = MakeAlpha(i, f, shadowSize);
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            int colorIndex = -1;
            QMap<int, int>::iterator it = alphaColorMap.find(alpha);
            if (it == alphaColorMap.end())
            {
                colorIndex = image.colorCount();
                image.setColor(colorIndex, qRgba(0, 0, 0, alpha));
            }
            else{
                colorIndex = it.value();
            }
            image.setPixel(x, y, colorIndex);
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    //top
    for (int y = 0; y < shadowSize; y++) {
        int i = y + 1;
        for (int x = shadowSize; x < szImage.width() - shadowSize; x++) {
            int alpha = MakeAlpha(i, f, shadowSize);
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            int colorIndex = -1;
            QMap<int, int>::iterator it = alphaColorMap.find(alpha);
            if (it == alphaColorMap.end())
            {
                colorIndex = image.colorCount();
                image.setColor(colorIndex, qRgba(0, 0, 0, alpha));
            }
            else{
                colorIndex = it.value();
            }
            image.setPixel(x, y, colorIndex);
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
        //
    }
    //bottom
    for (int y = szImage.height() - shadowSize - 1; y < szImage.height(); y++) {
        int i = szImage.height() - y;
        for (int x = shadowSize; x < szImage.width() - shadowSize; x++) {
            int alpha = MakeAlpha(i, f, shadowSize);
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            int colorIndex = -1;
            QMap<int, int>::iterator it = alphaColorMap.find(alpha);
            if (it == alphaColorMap.end())
            {
                colorIndex = image.colorCount();
                image.setColor(colorIndex, qRgba(0, 0, 0, alpha));
            }
            else{
                colorIndex = it.value();
            }
            image.setPixel(x, y, colorIndex);
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    //
    int parentRoundSize = 3;
    //
    for (int x = 0; x < shadowSize + parentRoundSize; x++) {
        for (int y = 0; y < shadowSize + parentRoundSize; y++) {
            int xx = (shadowSize + parentRoundSize) - x;
            int yy = (shadowSize + parentRoundSize) - y;
            int i = int(sqrt(double(xx * xx + yy * yy)));
            i = std::min<int>(shadowSize + parentRoundSize, i);
            i -= parentRoundSize;
            i = shadowSize - i;
            //
            int alpha = MakeAlpha(i, f, shadowSize);
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            int colorIndex = -1;
            QMap<int, int>::iterator it = alphaColorMap.find(alpha);
            if (it == alphaColorMap.end())
            {
                colorIndex = image.colorCount();
                image.setColor(colorIndex, qRgba(0, 0, 0, alpha));
            }
            else{
                colorIndex = it.value();
            }
            image.setPixel(x, y, colorIndex);
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    //
    for (int x = szImage.width() - shadowSize - parentRoundSize; x < szImage.width(); x++) {
        for (int y = 0; y < shadowSize + parentRoundSize; y++) {
            int xx = (shadowSize + parentRoundSize) - (szImage.width() - x);
            int yy = (shadowSize + parentRoundSize) - y;
            int i = int(sqrt(double(xx * xx + yy * yy)));
            i = std::min<int>(shadowSize + parentRoundSize, i);
            i -= parentRoundSize;
            i = shadowSize - i;
            //
            int alpha = MakeAlpha(i, f, shadowSize);
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            int colorIndex = -1;
            QMap<int, int>::iterator it = alphaColorMap.find(alpha);
            if (it == alphaColorMap.end())
            {
                colorIndex = image.colorCount();
                image.setColor(colorIndex, qRgba(0, 0, 0, alpha));
            }
            else{
                colorIndex = it.value();
            }
            image.setPixel(x, y, colorIndex);
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    //
    for (int x = 0; x < shadowSize + parentRoundSize; x++) {
        for (int y = szImage.height() - shadowSize - parentRoundSize; y < szImage.height(); y++) {
            int xx = (shadowSize + parentRoundSize) - x;
            int yy = (shadowSize + parentRoundSize) - (szImage.height() - y);
            int i = int(sqrt(double(xx * xx + yy * yy)));
            i = std::min<int>(shadowSize + parentRoundSize, i);
            i -= parentRoundSize;
            i = shadowSize - i;
            //
            int alpha = MakeAlpha(i, f, shadowSize);
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            int colorIndex = -1;
            QMap<int, int>::iterator it = alphaColorMap.find(alpha);
            if (it == alphaColorMap.end())
            {
                colorIndex = image.colorCount();
                image.setColor(colorIndex, qRgba(0, 0, 0, alpha));
            }
            else{
                colorIndex = it.value();
            }
            image.setPixel(x, y, colorIndex);
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    //
    for (int x = szImage.width() - shadowSize - parentRoundSize; x < szImage.width(); x++) {
        for (int y = szImage.height() - shadowSize - parentRoundSize; y < szImage.height(); y++) {
            int xx = (shadowSize + parentRoundSize) - (szImage.width() - x);
            int yy = (shadowSize + parentRoundSize) - (szImage.height() - y);
            int i = int(sqrt(double(xx * xx + yy * yy)));
            i = std::min<int>(shadowSize + parentRoundSize, i);
            i -= parentRoundSize;
            i = shadowSize - i;
            //
            int alpha = MakeAlpha(i, f, shadowSize);
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            int colorIndex = -1;
            QMap<int, int>::iterator it = alphaColorMap.find(alpha);
            if (it == alphaColorMap.end())
            {
                colorIndex = image.colorCount();
                image.setColor(colorIndex, qRgba(0, 0, 0, alpha));
            }
            else{
                colorIndex = it.value();
            }
            image.setPixel(x, y, colorIndex);
#else
            image.setPixelColor(x, y, QColor(0, 0, 0, alpha));
#endif
        }
    }
    //

    //
    int borderSize = 1;
    //left
    for (int i = 0; i < borderSize; i++) {
        for (int y = shadowSize - 1; y < szImage.height() - shadowSize + 1; y++) {
            int x = shadowSize - i - 1;
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            image.setPixel(x, y, 0);
#else
            image.setPixelColor(x, y, QColor(borderR, borderG, borderB, 255));
#endif
        }
    }
    //right
    for (int i = 0; i < borderSize; i++) {
        for (int y = shadowSize - 1; y < szImage.height() - shadowSize + 1; y++) {
            int x = szImage.width() - shadowSize - 1 + i + 1;
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            image.setPixel(x, y, 0);
#else
            image.setPixelColor(x, y, QColor(borderR, borderG, borderB, 255));
#endif
        }
    }
    //top
    for (int i = 0; i < borderSize; i++) {
        for (int x = shadowSize; x < szImage.width() - shadowSize; x++) {
            int y = shadowSize - i - 1;
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            image.setPixel(x, y, 0);
#else
            image.setPixelColor(x, y, QColor(borderR, borderG, borderB, 255));
#endif
        }
    }
    //bottom
    for (int i = 0; i < borderSize; i++) {
        for (int x = shadowSize; x < szImage.width() - shadowSize; x++) {
            int y = szImage.height() - shadowSize - 1 + i + 1;

#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
            image.setPixel(x, y, 0);
#else
            image.setPixelColor(x, y, QColor(borderR, borderG, borderB, 255));
#endif
        }
    }
    //
    return image;
}

bool Skin9GridImage::clear()
{
    if (!m_img.isNull()) {
        m_img = QImage();
    }
    return true;
}

bool Skin9GridImage::splitRect(const QRect &rcSrc, QPoint ptTopLeft, QRect *parrayRect, int nArrayCount)
{
    Q_ASSERT(nArrayCount == 9);
    //
    QRect* arrayRect = parrayRect;
    //
    int nWidth = rcSrc.width();
    int nHeight = rcSrc.height();
    //
    if (ptTopLeft.x() <= 0)
        return false;
    if (ptTopLeft.y() <= 0)
        return false;
    if (ptTopLeft.x() >= nWidth / 2)
        return false;
    if (ptTopLeft.y() >= nHeight / 2)
        return false;
    //
    int x1 = rcSrc.left() + 0;
    int x2 = rcSrc.left() + ptTopLeft.x();
    int x3 = rcSrc.left() + nWidth - ptTopLeft.x();
    int x4 = rcSrc.left() + nWidth;
    //
    int y1 = rcSrc.top() + 0;
    int y2 = rcSrc.top() + ptTopLeft.y();
    int y3 = rcSrc.top() + nHeight - ptTopLeft.y();
    int y4 = rcSrc.top() + nHeight;
    //
    arrayRect[0] = QRect(QPoint(x1, y1), QPoint(x2, y2));
    arrayRect[1] = QRect(QPoint(x2 + 1, y1), QPoint(x3, y2));
    arrayRect[2] = QRect(QPoint(x3 + 1, y1), QPoint(x4, y2));

    arrayRect[3] = QRect(QPoint(x1, y2 + 1), QPoint(x2, y3));
    arrayRect[4] = QRect(QPoint(x2 + 1, y2 + 1), QPoint(x3, y3));
    arrayRect[5] = QRect(QPoint(x3 + 1, y2 + 1), QPoint(x4, y3));

    arrayRect[6] = QRect(QPoint(x1, y3 + 1), QPoint(x2, y4));
    arrayRect[7] = QRect(QPoint(x2 + 1, y3 + 1), QPoint(x3, y4));
    arrayRect[8] = QRect(QPoint(x3 + 1, y3 + 1), QPoint(x4, y4));
    //
    return true;
}

bool Skin9GridImage::setImage(const QImage &image, QPoint ptTopLeft)
{
    clear();
    //
    m_img = image;
    //
    int nImageWidth = m_img.width();
    int nImageHeight = m_img.height();
    //
    return splitRect(QRect(0, 0, nImageWidth, nImageHeight), ptTopLeft, m_arrayImageGrid, 9);
}

void Skin9GridImage::drawBorder(QPainter *p, QRect rc) const
{
    QRect arrayDest[9];
    //
    splitRect(rc, m_arrayImageGrid[0].bottomRight(), arrayDest, 9);
    //
    for (int i = 0; i < 9; i++) {
        if (i == 4)
            continue;
        //
        const QRect& rcSrc = m_arrayImageGrid[i];
        const QRect& rcDest = arrayDest[i];
        //
        p->drawImage(rcDest, m_img, rcSrc);
    }
}


ShadowWidget::ShadowWidget(QWidget *parent)
    : QWidget(parent)
    , m_shadow(new Skin9GridImage())
{
    setAttribute(Qt::WA_TranslucentBackground);
    //setWindowFlag(Qt::FramelessWindowHint);
    setMouseTracking(true);
    //
//    QImage image = MakeShadowImage(shadowSize, true);
//    m_shadow->setImage(image, QPoint(shadowSize + 1, shadowSize + 1));
}

void ShadowWidget::setShadowSize(int shadowSize)
{
    m_shadowSize = shadowSize;
    QImage image = MakeShadowImage(shadowSize, true);
    m_shadow->setImage(image, QPoint(shadowSize + 1, shadowSize + 1));
    showShadow();
}

int ShadowWidget::shadowSize()
{
    return m_shadowSize;
}

void ShadowWidget::hideShadow()
{
    QLayout* lay = layout();
    if (lay)
        lay->setContentsMargins(0, 0, 0, 0);
}

void ShadowWidget::showShadow()
{
    QLayout* lay = layout();
    if (lay)
        lay->setContentsMargins(m_shadowSize, m_shadowSize, m_shadowSize, m_shadowSize);
}

void ShadowWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);
    m_shadow->drawBorder(&painter, rect());
}

//ShadowBaseWidget::ShadowBaseWidget(int shadowSize, QWidget *parent)
//    : QWidget(parent)
//{
//    resize(800, 600);
//    setAttribute(Qt::WA_TranslucentBackground);
////    setWindowFlags(Qt::FramelessWindowHint);
//    setContentsMargins(0, 0, 0, 0);

//    QVBoxLayout *pLayout = new QVBoxLayout(this);
//    pLayout->setContentsMargins(0, 0, 0, 0);
//    pLayout->setSpacing(0);
//    ShadowWidget *pShadowWidget = new ShadowWidget(this);
//    pShadowWidget->setShadowSize(shadowSize);
//    pShadowWidget->setContentsMargins(shadowSize, shadowSize, shadowSize, shadowSize);
//    pLayout->addWidget(pShadowWidget);

//    QLayout* rootLayout = new QBoxLayout(QBoxLayout::TopToBottom);
//    pShadowWidget->setLayout(rootLayout);
//    rootLayout->setContentsMargins(0, 0, 0, 0);
//    rootLayout->setSpacing(0);

//    shadowClientWidget = new QWidget(pShadowWidget);
//    shadowClientWidget->setAutoFillBackground(true);
//    rootLayout->addWidget(shadowClientWidget);
//}

//ShadowBaseDialog::ShadowBaseDialog(int shadowSize, QWidget *parent)
//    : QDialog(parent)
//{
//    resize(800, 600);
// //   setAttribute(Qt::WA_TranslucentBackground);
////    setWindowFlags(Qt::FramelessWindowHint);
//    setContentsMargins(0, 0, 0, 0);

//    QVBoxLayout *pLayout = new QVBoxLayout(this);
//    pLayout->setContentsMargins(0, 0, 0, 0);
//    pLayout->setSpacing(0);
//    ShadowWidget *pShadowWidget = new ShadowWidget(this);
//    pShadowWidget->setShadowSize(shadowSize);
//    pShadowWidget->setContentsMargins(shadowSize, shadowSize, shadowSize, shadowSize);
//    pLayout->addWidget(pShadowWidget);

//    QLayout* rootLayout = new QBoxLayout(QBoxLayout::TopToBottom);
//    pShadowWidget->setLayout(rootLayout);
//    rootLayout->setContentsMargins(0, 0, 0, 0);
//    rootLayout->setSpacing(0);

//    shadowClientWidget = new QWidget(pShadowWidget);
//    shadowClientWidget->setAutoFillBackground(true);
//    rootLayout->addWidget(shadowClientWidget);
//}


#ifdef Q_OS_WIN32

WinDwmapi::WinDwmapi()
    : dwmapi_dll_(LoadLibraryW(L"dwmapi.dll"))
    , dwm_is_composition_enabled_(NULL)
{
    if (dwmapi_dll_) {
        dwm_is_composition_enabled_ = \
                reinterpret_cast<DwmIsCompositionEnabledPtr>(GetProcAddress(dwmapi_dll_, "DwmIsCompositionEnabled"));
        dwm_extendframe_into_client_area_ = \
                reinterpret_cast<DwmExtendFrameIntoClientAreaPtr>(GetProcAddress(dwmapi_dll_, "DwmExtendFrameIntoClientArea"));
    }
}

WinDwmapi::~WinDwmapi()
{
    if (dwmapi_dll_) {
        FreeLibrary(dwmapi_dll_);
    }
}

HRESULT WinDwmapi::DwmIsCompositionEnabled(BOOL *pfEnabled) const
{
    if (dwm_is_composition_enabled_) {
        return dwm_is_composition_enabled_(pfEnabled);
    }

    return E_NOTIMPL;
}

HRESULT WinDwmapi::DwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS *pMarInset) const
{
    if (dwm_extendframe_into_client_area_) {
        return dwm_extendframe_into_client_area_(hWnd, pMarInset);
    }

    return E_NOTIMPL;
}

const WinDwmapi *WinDwmapi::instance()
{
    static const WinDwmapi s_dwmapi;
    return &s_dwmapi;
}

#pragma comment(lib, "user32.lib")
WinAPIShadowWidget::WinAPIShadowWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    HWND hwnd = (HWND)this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);

    // 此行代码可以带回Aero效果，同时也带回了标题栏和边框,在nativeEvent()会再次去掉标题栏
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
    //we better left 1 piexl width of border untouch, so OS can draw nice shadow around it
    const MARGINS shadow = { 1, 1, 1, 1 };
    WinDwmapi::instance()->DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
}

bool WinAPIShadowWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG *)message;
    switch (msg->message)
    {
    case WM_NCCALCSIZE:
    {
        // this kills the window frame and title bar we added with WS_THICKFRAME and WS_CAPTION
        *result = 0;
        return true;
    }
    default:
        return QWidget::nativeEvent(eventType, message, result);
    }
}

WinAPIShadowDialog::WinAPIShadowDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Dialog);
    HWND hwnd = (HWND)this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);

    // 此行代码可以带回Aero效果，同时也带回了标题栏和边框,在nativeEvent()会再次去掉标题栏
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
    //we better left 1 piexl width of border untouch, so OS can draw nice shadow around it
    const MARGINS shadow = { 1, 1, 1, 1 };
    WinDwmapi::instance()->DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
}

bool WinAPIShadowDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG *)message;
    switch (msg->message)
    {
    case WM_NCCALCSIZE:
    {
        // this kills the window frame and title bar we added with WS_THICKFRAME and WS_CAPTION
        *result = 0;
        return true;
    }
    default:
        return QDialog::nativeEvent(eventType, message, result);
    }
}
#endif
