# ESP32-S3 Terminal

基于 ESP32-S3、ESP-IDF 和 LVGL 的智能终端项目，提供触摸屏界面、Wi-Fi、天气、音乐播放和麦克风录音功能。

## 主要功能

- ST7789 LCD 与 FT6336U 触摸屏
- Wi-Fi 配网、自动连接和 SNTP 网络校时
- SD 卡音乐播放，音频通过 NS4168 输出
- 麦克风录音并保存为 WAV 文件
- 在 `record` 界面浏览、播放 SD 卡中的 WAV 文件
- LVGL 多页面用户界面

## 音频目录

SD 卡挂载后使用以下目录：

- `/S/MUSIC`：存放待播放的音乐文件
- `/S/WAV`：保存录音生成的 WAV 文件

音乐播放使用 I2S0，麦克风录音使用 I2S1。当前录音格式为 16 kHz、16 位、单声道 PCM WAV。

## 主要硬件引脚

| 功能 | 引脚 |
| --- | --- |
| NS4168 音频输出 | BCLK: GPIO40，LRC: GPIO41，DOUT: GPIO39 |
| 麦克风输入 | SCK: GPIO11，WS: GPIO13，SD: GPIO12 |
| SD 卡 | CLK: GPIO14，CMD: GPIO10，D0: GPIO17 |

LCD、触摸屏和其他外设引脚请参阅 `components/bsp` 下对应的头文件。

## 编译与烧录

请先安装 ESP-IDF，并在 ESP-IDF 环境中执行：

```bash
idf.py set-target esp32s3
idf.py build
idf.py -p PORT flash monitor
```

将 `PORT` 替换为实际串口号。项目使用 ESP-IDF 5.x，具体配置保存在 `sdkconfig` 中。

## 目录结构

```text
main/                         程序入口
components/app/               应用任务、UI、音乐、录音和网络功能
components/bsp/               LCD、触摸屏、SD 卡等板级驱动
components/ui/                LVGL 生成的界面代码
components/middleware/        LVGL 等中间件
components/i2s_examples_common/ WAV 辅助定义
```
