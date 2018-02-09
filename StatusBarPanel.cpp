
#include <cmath>
#include <QtCore/QFileInfo>
#include "StatusBarPanel.h"
#include "ImageViewInfoProvider.h"
#include "UnitsProvider.h"
#include "PageId.h"

StatusBarPanel::StatusBarPanel() {
    ui.setupUi(this);
}

void StatusBarPanel::updateMousePos(const QPointF& mousePos) {
    const QMutexLocker locker(&mutex);

    if (!mousePos.isNull()) {
        double x = mousePos.x();
        double y = mousePos.y();
        UnitsProvider::getInstance()->convertFrom(x, y, PIXELS);

        switch (UnitsProvider::getInstance()->getUnits()) {
            case PIXELS:
            case MILLIMETRES:
                x = ceil(x);
                y = ceil(y);
                break;
            default:
                x = ceil(x * 10) / 10;
                y = ceil(y * 10) / 10;
                break;
        }

        ui.mousePosLine->setVisible(true);
        ui.mousePosLabel->setText(QString("%1, %2").arg(x).arg(y));
    } else {
        ui.mousePosLabel->clear();
        ui.mousePosLine->setVisible(false);
    }
}

void StatusBarPanel::updatePhysSize(const QSizeF& physSize) {
    const QMutexLocker locker(&mutex);

    if (!physSize.isNull()) {
        double width = physSize.width();
        double height = physSize.height();
        UnitsProvider::getInstance()->convertFrom(width, height, PIXELS);

        const Units units = UnitsProvider::getInstance()->getUnits();
        switch (units) {
            case PIXELS:
                width = round(width);
                height = round(height);
                break;
            case MILLIMETRES:
                width = round(width);
                height = round(height);
                break;
            case CENTIMETRES:
                width = round(width * 10) / 10;
                height = round(height * 10) / 10;
                break;
            case INCHES:
                width = round(width * 10) / 10;
                height = round(height * 10) / 10;
                break;
        }

        ui.physSizeLine->setVisible(true);
        ui.physSizeLabel->setText(QString("%1 x %2 %3").arg(width).arg(height).arg(unitsToString(units)));
    } else {
        ui.physSizeLabel->clear();
        ui.physSizeLine->setVisible(false);
    }
}

void StatusBarPanel::updatePage(int pageNumber, const PageId& pageId) {
    ui.pageNoLabel->setText(tr("p. %1").arg(pageNumber));

    QString pageFileInfo = QFileInfo(pageId.imageId().filePath()).baseName();
    if (pageFileInfo.size() > 15) {
        pageFileInfo = "..." + pageFileInfo.right(13);
    }
    if (pageId.subPage() != PageId::SINGLE_PAGE) {
        pageFileInfo = pageFileInfo.right(11) + ((pageId.subPage() == PageId::LEFT_PAGE) ? tr(" [L]") : tr(" [R]"));
    }

    ui.pageInfoLine->setVisible(true);
    ui.pageInfo->setText(pageFileInfo);
}

void StatusBarPanel::clear() {
    ui.mousePosLabel->clear();
    ui.physSizeLabel->clear();
    ui.pageNoLabel->clear();
    ui.pageInfo->clear();

    ui.mousePosLine->setVisible(false);
    ui.physSizeLine->setVisible(false);
    ui.pageInfoLine->setVisible(false);
}

void StatusBarPanel::updateUnits(Units) {
    updateMousePos(ImageViewInfoProvider::getInstance()->getMousePos());
    updatePhysSize(ImageViewInfoProvider::getInstance()->getPhysSize());
}