# ESP32-S3 Terminal

基于 ESP32-S3、ESP-IDF 和 LVGL 的触摸屏智能终端项目。项目集成显示与触摸、Wi-Fi、天气信息、网络校时、SD 卡音乐播放以及麦克风录音等功能。

## 功能概览

- 使用 ST7789 LCD 显示 LVGL 图形界面。
- 使用 FT6336U 电容触摸控制器进行交互。
- 支持 Wi-Fi 扫描、连接、自动连接和 NVS 保存配置。
- Wi-Fi 连接后可通过 SNTP 同步系统时间。
- 从 SD 卡读取 WAV 音频，并通过 NS4168 功放播放。
- 通过数字麦克风录音，将录音保存为 PCM WAV 文件。
- 在 `record` 页面查看 SD 卡中的录音文件、查看时长并点击播放。
- 音乐播放页面支持播放列表、音量调节、播放进度和歌词显示。

## 硬件平台

- 主控：ESP32-S3（当前配置为 ESP32-S3，带 PSRAM）
- 显示屏：320 × 240 ST7789 SPI LCD
- 触摸控制器：FT6336U I2C
- 音频功放：NS4168
- 存储：SD 卡，1-bit SDMMC 模式
- 麦克风：I2S 数字麦克风

## 主要引脚

| 外设 | 信号 | GPIO |
| --- | --- | ---: |
| ST7789 LCD | MOSI | 7 |
| ST7789 LCD | SCLK | 15 |
| ST7789 LCD | CS | 4 |
| ST7789 LCD | DC | 6 |
| ST7789 LCD | RST | 5 |
| ST7789 LCD | 背光 | 16 |
| FT6336U | SDA | 3 |
| FT6336U | SCL | 18 |
| NS4168（I2S0） | DOUT | 39 |
| NS4168（I2S0） | BCLK | 40 |
| NS4168（I2S0） | LRC/WS | 41 |
| 麦克风（I2S1） | SD/DIN | 12 |
| 麦克风（I2S1） | SCK/BCLK | 11 |
| 麦克风（I2S1） | WS/LRCLK | 13 |
| SD 卡 | CLK | 14 |
| SD 卡 | CMD | 10 |
| SD 卡 | D0 | 17 |

LCD、触摸屏、SD 卡和音频相关引脚也分别定义在 `components/bsp`、`components/app/music` 和 `components/app/mic` 中。修改硬件连接后，应同步检查对应头文件中的宏定义。

## SD 卡文件布局

SD 卡挂载点为 `/S`，应用使用以下两个目录：

```text
/S/
├── MUSIC/       音乐播放页面扫描的 WAV 文件
└── WAV/         录音生成的 WAV 文件
```

首次使用时请确保 SD 卡已经格式化为 FAT 文件系统。程序不会在挂载失败时自动格式化 SD 卡；`/S/WAV` 目录在录音初始化时自动创建。

### 音乐文件

将 WAV 文件放入 `/S/MUSIC` 后，进入音乐页面即可刷新列表。当前播放链路主要面向 16-bit PCM WAV，支持 8～96 kHz、单声道或双声道文件。双声道数据会在输出前混合为单声道，以适配当前 NS4168 播放链路。

音乐文件如果有同名 `.lrc` 文件，也可以放在 `/S/MUSIC` 目录中，音乐页面会尝试加载对应歌词。

### 录音文件

录音默认格式为：

- 采样率：16 kHz
- 位宽：16 bit
- 声道：单声道
- 编码：PCM WAV

录音过程中先写入 `.part` 临时文件，停止录音并完成 WAV 头信息更新后，再保存为 `.wav` 文件。这样可以避免设备在录音中途断电时将未完成文件直接显示为有效录音。

当 SNTP 时间已经同步时，文件名包含创建时间，例如：

```text
record_2026-10-18_星期日_14-30-05_1.wav
```

如果时间尚未同步，则不会使用日期格式命名，而是使用设备运行计时值和序号避免文件名冲突。录音列表按最新完成的文件优先显示。

## 页面使用说明

### 音乐页面

1. 在主页面进入音乐功能。
2. 程序扫描 `/S/MUSIC` 下的 WAV 文件并显示列表。
3. 点击列表项开始播放。
4. 使用播放/暂停、上一首、下一首和音量控件控制播放。
5. 播放过程中会更新当前播放时间、总时长和进度条；存在对应歌词时显示歌词。
6. 离开音乐页面时会停止当前播放并释放播放状态。

### 录音页面

1. 进入 `record` 页面后，麦克风 I2S1 保持关闭，避免空闲时持续产生采样时钟。
2. 点击开始录音，程序停止共享播放器并启动麦克风采集。
3. 录音过程中显示录音时长，开始和结束按钮根据当前状态切换可用状态。
4. 点击结束录音，程序停止采集、完成 WAV 文件写入，并将新文件添加到列表顶部。
5. 点击列表中的 WAV 文件可以通过现有 NS4168 播放链路播放，时长显示为该文件的总时长。
6. 录音期间不会允许列表项播放，以避免录音和播放同时占用音频资源。

音乐播放使用 I2S0，麦克风录音使用 I2S1。两个功能共用应用层的播放任务，但使用各自的 I2S 通道。

## 软件架构

```text
main/
└── main.c                         初始化 NVS、LVGL 和应用任务

components/app/
├── app_task/                      Wi-Fi、音乐、录音等 FreeRTOS 任务
├── app_ui/                        音乐、录音、天气等页面的数据刷新
├── datetime/                      SNTP 时间同步和时间格式化
├── mic/                           I2S1 采集、录音队列和 WAV 文件写入
├── music/                         WAV 扫描、解码、I2S0 播放和歌词
├── wifi/                          Wi-Fi 配置、扫描和自动连接
├── weather/                       天气数据请求和缓存
└── xfyun_ai/                      语音/AI 相关功能

components/bsp/
├── bsp_lcd/                       ST7789 LCD 驱动
├── bsp_touch/                     FT6336U 触摸驱动
└── bsp_sdcard/                    SD 卡挂载和文件系统接口

components/ui/generated/           SquareLine/LVGL 生成的页面和事件代码
components/middleware/             LVGL 显示与触摸适配层
components/i2s_examples_common/   WAV 头辅助定义
```

应用入口顺序大致如下：

1. 初始化 NVS。
2. 初始化 LCD、触摸、LVGL 和 SD 卡。
3. 创建 Wi-Fi、音乐、天气、串口和录音相关任务。
4. Wi-Fi 连接成功后启动 SNTP 校时。
5. 用户通过 LVGL 页面操作音乐和录音功能。

## 编译与烧录

请先安装 ESP-IDF 5.x，并在 ESP-IDF 环境终端中进入项目根目录。当前工程目标芯片为 ESP32-S3。

```bash
idf.py set-target esp32s3
idf.py build
idf.py -p PORT flash monitor
```

将 `PORT` 替换为实际串口号，例如 Windows 下的 `COM3`：

```bash
idf.py -p COM3 flash monitor
```

如果只需要查看串口日志，可以使用：

```bash
idf.py -p COM3 monitor
```

退出 monitor 通常使用 `Ctrl+]`。工程的编译配置保存在 `sdkconfig` 中，修改芯片、Flash、PSRAM 或 FreeRTOS 配置后，建议确认配置与实际硬件一致。

## 常见问题

### SD 卡挂载失败或列表为空

- 检查 SD 卡是否插入并格式化为 FAT 文件系统。
- 检查 CLK、CMD、D0 三根信号线以及供电电压。
- 确认音乐文件放在 `/S/MUSIC`，录音文件放在 `/S/WAV`。
- 当前音乐列表只扫描 `.wav` 扩展名，扩展名大小写均可识别。

### 麦克风初始化失败

- 检查麦克风的 BCLK、WS 和 DIN 是否分别连接到 GPIO11、GPIO13 和 GPIO12。
- 确认麦克风使用 I2S 数字输出，并与当前左右声道配置一致。
- 确认 I2S1 没有被其他模块占用。
- 检查启动日志中 `I2S1`、任务创建和内存分配错误信息。

### 播放没有声音或声音异常

- 检查 NS4168 的 DOUT、BCLK、LRC 和地线连接。
- 确认 WAV 文件为有效的 16-bit PCM 音频。
- 检查音箱、功放供电以及音量设置。
- 如果只有录音播放异常，先确认录音文件已经完成保存且文件大小正常。

### 文件名没有日期时间

录音文件只有在 SNTP 成功同步后才会加入日期和星期信息。请先连接 Wi-Fi，并在日志中确认时间同步成功；未联网或校时失败时会使用无日期的备用命名方式。

## 相关目录

```text
main/                               程序入口
components/app/                     应用逻辑和业务任务
components/bsp/                     板级外设驱动
components/ui/                      LVGL 页面及事件代码
components/middleware/              LVGL 适配层
components/micro-wav/               WAV 解码组件
components/i2s_examples_common/     WAV 辅助定义
partitions_16MB.csv                 16 MB Flash 分区表
sdkconfig                           ESP-IDF 工程配置
```
