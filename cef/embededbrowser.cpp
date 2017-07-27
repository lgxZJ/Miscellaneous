#include "embededbrowser.h"

#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/cef_display_handler.h"
#include "include/cef_life_span_handler.h"

#include <qevent.h>
#include <tuple>

#include <qmessagebox.h>

namespace lgxZJ
{
    ////////////////////////////////////////////////////////////////

    qint32 JsArray::size() const
    {
        return m_array.size();
    }

    bool JsArray::empty() const
    {
        return m_array.empty();
    }

    const JsVariant& JsArray::at(unsigned index) const
    {
        return *m_array.at(index);
    }

    void JsArray::insert(unsigned indexBefore, const JsVariant& value)
    {
        m_array.insert(m_array.begin() + indexBefore,
                       std::shared_ptr<JsVariant>(new JsVariant(value)));
    }

    void JsArray::erase(unsigned index)
    {
        m_array.erase(m_array.begin() + index);
    }

    void JsArray::push_back(const JsVariant& value)
    {
        m_array.push_back(std::shared_ptr<JsVariant>(new JsVariant(value)));
    }

    ////////////////////////////////////////////////////////////////

    JsVariant::JsVariant()
        : m_type(JsType::Invalid)
        , m_variant()
    {
    }

    JsVariant::JsVariant(const JsArray& array)
        : m_type(JsType::Array)
        , m_array(array)
    {
    }

    JsVariant::JsVariant(const char * str)
        : m_type(JsType::String)
        , m_variant(str)
    {
    }

    JsVariant::JsVariant(const QString &value)
        : m_type(JsType::String)
        , m_variant(value)
    {
    }

    JsVariant::JsVariant(const bool value)
        : m_type(JsType::Bool)
        , m_variant(value)
    {
    }

    JsVariant::JsVariant(const qint32 value)
        : m_type(JsType::Int)
        , m_variant(qlonglong(value))
    {
    }

    JsVariant::JsVariant(const quint32 value)
        : m_type(JsType::UInt)
        , m_variant(qulonglong(value))
    {
    }

    JsVariant::JsVariant(const double value)
        : m_type(JsType::Double)
        , m_variant(value)
    {
    }

    JsVariant::JsVariant(const QDateTime & datetime)
        : m_type(JsType::Date)
        , m_variant(datetime)
    {
    }

    JsVariant JsVariant::Null()
    {
        JsVariant null;
        null.m_type = JsType::Null;

        return null;
    }

    JsVariant JsVariant::Undefined()
    {
        JsVariant undefined;
        undefined.m_type = JsType::Undefined;

        return undefined;
    }

    bool JsVariant::isValid() const
    {
        return m_type != JsType::Invalid;
    }

    bool JsVariant::isNull() const
    {
        return m_type == JsType::Null;
    }

    bool JsVariant::isUndefined() const
    {
        return m_type == JsType::Undefined;
    }

    bool JsVariant::isString() const
    {
        return m_type == JsType::String;
    }

    bool JsVariant::isBool() const
    {
        return m_type == JsType::Bool;
    }

    bool JsVariant::isInt() const
    {
        return m_type == JsType::Int;
    }

    bool JsVariant::isUInt() const
    {
        return m_type == JsType::UInt;
    }

    bool JsVariant::isDouble() const
    {
        return m_type == JsType::Double;
    }

    bool JsVariant::isArray() const
    {
        return m_type == JsType::Array;
    }

    bool JsVariant::isDate() const
    {
        return m_type == JsType::Date;
    }

    QString JsVariant::toString() const
    {
        return m_variant.toString();
    }

    bool JsVariant::toBool() const
    {
        return m_variant.toBool();
    }

    qint32 JsVariant::toInt() const
    {
        return m_variant.toLongLong();
    }

    quint32 JsVariant::toUInt() const
    {
        return m_variant.toULongLong();
    }

    double JsVariant::toDouble() const
    {
        return m_variant.toDouble();
    }

    JsArray JsVariant::toArray() const
    {
        return m_array;
    }

    QDateTime JsVariant::toDate() const
    {
        return m_variant.toDateTime();
    }

    //////////////////////////////////////////////////////////////////////

    EmbededBrowser::EmbededBrowser(QWidget* parent)
           : QWidget(parent)
           , m_parentWidget(parent)
           , m_windowPos(0, 0)
           , m_jsFuncEnvs()
           , m_nativeFuncMapping()
    {
        //  seems there is a bug when using CefRefPtr inside initialize list
        m_browserHandler = new BrowserHandler(parent);
        m_browserProc = new BrowserProc(m_browserHandler.get(), parent);
    }

    EmbededBrowser::~EmbededBrowser()
    {
        //  no need to delete m_browserProc and m_browserHandler, CEF do it
    }

    void EmbededBrowser::quit()
    {
        QCloseEvent event;
        closeEvent(&event);
    }

    void EmbededBrowser::hide()
    {
        assert (m_browserHandler->m_windowHandle != NULL);

        ShowWindow(m_browserHandler->m_windowHandle, SW_HIDE);
    }

    int EmbededBrowser::height() const
    {
        assert(m_browserHandler->m_windowSize.isValid());

        return m_browserHandler->m_windowSize.height();
    }

    int EmbededBrowser::width() const
    {
        assert(m_browserHandler->m_windowSize.isValid());

        return m_browserHandler->m_windowSize.width();
    }

    bool EmbededBrowser::isVisible() const
    {
        return IsWindowVisible(m_browserHandler->m_windowHandle);
    }

    void EmbededBrowser::move(int x, int y)
    {
        assert(m_browserHandler->m_windowHandle != NULL);
        assert (m_parentWidget != nullptr);

        m_windowPos = QPoint(m_windowPos.x() + x,
                             m_windowPos.y() + y);
        SetWindowPos(m_browserHandler->m_windowHandle, NULL,
                     m_windowPos.x(), m_windowPos.y(),
                     NULL, NULL,
                     SWP_NOZORDER | SWP_NOSIZE);
    }

    QPoint EmbededBrowser::pos() const
    {
        return m_windowPos;
    }

    void EmbededBrowser::setInitUrl(const QString& url)
    {
        m_browserProc->setUrl(url);
    }

    void EmbededBrowser::loadUrl(const QString& url)
    {
        assert(m_browserHandler->m_browser != NULL);
        //  QString uses Utf8 default, we change it to std::string(ascii)
        m_browserHandler->m_browser->GetMainFrame()
                ->LoadURL(std::string(url.toLocal8Bit()));
    }

    bool EmbededBrowser::canGoBack()
    {
        assert(m_browserHandler->m_browser != NULL);

        return m_browserHandler->m_browser->CanGoBack();
    }

    bool EmbededBrowser::canGoForward()
    {
        assert(m_browserHandler->m_browser != NULL);

        return m_browserHandler->m_browser->CanGoForward();
    }

    void EmbededBrowser::goBack()
    {
        assert(m_browserHandler->m_browser != NULL);
     
        m_browserHandler->m_browser->GoBack();
    }

    void EmbededBrowser::goForward()
    {
        assert(m_browserHandler->m_browser != NULL);

        m_browserHandler->m_browser->GoForward();
    }

    void EmbededBrowser::stopLoad()
    {
        assert(m_browserHandler->m_browser != NULL);
        m_browserHandler->m_browser->StopLoad();
    }

    void EmbededBrowser::setZoomLevel(double zoomLevel)
    {
        assert(m_browserHandler->m_browser != NULL);
        m_browserHandler->m_browser->GetHost()->SetZoomLevel(zoomLevel);
    }

    void EmbededBrowser::setWindowFrameless()
    {
        m_browserHandler->m_isFrameless = true;
        m_browserHandler->m_sizable = false;
    }

    void EmbededBrowser::enableCloseButton(bool enableFlag)
    {
        m_browserHandler->m_hasCloseButton = enableFlag;
    }

    void EmbededBrowser::setSizable(bool enableFlag)
    {
        m_browserHandler->m_sizable = enableFlag;
    }

    void EmbededBrowser::enableMaximizeButton(bool enableFlag)
    {
        m_browserHandler->m_hasMaxButton = enableFlag;
    }

    void EmbededBrowser::enableMinimizeButton(bool enableFlag)
    {
        m_browserHandler->m_hasMinButton = enableFlag;
    }

    void EmbededBrowser::resize(const QSize & newSize)
    {
        m_browserHandler->resizeWindow(newSize);
    }

    /*void EmbededBrowser::registerJsExtension(const QString &name, const QString &code, NativeFunction* handler)
    {
        m_renderProc->m_extensionBindings.push_back(RenderProc::ExtensionBinding(
            std::string(name.toLocal8Bit()),
            std::string(code.toLocal8Bit()),
            handler));
    }*/

    //
    //  Note Js and Native interaction happens in Render process, we need to
    //  use IPC to pass data. I use following rule to pass message and data:
    //          "lgxZJ.XXX"---- "XXX" stand for a message type
    //                              
    //


    void EmbededBrowser::execJsCode(const QString &jsCode)
    {
        assert (m_browserHandler->m_browser != NULL);
        
        m_browserHandler->m_browser->GetMainFrame()->ExecuteJavaScript(
            std::string(jsCode.toLocal8Bit()),
            "none", 0);
    }

    JsVariant EmbededBrowser::callJsFunc(const QString & funcName)
    {
        sendMessageToRender(IpcMsg::jsCall(funcName));
        //  todo: return value
        return JsVariant();
    }

    JsVariant EmbededBrowser::convertV8ValueToJsVariant(CefRefPtr<CefV8Value> value)
    {
        assert (value->IsValid());

        if (value->IsNull())            return JsVariant::Null();
        else if (value->IsUndefined())  return JsVariant::Undefined();
        else if (value->IsBool())       return JsVariant(value->GetBoolValue());
        else if (value->IsInt())        return JsVariant(value->GetIntValue());
        else if (value->IsUInt())       return JsVariant(value->GetUIntValue());
        else if (value->IsDouble())     return JsVariant(value->GetDoubleValue());
        else if (value->IsString())     return JsVariant(value->GetStringValue().ToString().c_str());
        else if (value->IsDate())       return JsVariant(QDateTime::fromTime_t(value->GetDateValue().GetTimeT()));
        else if (value->IsArray())      return JsVariant(convertV8ValueToJsArray(value));
        //  todo:
        else                            assert (false && "not supported js value type!");
    }

    JsArray EmbededBrowser::convertV8ValueToJsArray(CefRefPtr<CefV8Value> value)
    {
        JsArray jsArray;
        for (int i = 0; i < value->GetArrayLength(); ++i)
            jsArray.push_back(convertV8ValueToJsVariant(value->GetValue(i)));

        return jsArray;
    }

    void EmbededBrowser::bindJsNull(const QString &nullName)
    {
        //m_renderProc->m_nullBindings.push_back(RenderProc::NullBinding(nullName.toLocal8Bit()));
    }

    void EmbededBrowser::bindJsUndefined(const QString &undefinedName)
    {
        //m_renderProc->m_undefinedBindings.push_back(RenderProc::UndefinedBinding(undefinedName.toLocal8Bit()));
    }

    void EmbededBrowser::bindJsString(const QString &key, const QString &value)
    {
        /*m_renderProc->m_stringBindings.push_back(RenderProc::StringBinding(
            std::string(key.toLocal8Bit()),
            std::string(value.toLocal8Bit())));*/
    }

    void EmbededBrowser::bindJsBool(const QString &boolName, const bool boolValue)
    {
        //m_renderProc->m_boolBindings.push_back(RenderProc::BoolBinding(
        //    std::string(boolName.toLocal8Bit()),
        //    boolValue));
    }

    void EmbededBrowser::bindJsInt(const QString &intName, const qint32 intValue)
    {
        //m_renderProc->m_intBindings.push_back(RenderProc::IntBinding(
        //    std::string(intName.toLocal8Bit()),
        //    intValue));
    }

    void EmbededBrowser::bindJsUInt(const QString &uintName, const qint32 uintValue)
    {
        //m_renderProc->m_uintBindings.push_back(RenderProc::UIntBinding(
        //    std::string(uintName.toLocal8Bit()),
        //    uintValue));
    }

    void EmbededBrowser::bindJsDouble(const QString &doubleName, const double doubleValue)
    {
        //m_renderProc->m_doubleBindings.push_back(RenderProc::DoubleBinding(
        //    std::string(doubleName.toLocal8Bit()),
        //    doubleValue));
    }

    void EmbededBrowser::bindJsDate(const QString& dateName, const QDateTime& dateValue)
    {
        assert (!dateValue.isNull() && dateValue.isValid());

        //m_renderProc->m_dateBindings.push_back(RenderProc::DateBinding(
        //    std::string(dateName.toLocal8Bit()),
        //    dateValue));
    }

    void EmbededBrowser::bindJsArray(const QString &arrayName, const JsArray& arrayValue)
    {
        //m_renderProc->m_arrayBindings.push_back(RenderProc::ArrayBinding(
        //    std::string(arrayName.toLocal8Bit()),
        //    arrayValue));
    }

    void EmbededBrowser::bindNativeFunc(const QString &funcName, NativeFunction* handler)
    {
        assert (handler != nullptr);

        //auto funcNameStd = std::string(funcName.toLocal8Bit());
        //m_nativeFuncMapping[funcNameStd] = handler;
        //m_renderProc->m_functionBindings.push_back(RenderProc::FunctionBinding(funcNameStd, handler));
    }

    void EmbededBrowser::setInitSize(const QSize &size)
    {
        m_browserHandler->m_windowSize = size;
    }

    void EmbededBrowser::show()
    {
        static bool firstCalled = true;

        if (!firstCalled) {
            ShowWindow(m_browserHandler->m_windowHandle, SW_SHOW);
            return;
        }
        else {
            firstCalled = false;

            // Enable High-DPI support on Windows 7 or newer.
            CefEnableHighDPISupport();
            CefRefPtr<CefApp> app = createCorrectArgumentByCmdLine();

            if (app.get() != m_browserProc)     runSubProcesses(app);
            else                                initCef(app);
        }
    }

    CefApp* EmbededBrowser::createCorrectArgumentByCmdLine()
    {
        const char kProcessType[] = "type";
        const char kRendererProcess[] = "renderer";

        CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
        command_line->InitFromString(::GetCommandLineW());
        
        if (!command_line->HasSwitch(kProcessType))
            return new BrowserProc();
        else if (command_line->GetSwitchValue(kProcessType) == kRendererProcess)
            return new RenderProc();
        else
            return new OtherProc();
    }

    void EmbededBrowser::runSubProcesses(CefRefPtr<CefApp> app)
    {
        int exit_code = CefExecuteProcess(CefMainArgs(GetModuleHandle(NULL)), app.get(), nullptr);
        if (exit_code >= 0) {
            // The sub-process has completed so return here.
            exit(exit_code);
        }
    }

    void EmbededBrowser::initCef(CefRefPtr<CefApp>& app)
    {
        // Specify CEF global settings here.
        CefSettings settings;
        settings.single_process = false;
        settings.no_sandbox = true;
        settings.multi_threaded_message_loop = true;
        settings.log_severity = LOGSEVERITY_WARNING;
        
        // Initialize CEF.
        CefInitialize(CefMainArgs(GetModuleHandle(NULL)), settings, app, nullptr);
    }

    //  should be called by the parent
    void EmbededBrowser::close()
    {
        assert (m_browserHandler->m_windowHandle != NULL);

        if (m_browserHandler->m_browser != nullptr) {
            m_browserHandler->m_browser->GetHost()->CloseBrowser(false);
            return;
        }

        CefShutdown();
    }

    void EmbededBrowser::closeEvent(QCloseEvent * event)
    {
        DestroyWindow(m_browserHandler->m_browser->GetHost()->GetWindowHandle());
        close();
        sendMessageToRender(IpcMsg::renderExit());
        QWidget::closeEvent(event);
    }

    void EmbededBrowser::sendMessageToRender(CefRefPtr<CefProcessMessage> msg)
    {
        assert (m_browserHandler->m_browser != NULL);

        assert (m_browserHandler->m_browser->SendProcessMessage(
                PID_RENDERER,
                msg));
    }

    void EmbededBrowser::resizeEvent(QResizeEvent * event)
    {
        // if we dont resize here, seems the browser changes it with a little delay
        if (m_browserHandler->m_windowHandle == NULL)
            setInitSize(event->size());
        else
            resize(event->size());

        QWidget::resizeEvent(event);
    }

    void EmbededBrowser::showEvent(QShowEvent * event)
    {
        show();
    }

    void EmbededBrowser::reload()
    {
        assert (m_browserHandler->m_browser != NULL);

        m_browserHandler->m_browser->Reload();
    }
   
    bool EmbededBrowser::Execute(
        const CefString &name,              
        CefRefPtr<CefV8Value> object,
        const CefV8ValueList &arguments,    
        CefRefPtr<CefV8Value> &retval,
        CefString &exception)
    {
        if (callNativeFunc(name, arguments, retval) == true)
            return true;
        if (collectJsFuncEnvs(name, arguments) == true)
            return true;

        // Function does not exist.
        return false;
    }

    bool EmbededBrowser::callNativeFunc(const CefString &name, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval)
    {
        auto nativeFuncIter = m_nativeFuncMapping.find(std::string(name));

        //  if object is a native function, execute it
        if (nativeFuncIter != m_nativeFuncMapping.end()) {
            JsVariant result = JsVariant::Undefined();
            nativeFuncIter->second->definition(convertV8ValueListToJsVariantVec(arguments), &result);

            //  set return value
            retval = convertJsVariantToV8Value(result);
            return true;
        }

        return false;
    }

    NativeFunction::Args EmbededBrowser::convertV8ValueListToJsVariantVec(const CefV8ValueList &arguments)
    {
        NativeFunction::Args args;
        for (int i = 0; i  < arguments.size(); ++i)
            args.push_back(convertV8ValueToJsVariant(arguments.at(i)));

        return args;
    }

    CefRefPtr<CefV8Value> EmbededBrowser::convertJsVariantToV8Value(const JsVariant& jsVariant)
    {
        //  wrap a 1-element array to reuse convert function
        JsArray jsArray;
        jsArray.push_back(jsVariant);
        return RenderProc::convertJsArrayToV8Value(jsArray);
    }

    bool EmbededBrowser::collectJsFuncEnvs(const CefString &name, const CefV8ValueList &arguments)
    {
        if (name == "register") {
            assert (arguments.size() >= 1);

            //  functions multi-registered will be overwirtten
            if (arguments[0]->IsFunction()) {
                
                m_jsFuncEnvs[arguments[0]->GetFunctionName()] = 
                    JsFuncEnv(arguments[0],
                              JsFuncArgs(arguments.begin() + 1, arguments.end()),
                              CefV8Context::GetCurrentContext());
                return true;
            }
        }

        return false;
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    //                               BrowserProc   
    /////////////////////////////////////////////////////////////////////////////////////////

    EmbededBrowser::BrowserProc::BrowserProc(CefRefPtr<BrowserHandler> browserHandler, QWidget* parent)
        : CefApp()
        , CefBrowserProcessHandler()
        , m_url("www.baidu.com")
        , m_parentWidget(parent)
        , m_browserHandler(browserHandler)
    {
        assert (m_browserHandler != nullptr);
    }

    void EmbededBrowser::BrowserProc::setUrl(const QString& url)
    {
        m_url = url.toLocal8Bit();
    }
    
    void EmbededBrowser::BrowserProc::OnBeforeCommandLineProcessing(
        const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
    {
        command_line->AppendArgument("disable-gpu");
        command_line->AppendArgument("disable-gpu-compositing"); 
        command_line->AppendArgument("renderer-startup-dialo");
        command_line->AppendArgument("no-sandbox");
    }

    void EmbededBrowser::BrowserProc::OnContextInitialized()
    {
        //CEF_REQUIRE_UI_THREAD();

        if (m_url.empty())
            throw std::exception("Url error");

        // Information used when creating the native window.
        CefWindowInfo window_info;
        setAsChildIfHasParent(&window_info);

        CefBrowserSettings browserSettings;
        browserSettings.plugins = STATE_DISABLED;

        // Create the first browser window.
        CefBrowserHost::CreateBrowser(window_info,
                                      m_browserHandler,
                                      CefString(m_url),
                                      CefBrowserSettings(), NULL);
    }

    void EmbededBrowser::BrowserProc::OnRenderProcessTerminated(
        CefRefPtr<CefBrowser> browser,
        TerminationStatus status)
    {
    }

    void EmbededBrowser::BrowserProc::setAsChildIfHasParent(CefWindowInfo* windowInfo)
    {
#if defined(OS_WIN)
        // On Windows we need to specify certain flags that will be passed to
        // CreateWindowEx().
        if (m_parentWidget != nullptr) {
            RECT initRect;
            initRect.left = 0;
            initRect.top = 0;
            initRect.right = 0;
            initRect.bottom = 0;

            windowInfo->SetAsChild((HWND)m_parentWidget->winId(), initRect);
        }
        else {
            windowInfo->SetAsPopup(NULL, "EmbededBrowser");
        }
#endif
    }

    ////////////////////////////////////////////////////////////////////////////////

    EmbededBrowser::RenderProc::RenderProc(EmbededBrowser* browser)
        : CefApp()
        , CefRenderProcessHandler()
        , m_embededBrowser(browser)
        , m_browser(NULL)
        , m_undefinedBindings()
        , m_nullBindings()
        , m_intBindings()
        , m_uintBindings()
        , m_doubleBindings()
        , m_boolBindings()
        , m_stringBindings()
        , m_dateBindings()
        , m_arrayBindings()
        //, m_extensionBindings()
        , m_functionBindings()
    {
        assert (m_embededBrowser != nullptr);
    }

    void EmbededBrowser::RenderProc::OnContextCreated(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context)
    {
        m_browser = browser;
        // Retrieve the context's window object.
        CefRefPtr<CefV8Value> object = context->GetGlobal();
          
        bindVariables(object);
        bindFuncs(object);
        
        enableJsFuncRegistering(object);
    }

    void EmbededBrowser::RenderProc::bindVariables(CefRefPtr<CefV8Value>& object)
    {
        bindNulls(object);
        bindUndefined(object);
        bindBools(object);
        bindInts(object);
        bindUInts(object);
        bindDoubles(object);
        bindStrings(object);
        bindDates(object);
        bindArrays(object);
    }

    void EmbededBrowser::RenderProc::bindNulls(CefRefPtr<CefV8Value>& object)
    {
        for (auto& nullBinding : m_nullBindings)
            object->SetValue(nullBinding,
                             CefV8Value::CreateNull(),
                             V8_PROPERTY_ATTRIBUTE_NONE);
    }

    void EmbededBrowser::RenderProc::bindUndefined(CefRefPtr<CefV8Value>& object)
    {
        for (auto& undefinedBinding : m_undefinedBindings)
            object->SetValue(undefinedBinding,
                             CefV8Value::CreateUndefined(),
                             V8_PROPERTY_ATTRIBUTE_NONE);
    }

    void EmbededBrowser::RenderProc::bindBools(CefRefPtr<CefV8Value> &object)
    {
        for (auto& boolBinding : m_boolBindings)
            object->SetValue(boolBinding.first,
                             CefV8Value::CreateBool(boolBinding.second),
                             V8_PROPERTY_ATTRIBUTE_NONE);
    }

    void EmbededBrowser::RenderProc::bindInts(CefRefPtr<CefV8Value> &object)
    {
        for (auto& intBinding : m_intBindings)
            assert(object->SetValue(intBinding.first,
                             CefV8Value::CreateInt(intBinding.second),
                             V8_PROPERTY_ATTRIBUTE_NONE));
    }

    void EmbededBrowser::RenderProc::bindUInts(CefRefPtr<CefV8Value> &object)
    {
        for (auto& uintBinding : m_uintBindings)
            object->SetValue(uintBinding.first,
                             CefV8Value::CreateUInt(uintBinding.second),
                             V8_PROPERTY_ATTRIBUTE_NONE);
    }

    void EmbededBrowser::RenderProc::bindDoubles(CefRefPtr<CefV8Value> &object)
    {
        for (auto& doubleBinding : m_doubleBindings)
            object->SetValue(doubleBinding.first,
                             CefV8Value::CreateDouble(doubleBinding.second),
                             V8_PROPERTY_ATTRIBUTE_NONE);
    }

    void EmbededBrowser::RenderProc::bindStrings(CefRefPtr<CefV8Value>& object)
    {
        for (auto& stringBinding : m_stringBindings)
            object->SetValue(stringBinding.first,
                             CefV8Value::CreateString(stringBinding.second),
                             V8_PROPERTY_ATTRIBUTE_NONE);
    }

    void EmbededBrowser::RenderProc::bindDates(CefRefPtr<CefV8Value>& object)
    {
        for (auto& dateBinding : m_dateBindings)
            object->SetValue(dateBinding.first,
                             CefV8Value::CreateDate(CefTime(static_cast<time_t>(dateBinding.second.toTime_t()))),
                             V8_PROPERTY_ATTRIBUTE_NONE);
    }

    void EmbededBrowser::RenderProc::bindArrays(CefRefPtr<CefV8Value>& object)
    {
        for (auto& arrayBinding : m_arrayBindings) {
            object->SetValue(arrayBinding.first,
                             convertJsArrayToV8Value(arrayBinding.second),
                             V8_PROPERTY_ATTRIBUTE_NONE);
        }
    }

    CefRefPtr<CefV8Value> EmbededBrowser::RenderProc::convertJsArrayToV8Value(const JsArray& jsArray)
    {
        auto cefV8Array = CefV8Value::CreateArray(jsArray.size());

        for (int i = 0; i < jsArray.size(); ++i) {
            switch (jsArray.at(i).type()) {
            case JsVariant::JsType::Null:
                cefV8Array->SetValue(i, CefV8Value::CreateNull());      break;
            case JsVariant::JsType::Undefined:
                cefV8Array->SetValue(i, CefV8Value::CreateUndefined()); break;
            case JsVariant::JsType::Int:
                cefV8Array->SetValue(i, CefV8Value::CreateInt(jsArray.at(i).toInt())); break;
            case JsVariant::JsType::UInt:
                cefV8Array->SetValue(i, CefV8Value::CreateUInt(jsArray.at(i).toUInt())); break;
            case JsVariant::JsType::Bool:
                cefV8Array->SetValue(i, CefV8Value::CreateBool(jsArray.at(i).toBool())); break;
            case JsVariant::JsType::Double:
                cefV8Array->SetValue(i, CefV8Value::CreateDouble(jsArray.at(i).toDouble())); break;
            case JsVariant::JsType::String:
                cefV8Array->SetValue(i, CefV8Value::CreateString(std::string(jsArray.at(i).toString().toLocal8Bit()))); break;

            case JsVariant::JsType::Array:
                cefV8Array->SetValue(i, convertJsArrayToV8Value(jsArray.at(i).toArray())); break;
            
            //  todo:

            case JsVariant::JsType::Invalid:
                throw std::exception("Invalid JsVairant type!");
            }
        }

        return cefV8Array;
    }

    void EmbededBrowser::RenderProc::bindFuncs(CefRefPtr<CefV8Value>& object)
    {
        for (auto& functionBinding : m_functionBindings)
            object->SetValue(functionBinding.first,
                             CefV8Value::CreateFunction(functionBinding.first, m_embededBrowser),
                             V8_PROPERTY_ATTRIBUTE_NONE);
    }

    void EmbededBrowser::RenderProc::enableJsFuncRegistering(CefRefPtr<CefV8Value>& object)
    {
        //  create a special register() function for registering JS functions
        //  in .js files
        object->SetValue("register",
                         CefV8Value::CreateFunction("register", m_embededBrowser),
                         V8_PROPERTY_ATTRIBUTE_NONE);
    }

    bool EmbededBrowser::RenderProc::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                                              CefProcessId source_process,
                                                              CefRefPtr<CefProcessMessage> message)
    {
        assert (CefCurrentlyOn(TID_RENDERER) == true);

        if (IpcMsg::isRenderExit(message))
            ExitProcess(0);
        else if (IpcMsg::isJsCall(message))
            return callJsFunc(message);
        else
            return false;
    }

    bool EmbededBrowser::RenderProc::callJsFunc(CefRefPtr<CefProcessMessage> message)
    {
        std::string funcName = message->GetArgumentList()->GetString(0);
        auto& funcEnvIter = m_embededBrowser->m_jsFuncEnvs.find(funcName);

    #ifndef NDEBUG  //  debug mode, do checkings
        if (funcEnvIter == m_embededBrowser->m_jsFuncEnvs.end())
            assert(false && "Js function not registered!");
    #endif

        //  call js function
        auto& jsFuncEnv = funcEnvIter->second;
        auto funcObject = std::get<ObjectIndex>(jsFuncEnv);
        CefRefPtr<CefV8Value> returnValue = funcObject->ExecuteFunctionWithContext(
            std::get<ContextIndex>(jsFuncEnv),
            NULL,
            std::get<ArgsIndex>(jsFuncEnv));

        //  check result
        if (returnValue == NULL)
            assert(false && "method is called incorrectly or an exception is thrown!");
        //  todo: possible return value
        return true;
    }

    void EmbededBrowser::RenderProc::OnWebKitInitialized()
    {
        //  todo: pass a valid handler
        //  not provide extension this time
        /*for (auto& extensionBinding : m_extensionBindings)
            CefRegisterExtension(std::get<0>(extensionBinding),
                                 std::get<1>(extensionBinding),
                                 std::get<2>(extensionBinding));*/
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    //                                      BrowserHandler                                       

    EmbededBrowser::BrowserHandler::BrowserHandler(QWidget* parent)
        : m_hasMaxButton(true)
        , m_hasMinButton(true)
        , m_hasCloseButton(true)
        , m_isFrameless(false)
        , m_sizable(true)
        , m_parentWidget(parent)
        , m_windowHandle(NULL)
        , m_windowSize()    //  invalid size
    {
    }

    EmbededBrowser::BrowserHandler::~BrowserHandler()
    {
    }

    void EmbededBrowser::BrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
    {
        CEF_REQUIRE_UI_THREAD();

        m_browser = browser;
        m_windowHandle = m_browser->GetHost()->GetWindowHandle();
        setWindowStyles();

        if (!m_windowSize.isValid())    updateWindowSize();
        else                            resizeWindow(m_windowSize);
    }

    void EmbededBrowser::BrowserHandler::setWindowStyles()
    {
        if (!m_hasMinButton)
            SetWindowLong(m_windowHandle, GWL_STYLE, GetWindowLong(m_windowHandle, GWL_STYLE) & ~WS_MINIMIZEBOX);
        if (!m_hasMaxButton)
            SetWindowLong(m_windowHandle, GWL_STYLE, GetWindowLong(m_windowHandle, GWL_STYLE) & ~WS_MAXIMIZEBOX);
        if (!m_hasCloseButton)
            EnableMenuItem(GetSystemMenu(m_windowHandle, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_REMOVE);
        if (!m_sizable)
            SetWindowLong(m_windowHandle, GWL_STYLE, GetWindowLong(m_windowHandle, GWL_STYLE) & ~WS_SIZEBOX);
        if (m_isFrameless) {
            SetWindowLong(m_windowHandle, GWL_STYLE, GetWindowLong(m_windowHandle, GWL_STYLE) & ~WS_CAPTION);

            //  when remove the caption, browser retains the wrong window size,
            //  obtain the correct size and resize
            resizeWindow(getClientAreaSize());
        }
    }

    QSize EmbededBrowser::BrowserHandler::getClientAreaSize()
    {
        assert(m_windowHandle != NULL);

        RECT rectangle;
        GetClientRect(m_windowHandle, &rectangle);
        return QSize(   rectangle.right - rectangle.left,
                        rectangle.bottom - rectangle.top );
    }

    void EmbededBrowser::BrowserHandler::updateWindowSize()
    {
        assert(m_windowHandle != NULL);

        RECT windowRectangle;
        GetWindowRect(m_windowHandle, &windowRectangle);
        m_windowSize.setWidth(windowRectangle.right - windowRectangle.left);
        m_windowSize.setHeight(windowRectangle.bottom - windowRectangle.top);
    }

    void EmbededBrowser::BrowserHandler::resizeWindow(const QSize& newSize)
    {
        assert(m_windowHandle != NULL);

        m_windowSize = newSize;
        SetWindowPos(m_windowHandle, HWND_TOP, 0, 0,
                     m_windowSize.width(), m_windowSize.height(),
                     SWP_NOMOVE | SWP_NOZORDER);
    }

    bool EmbededBrowser::BrowserHandler::DoClose(CefRefPtr<CefBrowser> browser)
    {
        CEF_REQUIRE_UI_THREAD();

        return false;
    }

    void EmbededBrowser::BrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
    {
    }

    bool EmbededBrowser::BrowserHandler::OnBeforePopup(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& target_url,
        const CefString& target_frame_name,
        WindowOpenDisposition target_disposition,
        bool user_gesture,
        const CefPopupFeatures& popupFeatures,
        CefWindowInfo& windowInfo,
        CefRefPtr<CefClient>& client,
        CefBrowserSettings& settings,
        bool* no_javascript_access)
    {
        //  return false to let the new window pop up;
        //  return true to reject the new window.
        assert (m_browser->GetMainFrame() != NULL);

        CefPostTask(TID_UI, CefCreateClosureTask(base::Bind(
            &CefFrame::LoadURL, m_browser->GetMainFrame(), target_url)));
        return true;
    }

    bool EmbededBrowser::BrowserHandler::OnProcessMessageReceived(
        CefRefPtr<CefBrowser> browser,
        CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message)
    {
        //todo:
        return false;            
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    //                                      IpcMsg class

    const char* EmbededBrowser::IpcMsg::msgJsCall = "lgxZJ.JsCall";
    const char* EmbededBrowser::IpcMsg::msgRenderExit = "lgxZJ.RenderExit";
    const char* EmbededBrowser::IpcMsg::msgBrowserExit = "lgxZJ.BrowserExit";

    ///////////////////////////////

    CefRefPtr<CefProcessMessage> EmbededBrowser::IpcMsg::jsCall(const QString& jsFuncName)
    {
        auto msg = CefProcessMessage::Create(msgJsCall);

        auto& args = msg->GetArgumentList();
        args->SetString(0, std::string(jsFuncName.toLocal8Bit()));

        return msg;
    }

    CefRefPtr<CefProcessMessage> EmbededBrowser::IpcMsg::renderExit()
    {
        return CefProcessMessage::Create(msgRenderExit);
    }

    CefRefPtr<CefProcessMessage> EmbededBrowser::IpcMsg::browserExit()
    {
        return CefProcessMessage::Create(msgBrowserExit);
    }

    bool EmbededBrowser::IpcMsg::isJsCall(CefRefPtr<CefProcessMessage> msg)
    {
        return msg->GetName() == msgJsCall;
    }

    bool EmbededBrowser::IpcMsg::isRenderExit(CefRefPtr<CefProcessMessage> msg)
    {
        return msg->GetName() == msgRenderExit;
    }

    bool EmbededBrowser::IpcMsg::isBrowserExit(CefRefPtr<CefProcessMessage> msg)
    {
        return msg->GetName() == msgBrowserExit;
    }
}