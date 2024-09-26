## Videos

`video-mc.cpp`
> Requires ffmpeg installed

### Usage

Using ffmpeg, convert an mp4 to a rgb file. To do so run `ffmpeg -i filename.mp4 -f rawvideo -pix_fmt rgb24 filename.rgb`

Run `./video-mc filename.rgb width height scale frame_rate`

> Scale is used to divide the image size

### Examples

`./video-mc rick-roll.rgb 640 480 16 24`

`./video-mc bad-apple.rgb 320 240 5 30`

<img src="rick-roll.png" alt="Rick Roll" width="200"/><img src="bad-apple.png" alt="Bad Apple" width="200"/>

YouTube video:

[![BAD APPLE!!](https://img.youtube.com/vi/FUt5WGvXZw0/0.jpg)](https://www.youtube.com/watch?v=FUt5WGvXZw0)
