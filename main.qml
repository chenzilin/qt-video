import QtQuick 2.5

import Mpeg2Player 1.0

Rectangle {
    width: 800
    height: 480
    color: "green"

    Mpeg2Player {
        x: 10
        y: 10
        width: 450
        height: 250
        play: true
        source: "/home/chenzilin/Tmp/mpeg2-player/test-video.raw"
    }
}
