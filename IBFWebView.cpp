#include "IBFWebView.h"

IBFWebView::IBFWebView(QWidget *aParent) : QWebView(aParent)
{

}

IBFWebView::~IBFWebView()
{

}



//#include "QNotesWebView.h"

//QNotesWebView::QNotesWebView(QWidget * aParent) : QWebView(aParent)
//{
//    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(on_loadFinished_triggered(bool)));
//    connect(this->page()->mainFrame(), SIGNAL(contentsSizeChanged(QSize)), this, SLOT(on_contentSizeChanged_triggered(QSize)));
//}

//QNotesWebView::~QNotesWebView()
//{

//}

//void QNotesWebView::QNotesWebViewSetHtml(QString aString)
//{
//    QWebPage tWebPage;
//    tWebPage.mainFrame()->setHtml(aString);

//    QString tFilePath = "file:///" + QFileInfo("MathJax/MathJax.js").absoluteFilePath();

//    QString tMathJaxPath = "<script type=\"text/javascript\" src=\"" + tFilePath + "?config=TeX-AMS_HTML-full\"></script>";
//    QString tMathJaxConfig = "<script type=\"text/x-mathjax-config\">MathJax.Hub.Config({extensions: [\"tex2jax.js\"],jax: [\"input/TeX\",\"output/HTML-CSS\"],tex2jax: {inlineMath: [[\"$\",\"$\"],[\"\\\\(\",\"\\\\)\"]]}});</script>";

//    QWebElement tWebElement = tWebPage.mainFrame()->documentElement();

//    tWebElement.findFirst("body").prependInside(tMathJaxPath);
//    tWebElement.findFirst("body").prependInside(tMathJaxConfig);

//    this->setHtml(tWebElement.toOuterXml());
//}

//void QNotesWebView::QNotesWebViewResizeMargins()
//{
//    QWebElement tWebElement = this->page()->mainFrame()->documentElement();

//    if ( rect().width() > 600 )
//    {
//        int tMargin = ((rect().width() - 600) / 2);
//        QString tString = "<style type=\"text/css\">body{margin-top: 4; margin-right:" + QString::number(tMargin) + "; margin-bottom: 4; margin-left:" + QString::number(tMargin) + ";}</style>";

//        if ( !tWebElement.findFirst("body").findFirst("style").isNull() )
//        {
//            tWebElement.findFirst("body").findFirst("style").replace(tString);
//        }
//        else
//        {
//            tWebElement.findFirst("body").prependInside(tString);
//        }
//    }
//    else
//    {
//        if ( !tWebElement.findFirst("body").findFirst("style").isNull() )
//        {
//            tWebElement.findFirst("body").findFirst("style").replace("<style type=\"text/css\">body{margin: 4, 4, 4, 4}</style>");
//        }
//        else
//        {
//            tWebElement.findFirst("body").prependInside("<style type=\"text/css\">body{margin: 4, 4, 4, 4}</style>");
//        }
//    }
//}

//void QNotesWebView::QNotesWebViewSetScroll()
//{

//}

//void QNotesWebView::resizeEvent(QResizeEvent *e)
//{
//    QNotesWebViewResizeMargins();

//    QWebView::resizeEvent(e);
//}

//void QNotesWebView::on_loadFinished_triggered(bool aBool)
//{
//    this->page()->mainFrame()->scrollToAnchor("TEXTEDITANCHOR");
//}

//void QNotesWebView::on_contentSizeChanged_triggered(QSize aSize)
//{
//    this->QNotesWebViewResizeMargins();
//    this->page()->mainFrame()->scrollToAnchor("ANCHOR");
//}
