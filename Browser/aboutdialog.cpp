#include "aboutdialog.h"
#include "qbcommon.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWebEngineProfile>
#include "browsersettings.h"
#include <QQuickWindow>


AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("About QbqBrowser"));
    setFixedWidth(400);

    auto layout = new QVBoxLayout(this);

    auto logoLabel = new QLabel(this);

    logoLabel->setTextFormat(Qt::RichText);
    logoLabel->setText("<table><tr><td><img src=icons:qbq256.png width=60 height=60></td><td style='padding-top:8px'><span style='height:1000px;font-size:40px;font-weight:bold;color:green;'>QbqBrowser</span></td></tr></table>");
    layout->addWidget(logoLabel, 0, Qt::AlignHCenter);

    QString gApiName("Unknown");
    auto gApi = QQuickWindow::graphicsApi();
    if(QSGRendererInterface::GraphicsApi::Direct3D11 == gApi) {
        gApiName = "Direct3D 11";
    } else if(QSGRendererInterface::GraphicsApi::Metal == gApi) {
        gApiName = "Metal";
    } else if(QSGRendererInterface::GraphicsApi::Null == gApi) {
        gApiName = "No graphic API";
    } else if(QSGRendererInterface::GraphicsApi::OpenGL == gApi) {
        gApiName = "OpenGL";
    } else if(QSGRendererInterface::GraphicsApi::OpenVG == gApi) {
        gApiName = "OpenVG";
    } else if(QSGRendererInterface::GraphicsApi::Vulkan == gApi) {
        gApiName = "Vulkan";
    } else if(QSGRendererInterface::GraphicsApi::Software == gApi) {
        gApiName = "Software";
    }

    QString aboutHtml;
    aboutHtml += "<div style='margin:0px 20px;'>";
    aboutHtml += tr("<p><b>Application version %1</b><br/>").arg(Qb::VERSION);
    aboutHtml += tr("<b>QtWebEngine version %1</b></p>").arg(qVersion());
    aboutHtml += QString("<p>&copy; %1 %2<br/>").arg(Qb::COPYRIGHT, Qb::AUTHOR);
    aboutHtml += QString("<a href=%1>%1</a></p>").arg(Qb::WWWADDRESS);
    aboutHtml += "<p>" + BrowserSettings::instance()->defaultUserAgent() + "</p>";
    aboutHtml += "<p>" + QString("Graphics API: ") + gApiName + "</p>";
    if(!QQuickWindow::sceneGraphBackend().isEmpty()) {
        aboutHtml += "<p>" + QString("Graphics Backend: ") + QQuickWindow::sceneGraphBackend() + "</p>";
    }
    aboutHtml += "<br></div>";

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
