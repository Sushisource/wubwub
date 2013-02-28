#ifndef FIRSTVIZ_H
#define FIRSTVIZ_H
#include "vizwidget.h"

class FirstViz : public VizWidget
{
    Q_OBJECT
public:
    explicit FirstViz(QWidget *parent = 0);
    
protected:
    void vizInitialize();
    void paintGL();

signals:
    
public slots:
    
};

#endif // FIRSTVIZ_H
