#include "vizwidget.h"

VizWidget::VizWidget(QWidget *parent) :
    QGLWidget(parent),
    renderThread(this)
{
    setAutoBufferSwap(false);
    context()->moveToThread(&renderThread);
}

VizWidget::~VizWidget()
{
}

void VizWidget::setPlayBackPointer(PlaybackWidget *p)
{
    renderThread.playbackWidget = p;
}

void VizWidget::startRenderThread()
{
    doneCurrent();
    renderThread.start();
}

void VizWidget::stopRenderThread()
{
    renderThread.stop();
    renderThread.wait();
}

void VizWidget::resizeEvent(QResizeEvent *e)
{
    renderThread.resize(e->size().width(), e->size().height());
}

void VizWidget::closeEvent(QCloseEvent *evt)
{
    stopRenderThread();
    QGLWidget::closeEvent(evt);
}

void VizWidget::paintEvent(QPaintEvent *)
{
    //handled by render thread
}

void VizWidget::showEvent(QShowEvent *e)
{
    QGLWidget::showEvent(e);
    startRenderThread();
}

void VizWidget::hideEvent(QHideEvent *e)
{
    QGLWidget::hideEvent(e);
    stopRenderThread();
}
