DirectSound对于**单声道**的Wav文件（或者说对于单声道的PCM音频数据）提供了内置3D音效的模拟，你能够控制每一个声源和收听者的立体位置，对移动的物体应用多普勒效果等等。在单个应用程序中，可以有多个声源，但是只能有一个收听者。

## 1. 3D音效概念简介
1.  3D坐标：    坐标采用**左手笛卡尔坐标系**，x轴向右延伸，y轴向上延伸，z轴向前延伸。
2.  3D处理模式： DirectSound由三种3D环境处理模式：**普通模式**、**禁用模式**、**头部相关模式**。普通模式就是*完整*的3D模式，禁用模式不言而喻，头部相关模式是*部分3D模式*（因为该模式下所有的声源**参数都是参考听者位置**的，而不是参考原点）。
3.  延迟生效：   因此每次应用3D配置，DirectSound都要进行大量的计算，可能会家中cpu的负担。因此，声源和听者都支持一种叫做**延迟设置**的方式，多个以*延迟设置*设置的参数需要调用提交函数才能应用到音频数据上，这样可以避免多次重复计算。

![coordinate-pic]

### 1.1 声源（Source）相关概念
*   最小距离：       在最小距离范围内，听到的声源音量大小保持不变；在最小距离范围外，听到的音量开始成比例衰减（例如最小距离是10米，则听者站在距离声源20米处听到的声音是原始声音的1/2）。
*   最大距离：       在最大距离外的听者，不管再怎么朝外移动，听到的音量都不会再减小。
*   朝向：          朝向用来指定声源投射的方向，具体见下一点。
*   音量投射角度：   3D环境下的音量有一个锥形的投射角度（**当设置朝向时**），分内角和外角：内角的声音是正常的音量，外角的音量通常是衰减的音量，在内角边界向外角边界过渡时音量逐级衰减。

![cone-pic]

*   移动速度：       移动速度在整个3D环境里主要是用来计算多普勒效应（Doppler Effect）的。  

### 1.2 听者（Listener）相关概念
*   朝向：     听者的朝向角度由两个3维向量构成，实际上这个朝向是一个平面。两个三维向量都以听者头部中心为原点，一个向顶部延伸，一个向前延伸。

![orientation-pic]

*   距离因子：   距离因子（Distance Factor）表示三维空间中的单位，与**米**成正比（例如距离因子为2，则一个三维点(1,0,0)表示现实中距离原点x轴2米）。
*   多普勒因子： 表示产生的多普勒效应是真实效应的几倍。
*   滚降因子：   表示音量随距离的衰减程度。0表示不衰减，1表示衰减成程度与现实环境相同，等等。

## 2. 3D示例程序
### 2.1 获取和设置声源最大最小距离
>   IDirectSound3DBuffer8::GetMaxDistance(D3DVALUE * pflMaxDistance);  
>   IDirectSound3DBuffer8::GetMinDistance(D3DVALUE * pflMinDistance);  
>   IDirectSound3DBuffer8::SetMaxDistance(D3DVALUE flMaxDistance, DWORD dwApply);  
>   IDirectSound3DBuffer8::SetMinDistance(D3DVALUE flMinDistance, DWORD dwApply);  

**dwApply**参数用来指定是立即生效还是延迟生效（需要调用**CommitDeferredSettings**函数）。

### 2.2 获取和设置声源音量投射角度
因为投射角度中外角肯定要比内角大，为了保证这一点我们需要做些额外的判断处理：

    void MainWindow::on_coneAnglesInside_valueChanged(int value)
    {
        DWORD insideAngle, outsideAngle;
        if (m_wavPlayer.get3DSource()->GetConeAngles(&insideAngle, &outsideAngle) != DS_OK)
            throw std::exception("GetConeAngles error");

        if (static_cast<unsigned>(value) > outsideAngle)
            ui->coneAnglesOutside->setValue(value),
            outsideAngle = value;
        if (m_wavPlayer.get3DSource()->SetConeAngles(value, outsideAngle, DS3D_IMMEDIATE) != DS_OK)
            throw std::exception("SetConeAngles error");

        ui->coneAnglesInsideLabel->setText(QString("coneAnglesInside(%1)").arg(value));
    }

### 2.3 获取和设置声源朝向
这里也有一个特殊情况要处理：朝向的3个维度不能都为0，否则就是一个无效的方向：

    #define SET_ONE_VECTOR_WITH_ZERO_CHECK_OF_SOURCE(getFunc, setFunc, sliderName, valueX, valueY, valueZ)  \
        D3DVECTOR vector;                                                                                   \
        if (m_wavPlayer.get3DSource()->getFunc(&vector) != DS_OK)                                           \
            throw std::exception(#getFunc " error");                                                        \
                                                                                                            \
        if (!(valueX == 0.0 && valueY == 0.0 && valueZ == 0.0))                                             \
            if (m_wavPlayer.get3DSource()->setFunc(valueX, valueY, valueZ, DS3D_IMMEDIATE) != DS_OK)        \
                throw std::exception(#setFunc " error");                                                    \
                                                                                                            \
        ui->sliderName##Label->setText(QString(#sliderName "(%1)").arg(value));

    void MainWindow::on_coneOrientationX_valueChanged(int value)
    {
        SET_ONE_VECTOR_WITH_ZERO_CHECK_OF_SOURCE(GetConeOrientation, SetConeOrientation, coneOrientationX,
                                                value, vector.y, vector.z);
    }

其他相关设置这里就不给出代码了，都比较简单，有需要的可以看[官方文档][msdn-link]。

## 3. 运行结果示意图
界面太大，我就分成3张图展示了：

![reuslt-pic1] ![reuslt-pic2] ![reuslt-pic3]

完整代码见[链接][code-link]。


[coordinate-pic]:   https://i.loli.net/2018/03/10/5aa3b3ab54fa3.gif
[orientation-pic]:  https://i.loli.net/2018/03/10/5aa3c3d01e614.gif
[cone-pic]:         https://i.loli.net/2018/03/10/5aa3c8229b76a.gif
[msdn-link]:        https://msdn.microsoft.com/en-us/library/windows/desktop/ee416965(v=vs.85).aspx
[reuslt-pic1]:      https://i.loli.net/2018/03/10/5aa3cdbe2b24b.png
[reuslt-pic2]:      https://i.loli.net/2018/03/10/5aa3cdcd04062.png
[reuslt-pic3]:      https://i.loli.net/2018/03/10/5aa3cdd8a1168.png
[code-link]:        https://github.com/lgxZJ/Miscellaneous/tree/master/Audio/DirectSoundPlay