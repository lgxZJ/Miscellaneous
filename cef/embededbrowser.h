#ifndef EMBEDEDBROWSER_H
#define EMBEDEDBROWSER_H

#include <qwidget.h>
#include <qrect.h>
#include <QVariant>
#include <qdatetime.h>

#include <vector>
#include <utility>

#include "include/cef_app.h"
#include "include/cef_client.h"

namespace lgxZJ
{
    class JsVariant;

    //  -------An interface of native function definitions-------
    //
    //  Inside this class, users cannot call js related functions(callJsFunc(), bindXXX()...).
    class NativeFunction
    {
    public:
        typedef std::vector<JsVariant>  Args;

        virtual void definition(const Args& args, JsVariant* result) = 0;
    };

    //  a c++ style Js array object class
    class JsArray
    {
    public:
        explicit JsArray() {}

        qint32 size() const;
        bool empty() const;
        const JsVariant& at(unsigned index) const;
        void insert(unsigned indexBefore, const JsVariant& value);
        void erase(unsigned index);
        void push_back(const JsVariant& value);

    private:
        std::vector<std::shared_ptr<JsVariant>> m_array;
    };

    //  similar to QVariant
    class JsVariant
    {
    public:
        enum class JsType {
            //  todo:
            Null, Undefined, String, Bool, Int, UInt, Double,
            Array, /*Func*//*Object*/ Date,
            Invalid,
        };

        explicit JsVariant();   //  invalid state
        explicit JsVariant(const JsArray& array);
        explicit JsVariant(const char* str);
        explicit JsVariant(const QString& value);
        explicit JsVariant(const bool value);
        explicit JsVariant(const qint32 value);
        explicit JsVariant(const quint32 value);
        explicit JsVariant(const double value);
        explicit JsVariant(const QDateTime& datetime);

        static JsVariant Null();
        static JsVariant Undefined();

        bool isValid() const;
        bool isNull() const;
        bool isUndefined() const;
        bool isString() const;
        bool isBool() const;
        bool isInt() const;
        bool isUInt() const;
        bool isDouble() const;
        bool isArray() const;
        bool isDate() const;

        QString toString() const;
        bool    toBool() const;
        qint32  toInt() const;
        quint32 toUInt() const;
        double  toDouble() const;
        JsArray toArray() const;
        QDateTime toDate() const;

        JsType type() const { return m_type; }

    private:
        JsType      m_type;
        QVariant    m_variant;

        //  QVariant does not support custome types perfectly
        JsArray     m_array;
    };

    //  the browser class.
    class EmbededBrowser : public QWidget
                         , public CefV8Handler
    {
        Q_OBJECT
    public:
        explicit EmbededBrowser(QWidget* parent);
        ~EmbededBrowser();

    public:
        //  todo: only quit browser
        void quit();
        int height() const;
        int width() const;
        bool isVisible() const;
        void move(int x, int y);
        QPoint pos() const;

        //  following functions should be called before show()
        void setInitUrl(const QString& url);
        void setInitSize(const QSize& size);
            //  following functions are only valid in non-embedded mode
        void setWindowFrameless();
        void enableMaximizeButton(bool enableFlag);
        void enableMinimizeButton(bool enableFlag);
        void enableCloseButton(bool enableFlag);
        void setSizable(bool enableFlag);
        
        

        //  called after showBrowser()
        bool canGoBack();
        bool canGoForward();

        //
        //  Interaction with JS and Native code. Most of the following function
        //  should be called before the browser window is shown.
        //
        //  Js code should be asyncronous!!!
        //

        //
        //  NOTE:   This function is asyncronous, there seems no way to make is syncronouse,
        //          calling this function continuously would makes following callings unwork.
        //
        void execJsCode(const QString& javascriptStr);

        //
        //  todo:   bind to special frame, make it callable in runtime
        //
        //  Binding values to the WINDOW object, the value is loaded every time the frame
        //  is reloaded, so you can call this function many times
        void bindJsNull(const QString& nullName);
        void bindJsUndefined(const QString& undefinedName);
        void bindJsString(const QString& stirngName, const QString& stirngValue);
        void bindJsBool(const QString& boolName, const bool boolValue);
        void bindJsInt(const QString& intName, const qint32 intValue);
        void bindJsUInt(const QString& uintName, const qint32 uintValue);
        void bindJsDouble(const QString& doubleName, const double doubleValue);
        void bindJsDate(const QString& dateName, const QDateTime& dateTime);
        void bindJsArray(const QString& arrayName, const JsArray& arrayValue);
        void bindNativeFunc(const QString& funcName, NativeFunction* handler);
        
        //  todo: bind object

        //  todo: this time, we do not provide this functionality
        //
        //  Extensions are like window bindings except they are loaded into
        //  the context for every frame and cannot be modified once loaded.
        //  void registerJsExtension(const QString& extensionName, const QString& extensionCode, NativeFunction* handler);

        //  Calling JS functions defined in .js files
        JsVariant callJsFunc(const QString& funcName);

    public:
        bool EmbededBrowser::Execute(
            const CefString &name, 
            CefRefPtr<CefV8Value> object,
            const CefV8ValueList &arguments, 
            CefRefPtr<CefV8Value> &retval,
            CefString &exception) OVERRIDE;

    public slots:
        //  
        //  Note:
        //      1.When embeded into a widget, please pass a parent widget pointer
        //        to the constructor and do not call this function explicitly.
        //      2.When not embeded, please pass a null pointer and call this function
        //        explicitly.
        //
        void show();

        void hide();
        void close();

        void reload();
        void loadUrl(const QString& url);
        void goBack();
        void goForward();
        void stopLoad();
        void setZoomLevel(double zoomLevel);
        void resize(const QSize& newSize);

    protected:
        virtual void closeEvent(QCloseEvent * event);
        virtual void resizeEvent(QResizeEvent * event);
        virtual void showEvent(QShowEvent * event);

    private:
        //  can use std::map instead, do tests
        typedef std::string                                                     JsFuncName;
        typedef CefRefPtr<CefV8Value>                                           JsFuncObject;
        typedef std::vector<CefRefPtr<CefV8Value>>                              JsFuncArgs;
        typedef CefRefPtr<CefV8Context>                                         JsFuncContext;
        typedef std::tuple<JsFuncObject, JsFuncArgs, JsFuncContext>             JsFuncEnv;
        typedef std::map<JsFuncName, JsFuncEnv>                                 JsFuncEnvs;
        typedef std::map<std::string, NativeFunction*>                          NativeFuncMappings;

    private:
        static CefRefPtr<CefV8Value> convertJsVariantToV8Value(const JsVariant& jsVariant);
        static JsVariant convertV8ValueToJsVariant(CefRefPtr<CefV8Value> value);
        static JsArray convertV8ValueToJsArray(CefRefPtr<CefV8Value> value);
        static NativeFunction::Args convertV8ValueListToJsVariantVec(const CefV8ValueList &arguments);

        CefApp* createCorrectArgumentByCmdLine();
        void runSubProcesses(CefRefPtr<CefApp> app);
        void initCef(CefRefPtr<CefApp>& app);
        void sendMessageToRender(CefRefPtr<CefProcessMessage> msg);
        bool callNativeFunc(const CefString &name, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval);
        bool collectJsFuncEnvs(const CefString &name, const CefV8ValueList &arguments);
        

    private:
        QWidget* m_parentWidget;
        QPoint m_windowPos;

        class BrowserProc;
        BrowserProc* m_browserProc;

        class BrowserHandler;
        CefRefPtr<BrowserHandler> m_browserHandler;

        static const int ObjectIndex = 0;
        static const int ArgsIndex = 1;
        static const int ContextIndex = 2;

        JsFuncEnvs          m_jsFuncEnvs;
        NativeFuncMappings  m_nativeFuncMapping;

        IMPLEMENT_REFCOUNTING(EmbededBrowser);
        DISALLOW_COPY_AND_ASSIGN(EmbededBrowser);

    //////////////////////////////////////////////////////////////////////////////////////
    //                                  Nested classes
    ///////////////////////////////////////////////////////////////////////////////////////

    private:
        //
        //  nested class hiding implemention details
        //

        // SimpleApp implements application-level callbacks for the browser process.
        // It will create the first browser instance in OnContextInitialized() after
        // CEF has initialized.
        class BrowserProc : public CefApp,
                            public CefBrowserProcessHandler,
                            public CefRequestHandler
        {
            friend class EmbededBrowser;

        public:
            explicit BrowserProc(CefRefPtr<BrowserHandler> browserHandler, QWidget* parent = nullptr);

            void setUrl(const QString& url);

            //  CefApp methods:
            virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }
            virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { return NULL; }
            virtual void OnBeforeCommandLineProcessing(
                const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;

            // CefBrowserProcessHandler methods:
            virtual void OnContextInitialized() OVERRIDE;

            virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) OVERRIDE; 

        private:
            void setAsChildIfHasParent(CefWindowInfo* windowInfo);

            std::string                 m_url;
            QWidget*                    m_parentWidget;
            CefRefPtr<BrowserHandler>   m_browserHandler;

            DWORD m_renderProcId;
            DWORD m_gpuProcId;
          
            IMPLEMENT_REFCOUNTING(BrowserProc);
        };

        class RenderProc : public CefApp
                         , public CefRenderProcessHandler
        {
            friend class EmbededBrowser;

        public:
            explicit RenderProc();

            //  CefApp methods:
            virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return NULL; }
            virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { return this; }

            // CefRenderProcessHandler methods:
            virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefV8Context> context) OVERRIDE;
            virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                                  CefProcessId source_process,
                                                  CefRefPtr<CefProcessMessage> message) OVERRIDE;
            virtual void OnWebKitInitialized() OVERRIDE;

        private:
            static CefRefPtr<CefV8Value> convertJsArrayToV8Value(const JsArray& jsArray);

            void bindVariables(CefRefPtr<CefV8Value>& object);
            void bindStrings(CefRefPtr<CefV8Value>& object);
            void bindNulls(CefRefPtr<CefV8Value>& object);
            void bindUndefined(CefRefPtr<CefV8Value>& object);
            void bindBools(CefRefPtr<CefV8Value>& object);
            void bindInts(CefRefPtr<CefV8Value>& object);
            void bindUInts(CefRefPtr<CefV8Value>& object);
            void bindDoubles(CefRefPtr<CefV8Value>& object);
            void bindDates(CefRefPtr<CefV8Value>& object);
            void bindArrays(CefRefPtr<CefV8Value>& object);
            void bindFuncs(CefRefPtr<CefV8Value>& object);
            void enableJsFuncRegistering(CefRefPtr<CefV8Value>& object);
            bool callJsFunc(CefRefPtr<CefProcessMessage> message);

        private:
            //  types in window binding between JS and Native code
            typedef std::string                             NullBinding;
            typedef std::string                             UndefinedBinding;

            typedef std::string                             BoolName;
            typedef bool                                    BoolValue;
            typedef std::pair<BoolName, BoolValue>          BoolBinding;

            typedef std::string                             IntName;
            typedef qint32                                  IntValue;
            typedef std::pair<IntName, IntValue>            IntBinding;

            typedef std::pair<IntName, quint32>             UIntBinding;

            typedef std::string                             DoubleName;
            typedef double                                  DoubleValue;
            typedef std::pair<DoubleName, DoubleValue>      DoubleBinding;

            typedef std::string                             StringName;
            typedef std::string                             StringValue;
            typedef std::pair<StringName, StringValue>      StringBinding;

            typedef QDateTime                               DateValue;
            typedef std::pair<std::string, DateValue>       DateBinding;

            typedef std::pair<std::string, JsArray>         ArrayBinding;

            typedef std::string                                     ExtName;
            typedef std::string                                     ExtCode;
            typedef std::tuple<ExtName, ExtCode, NativeFunction*>   ExtensionBinding;

            typedef std::string                             FuncName;
            typedef NativeFunction*                         FuncHandler;
            typedef std::pair<FuncName, FuncHandler>        FunctionBinding;

            typedef std::vector<std::string>        NullBindings;
            typedef std::vector<std::string>        UndefinedBindings;
            typedef std::vector<BoolBinding>        BoolBindings;
            typedef std::vector<IntBinding>         IntBindings;
            typedef std::vector<UIntBinding>        UIntBindings;
            typedef std::vector<DoubleBinding>      DoubleBindings;
            typedef std::vector<StringBinding>      StringBindings;
            typedef std::vector<DateBinding>        DateBindings;
            typedef std::vector<ArrayBinding>       ArrayBindings;
            typedef std::vector<ExtensionBinding>   ExtensionBindings;
            typedef std::vector<FunctionBinding>    FunctionBindings;

            NullBindings        m_nullBindings;
            UndefinedBindings   m_undefinedBindings;
            BoolBindings        m_boolBindings;
            IntBindings         m_intBindings;
            UIntBindings        m_uintBindings;
            DoubleBindings      m_doubleBindings;
            StringBindings      m_stringBindings;
            DateBindings        m_dateBindings;
            ArrayBindings       m_arrayBindings;
            //ExtensionBindings   m_extensionBindings;
            FunctionBindings    m_functionBindings;

            CefRefPtr<CefBrowser>   m_browser;

            IMPLEMENT_REFCOUNTING(RenderProc);
        };

        class OtherProc : public CefApp
        {
        public:
            IMPLEMENT_REFCOUNTING(OtherProc);
        };

        class BrowserHandler : public CefClient,
                               public CefDisplayHandler,
                               public CefLifeSpanHandler,
                               public CefLoadHandler
        {
        public:
            explicit BrowserHandler(QWidget* parent);
            ~BrowserHandler();

            friend class BrowserProc;
            friend class EmbededBrowser;

        public:
            // CefClient methods:
            virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE { return this; }
            virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }
            virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

            virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
            virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
            virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
            virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                CefRefPtr<CefFrame> frame,
                const CefString& target_url,
                const CefString& target_frame_name,
                WindowOpenDisposition target_disposition,
                bool user_gesture,
                const CefPopupFeatures& popupFeatures,
                CefWindowInfo& windowInfo,
                CefRefPtr<CefClient>& client,
                CefBrowserSettings& settings,
                bool* no_javascript_access)OVERRIDE;
            virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                                  CefProcessId source_process,
                                                  CefRefPtr<CefProcessMessage> message) OVERRIDE;

        private:
            void setWindowStyles();
            void updateWindowSize();
            QSize getClientAreaSize();
            void resizeWindow(const QSize& newSize);

        private:
            CefRefPtr<CefBrowser> m_browser;
            QWidget* m_parentWidget;
            HWND m_windowHandle;
            QSize m_windowSize;
            bool m_hasMinButton;
            bool m_hasMaxButton;
            bool m_hasCloseButton;
            bool m_isFrameless;
            bool m_sizable;

            IMPLEMENT_REFCOUNTING(BrowserHandler);
        };

        class IpcMsg
        {
        public:
            static CefRefPtr<CefProcessMessage> jsCall(const QString& jsFuncName);
            static CefRefPtr<CefProcessMessage> renderExit();
            static CefRefPtr<CefProcessMessage> browserExit();

            static bool isJsCall(CefRefPtr<CefProcessMessage> msg);
            static bool isRenderExit(CefRefPtr<CefProcessMessage> msg);
            static bool isBrowserExit(CefRefPtr<CefProcessMessage> msg);

        private:
            static const char* msgJsCall;
            static const char* msgRenderExit;
            static const char* msgBrowserExit;
        };
    };
}

#endif // EMBEDEDBROWSER_H
