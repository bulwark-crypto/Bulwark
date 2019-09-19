// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/bulwark/settings/settingsconsolewidget.h"
#include "qt/bulwark/settings/forms/ui_settingsconsolewidget.h"
#include "QGraphicsDropShadowEffect"
#include "qt/bulwark/qtutils.h"

#include "clientmodel.h"
#include "guiutil.h"

#include "chainparams.h"
#include "main.h"
#include "rpcclient.h"
#include "rpcserver.h"
#include "util.h"
#ifdef ENABLE_WALLET
#include "wallet/wallet.h"
#endif // ENABLE_WALLET

#include <openssl/crypto.h>

#include <univalue.h>

#ifdef ENABLE_WALLET
#include <db_cxx.h>
#endif

#include <QDir>
#include <QKeyEvent>
#include <QMenu>
#include <QScrollBar>
#include <QSignalMapper>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QStringList>
#include "qt/bulwark/qtutils.h"
#include "utilitydialog.h"

const int CONSOLE_HISTORY = 50;

const struct {
    const char* url;
    const char* source;
} ICON_MAPPING[] = {
        {"cmd-request", ":/icons/ic-transaction-received"},
        {"cmd-reply", ":/icons/ic-transaction-sent"},
        {"cmd-error", ":/icons/ic-transaction-sent"},
        {"misc", ":/icons/ic-transaction-staked"},
        {NULL, NULL}};

/* Object for executing console RPC commands in a separate thread.
*/
class RPCExecutor : public QObject
{
    Q_OBJECT

public slots:
     void requestCommand(const QString& command);

signals:
     void reply(int category, const QString& command);
};

/** Class for handling RPC timers
 * (used for e.g. re-locking the wallet after a timeout)
 */
class QtRPCTimerBase: public QObject, public RPCTimerBase
{
    Q_OBJECT
public:
    QtRPCTimerBase(boost::function<void(void)>& func, int64_t millis):
            func(func)
    {
        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
        timer.start(millis);
    }
    ~QtRPCTimerBase() {}
private slots:
            void timeout() { func(); }
private:
    QTimer timer;
    boost::function<void(void)> func;
};

class QtRPCTimerInterface: public RPCTimerInterface
{
public:
    ~QtRPCTimerInterface() {}
    const char *Name() { return "Qt"; }
    RPCTimerBase* NewTimer(boost::function<void(void)>& func, int64_t millis)
    {
        return new QtRPCTimerBase(func, millis);
    }
};

#include "qt/bulwark/settings/moc_settingsconsolewidget.cpp"

/**
 * Split shell command line into a list of arguments. Aims to emulate \c bash and friends.
 *
 * - Arguments are delimited with whitespace
 * - Extra whitespace at the beginning and end and between arguments will be ignored
 * - Text can be "double" or 'single' quoted
 * - The backslash \c \ is used as escape character
 *   - Outside quotes, any character can be escaped
 *   - Within double quotes, only escape \c " and backslashes before a \c " or another backslash
 *   - Within single quotes, no escaping is possible and no special interpretation takes place
 *
 * @param[out]   args        Parsed arguments will be appended to this list
 * @param[in]    strCommand  Command line to split
 */
bool parseCommandLineSettings(std::vector<std::string>& args, const std::string& strCommand)
{
    enum CmdParseState {
        STATE_EATING_SPACES,
        STATE_ARGUMENT,
        STATE_SINGLEQUOTED,
        STATE_DOUBLEQUOTED,
        STATE_ESCAPE_OUTER,
        STATE_ESCAPE_DOUBLEQUOTED
    } state = STATE_EATING_SPACES;
    std::string curarg;
    foreach (char ch, strCommand) {
        switch (state) {
            case STATE_ARGUMENT:      // In or after argument
            case STATE_EATING_SPACES: // Handle runs of whitespace
                switch (ch) {
                    case '"':
                        state = STATE_DOUBLEQUOTED;
                        break;
                    case '\'':
                        state = STATE_SINGLEQUOTED;
                        break;
                    case '\\':
                        state = STATE_ESCAPE_OUTER;
                        break;
                    case ' ':
                    case '\n':
                    case '\t':
                        if (state == STATE_ARGUMENT) // Space ends argument
                        {
                            args.push_back(curarg);
                            curarg.clear();
                        }
                        state = STATE_EATING_SPACES;
                        break;
                    default:
                        curarg += ch;
                        state = STATE_ARGUMENT;
                }
                break;
            case STATE_SINGLEQUOTED: // Single-quoted string
                switch (ch) {
                    case '\'':
                        state = STATE_ARGUMENT;
                        break;
                    default:
                        curarg += ch;
                }
                break;
            case STATE_DOUBLEQUOTED: // Double-quoted string
                switch (ch) {
                    case '"':
                        state = STATE_ARGUMENT;
                        break;
                    case '\\':
                        state = STATE_ESCAPE_DOUBLEQUOTED;
                        break;
                    default:
                        curarg += ch;
                }
                break;
            case STATE_ESCAPE_OUTER: // '\' outside quotes
                curarg += ch;
                state = STATE_ARGUMENT;
                break;
            case STATE_ESCAPE_DOUBLEQUOTED:                  // '\' in double-quoted text
                if (ch != '"' && ch != '\\') curarg += '\\'; // keep '\' for everything but the quote and '\' itself
                curarg += ch;
                state = STATE_DOUBLEQUOTED;
                break;
        }
    }
    switch (state) // final state
    {
        case STATE_EATING_SPACES:
            return true;
        case STATE_ARGUMENT:
            args.push_back(curarg);
            return true;
        default: // ERROR to end in one of the other states
            return false;
    }
}

void RPCExecutor::requestCommand(const QString& command)
{
    std::vector<std::string> args;
    if (!parseCommandLineSettings(args, command.toStdString())) {
        emit reply(SettingsConsoleWidget::CMD_ERROR, QString("Parse error: unbalanced ' or \""));
        return;
    }
    if (args.empty())
        return; // Nothing to do
    try {
        std::string strPrint;
        // Convert argument list to JSON objects in method-dependent way,
        // and pass it along with the method name to the dispatcher.
        UniValue result = tableRPC.execute(
                args[0],
                RPCConvertValues(args[0], std::vector<std::string>(args.begin() + 1, args.end())));

        // Format result reply
        if (result.isNull())
            strPrint = "";
        else if (result.isStr())
            strPrint = result.get_str();
        else
            strPrint = result.write(2);

        emit reply(SettingsConsoleWidget::CMD_REPLY, QString::fromStdString(strPrint));
    } catch (UniValue& objError) {
        try // Nice formatting for standard-format error
        {
            int code = find_value(objError, "code").get_int();
            std::string message = find_value(objError, "message").get_str();
            emit reply(SettingsConsoleWidget::CMD_ERROR, QString::fromStdString(message) + " (code " + QString::number(code) + ")");
        } catch (std::runtime_error&) // raised when converting to invalid type, i.e. missing code or message
        {                             // Show raw JSON object
            emit reply(SettingsConsoleWidget::CMD_ERROR, QString::fromStdString(objError.write()));
        }
    } catch (std::exception& e) {
        emit reply(SettingsConsoleWidget::CMD_ERROR, QString("Error: ") + QString::fromStdString(e.what()));
    }
}

SettingsConsoleWidget::SettingsConsoleWidget(BWKGUI* _window, QWidget *parent) :
    PWidget(_window,parent),
    ui(new Ui::SettingsConsoleWidget)
{
    ui->setupUi(this);

    this->setStyleSheet(parent->styleSheet());

    // Containers
    ui->left->setProperty("cssClass", "container");
    ui->left->setContentsMargins(10,10,10,10);
    ui->messagesWidget->setProperty("cssClass", "container");

    // Title
    ui->labelTitle->setText(tr("Console"));
    setCssTitleScreen(ui->labelTitle);

    // Console container
    ui->consoleWidget->setProperty("cssClass", "container-square");
    setShadow(ui->consoleWidget);

    // Edit
    ui->lineEdit->setPlaceholderText(tr("Console input"));
    initCssEditLine(ui->lineEdit);

    // Buttons
    ui->pushButton->setProperty("cssClass", "ic-arrow");
    ui->pushButtonCommandOptions->setText(tr("Command Line Options "));
    ui->pushButtonOpenDebug->setText(tr("Open Debug File"));
    setCssBtnSecondary(ui->pushButtonOpenDebug);
    setCssBtnSecondary(ui->pushButtonCommandOptions);

    connect(ui->pushButtonOpenDebug, &QPushButton::clicked, [this](){
        if(!GUIUtil::openDebugLogfile()){
            inform(tr("Cannot open debug file.\nVerify that you have installed a predetermined text editor."));
        }
    });
    connect(ui->pushButtonCommandOptions, SIGNAL(clicked()), this, SLOT(onCommandsClicked()));

    // Install event filter for up and down arrow
    ui->lineEdit->installEventFilter(this);
    ui->messagesWidget->installEventFilter(this);

    // Register RPC timer interface
    rpcTimerInterface = new QtRPCTimerInterface();
    // avoid accidentally overwriting an existing, non QTThread
    // based timer interface
    RPCSetTimerInterfaceIfUnset(rpcTimerInterface);

    startExecutor();
    clear();
}

SettingsConsoleWidget::~SettingsConsoleWidget()
{
    GUIUtil::saveWindowGeometry("nRPCConsoleWindow", this);
    emit stopExecutor();
    RPCUnsetTimerInterface(rpcTimerInterface);
    delete rpcTimerInterface;
    delete ui;
}


bool SettingsConsoleWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) // Special key handling
    {
        QKeyEvent* keyevt = static_cast<QKeyEvent*>(event);
        int key = keyevt->key();
        Qt::KeyboardModifiers mod = keyevt->modifiers();
        switch (key) {
            case Qt::Key_Up:
                if (obj == ui->lineEdit) {
                    browseHistory(-1);
                    return true;
                }
                break;
            case Qt::Key_Down:
                if (obj == ui->lineEdit) {
                    browseHistory(1);
                    return true;
                }
                break;
            case Qt::Key_PageUp: /* pass paging keys to messages widget */
            case Qt::Key_PageDown:
                if (obj == ui->lineEdit) {
                    QApplication::postEvent(ui->messagesWidget, new QKeyEvent(*keyevt));
                    return true;
                }
                break;
            case Qt::Key_Return:
            case Qt::Key_Enter:
                // forward these events to lineEdit
                if(obj == autoCompleter->popup()) {
                    QApplication::postEvent(ui->lineEdit, new QKeyEvent(*keyevt));
                    return true;
                }
                break;
            default:
                // Typing in messages widget brings focus to line edit, and redirects key there
                // Exclude most combinations and keys that emit no text, except paste shortcuts
                if (obj == ui->messagesWidget && ((!mod && !keyevt->text().isEmpty() && key != Qt::Key_Tab) ||
                                                  ((mod & Qt::ControlModifier) && key == Qt::Key_V) ||
                                                  ((mod & Qt::ShiftModifier) && key == Qt::Key_Insert))) {
                    ui->lineEdit->setFocus();
                    QApplication::postEvent(ui->lineEdit, new QKeyEvent(*keyevt));
                    return true;
                }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void SettingsConsoleWidget::loadClientModel() {
    if (clientModel){

        //Setup autocomplete and attach it
        QStringList wordList;
        std::vector<std::string> commandList = tableRPC.listCommands();
        for (size_t i = 0; i < commandList.size(); ++i)
        {
            wordList << commandList[i].c_str();
        }

        autoCompleter = new QCompleter(wordList, this);
        ui->lineEdit->setCompleter(autoCompleter);

        // clear the lineEdit after activating from QCompleter
        autoCompleter->popup()->installEventFilter(this);
    }
}

static QString categoryClass(int category)
{
    switch (category) {
        case SettingsConsoleWidget::CMD_REQUEST:
            return "cmd-request";
        case SettingsConsoleWidget::CMD_REPLY:
            return "cmd-reply";
        case SettingsConsoleWidget::CMD_ERROR:
            return "cmd-error";
        default:
            return "misc";
    }
}

void SettingsConsoleWidget::clear(){
    ui->messagesWidget->clear();
    history.clear();
    historyPtr = 0;
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();

    // Add smoothly scaled icon images.
    // (when using width/height on an img, Qt uses nearest instead of linear interpolation)
    for (int i = 0; ICON_MAPPING[i].url; ++i) {
        ui->messagesWidget->document()->addResource(
                QTextDocument::ImageResource,
                QUrl(ICON_MAPPING[i].url),
                QImage(ICON_MAPPING[i].source));
    }

    QString theme;
    changeTheme(isLightTheme(), theme);

#ifdef Q_OS_MAC
    QString clsKey = "(⌘)-L";
#else
    QString clsKey = "Ctrl-L";
#endif

    message(CMD_REPLY, (tr("Welcome to the PIVX RPC console.") + "<br>" +
                        tr("Use up and down arrows to navigate history, and %1 to clear screen.").arg("<b>"+clsKey+"</b>") + "<br>" +
                        tr("Type <b>help</b> for an overview of available commands.") +
                        "<br><span class=\"secwarning\"><br>" +
                        tr("WARNING: Scammers have been active, telling users to type commands here, stealing their wallet contents. Do not use this console without fully understanding the ramifications of a command.") +
                        "</span>"),
            true);
}

void SettingsConsoleWidget::message(int category, const QString& message, bool html)
{
    QTime time = QTime::currentTime();
    QString timeString = time.toString();
    QString out;
    out += "<table><tr><td class=\"time\" width=\"65\">" + timeString + "</td>";
    out += "<td class=\"icon\" width=\"32\"><img src=\"" + categoryClass(category) + "\"></td>";
    out += "<td class=\"message " + categoryClass(category) + "\" valign=\"middle\">";
    if (html)
        out += message;
    else
        out += GUIUtil::HtmlEscape(message, true);
    out += "</td></tr></table>";
    ui->messagesWidget->append(out);
}

void SettingsConsoleWidget::on_lineEdit_returnPressed()
{
    QString cmd = ui->lineEdit->text();
    ui->lineEdit->clear();

    if (!cmd.isEmpty()) {
        message(CMD_REQUEST, cmd);
        emit cmdCommandRequest(cmd);
        // Remove command, if already in history
        history.removeOne(cmd);
        // Append command to history
        history.append(cmd);
        // Enforce maximum history size
        while (history.size() > CONSOLE_HISTORY)
            history.removeFirst();
        // Set pointer to end of history
        historyPtr = history.size();
        // Scroll console view to end
        scrollToEnd();
    }
}


void SettingsConsoleWidget::browseHistory(int offset)
{
    historyPtr += offset;
    if (historyPtr < 0)
        historyPtr = 0;
    if (historyPtr > history.size())
        historyPtr = history.size();
    QString cmd;
    if (historyPtr < history.size())
        cmd = history.at(historyPtr);
    ui->lineEdit->setText(cmd);
}

void SettingsConsoleWidget::startExecutor()
{
    QThread* thread = new QThread;
    RPCExecutor* executor = new RPCExecutor();
    executor->moveToThread(thread);

    // Replies from executor object must go to this object
    connect(executor, SIGNAL(reply(int, QString)), this, SLOT(message(int, QString)));
    // Requests from this object must go to executor
    connect(this, &SettingsConsoleWidget::cmdCommandRequest, executor, &RPCExecutor::requestCommand);

    // On stopExecutor signal
    // - queue executor for deletion (in execution thread)
    // - quit the Qt event loop in the execution thread
    connect(this, SIGNAL(stopExecutor()), executor, SLOT(deleteLater()));
    connect(this, SIGNAL(stopExecutor()), thread, SLOT(quit()));
    // Queue the thread for deletion (in this thread) when it is finished
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    // Default implementation of QThread::run() simply spins up an event loop in the thread,
    // which is what we want.
    thread->start();
}

void SettingsConsoleWidget::scrollToEnd()
{
    QScrollBar* scrollbar = ui->messagesWidget->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
}


void SettingsConsoleWidget::changeTheme(bool isLightTheme, QString &theme)
{
    // Set default style sheet
    if (isLightTheme) {
        ui->messagesWidget->document()->setDefaultStyleSheet(
                "table { color: #707070;  }"
                "td.time { color: #808080; padding-top: 3px; } "
                "td.message { color: #707070;font-family: Courier, Courier New, Lucida Console, monospace; font-size: 12px; } " // Todo: Remove fixed font-size
                "td.cmd-request { color: #006060; } "
                "td.cmd-error { color: red; } "
                ".secwarning { color: red; }"
                "b { color: #707070; } ");
    } else {
        ui->messagesWidget->document()->setDefaultStyleSheet(
                "table { color: #FFFFFF; }"
                "td.time { color: #808080; padding-top: 3px; } "
                "td.message { color: #FFFFFF;font-family: Courier, Courier New, Lucida Console, monospace; font-size: 12px; } " // Todo: Remove fixed font-size
                "td.cmd-request { color: #006060; } "
                "td.cmd-error { color: red; } "
                ".secwarning { color: red; }"
                "b { color: #FFFFFF; } ");
    }
    updateStyle(ui->messagesWidget);
}

void SettingsConsoleWidget::onCommandsClicked() {
    if (!clientModel)
        return;

    HelpMessageDialog dlg(this, false);
    dlg.exec();

}
