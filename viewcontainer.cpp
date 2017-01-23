#include "viewcontainer.h"

#include "views/perspectiveview.h"
#include "views/uvview.h"

ViewContainer::ViewContainer(QWidget *parent) : QGLWidget(parent)
{
    _views.insert("perspective", QSharedPointer<GLView>(new PerspectiveView()));
    _views.insert("uv", QSharedPointer<GLView>(new UVView()));
}

void ViewContainer::initializeGL()
{
    /*
    _meshShader = ShaderFactory::buildMeshShader(this);
    _bakeShader = ShaderFactory::buildBakeShader(this);
#if DEBUG_PAINT_LAYER
        _paintDebugShader = ShaderFactory::buildPaintDebugShader(this);
#endif
*/
    _logger = new QOpenGLDebugLogger(this);
    _logger->initialize();
}

QGLFormat ViewContainer::defaultFormat()
{
    QGLFormat format;
    //format.setVersion(3,2);
    //format.setRenderableType(QSurfaceFormat::OpenGL);
    //format.setAlpha(true);
    //format.setStencil(true);
    //format.setVersion(3,1);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    //format.setOption(QSurfaceFormat::DebugContext);
    return format;
}

void ViewContainer::resizeGL(int w, int h)
{
    //_views["perspective"]->setWidth(w);
    //_views["perspective"]->setHeight(h);
}

void ViewContainer::paintGL()
{
    QList<QOpenGLDebugMessage> messages = _logger->loggedMessages();
    foreach (const QOpenGLDebugMessage &message, messages)
        qDebug() << message;

    QPainter painter;
    painter.begin(this);
    painter.beginNativePainting();

    glClearColor(.2,.2,.2,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_viewMode == ViewMode::PERSPECTIVE) {
        drawViewGLPass("perspective", getFullRegion());
    } else if (_viewMode == ViewMode::UV) {
        drawViewGLPass("uv", getFullRegion());
    } else if (_viewMode == ViewMode::PERSPECTIVE_AND_UV) {
        drawViewGLPass("perspective", getLeftRegion());
        drawViewGLPass("uv", getRightRegion());
    }

    // draw strokes onto paint FBO
//    drawPaintStrokes();

//#if DEBUG_PAINT_LAYER
//    drawPaintLayer();
//#endif

//    if (_bakePaintLayer) {
//        bakePaintLayer();
//        setBusyMessage("baking", 400);
//    }

    painter.endNativePainting();

    if (_viewMode == ViewMode::PERSPECTIVE) {
        drawViewPainterPass("perspective", getFullRegion(), &painter);
    } else if (_viewMode == ViewMode::UV) {
        drawViewPainterPass("uv", getFullRegion(), &painter);
    } else if (_viewMode == ViewMode::PERSPECTIVE_AND_UV) {
        drawViewPainterPass("perspective", getLeftRegion(), &painter);
        drawViewPainterPass("uv", getRightRegion(), &painter);
    }
    //painterPass(&painter);

    /*
    if (_messageTimer.isActive()) {
        QFont prevFont = painter.font();
        QFont bakingFont(prevFont.family(), 20);
        QFontMetrics fm(bakingFont);
        painter.setFont(bakingFont);

        const int bottomOffset = 50;
        QRect bgRect(0, height() - bottomOffset, width(), fm.height());

        painter.fillRect(bgRect, QColor(0,0,0, 200));
        painter.drawText((width()-fm.width(_busyMessage)) / 2, bgRect.y() + bgRect.height() - fm.descent() - 4, _busyMessage);

        painter.setFont(prevFont);
    }
    */

    painter.end();
}

void ViewContainer::drawViewGLPass(QString viewName, QRect region)
{
    // bound rendering to this view
    glViewport(region.x(), height() - region.y() - region.height(), region.width(), region.height());

    _views[viewName]->setSize(region.width(), region.height());
    _views[viewName]->glPass(_glResourceContext);
}

void ViewContainer::drawViewPainterPass(QString viewName, QRect region, QPainter* painter)
{
    // bound painting to this view
    painter->setClipRect(region);

    QTransform t = painter->transform();
    t.translate(region.x(), 0); // TODO: handle y offset
    painter->setTransform(t);

    _views[viewName]->painterPass(painter);

    // draw border around view
    QPen nwLines(QColor("#888888"), 4);//, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);
    painter->setPen(nwLines);
    painter->drawLine(0, 0, 0, region.height());
    painter->drawLine(0, 0, region.width(), 0);
    QPen seLines(QColor("#111111"), 4);
    painter->setPen(seLines);
    painter->drawLine(0, region.height(), region.width(), region.height());
    painter->drawLine(region.width(), 0, region.width(), region.height());


    drawOutlinedText(painter, 60, 60, _views[viewName]->viewLabel(), QColor(0,0,0), QColor(255,255,255));


    t.translate(-region.x(), 0);
    painter->setTransform(t);


}

// returns the full-window region in y-down coordinates
QRect ViewContainer::getFullRegion()
{
    return QRect(0, 0, width(), height());
}

// returns the left region in y-down coordinates
QRect ViewContainer::getLeftRegion()
{
    return QRect(0, 0, width() / 2, height());
}

// returns the right region in y-down coordinates
QRect ViewContainer::getRightRegion()
{
    return QRect(width() / 2, 0, width() - width() / 2, height());
}

QMouseEvent ViewContainer::convertToRegion(QMouseEvent *event, QRect region)
{
    QPointF pos = event->localPos() - region.topLeft();
    return QMouseEvent(event->type(), pos, event->button(), event->buttons(), event->modifiers());
}

void ViewContainer::mousePressEvent(QMouseEvent* event)
{
    if (_viewOwningMouse == "") {
        // get the view name and region
        QString viewName;
        QRect region;
        if (_viewMode == ViewMode::PERSPECTIVE) {
            viewName = "perspective";
            region = getFullRegion();
        } else if (_viewMode == ViewMode::UV) {
            viewName = "uv";
            region = getFullRegion();
        } else { // (_viewMode == ViewMode::PERSPECTIVE_AND_UV) {
            QRect left = getLeftRegion();
            if (left.contains(event->pos())) {
                viewName = "perspective";
                region = left;
            } else {
                viewName = "uv";
                region = getRightRegion();
            }
        }

        _viewOwningMouse = viewName;

        QSharedPointer<GLView> view = _views[_viewOwningMouse];
        QMouseEvent localEvent = convertToRegion(event, region);
        view->mousePressEvent(&localEvent);

        _mousePressButton = event->button();
        _owningRegion = region;
    }
}

void ViewContainer::mouseDoubleClickEvent(QMouseEvent *)
{

}

void ViewContainer::mouseReleaseEvent(QMouseEvent* event)
{
    if (_viewOwningMouse != "" && event->button() == _mousePressButton) {
        QSharedPointer<GLView> view = _views[_viewOwningMouse];

        QMouseEvent localEvent = convertToRegion(event, _owningRegion);
        view->mouseReleaseEvent(&localEvent);


        _viewOwningMouse = "";
    }
}

void ViewContainer::mouseMoveEvent(QMouseEvent* event)
{
    if (_viewOwningMouse != "") {
        mouseDragEvent(event);
    }

    update();
}

void ViewContainer::mouseDragEvent(QMouseEvent* event)
{
    std::cout << "drag" << std::endl;
    if (_viewOwningMouse != "") {
        QSharedPointer<GLView> view = _views[_viewOwningMouse];
        QMouseEvent localEvent = convertToRegion(event, _owningRegion);
        view->mouseDragEvent(&localEvent);
        std::cout << "dragging" << std::endl;
    }
}

void ViewContainer::keyPressEvent(QKeyEvent* event)
{

}

