#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QProcess>
#include <QKeyEvent>
#include <QFile>
#include <QIcon>
#include <QDir>
#include <QPainter>
#include <QPixmap>
#include <QStandardPaths>
#include <QTimer>
#include <QDebug>

class HoverButton : public QPushButton {
    Q_OBJECT
public:
    HoverButton(const QString& iconName, const QString& fallbackThemeIcon, QWidget* parent = nullptr)
        : QPushButton(parent)
    {
        setFixedSize(100, 100);
        setCursor(Qt::PointingHandCursor);
        setObjectName("action-btn");
        setFocusPolicy(Qt::StrongFocus);

        QStringList searchPaths;
        // 1. Current working dir assets
        searchPaths << QDir::currentPath() + "/assets/" + iconName + ".svg";
        // 2. Relative to executable (e.g. ../assets if running from build/)
        searchPaths << QCoreApplication::applicationDirPath() + "/../assets/" + iconName + ".svg";
        searchPaths << QCoreApplication::applicationDirPath() + "/assets/" + iconName + ".svg";
        // 3. User config
        searchPaths << QDir::homePath() + "/.config/pod/assets/" + iconName + ".svg";
        // 4. Embedded Resources
        searchPaths << ":/assets/" + iconName + ".svg";
        // 5. System install
        searchPaths << "/usr/share/pod/assets/" + iconName + ".svg";

        QString finalPath;
        for (const QString &path : searchPaths) {
            if (QFile::exists(path)) {
                finalPath = path;
                break;
            }
        }

        if (!finalPath.isEmpty()) {
            QPixmap basePix(finalPath);
            if (!basePix.isNull()) {
                basePix = basePix.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                m_normalIcon.addPixmap(basePix);

                QPixmap blackPix(basePix.size());
                blackPix.fill(Qt::transparent);
                QPainter painter(&blackPix);
                painter.drawPixmap(0, 0, basePix);
                painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
                painter.fillRect(blackPix.rect(), Qt::black);
                painter.end();

                m_hoverIcon.addPixmap(blackPix);

                setIcon(m_normalIcon);
                setIconSize(QSize(48, 48));
            } else {
                 setIcon(QIcon::fromTheme(fallbackThemeIcon));
                 m_useThemeFallback = true;
            }
        } else {
            setIcon(QIcon::fromTheme(fallbackThemeIcon));
            m_useThemeFallback = true;
        }
    }

    void updateVisuals() {
        if (m_useThemeFallback) return;

        if (hasFocus() || underMouse()) {
            setIcon(m_hoverIcon);
        } else {
            setIcon(m_normalIcon);
        }
    }

protected:
    void enterEvent(QEnterEvent *event) override {
        updateVisuals();
        QPushButton::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override {
        updateVisuals();
        QPushButton::leaveEvent(event);
    }

    void focusInEvent(QFocusEvent *event) override {
        updateVisuals();
        QPushButton::focusInEvent(event);
    }

    void focusOutEvent(QFocusEvent *event) override {
        updateVisuals();
        QPushButton::focusOutEvent(event);
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            animateClick();
            return;
        }
        QPushButton::keyPressEvent(event);
    }

private:
    QIcon m_normalIcon;
    QIcon m_hoverIcon;
    bool m_useThemeFallback = false;
};

class PodWindow : public QWidget {
public:
    PodWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // Window Setup
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setWindowTitle("Pod");

        // Layout
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        addButton(layout, "poweroff", "Shutdown", "system-shutdown", "systemctl poweroff");
        addButton(layout, "reboot",   "Reboot",   "system-reboot",   "systemctl reboot");
        HoverButton* lockBtn = addButton(layout, "lock",     "Lock",     "system-lock-screen", "i3lock -c 000000");
        addButton(layout, "sleep",    "Suspend",  "system-suspend",  "loginctl suspend");
        addButton(layout, "logout",   "Logout",   "system-log-out",  "i3-msg exit");

        resize(500, 100);

        // Set Default Focus to Lock
        if (lockBtn) {
            QTimer::singleShot(0, lockBtn, [lockBtn](){
                lockBtn->setFocus();
                lockBtn->updateVisuals();
            });
        }
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Escape) {
            QApplication::quit();
        }
        QWidget::keyPressEvent(event);
    }

private:
    HoverButton* addButton(QHBoxLayout *layout, const QString &iconName, const QString &tooltip, const QString &fallbackIcon, const QString &cmd) {
        HoverButton *btn = new HoverButton(iconName, fallbackIcon, this);
        btn->setToolTip(tooltip);

        connect(btn, &QPushButton::clicked, [cmd]() {
            QProcess::startDetached("/bin/bash", QStringList() << "-c" << cmd);
            QApplication::quit();
        });

        layout->addWidget(btn);
        return btn;
    }
};

void loadStyle(QApplication &app) {
    QStringList searchPaths;
    searchPaths << QDir::homePath() + "/.config/pod/style.qss";
    searchPaths << ":/style.qss";
    searchPaths << "style.qss";
    searchPaths << "../style.qss";
    searchPaths << "/usr/share/pod/style.qss";

    for (const QString &path : searchPaths) {
        if (QFile::exists(path)) {
            QFile file(path);
            if (file.open(QFile::ReadOnly)) {
                app.setStyleSheet(file.readAll());
                return;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    loadStyle(app);

    PodWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"
