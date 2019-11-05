# static-movie-generator

本程序使用静态图片生成y4m格式的视频，并带有专辑封面自动风格转换功能。

用法简介：

请使用 `static-movie-generator.exe --help` 查看使用说明。

添加 `-c` 参数时将开启专辑封面自动转换功能。若没有添加`-c`参数，则输入图片视为所需输出的图片。

使用例子：

```bash
static-movie-generator.exe --input "cover.jpg" -d 5000 -c | QSVEncC64.exe --y4m -i - -o "output.mp4" --la 2400
```

## 依赖

* cmdline (已在源码库中附带)
* opencv 4.1.1

