#ifndef MAINSCROLLAREA_H
#define MAINSCROLLAREA_H

#include <QScrollArea>

class MainScrollArea : public QScrollArea
{

    void saveGeometry();
    void restoreGeometry();


public:
    MainScrollArea(QWidget *parent = 0);
    ~MainScrollArea();
};

#endif // MAINSCROLLAREA_H
