#include "aboutdialog.h"
#include "qbcommon.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWebEngineProfile>
#include "browsersettings.h"

AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("About QbqBrowser"));
    setFixedWidth(400);

    auto layout = new QVBoxLayout(this);

    auto logoLabel = new QLabel(this);

    logoLabel->setTextFormat(Qt::RichText);
    logoLabel->setText("<table><tr><td><img src=icons:qbqb.svg width=60 height=60></td><td style='padding-top:8px'><span style='height:1000px;font-size:40px;font-weight:bold;color:green;'>rowser</span></td></tr></table>");
    layout->addWidget(logoLabel, 0, Qt::AlignHCenter);

    QString aboutHtml;
    aboutHtml += "<div style='margin:0px 20px;'>";
    aboutHtml += tr("<p><b>Application version %1</b><br/>").arg(Qb::VERSION);
    aboutHtml += tr("<b>QtWebEngine version %1</b></p>").arg(qVersion());
    aboutHtml += QString("<p>&copy; %1 %2<br/>").arg(Qb::COPYRIGHT, Qb::AUTHOR);
    aboutHtml += QString("<a href=%1>%1</a></p>").arg(Qb::WWWADDRESS);
    aboutHtml += "<p>" + BrowserSettings::instance()->defaultUserAgent() + "</p>";
    aboutHtml += "</div>";

    auto label = new QLabel(this);
    label->setWordWrap(true);
    label->setText(aboutHtml);
    layout->addWidget(label, 0, Qt::AlignHCenter);

    QPushButton *closeButton = new QPushButton("Close", this);
    closeButton->setFixedWidth(100);

    connect(closeButton, &QAbstractButton::clicked, this, &QWidget::close);
    layout->addWidget(closeButton, 0, Qt::AlignHCenter);

    setLayout(layout);
}
