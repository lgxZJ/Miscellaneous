import QtQuick 2.7
import QtQuick.Particles 2.0

Rectangle {
    color: "black"

    ParticleSystem {
        id: particleSystem
        anchors.fill: parent

        ImageParticle {
            groups: ["lgxZJ"]
            source: "qrc:///resources/star.png"
            anchors.fill: parent
        }

        Emitter {
            group: "lgxZJ"
            system: particleSystem

            emitRate:100
            lifeSpan: 1000
            size: 16
            sizeVariation: 10
            anchors.fill: parent
        }

        Turbulence {
            strength: 20
        }
    }


}
