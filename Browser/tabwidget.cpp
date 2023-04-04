/****************************************************************************
*
* QmlBrowser - Web browser with QML page support
* Copyright (C) 2022 Denis Solomatin <toorion@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* This library baset on QT5 QTabWidget source code
****************************************************************************/

#include "tabwidget.h"

#include "private/qapplication_p.h"
#include "private/qtabbar_p.h"
#include "qapplication.h"
#include "qevent.h"
#include "qstackedwidget.h"
#include "qstyle.h"
#include "qstyleoption.h"
#include "qstylepainter.h"
#include "qtabbar.h"
#include <QToolBar>
#include <QLabel>
#include <QRect>
#include <QPushButton>


#include <QBoxLayout>
#include <QSplitter>
#include "private/qwindowcontainer_p.h"

QT_BEGIN_NAMESPACE


class TabWidgetPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(TabWidget)

public:
    TabWidgetPrivate();
    ~TabWidgetPrivate();
    void updateTabBarPosition();
    void _q_showTab(int);
    void _q_removeTab(int);
    void _q_tabMoved(int from, int to);
    void init();
    bool isAutoHidden() const
    {
        // see QTabBarPrivate::autoHideTabs()
        return (tabs->autoHide() && tabs->count() <= 1);
    }

    void initBasicStyleOption(QStyleOptionTabWidgetFrame *option) const;

    QTabBar *tabs;
    QToolBar *toolBar;
    QStackedWidget *stack;
    QRect panelRect;
    bool dirty;
    TabWidget::TabPosition pos;
    TabWidget::TabShape shape;
    QWidget *leftCornerWidget;
    QWidget *rightCornerWidget;
};


TabWidgetPrivate::TabWidgetPrivate()
    : tabs(nullptr), stack(nullptr), dirty(true),
      pos(TabWidget::North), shape(TabWidget::Rounded),
      leftCornerWidget(nullptr), rightCornerWidget(nullptr)
{}

TabWidgetPrivate::~TabWidgetPrivate()
{}

void TabWidgetPrivate::init()
{
    Q_Q(TabWidget);

    stack = new QStackedWidget(q);
    stack->setObjectName(QLatin1String("qt_tabwidget_stackedwidget"));
    stack->setLineWidth(0);
    // hack so that QMacStyle::layoutSpacing() can detect tab widget pages
    stack->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::TabWidget));

    QObject::connect(stack, SIGNAL(widgetRemoved(int)), q, SLOT(_q_removeTab(int)));
    QTabBar *tabBar = new QTabBar(q);
    tabBar->setObjectName(QLatin1String("qt_tabwidget_tabbar"));
    tabBar->setDrawBase(false);
    q->setTabBar(tabBar);

    QToolBar *toolBar = new QToolBar(q);
    toolBar->setObjectName(QLatin1String("qt_tabwidget_toolbar"));
    q->setToolBar(toolBar);

    q->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding,
                                 QSizePolicy::TabWidget));
#ifdef QT_KEYPAD_NAVIGATION
    if (QApplicationPrivate::keypadNavigationEnabled())
        q->setFocusPolicy(Qt::NoFocus);
    else
#endif
    q->setFocusPolicy(Qt::TabFocus);
    q->setFocusProxy(tabs);
    q->setTabPosition(static_cast<TabWidget::TabPosition> (q->style()->styleHint(
                      QStyle::SH_TabWidget_DefaultTabPosition, nullptr, q )));

}

/*!
    \reimp
*/

bool TabWidget::hasHeightForWidth() const
{
    Q_D(const TabWidget);
    bool has = d->size_policy.hasHeightForWidth();
    if (!has && d->stack)
        has = d->stack->hasHeightForWidth();
    return has;
}

void TabWidgetPrivate::initBasicStyleOption(QStyleOptionTabWidgetFrame *option) const
{
    Q_Q(const TabWidget);
    option->initFrom(q);

    if (q->documentMode())
        option->lineWidth = 0;
    else
        option->lineWidth = q->style()->pixelMetric(QStyle::PM_DefaultFrameWidth, nullptr, q);

    switch (pos) {
    case TabWidget::North:
        option->shape = shape == TabWidget::Rounded ? QTabBar::RoundedNorth
                                                     : QTabBar::TriangularNorth;
        break;
    case TabWidget::South:
        option->shape = shape == TabWidget::Rounded ? QTabBar::RoundedSouth
                                                     : QTabBar::TriangularSouth;
        break;
    case TabWidget::West:
        option->shape = shape == TabWidget::Rounded ? QTabBar::RoundedWest
                                                     : QTabBar::TriangularWest;
        break;
    case TabWidget::East:
        option->shape = shape == TabWidget::Rounded ? QTabBar::RoundedEast
                                                     : QTabBar::TriangularEast;
        break;
    }

    option->tabBarRect = q->tabBar()->geometry();
}

void TabWidget::initStyleOption(QStyleOptionTabWidgetFrame *option) const
{
    if (!option)
        return;

    Q_D(const TabWidget);
    d->initBasicStyleOption(option);

    int exth = style()->pixelMetric(QStyle::PM_TabBarBaseHeight, nullptr, this);
    QSize t(0, d->stack->frameWidth());
    if (d->tabs->isVisibleTo(const_cast<TabWidget *>(this))) {
        t = d->tabs->sizeHint();
        if (documentMode()) {
            if (tabPosition() == East || tabPosition() == West) {
                t.setHeight(height());
            } else {
                t.setWidth(width());
            }
        }
    }

    if (d->rightCornerWidget) {
        const QSize rightCornerSizeHint = d->rightCornerWidget->sizeHint();
        const QSize bounds(rightCornerSizeHint.width(), t.height() - exth);
        option->rightCornerWidgetSize = rightCornerSizeHint.boundedTo(bounds);
    } else {
        option->rightCornerWidgetSize = QSize(0, 0);
    }

    if (d->leftCornerWidget) {
        const QSize leftCornerSizeHint = d->leftCornerWidget->sizeHint();
        const QSize bounds(leftCornerSizeHint.width(), t.height() - exth);
        option->leftCornerWidgetSize = leftCornerSizeHint.boundedTo(bounds);
    } else {
        option->leftCornerWidgetSize = QSize(0, 0);
    }

    option->tabBarSize = t;

    QRect selectedTabRect = tabBar()->tabRect(tabBar()->currentIndex());
    selectedTabRect.moveTopLeft(selectedTabRect.topLeft() + option->tabBarRect.topLeft());
    option->selectedTabRect = selectedTabRect;
}

TabWidget::TabWidget(QWebEngineProfile *profile, QWidget *parent)
    : QWidget(*new TabWidgetPrivate, parent, { }),
      m_profile(profile)
{
    Q_D(TabWidget);
    d->init();

    QPushButton *bPlus = new QPushButton(QIcon(QStringLiteral("icons:plus.png")), "", d->tabs);

    _insertTab(-1, new QWidget(), QIcon(), "");

    d->tabs->setTabButton(0, QTabBar::RightSide, bPlus);
    d->tabs->setTabEnabled(0, false);

    connect(bPlus, SIGNAL(released()), this, SIGNAL(plusClicked()));
    connect(d->tabs, &QTabBar::tabCloseRequested, this, &TabWidget::closeTab);

    if (profile->isOffTheRecord()) {
        QLabel *icon = new QLabel(this);
        QPixmap pixmap(QStringLiteral("icons:ninja.png"));
        icon->setPixmap(pixmap.scaledToHeight(d->tabs->height()));
        setStyleSheet(QStringLiteral("TabWidget::tab-bar { left: %1px; }").
                      arg(icon->pixmap(Qt::ReturnByValue).width()));
    }

}


TabWidget::~TabWidget()
{
}


TabView * TabWidget::createTab()
{
    TabView *tabView = new TabView(m_profile, this);

    int index = addTab(tabView, tr("New tab"));

    connect(tabView, &TabView::titleChanged, [this, index](const QString &title) {
        setTabText(index, title);
        emit titleChanged(title);
    });

    connect(tabView, &TabView::iconChanged, [this, index](const QIcon &icon) {
        setTabIcon(index, icon);
    });

    return tabView;
}


TabView * TabWidget::createActiveTab()
{
    TabView *tabView = createTab();
    setCurrentWidget(tabView);
    return tabView;
}


void TabWidget::closeTab(int index)
{
    Q_D(TabWidget);

    if (QWidget *view = widget(index)) {
        bool hasFocus = index == currentIndex();
        removeTab(index);
        if (hasFocus && d->tabs->count() > 1) {
            index = index > 0 ? index - 1 : d->tabs->count() - 2;
            setCurrentWidget(widget(index));
            currentWidget()->setFocus();
        }
        if (count() == 1)
            createActiveTab();
        view->deleteLater();
    }
}


int TabWidget::addTab(QWidget *child, const QString &label)
{
    return insertTab(-1, child, label);
}


int TabWidget::addTab(QWidget *child, const QIcon& icon, const QString &label)
{
    return insertTab(-1, child, icon, label);
}


int TabWidget::insertTab(int index, QWidget *w, const QString &label)
{
    return insertTab(index, w, QIcon(), label);
}


int TabWidget::insertTab(int index, QWidget *w, const QIcon& icon, const QString &label)
{
    Q_D(TabWidget);
    if(!w)
        return -1;
    if(index == -1 || index == d->tabs->count()) {
        index = d->tabs->count() - 1;
    }
    index = d->stack->insertWidget(index, w);
    d->tabs->insertTab(index, icon, label);
    setUpLayout();
    tabInserted(index);

    return index;
}

int TabWidget::_insertTab(int index, QWidget *w, const QIcon& icon, const QString &label)
{
    Q_D(TabWidget);
    if(!w)
        return -1;
    index = d->stack->insertWidget(index, w);
    d->tabs->insertTab(index, icon, label);
    setUpLayout();
    tabInserted(index);

    return index;
}

void TabWidget::setTabText(int index, const QString &label)
{
    Q_D(TabWidget);
    d->tabs->setTabText(index, label);
    d->tabs->setTabToolTip(index, label);
    setUpLayout();
}

QString TabWidget::tabText(int index) const
{
    Q_D(const TabWidget);
    return d->tabs->tabText(index);
}

void TabWidget::setTabIcon(int index, const QIcon &icon)
{
    Q_D(TabWidget);
    d->tabs->setTabIcon(index, icon);
    setUpLayout();
}

QIcon TabWidget::tabIcon(int index) const
{
    Q_D(const TabWidget);
    return d->tabs->tabIcon(index);
}

/*!
    Returns \c true if the page at position \a index is enabled; otherwise returns \c false.

    \sa setTabEnabled(), QWidget::isEnabled()
*/

bool TabWidget::isTabEnabled(int index) const
{
    Q_D(const TabWidget);
    return d->tabs->isTabEnabled(index);
}


void TabWidget::setTabEnabled(int index, bool enable)
{
    Q_D(TabWidget);
    d->tabs->setTabEnabled(index, enable);
    if (QWidget *widget = d->stack->widget(index))
        widget->setEnabled(enable);
}

bool TabWidget::isTabVisible(int index) const
{
    Q_D(const TabWidget);
    return d->tabs->isTabVisible(index);
}


void TabWidget::setTabVisible(int index, bool visible)
{
    Q_D(TabWidget);
    QWidget *widget = d->stack->widget(index);
    bool currentVisible = d->tabs->isTabVisible(d->tabs->currentIndex());
    d->tabs->setTabVisible(index, visible);
    if (!visible) {
        if (widget)
            widget->setVisible(false);
    } else if (!currentVisible) {
        setCurrentIndex(index);
        if (widget)
            widget->setVisible(true);
    }
    setUpLayout();
}

void TabWidget::setCornerWidget(QWidget * widget, Qt::Corner corner)
{
    Q_D(TabWidget);
    if (widget && widget->parentWidget() != this)
        widget->setParent(this);

    if (corner & Qt::TopRightCorner) {
        if (d->rightCornerWidget)
            d->rightCornerWidget->hide();
        d->rightCornerWidget = widget;
    } else {
        if (d->leftCornerWidget)
            d->leftCornerWidget->hide();
        d->leftCornerWidget = widget;
    }
    setUpLayout();
}

QWidget * TabWidget::cornerWidget(Qt::Corner corner) const
{
    Q_D(const TabWidget);
    if (corner & Qt::TopRightCorner)
        return d->rightCornerWidget;
    return d->leftCornerWidget;
}

void TabWidget::removeTab(int index)
{
    Q_D(TabWidget);

    if (QWidget *w = d->stack->widget(index))
        d->stack->removeWidget(w);
}


QWidget * TabWidget::currentWidget() const
{
    Q_D(const TabWidget);
    return d->stack->currentWidget();
}

void TabWidget::setCurrentWidget(QWidget *widget)
{
    Q_D(const TabWidget);
    d->tabs->setCurrentIndex(indexOf(widget));
}



int TabWidget::currentIndex() const
{
    Q_D(const TabWidget);
    return d->tabs->currentIndex();
}

void TabWidget::setCurrentIndex(int index)
{
    Q_D(TabWidget);
    d->tabs->setCurrentIndex(index);
}


int TabWidget::indexOf(QWidget* w) const
{
    Q_D(const TabWidget);
    return d->stack->indexOf(w);
}


void TabWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    setUpLayout();
}

void TabWidget::setTabBar(QTabBar* tb)
{
    Q_D(TabWidget);
    Q_ASSERT(tb);

    if (tb->parentWidget() != this) {
        tb->setParent(this);
        tb->show();
    }
    delete d->tabs;
    d->tabs = tb;
    setFocusProxy(d->tabs);
    connect(d->tabs, SIGNAL(currentChanged(int)),
            this, SLOT(_q_showTab(int)));
    connect(d->tabs, SIGNAL(tabMoved(int,int)),
            this, SLOT(_q_tabMoved(int,int)));
    connect(d->tabs, SIGNAL(tabBarClicked(int)),
            this, SIGNAL(tabBarClicked(int)));
    connect(d->tabs, SIGNAL(tabBarDoubleClicked(int)),
            this, SIGNAL(tabBarDoubleClicked(int)));
    if (d->tabs->tabsClosable())
        connect(d->tabs, SIGNAL(tabCloseRequested(int)),
                this, SIGNAL(tabCloseRequested(int)));
    tb->setExpanding(!documentMode());
    setUpLayout();
}


QTabBar* TabWidget::tabBar() const
{
    Q_D(const TabWidget);
    return d->tabs;
}


void TabWidget::setToolBar(QToolBar* tb)
{
    Q_D(TabWidget);
    Q_ASSERT(tb);

    if (tb->parentWidget() != this) {
        tb->setParent(this);
        tb->show();
    }
//    delete d->toolBar;
    d->toolBar = tb;
    setUpLayout();
}

TabView *TabWidget::currentView()
{
    return qobject_cast<TabView*>(currentWidget());
}

QToolBar* TabWidget::toolBar() const
{
    Q_D(const TabWidget);
    return d->toolBar;
}



void TabWidgetPrivate::_q_showTab(int index)
{
    Q_Q(TabWidget);

    if (index < stack->count() && index >= 0)
        stack->setCurrentIndex(index);

    emit q->currentChanged(index);
    emit q->titleChanged(q->tabText(index));
}

void TabWidgetPrivate::_q_removeTab(int index)
{
    Q_Q(TabWidget);
    tabs->removeTab(index);
    q->setUpLayout();
    q->tabRemoved(index);
}

void TabWidgetPrivate::_q_tabMoved(int from, int to)
{
    const QSignalBlocker blocker(stack);
    QWidget *w = stack->widget(from);
    stack->removeWidget(w);
    stack->insertWidget(to, w);
}

void TabWidget::setUpLayout(bool onlyCheck)
{
    Q_D(TabWidget);
    if (onlyCheck && !d->dirty)
        return; // nothing to do

    if (!isVisible()) {
        // this must be done immediately, because QWidgetItem relies on it (even if !isVisible())
        QStyleOptionTabWidgetFrame basicOption;
        d->initBasicStyleOption(&basicOption);
        d->setLayoutItemMargins(QStyle::SE_TabWidgetLayoutItem, &basicOption);
        d->dirty = true;
        return; // we'll do it later
    }

    QStyleOptionTabWidgetFrame option;
    initStyleOption(&option);
    d->setLayoutItemMargins(QStyle::SE_TabWidgetLayoutItem, &option);

    QRect tabRect = style()->subElementRect(QStyle::SE_TabWidgetTabBar, &option, this);
    d->panelRect = style()->subElementRect(QStyle::SE_TabWidgetTabPane, &option, this);
    QRect contentsRect = style()->subElementRect(QStyle::SE_TabWidgetTabContents, &option, this);
    QRect leftCornerRect = style()->subElementRect(QStyle::SE_TabWidgetLeftCorner, &option, this);
    QRect rightCornerRect = style()->subElementRect(QStyle::SE_TabWidgetRightCorner, &option, this);

    d->tabs->setGeometry(tabRect);

    QRect toolRect = d->toolBar->geometry();

    d->toolBar->setGeometry(QRect(contentsRect.x(), tabRect.height(), contentsRect.width(), toolRect.height()));
    d->stack->setGeometry(QRect(contentsRect.x(), tabRect.height() + toolRect.height(), contentsRect.width(), contentsRect.height() - toolRect.height()));

    if (d->leftCornerWidget)
        d->leftCornerWidget->setGeometry(leftCornerRect);
    if (d->rightCornerWidget)
        d->rightCornerWidget->setGeometry(rightCornerRect);

    if (!onlyCheck)
        update();
    updateGeometry();
}

static inline QSize basicSize(
    bool horizontal, const QSize &lc, const QSize &rc, const QSize &s, const QSize &t)
{
    return horizontal
        ? QSize(qMax(s.width(), t.width() + rc.width() + lc.width()),
                s.height() + (qMax(rc.height(), qMax(lc.height(), t.height()))))
        : QSize(s.width() + (qMax(rc.width(), qMax(lc.width(), t.width()))),
                qMax(s.height(), t.height() + rc.height() + lc.height()));
}

QSize TabWidget::sizeHint() const
{
    Q_D(const TabWidget);
    QSize lc(0, 0), rc(0, 0);
    QStyleOptionTabWidgetFrame opt;
    initStyleOption(&opt);
    opt.state = QStyle::State_None;

    if (d->leftCornerWidget)
        lc = d->leftCornerWidget->sizeHint();
    if(d->rightCornerWidget)
        rc = d->rightCornerWidget->sizeHint();
    if (!d->dirty) {
        TabWidget *that = const_cast<TabWidget*>(this);
        that->setUpLayout(true);
    }
    QSize s;
    for (int i=0; i< d->stack->count(); ++i) {
        if (const QWidget* w = d->stack->widget(i)) {
            if (d->tabs->isTabVisible(i))
                s = s.expandedTo(w->sizeHint());
        }
    }
    QSize t;
    if (!d->isAutoHidden()) {
        t = d->tabs->sizeHint();
        if (usesScrollButtons())
            t = t.boundedTo(QSize(200,200));
        else
            t = t.boundedTo(QApplication::primaryScreen()->virtualGeometry().size());
    }

    QSize sz = basicSize(d->pos == North || d->pos == South, lc, rc, s, t);

    return style()->sizeFromContents(QStyle::CT_TabWidget, &opt, sz, this);
}


QSize TabWidget::minimumSizeHint() const
{
    Q_D(const TabWidget);
    QSize lc(0, 0), rc(0, 0);

    if(d->leftCornerWidget)
        lc = d->leftCornerWidget->minimumSizeHint();
    if(d->rightCornerWidget)
        rc = d->rightCornerWidget->minimumSizeHint();
    if (!d->dirty) {
        TabWidget *that = const_cast<TabWidget*>(this);
        that->setUpLayout(true);
    }
    QSize s(d->stack->minimumSizeHint());
    QSize t;
    if (!d->isAutoHidden())
        t = d->tabs->minimumSizeHint();

    QSize sz = basicSize(d->pos == North || d->pos == South, lc, rc, s, t);

    QStyleOptionTabWidgetFrame opt;
    initStyleOption(&opt);
    opt.palette = palette();
    opt.state = QStyle::State_None;
    return style()->sizeFromContents(QStyle::CT_TabWidget, &opt, sz, this);
}

int TabWidget::heightForWidth(int width) const
{
    Q_D(const TabWidget);
    QStyleOptionTabWidgetFrame opt;
    initStyleOption(&opt);
    opt.state = QStyle::State_None;

    QSize zero(0,0);
    const QSize padding = style()->sizeFromContents(QStyle::CT_TabWidget, &opt, zero, this);

    QSize lc(0, 0), rc(0, 0);
    if (d->leftCornerWidget)
        lc = d->leftCornerWidget->sizeHint();
    if(d->rightCornerWidget)
        rc = d->rightCornerWidget->sizeHint();
    if (!d->dirty) {
        TabWidget *that = const_cast<TabWidget*>(this);
        that->setUpLayout(true);
    }
    QSize t;
    if (!d->isAutoHidden()) {
        t = d->tabs->sizeHint();
        if (usesScrollButtons())
            t = t.boundedTo(QSize(200,200));
        else
            t = t.boundedTo(QApplication::primaryScreen()->virtualSize());
    }

    const bool tabIsHorizontal = (d->pos == North || d->pos == South);
    const int contentsWidth = width - padding.width();
    int stackWidth = contentsWidth;
    if (!tabIsHorizontal)
        stackWidth -= qMax(t.width(), qMax(lc.width(), rc.width()));

    int stackHeight = d->stack->heightForWidth(stackWidth);
    QSize s(stackWidth, stackHeight);

    QSize contentSize = basicSize(tabIsHorizontal, lc, rc, s, t);
    return (contentSize + padding).height();
}


void TabWidget::showEvent(QShowEvent *)
{
    setUpLayout();
}

void TabWidgetPrivate::updateTabBarPosition()
{
    Q_Q(TabWidget);
    switch (pos) {
    case TabWidget::North:
        tabs->setShape(shape == TabWidget::Rounded ? QTabBar::RoundedNorth
                                                    : QTabBar::TriangularNorth);
        break;
    case TabWidget::South:
        tabs->setShape(shape == TabWidget::Rounded ? QTabBar::RoundedSouth
                                                    : QTabBar::TriangularSouth);
        break;
    case TabWidget::West:
        tabs->setShape(shape == TabWidget::Rounded ? QTabBar::RoundedWest
                                                    : QTabBar::TriangularWest);
        break;
    case TabWidget::East:
        tabs->setShape(shape == TabWidget::Rounded ? QTabBar::RoundedEast
                                                    : QTabBar::TriangularEast);
        break;
    }
    q->setUpLayout();
}

TabWidget::TabPosition TabWidget::tabPosition() const
{
    Q_D(const TabWidget);
    return d->pos;
}

void TabWidget::setTabPosition(TabPosition pos)
{
    Q_D(TabWidget);
    if (d->pos == pos)
        return;
    d->pos = pos;
    d->updateTabBarPosition();
}

bool TabWidget::tabsClosable() const
{
    return tabBar()->tabsClosable();
}

void TabWidget::setTabsClosable(bool closeable)
{
    if (tabsClosable() == closeable)
        return;

    tabBar()->setTabsClosable(closeable);
    if (closeable)
        connect(tabBar(), SIGNAL(tabCloseRequested(int)),
                this, SIGNAL(tabCloseRequested(int)));
    else
        disconnect(tabBar(), SIGNAL(tabCloseRequested(int)),
                  this, SIGNAL(tabCloseRequested(int)));
    setUpLayout();
}


bool TabWidget::isMovable() const
{
    return tabBar()->isMovable();
}

void TabWidget::setMovable(bool movable)
{
    tabBar()->setMovable(movable);
}


TabWidget::TabShape TabWidget::tabShape() const
{
    Q_D(const TabWidget);
    return d->shape;
}

void TabWidget::setTabShape(TabShape s)
{
    Q_D(TabWidget);
    if (d->shape == s)
        return;
    d->shape = s;
    d->updateTabBarPosition();
}

bool TabWidget::event(QEvent *ev)
{
    if (ev->type() == QEvent::LayoutRequest)
        setUpLayout();
    return QWidget::event(ev);
}

void TabWidget::changeEvent(QEvent *ev)
{
    if (ev->type() == QEvent::StyleChange
#ifdef Q_OS_MAC
            || ev->type() == QEvent::MacSizeChange
#endif
            )
        setUpLayout();
    QWidget::changeEvent(ev);
}


void TabWidget::keyPressEvent(QKeyEvent *e)
{
    Q_D(TabWidget);
    if (((e->key() == Qt::Key_Tab || e->key() == Qt::Key_Backtab) &&
          count() > 1 && e->modifiers() & Qt::ControlModifier)
#ifdef QT_KEYPAD_NAVIGATION
          || QApplicationPrivate::keypadNavigationEnabled() && (e->key() == Qt::Key_Left || e->key() == Qt::Key_Right) && count() > 1
#endif
       ) {
        int pageCount = d->tabs->count();
        int page = currentIndex();
        int dx = (e->key() == Qt::Key_Backtab || e->modifiers() & Qt::ShiftModifier) ? -1 : 1;
#ifdef QT_KEYPAD_NAVIGATION
        if (QApplicationPrivate::keypadNavigationEnabled() && (e->key() == Qt::Key_Left || e->key() == Qt::Key_Right))
            dx = e->key() == (isRightToLeft() ? Qt::Key_Right : Qt::Key_Left) ? -1 : 1;
#endif
        for (int pass = 0; pass < pageCount; ++pass) {
            page+=dx;
            if (page < 0
#ifdef QT_KEYPAD_NAVIGATION
                && !e->isAutoRepeat()
#endif
               ) {
                page = count() - 1;
            } else if (page >= pageCount
#ifdef QT_KEYPAD_NAVIGATION
                       && !e->isAutoRepeat()
#endif
                      ) {
                page = 0;
            }
            if (d->tabs->isTabEnabled(page)) {
                setCurrentIndex(page);
                break;
            }
        }
        if (!QApplication::focusWidget())
            d->tabs->setFocus();
    } else {
        e->ignore();
    }
}

QWidget *TabWidget::widget(int index) const
{
    Q_D(const TabWidget);
    return d->stack->widget(index);
}


int TabWidget::count() const
{
    Q_D(const TabWidget);
    return d->tabs->count();
}

#ifndef QT_NO_TOOLTIP
void TabWidget::setTabToolTip(int index, const QString & tip)
{
    Q_D(TabWidget);
    d->tabs->setTabToolTip(index, tip);
}

QString TabWidget::tabToolTip(int index) const
{
    Q_D(const TabWidget);
    return d->tabs->tabToolTip(index);
}
#endif // QT_NO_TOOLTIP

#if QT_CONFIG(whatsthis)
void TabWidget::setTabWhatsThis(int index, const QString &text)
{
    Q_D(TabWidget);
    d->tabs->setTabWhatsThis(index, text);
}

QString TabWidget::tabWhatsThis(int index) const
{
    Q_D(const TabWidget);
    return d->tabs->tabWhatsThis(index);
}
#endif // QT_CONFIG(whatsthis)

void TabWidget::tabInserted(int index)
{
    Q_UNUSED(index)
}

void TabWidget::tabRemoved(int index)
{
    Q_UNUSED(index)
}

void TabWidget::paintEvent(QPaintEvent *)
{
    Q_D(TabWidget);
    if (documentMode()) {
        QStylePainter p(this, tabBar());
        if (QWidget *w = cornerWidget(Qt::TopLeftCorner)) {
            QStyleOptionTabBarBase opt;
            QTabBarPrivate::initStyleBaseOption(&opt, tabBar(), w->size());
            opt.rect.moveLeft(w->x() + opt.rect.x());
            opt.rect.moveTop(w->y() + opt.rect.y());
            p.drawPrimitive(QStyle::PE_FrameTabBarBase, opt);
        }
        if (QWidget *w = cornerWidget(Qt::TopRightCorner)) {
            QStyleOptionTabBarBase opt;
            QTabBarPrivate::initStyleBaseOption(&opt, tabBar(), w->size());
            opt.rect.moveLeft(w->x() + opt.rect.x());
            opt.rect.moveTop(w->y() + opt.rect.y());
            p.drawPrimitive(QStyle::PE_FrameTabBarBase, opt);
        }
        return;
    }
    QStylePainter p(this);

    QStyleOptionTabWidgetFrame opt;
    initStyleOption(&opt);
    opt.rect = d->panelRect;
    p.drawPrimitive(QStyle::PE_FrameTabWidget, opt);
}

QSize TabWidget::iconSize() const
{
    return d_func()->tabs->iconSize();
}

void TabWidget::setIconSize(const QSize &size)
{
    d_func()->tabs->setIconSize(size);
}

Qt::TextElideMode TabWidget::elideMode() const
{
    return d_func()->tabs->elideMode();
}

void TabWidget::setElideMode(Qt::TextElideMode mode)
{
    d_func()->tabs->setElideMode(mode);
}

bool TabWidget::usesScrollButtons() const
{
    return d_func()->tabs->usesScrollButtons();
}

void TabWidget::setUsesScrollButtons(bool useButtons)
{
    d_func()->tabs->setUsesScrollButtons(useButtons);
}

bool TabWidget::documentMode() const
{
    Q_D(const TabWidget);
    return d->tabs->documentMode();
}

void TabWidget::setDocumentMode(bool enabled)
{
    Q_D(TabWidget);
    d->tabs->setDocumentMode(enabled);
    d->tabs->setExpanding(!enabled);
    d->tabs->setDrawBase(enabled);
    setUpLayout();
}


bool TabWidget::tabBarAutoHide() const
{
    Q_D(const TabWidget);
    return d->tabs->autoHide();
}

void TabWidget::setTabBarAutoHide(bool enabled)
{
    Q_D(TabWidget);
    return d->tabs->setAutoHide(enabled);
}

void TabWidget::clear()
{
    // ### optimize by introduce QStackedLayout::clear()
    while (count())
        removeTab(0);
}

QT_END_NAMESPACE

#include "moc_tabwidget.cpp"
