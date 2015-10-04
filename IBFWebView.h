#ifndef IBFWEBVIEW_H
#define IBFWEBVIEW_H

#include <QWebView>

#include <QFileInfo>
#include <QWebElement>
#include <QWebFrame>
#include <QWebPage>

class IBFWebView : public QWebView
{
    Q_OBJECT

public:
    IBFWebView(QWidget * aParent = 0);
    ~IBFWebView();

    void IBFSetHtml(QString aString)
    {
        QWebPage tWebPage;
        tWebPage.mainFrame()->setHtml(aString);

        QString tFilePath = "file:///" + QFileInfo("MathJax/MathJax.js").absoluteFilePath();

        QString tMathJaxPath = "<script type=\"text/javascript\" src=\"" + tFilePath + "?config=TeX-AMS_HTML-full\"></script>";
        QString tMathJaxConfig = "<script type=\"text/x-mathjax-config\">MathJax.Hub.Config({extensions: [\"tex2jax.js\"],jax: [\"input/TeX\",\"output/HTML-CSS\"],tex2jax: {inlineMath: [[\"$\",\"$\"],[\"\\\\(\",\"\\\\)\"]]}});</script>";

        QWebElement tWebElement = tWebPage.mainFrame()->documentElement();

        tWebElement.findFirst("body").prependInside(tMathJaxPath);
        tWebElement.findFirst("body").prependInside(tMathJaxConfig);

        this->setHtml(tWebElement.toOuterXml());
    }
};

#endif // IBFWEBVIEW_H


//#ifndef QNOTESWEBVIEW_H
//#define QNOTESWEBVIEW_H

//#include <QWebView>
//#include <QWebPage>
//#include <QWebFrame>
//#include <QWebElement>
//#include <QFileInfo>

//class QNotesWebView : public QWebView
//{
//    Q_OBJECT

//    //
//    //  Functions:
//public:
//    QNotesWebView(QWidget *aParent = 0);
//    ~QNotesWebView();

//    //Convenience functions:
//    //  QNotesWebViewSetMargins();
//    //  QNotesWebViewSetMathJax();
//    //  QNotesWebViewSetStyle(QString aString);

//    void QNotesWebViewSetHtml(QString aString);

//    void QNotesWebViewResizeMargins();
//    void QNotesWebViewSetScroll();

//protected:
//    void resizeEvent(QResizeEvent * e);

//private:

//    //
//    //  Variables:
//public:
////    int mScrollBarPosition;
////    QPoint mScrollPosition;

//protected:

//private:


//    //
//    //  Slots:
//public slots:

//protected slots:
//    void on_loadFinished_triggered(bool aBool);
//    void on_contentSizeChanged_triggered(QSize aSize);

//private slots:

//};

//#endif // QNOTESWEBVIEW_H
