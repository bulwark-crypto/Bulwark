// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/bulwark/bulwarkgui.h"

#ifdef Q_OS_MAC
#include "macdockiconhandler.h"
#endif

#include <qt/guiutil.h>
#include "clientmodel.h"
#include "optionsmodel.h"
#include "networkstyle.h"
#include "notificator.h"
#include "guiinterface.h"
#include "qt/bulwark/qtutils.h"
#include "qt/bulwark/defaultdialog.h"
#include "qt/bulwark/settings/settingsfaqwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QColor>
#include <QShortcut>
#include <QKeySequence>

#include "util.h"


const QString BWKGUI::DEFAULT_WALLET = "~Default";

BWKGUI::BWKGUI(const NetworkStyle* networkStyle, QWidget* parent) :
        QMainWindow(parent),
        clientModel(0){

    /* Open CSS when configured */
    this->setStyleSheet(GUIUtil::loadStyleSheet());
    this->setMinimumSize(1200, 740);
    GUIUtil::restoreWindowGeometry("nWindow", QSize(1200, 740), this);

    QString windowTitle = tr("Bulwark Core") + " - ";
#ifdef ENABLE_WALLET
    /* if compiled with wallet support, -disablewallet can still disable the wallet */
    enableWallet = !GetBoolArg("-disablewallet", false);
#else
    enableWallet = false;
#endif // ENABLE_WALLET

    if (enableWallet) {
        windowTitle += tr("Wallet");
    } else {
        windowTitle += tr("Node");
    }

    setWindowTitle(windowTitle);

#ifndef Q_OS_MAC
    QApplication::setWindowIcon(networkStyle->getAppIcon());
    setWindowIcon(networkStyle->getAppIcon());
#else
    MacDockIconHandler::instance()->setIcon(networkStyle->getAppIcon());
#endif




#ifdef ENABLE_WALLET
    // Create wallet frame
    if(enableWallet){

        QFrame* centralWidget = new QFrame(this);
        this->setMinimumWidth(1200);
        this->setMinimumHeight(740);
        QHBoxLayout* centralWidgetLayouot = new QHBoxLayout();
        centralWidget->setLayout(centralWidgetLayouot);
        centralWidgetLayouot->setContentsMargins(0,0,0,0);
        centralWidgetLayouot->setSpacing(0);

        centralWidget->setProperty("cssClass", "container");
        centralWidget->setStyleSheet("padding:0px; border:none; margin:0px;");

        // First the nav
        navMenu = new NavMenuWidget(this);
        centralWidgetLayouot->addWidget(navMenu);

        this->setCentralWidget(centralWidget);
        this->setContentsMargins(0,0,0,0);

        QFrame *container = new QFrame(centralWidget);
        container->setContentsMargins(0,0,0,0);
        centralWidgetLayouot->addWidget(container);

        // Then topbar + the stackedWidget
        QVBoxLayout *baseScreensContainer = new QVBoxLayout(this);
        baseScreensContainer->setMargin(0);
        baseScreensContainer->setSpacing(0);
        baseScreensContainer->setContentsMargins(0,0,0,0);
        container->setLayout(baseScreensContainer);

        // Insert the topbar
        topBar = new TopBar(this);
        topBar->setContentsMargins(0,0,0,0);
        baseScreensContainer->addWidget(topBar);

        // Now stacked widget
        stackedContainer = new QStackedWidget(this);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        stackedContainer->setSizePolicy(sizePolicy);
        stackedContainer->setContentsMargins(0,0,0,0);
        baseScreensContainer->addWidget(stackedContainer);

        // Init
        dashboard = new DashboardWidget(this);
        sendWidget = new SendWidget(this);
        receiveWidget = new ReceiveWidget(this);
        addressesWidget = new AddressesWidget(this);
        privacyWidget = new PrivacyWidget(this);
        masterNodesWidget = new MasterNodesWidget(this);
        settingsWidget = new SettingsWidget(this);

        // Add to parent
        stackedContainer->addWidget(dashboard);
        stackedContainer->addWidget(sendWidget);
        stackedContainer->addWidget(receiveWidget);
        stackedContainer->addWidget(addressesWidget);
        stackedContainer->addWidget(privacyWidget);
        stackedContainer->addWidget(masterNodesWidget);
        stackedContainer->addWidget(settingsWidget);
        stackedContainer->setCurrentWidget(dashboard);

    } else
#endif
    {
        // When compiled without wallet or -disablewallet is provided,
        // the central widget is the rpc console.
        rpcConsole = new RPCConsole(enableWallet ? this : 0);
        setCentralWidget(rpcConsole);
    }

    // Create actions for the toolbar, menu bar and tray/dock icon
    createActions(networkStyle);

    // Create system tray icon and notification
    createTrayIcon(networkStyle);

    // Connect events
    connectActions();

    // TODO: Add event filter??
    // // Install event filter to be able to catch status tip events (QEvent::StatusTip)
    //    this->installEventFilter(this);

    // Subscribe to notifications from core
    subscribeToCoreSignals();

}

void BWKGUI::createActions(const NetworkStyle* networkStyle){
    toggleHideAction = new QAction(networkStyle->getAppIcon(), tr("&Show / Hide"), this);
    toggleHideAction->setStatusTip(tr("Show or hide the main Window"));

    quitAction = new QAction(QIcon(":/icons/quit"), tr("E&xit"), this);
    quitAction->setStatusTip(tr("Quit application"));
    quitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    quitAction->setMenuRole(QAction::QuitRole);

    connect(toggleHideAction, SIGNAL(triggered()), this, SLOT(toggleHidden()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

/**
 * Here add every event connection
 */
void BWKGUI::connectActions() {
    QShortcut *consoleShort = new QShortcut(this);
    consoleShort->setKey(QKeySequence(SHORT_KEY + Qt::Key_C));
    connect(consoleShort, &QShortcut::activated, [this](){
        navMenu->selectSettings();
        settingsWidget->showDebugConsole();
        goToSettings();
    });
    connect(topBar, &TopBar::showHide, this, &BWKGUI::showHide);
    connect(topBar, &TopBar::themeChanged, this, &BWKGUI::changeTheme);
    connect(settingsWidget, &SettingsWidget::showHide, this, &BWKGUI::showHide);
    connect(sendWidget, &SendWidget::showHide, this, &BWKGUI::showHide);
    connect(receiveWidget, &ReceiveWidget::showHide, this, &BWKGUI::showHide);
    connect(addressesWidget, &AddressesWidget::showHide, this, &BWKGUI::showHide);
    connect(privacyWidget, &PrivacyWidget::showHide, this, &BWKGUI::showHide);
    connect(masterNodesWidget, &MasterNodesWidget::showHide, this, &BWKGUI::showHide);
    connect(masterNodesWidget, &MasterNodesWidget::execDialog, this, &BWKGUI::execDialog);
    connect(settingsWidget, &SettingsWidget::execDialog, this, &BWKGUI::execDialog);
}


void BWKGUI::createTrayIcon(const NetworkStyle* networkStyle) {
#ifndef Q_OS_MAC
    trayIcon = new QSystemTrayIcon(this);
    QString toolTip = tr("Bulwark Core client") + " " + networkStyle->getTitleAddText();
    trayIcon->setToolTip(toolTip);
    trayIcon->setIcon(networkStyle->getAppIcon());
    trayIcon->hide();
#endif
    notificator = new Notificator(QApplication::applicationName(), trayIcon, this);
}

//
BWKGUI::~BWKGUI() {
    // Unsubscribe from notifications from core
    unsubscribeFromCoreSignals();

    GUIUtil::saveWindowGeometry("nWindow", this);
    if (trayIcon) // Hide tray icon, as deleting will let it linger until quit (on Ubuntu)
        trayIcon->hide();
#ifdef Q_OS_MAC
    MacDockIconHandler::cleanup();
#endif
}


/** Get restart command-line parameters and request restart */
void BWKGUI::handleRestart(QStringList args){
    if (!ShutdownRequested())
        emit requestedRestart(args);
}


void BWKGUI::setClientModel(ClientModel* clientModel) {
    this->clientModel = clientModel;
    if(this->clientModel) {

        // Create system tray menu (or setup the dock menu) that late to prevent users from calling actions,
        // while the client has not yet fully loaded
        createTrayIconMenu();

        topBar->setClientModel(clientModel);
        dashboard->setClientModel(clientModel);
        sendWidget->setClientModel(clientModel);
        settingsWidget->setClientModel(clientModel);

        // Receive and report messages from client model
        connect(clientModel, SIGNAL(message(QString, QString, unsigned int)), this, SLOT(message(QString, QString, unsigned int)));
        connect(topBar, SIGNAL(walletSynced(bool)), dashboard, SLOT(walletSynced(bool)));

        // Get restart command-line parameters and handle restart
        connect(settingsWidget, &SettingsWidget::handleRestart, [this](QStringList arg){handleRestart(arg);});

        if (rpcConsole) {
            rpcConsole->setClientModel(clientModel);
        }

        if (trayIcon) {
            trayIcon->show();
        }
    } else {
        // Disable possibility to show main window via action
        toggleHideAction->setEnabled(false);
        if (trayIconMenu) {
            // Disable context menu on tray icon
            trayIconMenu->clear();
        }
    }
}

void BWKGUI::createTrayIconMenu() {
#ifndef Q_OS_MAC
    // return if trayIcon is unset (only on non-Mac OSes)
    if (!trayIcon)
        return;

    trayIconMenu = new QMenu(this);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
#else
    // Note: On Mac, the dock icon is used to provide the tray's functionality.
    MacDockIconHandler* dockIconHandler = MacDockIconHandler::instance();
    dockIconHandler->setMainWindow((QMainWindow*)this);
    trayIconMenu = dockIconHandler->dockMenu();
#endif

    // Configuration of the tray icon (or dock icon) icon menu
    trayIconMenu->addAction(toggleHideAction);
    trayIconMenu->addSeparator();

#ifndef Q_OS_MAC // This is built-in on Mac
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
#endif
}

#ifndef Q_OS_MAC
void BWKGUI::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        // Click on system tray icon triggers show/hide of the main window
        toggleHidden();
    }
}
#endif

void BWKGUI::changeEvent(QEvent* e)
{
    QMainWindow::changeEvent(e);
#ifndef Q_OS_MAC // Ignored on Mac
    if (e->type() == QEvent::WindowStateChange) {
        if (clientModel && clientModel->getOptionsModel() && clientModel->getOptionsModel()->getMinimizeToTray()) {
            QWindowStateChangeEvent* wsevt = static_cast<QWindowStateChangeEvent*>(e);
            if (!(wsevt->oldState() & Qt::WindowMinimized) && isMinimized()) {
                QTimer::singleShot(0, this, SLOT(hide()));
                e->ignore();
            }
        }
    }
#endif
}

void BWKGUI::closeEvent(QCloseEvent* event)
{
#ifndef Q_OS_MAC // Ignored on Mac
    if (clientModel && clientModel->getOptionsModel()) {
        if (!clientModel->getOptionsModel()->getMinimizeOnClose()) {
            QApplication::quit();
        }
    }
#endif
    QMainWindow::closeEvent(event);
}


void BWKGUI::messageInfo(const QString& text){
    if(!this->snackBar) this->snackBar = new SnackBar(this, this);
    this->snackBar->setText(text);
    this->snackBar->resize(this->width(), snackBar->height());
    openDialog(this->snackBar, this);
}


void BWKGUI::message(const QString& title, const QString& message, unsigned int style, bool* ret) {
    QString strTitle =  tr("Bulwark Core"); // default title
    // Default to information icon
    int nNotifyIcon = Notificator::Information;

    QString msgType;

    // Prefer supplied title over style based title
    if (!title.isEmpty()) {
        msgType = title;
    } else {
        switch (style) {
            case CClientUIInterface::MSG_ERROR:
                msgType = tr("Error");
                break;
            case CClientUIInterface::MSG_WARNING:
                msgType = tr("Warning");
                break;
            case CClientUIInterface::MSG_INFORMATION:
                msgType = tr("Information");
                break;
            default:
                msgType = tr("System Message");
                break;
        }
    }

    // Check for error/warning icon
    if (style & CClientUIInterface::ICON_ERROR) {
        nNotifyIcon = Notificator::Critical;
    } else if (style & CClientUIInterface::ICON_WARNING) {
        nNotifyIcon = Notificator::Warning;
    }

    // Display message
    if (style & CClientUIInterface::MODAL) {
        // Check for buttons, use OK as default, if none was supplied
        int r = 0;
        showNormalIfMinimized();
        if(style & CClientUIInterface::BTN_MASK){
            r = openStandardDialog(
                    (title.isEmpty() ? strTitle : title), message, "OK", "CANCEL"
                );
        }else{
            r = openStandardDialog((title.isEmpty() ? strTitle : title), message, "OK");
        }
        if (ret != NULL)
            *ret = r;
    } else if(style & CClientUIInterface::MSG_INFORMATION_SNACK){
        messageInfo(message);
    }else {
        // Append title to "Bulwark - "
        if (!msgType.isEmpty())
            strTitle += " - " + msgType;
        notificator->notify((Notificator::Class) nNotifyIcon, strTitle, message);
    }
}

bool BWKGUI::openStandardDialog(QString title, QString body, QString okBtn, QString cancelBtn){
    DefaultDialog *dialog;
    if (isVisible()) {
        showHide(true);
        dialog = new DefaultDialog(this);
        dialog->setText(title, body, okBtn, cancelBtn);
        dialog->adjustSize();
        openDialogWithOpaqueBackground(dialog, this);
    } else {
        dialog = new DefaultDialog();
        dialog->setText(title, body, okBtn);
        dialog->setWindowTitle(tr("Bulwark Core"));
        dialog->adjustSize();
        dialog->raise();
        dialog->exec();
    }
    bool ret = dialog->isOk;
    dialog->deleteLater();
    return ret;
}


void BWKGUI::showNormalIfMinimized(bool fToggleHidden) {
    if (!clientModel)
        return;
    // activateWindow() (sometimes) helps with keyboard focus on Windows
    if (isHidden()) {
        show();
        activateWindow();
    } else if (isMinimized()) {
        showNormal();
        activateWindow();
    } else if (GUIUtil::isObscured(this)) {
        raise();
        activateWindow();
    } else if (fToggleHidden)
        hide();
}

void BWKGUI::toggleHidden() {
    showNormalIfMinimized(true);
}

void BWKGUI::detectShutdown() {
    if (ShutdownRequested()) {
        if (rpcConsole)
            rpcConsole->hide();
        qApp->quit();
    }
}

void BWKGUI::goToDashboard(){
    if(stackedContainer->currentWidget() != dashboard){
        stackedContainer->setCurrentWidget(dashboard);
        topBar->showBottom();
    }
}

void BWKGUI::goToSend(){
    showTop(sendWidget);
}

void BWKGUI::goToAddresses(){
    showTop(addressesWidget);
}

void BWKGUI::goToPrivacy(){
    showTop(privacyWidget);
}

void BWKGUI::goToMasterNodes(){
    showTop(masterNodesWidget);
}

void BWKGUI::goToSettings(){
    showTop(settingsWidget);
}

void BWKGUI::goToReceive(){
    showTop(receiveWidget);
}

void BWKGUI::showTop(QWidget* view){
    if(stackedContainer->currentWidget() != view){
        stackedContainer->setCurrentWidget(view);
        topBar->showTop();
    }
}

void BWKGUI::changeTheme(bool isLightTheme){

    QString css = GUIUtil::loadStyleSheet();
    this->setStyleSheet(css);

    // Notify
    emit themeChanged(isLightTheme, css);

    // Update style
    updateStyle(this);
}

void BWKGUI::resizeEvent(QResizeEvent* event){
    // Parent..
    QMainWindow::resizeEvent(event);
    // background
    showHide(opEnabled);
    // Notify
    emit windowResizeEvent(event);
}

bool BWKGUI::execDialog(QDialog *dialog, int xDiv, int yDiv){
    return openDialogWithOpaqueBackgroundY(dialog, this);
}

void BWKGUI::showHide(bool show){
    if(!op) op = new QLabel(this);
    if(!show){
        op->setVisible(false);
        opEnabled = false;
    }else{
        QColor bg("#000000");
        bg.setAlpha(200);
        if(!isLightTheme()){
            bg = QColor("#00000000");
            bg.setAlpha(150);
        }

        QPalette palette;
        palette.setColor(QPalette::Window, bg);
        op->setAutoFillBackground(true);
        op->setPalette(palette);
        op->setWindowFlags(Qt::CustomizeWindowHint);
        op->move(0,0);
        op->show();
        op->activateWindow();
        op->resize(width(), height());
        op->setVisible(true);
        opEnabled = true;
    }
}

int BWKGUI::getNavWidth(){
    return this->navMenu->width();
}

void BWKGUI::openFAQ(int section){
    showHide(true);
    SettingsFaqWidget* dialog = new SettingsFaqWidget(this);
    if (section > 0) dialog->setSection(section);
    openDialogWithOpaqueBackgroundFullScreen(dialog, this);
    dialog->deleteLater();
}


#ifdef ENABLE_WALLET
bool BWKGUI::addWallet(const QString& name, WalletModel* walletModel)
{
    // Single wallet supported for now..
    if(!stackedContainer || !clientModel || !walletModel)
        return false;

    // todo: show out of sync warning..
    // todo: complete this next method
    //connect(walletView, SIGNAL(showNormalIfMinimized()), gui, SLOT(showNormalIfMinimized()));

    // set the model for every view
    dashboard->setWalletModel(walletModel);
    topBar->setWalletModel(walletModel);
    receiveWidget->setWalletModel(walletModel);
    sendWidget->setWalletModel(walletModel);
    addressesWidget->setWalletModel(walletModel);
    privacyWidget->setWalletModel(walletModel);
    masterNodesWidget->setWalletModel(walletModel);
    settingsWidget->setWalletModel(walletModel);

    // Connect actions..
    connect(privacyWidget, &PrivacyWidget::message, this, &BWKGUI::message);
    connect(masterNodesWidget, &MasterNodesWidget::message, this, &BWKGUI::message);
    connect(topBar, &TopBar::message, this, &BWKGUI::message);
    connect(sendWidget, &SendWidget::message,this, &BWKGUI::message);
    connect(receiveWidget, &ReceiveWidget::message,this, &BWKGUI::message);
    connect(addressesWidget, &AddressesWidget::message,this, &BWKGUI::message);
    connect(settingsWidget, &SettingsWidget::message, this, &BWKGUI::message);

    // Pass through transaction notifications
    connect(dashboard, SIGNAL(incomingTransaction(QString, int, CAmount, QString, QString)), this, SLOT(incomingTransaction(QString, int, CAmount, QString, QString)));

    return true;
}

bool BWKGUI::setCurrentWallet(const QString& name) {
    // Single wallet supported.
    return true;
}

void BWKGUI::removeAllWallets() {
    // Single wallet supported.
}

void BWKGUI::incomingTransaction(const QString& date, int unit, const CAmount& amount, const QString& type, const QString& address) {
    // Only send notifications when not disabled
    if(!bdisableSystemnotifications){
        // On new transaction, make an info balloon
        message((amount) < 0 ? (pwalletMain->fMultiSendNotify == true ? tr("Sent MultiSend transaction") : tr("Sent transaction")) : tr("Incoming transaction"),
            tr("Date: %1\n"
               "Amount: %2\n"
               "Type: %3\n"
               "Address: %4\n")
                .arg(date)
                .arg(BitcoinUnits::formatWithUnit(unit, amount, true))
                .arg(type)
                .arg(address),
            CClientUIInterface::MSG_INFORMATION);

        pwalletMain->fMultiSendNotify = false;
    }
}

#endif // ENABLE_WALLET


static bool ThreadSafeMessageBox(BWKGUI* gui, const std::string& message, const std::string& caption, unsigned int style)
{
    bool modal = (style & CClientUIInterface::MODAL);
    // The SECURE flag has no effect in the Qt GUI.
    // bool secure = (style & CClientUIInterface::SECURE);
    style &= ~CClientUIInterface::SECURE;
    bool ret = false;
    std::cout << "thread safe box: " << message << std::endl;
    // In case of modal message, use blocking connection to wait for user to click a button
    QMetaObject::invokeMethod(gui, "message",
              modal ? GUIUtil::blockingGUIThreadConnection() : Qt::QueuedConnection,
              Q_ARG(QString, QString::fromStdString(caption)),
              Q_ARG(QString, QString::fromStdString(message)),
              Q_ARG(unsigned int, style),
              Q_ARG(bool*, &ret));
    return ret;
}


void BWKGUI::subscribeToCoreSignals()
{
    // Connect signals to client
    uiInterface.ThreadSafeMessageBox.connect(boost::bind(ThreadSafeMessageBox, this, _1, _2, _3));
}

void BWKGUI::unsubscribeFromCoreSignals()
{
    // Disconnect signals from client
    uiInterface.ThreadSafeMessageBox.disconnect(boost::bind(ThreadSafeMessageBox, this, _1, _2, _3));
}
