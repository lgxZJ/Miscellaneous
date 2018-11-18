DirectSound的音频捕获原理和播放原理差不多，内部在一个缓冲区上循环写入捕获到的数据，并且提供notify通知功能。

## 1. 音频捕获
因为捕获流程和播放流程类似，我们就不在这里赘述了，只给出简单的函数引用和图示：

最后两个是音效捕获接口，用来控制Aec（回声消除）、Noise Fill（噪音填充）、Ns（噪音压制）的开启与关闭：

>   HRESULT DirectSoundCaptureEnumerate(LPDSENUMCALLBACK lpDSEnumCallback,LPVOID lpContext)  
>   HRESULT DirectSoundCaptureCreate8(LPCGUID lpcGUID, LPDIRECTSOUNDCAPTURE8 * lplpDSC, LPUNKNOWN pUnkOuter)  
>   IDirectSoundCapture8::GetCaps(LPDSCCAPS pDSCCaps)  
>   HRESULT IDirectSoundCapture8::CreateCaptureBuffer(LPCDSCBUFFERDESC pcDSCBufferDesc, LPDIRECTSOUNDCAPTUREBUFFER * ppDSCBuffer, LPUNKNOWN pUnkOuter)  
>   HRESULT IDirectSoundBuffer8::GetObjectInPath(REFGUID rguidObject, DWORD dwIndex, REFGUID rguidInterface, LPVOID * ppObject)  
>   IDirectSoundCaptureFXAec8  
>   IDirectSoundCaptureFXNoiseSuppress8

流程图如下：

![capture-pic]

## 2. 写入Wav
写入Wav文件，我们可以使用微软自带的**mmio**系列函数，但是切记不能与自己读写内存位置混用操作，不然会出各种无脑问题。

>   HMMIO mmioOpen(LPTSTR szFilename, LPMMIOINFO lpmmioinfo, DWORD dwOpenFlags);  
>   MMRESULT mmioCreateChunk(HMMIO hmmio, LPMMCKINFO lpck, UINT wFlag);  
>   LONG mmioWrite(HMMIO hmmio, char _huge *pch, LONG cch);  
>   MMRESULT mmioAscend(HMMIO hmmio, LPMMCKINFO lpck, UINT wFlags);  
>   MMRESULT mmioClose(HMMIO hmmio, UINT wFlags);  

![wav-pic]

## 3. Qml/C++ 集成交互
这次我尝试使用了Qml来进行界面绘制，怎么说呢：数据绑定确实很好用，许多仅停留在UI层的相互逻辑可以直接在UI代码上解决，不用深入到C++逻辑代码；但是C++数据类型转换到Qml不是很方便，且两者的类型系统衔接不自然（例如在Qml中使用C++类型的时候，需要register，额外操作太多）。

在Qml中私用C++类型，必须要借助Qt的元对象系统，因此我们需要：

*   **继承**自QObject并且带上Q_OBJECT宏。
*   **属性**用Q_PROPERTY声明。
*   **枚举**用Q_ENUM描述。
*   **信号、槽**是默认支持的，其他的函数需要用Q_INVOKABLE声明。

`在这个例子中，我需要传给Qml一个设备列表，但是Qml的list在Qt/C++中并没有直接对应的类型，这里需要用到Qt的QVariantList，在C++传给Qml时QVariantList会被直接转换成list。此外i，QVariantMap可以在Qml中可以被隐式转换成JavaScript的Array，但是不是那么自然。`

例子中用到的大概就是这样：

    class CppDsCapture : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QVariantList avaiableDeviceNames READ avaiableDeviceNames)

    public:
        enum EffectType {
            AcousticEchoCancellationMicrosoft,
            AcousticEchoCancellationSystem,
            NoiseSuppressionMicrosoft,
            NoiseSuppressionSystem,
            None
        };
        Q_ENUM(EffectType)

        ...


        //////////////////////////////////////////////////////////////
        //  QML functions
        Q_INVOKABLE void    openDevice(unsigned deviceIndex);

        ...
    }

## 4. 运行结果
![result-pic1] ![result-pic2] ![result-pic3]

完整代码见[链接][link]。


[capture-pic]:  https://i.loli.net/2018/03/31/5abefc0021e9e.png
[wav-pic]:      https://i.loli.net/2018/03/31/5abefb1c38e13.png
[result-pic1]:  https://i.loli.net/2018/03/31/5abf01422d149.png
[result-pic2]:  https://i.loli.net/2018/03/31/5abf01515e29c.png
[result-pic3]:  https://i.loli.net/2018/03/31/5abf015ce0453.png
[link]:         https://github.com/lgxZJ/Miscellaneous/tree/master/Audio/QmlDirectSoundCapture