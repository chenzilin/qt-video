import QtQuick 2.5

import VideoPlayer 1.0

Rectangle {
    width: 800
    height: 480
    color: "green"

    VideoPlayer {
        x: 10
        y: 10
        width: 720
        height: 272
        play: true
        source: "/home/chenzilin/Tmp/qt-video/mpeg2-video.raw"
    }
}
