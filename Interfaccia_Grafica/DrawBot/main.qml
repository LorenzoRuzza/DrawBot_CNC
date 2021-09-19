import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.0
import stringCommands 1.0

Window {
    width: 800
    height: 1000
    visible: true
    title: qsTr("DrawBot")
    id: root

    Row {
        id: menu

        Button {
            id: clear
            width: 50
            height: 50

            Image {
                anchors.centerIn: clear
                source: "qrc:/Immagini/Pulisci.png"
                width: 30
                height: 34
            }
            onClicked: {
                canvas.clear()
            }
        }

        Button {
            id: save
            width: 50
            height: 50

            Image {
                anchors.centerIn: save
                source: "qrc:/Immagini/Salva.png"
                width: 48
                height: 46
            }
            onClicked: {
                fileDialog.visible = true
            }
        }

        Button {
            id: stamp
            width: 50
            height: 50
            property bool stamp_button: false

            Image {
                anchors.centerIn: stamp
                source: "qrc:/Immagini/Stampa.png"
                width: 48
                height: 48
            }
            onClicked: {
                palette.button = "silver"
                pause.palette.button = "#e1e1e2"

                if (stamp_button == false) {
                    canvas.gcodeString += "M30;"
                    gcode.gcode = canvas.gcodeString
                }

                stamp_button = true

                gcode.paused = false
                gcode.writeData()
            }
        }

        Button {
            id: pause
            width: 50
            height: 50

            Image {
                anchors.centerIn: pause
                source: "qrc:/Immagini/Pausa.png"
                width: 48
                height: 48
            }
            onClicked: {
                palette.button = "silver"
                stamp.palette.button = "#e1e1e2"

                gcode.paused = true
            }
        }

        Button {
            id: debug
            width: 50
            height: 50

            Image {
                anchors.centerIn: debug
                source: "qrc:/Immagini/Debug.png"
                width: 48
                height: 48
            }
            onClicked: {
                console.log(canvas.gcodeString)
            }
        }

        Button {
            id: homing
            width: 50
            height: 50

            Image {
                anchors.centerIn: homing
                source: "qrc:/Immagini/Homing.png"
                width: 48
                height: 48
            }
            onClicked: {
                gcode.paused = false
                gcode.gcode = "G28;"
                gcode.writeData()
            }
        }

        Button {
            id: quit
            width: 50
            height: 50

            Image {
                anchors.centerIn: quit
                source: "qrc:/Immagini/Esci.png"
                width: 48
                height: 48
            }
            onClicked: {
                Qt.quit()
            }
        }
    }

    Column {
        id: formats
        anchors.top: menu.bottom

        Button {
            id: formatoA4
            text: "Formato A4"
            height: 25
            onClicked: {
                palette.button = "silver"
                formatoA5.palette.button = "#e1e1e2"

                // Formato A4: Centrimetri: 210 x 297 mm; Pixel 72 DPI: 595 x 842 - Pixel 150 DPI: 1240 x 1754 - Pixel 300 DPI: 2480 x 3508 - Pixel 600 DPI: 4960 x 7016
                page.width = (595 + 2)
                page.height = (842 + 2)
                canvas.width = 595
                canvas.height = 842

                canvas.conversionX = (210 / canvas.width).toFixed(2)    // 2 cifre decimali per un'accuratezza al centesimo di millimetro
                canvas.conversionY = (297 / canvas.height).toFixed(2)

                canvas.clear()
            }
        }

        Button {
            id: formatoA5
            text: "Formato A5"
            height: 25
            onClicked: {
                palette.button = "silver"
                formatoA4.palette.button = "#e1e1e2"

                // Formato A5: Centrimetri: 148 x 210 mm; Pixel 72 DPI: 420 x 595 - Pixel 150 DPI: 874 x 1240 - Pixel 300 DPI: 1748 x 2480 - Pixel 600 DPI: 3508 x 4960
                page.width = (420 + 2)
                page.height = (595 + 2)
                canvas.width = 420
                canvas.height = 595

                canvas.conversionX = (148 / canvas.width).toFixed(2)    // 2 cifre decimali per un'accuratezza al centesimo di millimetro
                canvas.conversionY = (210 / canvas.height).toFixed(2)

                canvas.clear()
            }
        }
    }

    Row {
        id: tools
        anchors.top: menu.bottom
        anchors.left: formats.right

        Button {
            id: pen
            width: 50
            height: 50
            palette.button: "silver"
            property bool pen_button: true

            Image {
                anchors.centerIn: pen
                source: "qrc:/Immagini/Penna.png"
                width: 48
                height: 48
            }
            onClicked: {
                palette.button = "silver"
                rubber.palette.button = "#e1e1e2"

                pen_button = true
                if (rubber.rubber_button == true) {
                    rubber.rubber_button = false
                    canvas.tool = true
                }
            }
        }

        Button {
            id: rubber
            width: 50
            height: 50
            property bool rubber_button: false

            Image {
                anchors.centerIn: rubber
                source: "qrc:/Immagini/Gomma.png"
                width: 48
                height: 48
            }
            onClicked: {
                palette.button = "silver"
                pen.palette.button = "#e1e1e2"

                pen.pen_button = false
                if (rubber_button == false) {
                    canvas.tool = true
                    rubber_button = true
                }
            }
        }

        Button {
            id: bucket
            width: 50
            height: 50
            property bool fill_button: false

            Image {
                anchors.centerIn: bucket
                source: "qrc:/Immagini/Secchiello.png"
                width: 48
                height: 48
            }
            onClicked: {
                palette.button = "silver"

                fill_button = true
            }
        }
    }

    Column {
        id: figures
        anchors.top: tools.bottom

        Button {
            id: segment
            width: 50
            height: 50
            property bool segment_shape: false

            Image {
                anchors.centerIn: segment
                source: "qrc:/Immagini/Segmento.png"
                width: 48
                height: 44
            }
            onClicked: {
                palette.button = "silver"
                pen.palette.button = "#e1e1e2"
                rubber.palette.button = "#e1e1e2"
                circle.palette.button = "#e1e1e2"
                square.palette.button = "#e1e1e2"
                rectangle.palette.button = "#e1e1e2"
                triangle.palette.button = "#e1e1e2"

                segment.segment_shape = true
                circle.circle_shape = false
                square.square_shape = false
                rectangle.rectangle_shape = false
                triangle.triangle_shape = false

                coordinates_segment.visible = true
                coordinates_quadrilateral.visible = false
                coordinates_triangle.visible = false
                radius.visible = false
            }
        }

        Button {
            id: square
            width: 50
            height: 50
            property bool square_shape: false

            Image {
                anchors.centerIn: square
                source: "qrc:/Immagini/Quadrato.png"
                width: 48
                height: 48
            }
            onClicked: {
                palette.button = "silver"
                pen.palette.button = "#e1e1e2"
                rubber.palette.button = "#e1e1e2"
                segment.palette.button = "#e1e1e2"
                circle.palette.button = "#e1e1e2"
                rectangle.palette.button = "#e1e1e2"
                triangle.palette.button = "#e1e1e2"

                segment.segment_shape = false
                circle.circle_shape = false
                square.square_shape = true
                rectangle.rectangle_shape = false
                triangle.triangle_shape = false

                coordinates_segment.visible = false
                coordinates_square.visible = true
                coordinates_quadrilateral.visible = true
                coordinates_triangle.visible = false
                radius.visible = false
            }
        }

        Button {
            id: rectangle
            width: 50
            height: 50
            property bool rectangle_shape: false

            Image {
                anchors.centerIn: rectangle
                source: "qrc:/Immagini/Rettangolo.png"
                width: 44
                height: 44
            }
            onClicked: {
                palette.button = "silver"
                pen.palette.button = "#e1e1e2"
                rubber.palette.button = "#e1e1e2"
                segment.palette.button = "#e1e1e2"
                square.palette.button = "#e1e1e2"
                circle.palette.button = "#e1e1e2"
                triangle.palette.button = "#e1e1e2"

                segment.segment_shape = false
                circle.circle_shape = false
                square.square_shape = false
                rectangle.rectangle_shape = true
                triangle.triangle_shape = false

                coordinates_segment.visible = false
                coordinates_square.visible = false
                coordinates_quadrilateral.visible = true
                radius.visible = false
            }
        }

        Button {
            id: triangle
            width: 50
            height: 50
            property bool triangle_shape: false

            Image {
                anchors.centerIn: triangle
                source: "qrc:/Immagini/Triangolo.png"
                width: 36
                height: 36
            }
            onClicked: {
                palette.button = "silver"
                pen.palette.button = "#e1e1e2"
                rubber.palette.button = "#e1e1e2"
                segment.palette.button = "#e1e1e2"
                square.palette.button = "#e1e1e2"
                rectangle.palette.button = "#e1e1e2"
                circle.palette.button = "#e1e1e2"

                segment.segment_shape = false
                circle.circle_shape = false
                square.square_shape = false
                rectangle.rectangle_shape = false
                triangle.triangle_shape = true

                coordinates_segment.visible = true
                coordinates_quadrilateral.visible = false
                coordinates_triangle.visible = true
                radius.visible = false
            }
        }

        Button {
            id: circle
            width: 50
            height: 50
            property bool circle_shape: false

            Image {
                anchors.centerIn: circle
                source: "qrc:/Immagini/Cerchio.png"
                width: 54
                height: 54
            }
            onClicked: {
                palette.button = "silver"
                pen.palette.button = "#e1e1e2"
                rubber.palette.button = "#e1e1e2"
                segment.palette.button = "#e1e1e2"
                square.palette.button = "#e1e1e2"
                rectangle.palette.button = "#e1e1e2"
                triangle.palette.button = "#e1e1e2"

                segment.segment_shape = false
                circle.circle_shape = true
                square.square_shape = false
                rectangle.rectangle_shape = false
                triangle.triangle_shape = false

                coordinates_segment.visible = false
                coordinates_quadrilateral.visible = false
                radius.visible = true
            }
        }
    }

    // Coordinate punto rettangolo e quadrato
    Rectangle {
        id: coordinates_quadrilateral
        visible: false
        width: 50
        height: 60
        anchors.top: figures.bottom
        anchors.topMargin: 10

        Text {
            id: fromLeft_text
            text: " From left:"
        }

        Rectangle {
            anchors.top: fromLeft_text.bottom
            anchors.topMargin: 3
            width: parent.width
            height: 20
            border.color: "black"
            radius: 4

            TextInput {
                id: fromLeft_input
                anchors.fill: parent
                anchors.margins: 4
            }
        }

        Text {
            id: fromTop_text
            anchors.top: fromLeft_text.bottom
            anchors.topMargin: 30
            text: " From top:"
        }

        Rectangle {
            anchors.top: fromTop_text.bottom
            anchors.topMargin: 3
            width: parent.width
            height: 20
            border.color: "black"
            radius: 4

            TextInput {
                id: fromTop_input
                anchors.fill: parent
                anchors.margins: 4
            }
        }

        // Lato quadrato
        Rectangle {
            id: coordinates_square
            visible: false
            width: 50
            height: 60
            anchors.top: fromTop_text.bottom
            anchors.topMargin: 30

            Text {
                id: side_text
                text: " Side:"
            }

            Rectangle {
                anchors.top: side_text.bottom
                anchors.topMargin: 3
                width: parent.width
                height: 20
                border.color: "black"
                radius: 4

                TextInput {
                    id: side_input
                    anchors.fill: parent
                    anchors.margins: 4
                }
            }
        }

    }

    // Coordinate punti segmento e triangolo
    Rectangle {
        id: coordinates_segment
        visible: false
        width: 50
        height: 200
        anchors.top: figures.bottom
        anchors.topMargin: 10

        Text {
            id: pointA_text
            text: " Point A(x,y):"
        }

        Rectangle {
            anchors.top: pointA_text.bottom
            anchors.topMargin: 3
            width: parent.width
            height: 20
            border.color: "black"
            radius: 4

            TextInput {
                id: pointA_xinput
                anchors.fill: parent
                anchors.margins: 4
            }
        }
        Rectangle {
            anchors.top: pointA_text.bottom
            anchors.topMargin: 25
            width: parent.width
            height: 20
            border.color: "black"
            radius: 4

            TextInput {
                id: pointA_yinput
                anchors.fill: parent
                anchors.margins: 4
            }
        }

        Text {
            id: pointB_text
            anchors.top: pointA_text.bottom
            anchors.topMargin: 53
            text: " Point B(x,y):"
        }

        Rectangle {
            anchors.top: pointB_text.bottom
            anchors.topMargin: 3
            width: parent.width
            height: 20
            border.color: "black"
            radius: 4

            TextInput {
                id: pointB_xinput
                anchors.fill: parent
                anchors.margins: 4
            }
        }

        Rectangle {
            anchors.top: pointB_text.bottom
            anchors.topMargin: 25
            width: parent.width
            height: 20
            border.color: "black"
            radius: 4

            TextInput {
                id: pointB_yinput
                anchors.fill: parent
                anchors.margins: 4
            }
        }

        // Terzo punto per il triangolo
        Rectangle {
            id: coordinates_triangle
            visible: false
            width: 50
            height: 60
            anchors.top: pointB_text.bottom
            anchors.topMargin: 53

            Text {
                id: pointC_text
                text: " Point C(x,y):"
            }

            Rectangle {
                anchors.top: pointC_text.bottom
                anchors.topMargin: 3
                width: parent.width
                height: 20
                border.color: "black"
                radius: 4

                TextInput {
                    id: pointC_xinput
                    anchors.fill: parent
                    anchors.margins: 4
                }
            }
            Rectangle {
                anchors.top: pointC_text.bottom
                anchors.margins: 25
                width: parent.width
                height: 20
                border.color: "black"
                radius: 4

                TextInput {
                    id: pointC_yinput
                    anchors.fill: parent
                    anchors.margins: 4
                }
            }
        }
    }

    // Raggio del cerchio
    Rectangle {
        id: radius
        visible: false
        width: 52
        height: 50
        anchors.top: figures.bottom
        anchors.topMargin: 10

        Text {
            id: radius_text
            text:  " Radius:"
        }

        Rectangle {
            anchors.top: radius_text.bottom
            anchors.topMargin: 3
            width: parent.width
            height: 20
            border.color: "black"
            radius: 4

            TextInput {
                id: radius_input
                anchors.fill: parent
                anchors.margins: 4
            }
        }
    }

    Rectangle {
        id: table
        anchors.top: tools.bottom
        anchors.topMargin: 10
        anchors.left: figures.right
        anchors.leftMargin: 35
        width: 1.1*page.width
        height: page.height

        ScrollView {
            clip: true
            anchors.fill: parent
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            Flickable {
                id: frame
                contentHeight: 1.5*page.height
                width: page.width

                Rectangle {
                    id: page
                    border.color: "black"

                    Canvas {
                        id: canvas
                        anchors.centerIn: page

                        property int lastX: 0
                        property int lastY: 0

                        property string gcodeString: ""
                        property int oldX: 0
                        property int oldY: 0
                        property bool sameX: false
                        property bool sameY: false

                        property bool beginPath: true
                        property bool start: false
                        property bool tool: false
                        property bool click: false

                        property int maxPointY: 0
                        property int maxPointX: 0
                        property bool line: false

                        property real conversionX: 0
                        property real conversionY: 0

                        function clear() {      // funzione foglio bianco
                            var ctx = getContext("2d")
                            ctx.reset()
                            canvas.requestPaint()

                            gcodeString = ""
                            start = false
                            tool = false

                            mouse_xcoordinate.visible = false
                            mouse_ycoordinate.visible = false

                            stamp.stamp_button = false
                            stamp.palette.button = "#e1e1e2"
                        }

                        function draw_segment() {
                            var ctx = getContext("2d")

                            ctx.strokeStyle = "black"
                            ctx.lineWidth = 2

                            ctx.beginPath()
                            ctx.moveTo((1*pointA_xinput.text / conversionX).toFixed(2), (1*pointA_yinput.text / conversionY).toFixed(2))
                            ctx.lineTo((1*pointB_xinput.text / conversionX).toFixed(2), (1*pointB_yinput.text / conversionY).toFixed(2))

                            // codice G del segmento
                            if (start == true) {
                                gcodeString += "M00; "
                            }
                            gcodeString += "G90; G00 X" + pointA_xinput.text + " Y" + pointA_yinput.text + "; "
                            start = true

                            if (1*pointA_xinput.text == 1*pointB_xinput.text) {
                                gcodeString += "G01 Y" + pointB_yinput.text + "; "
                            }
                            else if (1*pointA_yinput.text == 1*pointB_yinput.text) {
                                gcodeString += "G01 X" + pointB_xinput.text + "; "
                            }
                            else {
                                gcodeString += "G01 X" + pointB_xinput.text + " Y" + pointB_yinput.text + "; "
                            }
                        }

                        function fill_shape(originX, originY, virtual_width, virtual_height) {
                            var ctx = getContext("2d")

                            gcodeString += "G90; "
                            for (var l = originY; l <= (originY + virtual_height); l++) {
                                if (l % 2 == 0) {
                                    for (var m = originX - 2; m <= (originX + virtual_width) + 2; m++) {
                                        if (ctx.isPointInPath(m, l) === true && line == false) {
                                            gcodeString += "G00 X" + (m * conversionX).toFixed(2) + " Y" + (l * conversionY).toFixed(2) + "; "
                                            line = true
                                        }
                                        else if (ctx.isPointInPath(m, l) === false && line == true) {
                                            gcodeString += "G01 X" + ((m - 1) * conversionX).toFixed(2) + "; "
                                            line = false
                                        }
                                    }
                                }
                                else {
                                    for (var n = (originX + virtual_width) + 2; n >= originX - 2 + 1; n--) {
                                        if (ctx.isPointInPath(n, l) === true && line == false) {
                                            gcodeString += "G00 X" + (n * conversionX).toFixed(2) + " Y" + (l * conversionY).toFixed(2) + "; "
                                            line = true
                                        }
                                        else if (ctx.isPointInPath(n, l) === false && line == true) {
                                            gcodeString += "G01 X" + ((n + 1) * conversionX).toFixed(2) + "; "
                                            line = false
                                        }
                                    }
                                }
                            }
                            line = false
                        }

                        onPaint: {

                            frame.interactive = false
                            var ctx = getContext("2d")

                            if (pen.pen_button == true) {     // impostazione colore per la penna
                                ctx.strokeStyle = "black"
                                ctx.lineWidth = 2
                            }

                            else if (rubber.rubber_button == true) {      // impostazione colore per la gomma
                                ctx.strokeStyle = "white"
                                ctx.lineWidth = 10
                            }

                            //    disegno con penna o gomma e creazione G code per la penna
                            if ((pen.pen_button == true || rubber.rubber_button == true) && area.pressedButtons) {
                                ctx.beginPath()
                                ctx.moveTo(lastX, lastY)
                                lastX = area.mouseX
                                lastY = area.mouseY
                                ctx.lineTo(lastX, lastY)

                                if (lastX < 0 || lastX > canvas.width || lastY < 0 || lastY > canvas.height) {
                                    beginPath = true
                                }
                                else {
                                    if (gcodeString == "") {    // inizializzazione G code
                                        gcodeString += "M602; "
                                    }
                                    else {      // istruzioni G code per il tratto manuale
                                        if(beginPath == true) {       // comando G spostamento penna
                                            if (tool == true) {
                                                if (pen.pen_button == true) {
                                                    gcodeString += "T01; "
                                                    tool = false
                                                }
                                                else if (rubber.rubber_button == true) {
                                                    gcodeString += "T02; "
                                                    tool = false
                                                }
                                            }

                                            if (start == true) {
                                                gcodeString += "M00; "
                                            }
                                            gcodeString += "G90; G00 X" + (lastX * conversionX).toFixed(2) + " Y" + (lastY * conversionY).toFixed(2) + "; "
                                            beginPath = false
                                            start = true

                                            oldX = lastX
                                            oldY = lastY
                                        }   // comandi tracciamento linea retta verticale, orrizontale ed obliqua
                                        else {
                                            if (oldX == lastX) {
                                                sameX = true
                                            }
                                            else if (oldY == lastY) {
                                                sameY = true
                                            }
                                            else {
                                                if (sameX == true) {
                                                    gcodeString += "G01 Y" + (oldY * conversionY).toFixed(2) + "; "
                                                    sameX = false
                                                }
                                                else if (sameY == true) {
                                                    gcodeString += "G01 X" + (oldX * conversionX).toFixed(2) + "; "
                                                    sameY = false
                                                }
                                                else {
                                                    gcodeString += "G01 X" + (lastX * conversionX).toFixed(2) + " Y" + (lastY * conversionY).toFixed(2) + "; "
                                                }
                                            }
                                            oldX = lastX
                                            oldY = lastY
                                        }
                                    }
                                }

                                ctx.stroke()
                            }

                            // disegno del quadrato
                            if (square.square_shape == true && area.pressedButtons) {
                                pen.pen_button = false
                                rubber.rubber_button = false

                                if (fromLeft_input.text == "" && fromTop_input.text == "") {
                                    fromLeft_input.text = ((lastX * conversionX).toFixed(2)).toString()
                                    fromTop_input.text = ((lastY * conversionY).toFixed(2)).toString()
                                }
                                else {
                                    ctx.strokeStyle = "black"
                                    ctx.lineWidth = 2

                                    ctx.beginPath()
                                    ctx.rect((1*fromLeft_input.text / conversionX).toFixed(2), (1*fromTop_input.text / conversionY).toFixed(2), (1*side_input.text / conversionX).toFixed(2), (1*side_input.text / conversionX).toFixed(2));
                                    click = true

                                    // G code del quadrato
                                    if (start == true) {
                                        gcodeString += "M00; "
                                    }
                                    gcodeString += "G90; G00 X" + fromLeft_input.text + " Y" + fromTop_input.text + "; "
                                    start = true

                                    // perimetro quadrato in codice G
                                    gcodeString += "G91; G01 X" + side_input.text + "; "
                                    gcodeString += "G01 Y" + side_input.text + "; "
                                    gcodeString += "G01 X-" + side_input.text + "; "
                                    gcodeString += "G01 Y-" + side_input.text + "; "

                                    if (bucket.fill_button == true) {   // riempimento quadrato e relativo codice G
                                        ctx.fill()
                                        bucket.fill_button = false

                                        oldX = (1*fromLeft_input.text / conversionX).toFixed(2)
                                        oldY = (1*fromTop_input.text / conversionX).toFixed(2)
                                        maxPointX = (1*side_input.text / conversionX).toFixed(2)
                                        maxPointY = (1*side_input.text / conversionX).toFixed(2)
                                        fill_shape(oldX, oldY, maxPointX, maxPointY)
                                    }
                                }
                                ctx.stroke()

                                if (click == true) {
                                    fromLeft_input.text = ""
                                    fromTop_input.text = ""
                                    click = false

                                    square.palette.button = "#e1e1e2"
                                    square.square_shape = false
                                }
                            }

                            // disegno del rettangolo
                            if (rectangle.rectangle_shape == true && area.pressedButtons) {
                                pen.pen_button = false
                                rubber.rubber_button = false

                                if (fromLeft_input.text == "" && fromTop_input.text == "") {
                                    fromLeft_input.text = ((lastX * conversionX).toFixed(2)).toString()
                                    fromTop_input.text = ((lastY * conversionY).toFixed(2)).toString()
                                }
                                else {
                                    ctx.strokeStyle = "black"
                                    ctx.lineWidth = 2

                                    ctx.beginPath()
                                    ctx.moveTo(lastX, lastY)
                                    ctx.lineTo(lastX, (1*fromTop_input.text / conversionY).toFixed(2))
                                    ctx.lineTo((1*fromLeft_input.text / conversionX).toFixed(2), (1*fromTop_input.text / conversionY).toFixed(2))
                                    ctx.lineTo((1*fromLeft_input.text / conversionX).toFixed(2), lastY)
                                    ctx.lineTo(lastX, lastY)
                                    click = true

                                    // G code del rettangolo
                                    if (start == true) {
                                        gcodeString += "M00; "
                                    }
                                    gcodeString += "G90; G00 X" + fromLeft_input.text + " Y" + fromTop_input.text + "; "
                                    start = true

                                    // perimetro del rettangolo in codice G
                                    gcodeString += "G01 X" + (lastX * conversionX).toFixed(2) + "; "
                                    gcodeString += "G01 Y" + (lastY * conversionY).toFixed(2) + "; "
                                    gcodeString += "G01 X" + fromLeft_input.text + "; "
                                    gcodeString += "G01 Y" + fromTop_input.text + "; "

                                    if (bucket.fill_button == true) {    // riempimento del rettangolo e relativo codice G
                                        ctx.fill()
                                        bucket.fill_button = false

                                        if ((1*fromLeft_input.text / conversionX).toFixed(2) < lastX) {
                                            oldX = (1*fromLeft_input.text / conversionX).toFixed(2)
                                            maxPointX = lastX
                                        }
                                        else {
                                            oldX = lastX
                                            maxPointX = (1*fromLeft_input.text / conversionX).toFixed(2)
                                        }

                                        if ((1*fromTop_input.text / conversionY).toFixed(2) < lastY) {
                                            oldY = (1*fromTop_input.text / conversionY).toFixed(2)
                                            maxPointY = lastY
                                        }
                                        else {
                                            oldY = lastY
                                            maxPointY = (1*fromTop_input.text / conversionY).toFixed(2)
                                        }

                                        fill_shape(oldX, (oldY + 1), maxPointX, maxPointY)
                                    }
                                }
                                ctx.stroke()

                                if (click == true) {
                                    fromLeft_input.text = ""
                                    fromTop_input.text = ""
                                    click = false

                                    rectangle.palette.button = "#e1e1e2"
                                    rectangle.rectangle_shape = false
                                }
                            }

                            // disegno del segmento o del triangolo
                            if ((segment.segment_shape == true || triangle.triangle_shape == true) && area.pressedButtons) {
                                pen.pen_button = false
                                rubber.rubber_button = false

                                if (pointA_xinput.text == "" && pointA_yinput.text == "") {
                                    pointA_xinput.text = ((lastX * conversionX).toFixed(2)).toString()
                                    pointA_yinput.text = ((lastY * conversionY).toFixed(2)).toString()
                                }
                                else if ((pointB_xinput.text == "" && pointB_yinput.text == "") &&
                                         (pointA_xinput.text != "" && pointA_yinput.text != "")) {
                                    pointB_xinput.text = ((lastX * conversionX).toFixed(2)).toString()
                                    pointB_yinput.text = ((lastY * conversionY).toFixed(2)).toString()
                                }
                                else if (triangle.triangle_shape == true &&
                                         (pointC_xinput.text == "" && pointC_yinput.text == "") &&
                                         (pointB_xinput.text != "" && pointB_yinput.text != "") &&
                                         (pointA_xinput.text != "" && pointA_yinput.text != "")) {
                                    pointC_xinput.text = ((lastX * conversionX).toFixed(2)).toString()
                                    pointC_yinput.text = ((lastY * conversionY).toFixed(2)).toString()
                                }

                                // disegno del triangolo e relativo codice G
                                if (triangle.triangle_shape == true &&
                                        (pointC_xinput.text != "" && pointC_yinput.text != "") &&
                                        (pointB_xinput.text != "" && pointB_yinput.text != "") &&
                                        (pointA_xinput.text != "" && pointA_yinput.text != "")) {
                                    draw_segment()

                                    console.log(true)
                                    ctx.lineTo((1*pointC_xinput.text / conversionX).toFixed(2), (1*pointC_yinput.text / conversionX).toFixed(2))
                                    ctx.lineTo((1*pointA_xinput.text / conversionX).toFixed(2), (1*pointA_yinput.text / conversionX).toFixed(2))
                                    click = true

                                    if (1*pointC_xinput.text == 1*pointB_xinput.text) {
                                        gcodeString += "G01 Y" + pointC_yinput.text + "; "
                                    }
                                    else if (1*pointC_yinput.text == 1*pointB_yinput.text) {
                                        gcodeString += "G01 X" + pointC_xinput.text + "; "
                                    }
                                    else {
                                        gcodeString += "G01 X" + pointC_xinput.text + " Y" + pointC_yinput.text + "; "
                                    }

                                    if (1*pointA_xinput.text == 1*pointC_xinput.text) {
                                        gcodeString += "G01 Y" + pointA_yinput.text + "; "
                                    }
                                    else if (1*pointA_yinput.text == 1*pointC_yinput.text) {
                                        gcodeString += "G01 X" + pointA_xinput.text + "; "
                                    }
                                    else {
                                        gcodeString += "G01 X" + pointA_xinput.text + " Y" + pointA_yinput.text + "; "
                                    }

                                    if (bucket.fill_button == true) {   // riempimento triangolo e relativo G code
                                        ctx.fill()
                                        bucket.fill_button = false

                                        // determinazione punti massimo e minimo
                                        if (1*pointA_yinput.text > 1*pointB_yinput.text) {
                                            maxPointY = (1*pointA_yinput.text / conversionY).toFixed(0)
                                            oldY = (1*pointB_yinput.text / conversionY).toFixed(0)
                                            if (1*pointC_yinput.text > 1*pointA_yinput.text) {
                                                maxPointY = (1*pointC_yinput.text / conversionY).toFixed(0)
                                            }
                                            else if (1*pointC_yinput.text < 1*pointB_yinput.text) {
                                                oldY = (1*pointC_yinput.text / conversionY).toFixed(0)
                                            }
                                        }
                                        else {
                                            maxPointY = (1*pointB_yinput.text / conversionY).toFixed(0)
                                            oldY = (1*pointA_yinput.text / conversionY).toFixed(0)
                                            if (1*pointC_yinput.text > 1*pointB_yinput.text) {
                                                maxPointY = (1*pointC_yinput.text / conversionY).toFixed(0)
                                            }
                                            else if (1*pointC_yinput.text < 1*pointA_yinput.text) {
                                                oldY = (1*pointC_yinput.text / conversionY).toFixed(0)
                                            }
                                        }

                                        // determinazione estremo sinistro e destro
                                        if (1*pointA_xinput.text > 1*pointB_xinput.text) {
                                            maxPointX = (1*pointA_xinput.text / conversionX).toFixed(0)
                                            oldX = (1*pointB_xinput.text / conversionX).toFixed(0)
                                            if (1*pointC_xinput.text > 1*pointA_xinput.text) {
                                                maxPointX = (1*pointC_xinput.text / conversionX).toFixed(0)
                                            }
                                            else if (1*pointC_xinput.text < 1*pointB_xinput.text) {
                                                oldX = (1*pointC_xinput.text / conversionX).toFixed(0)
                                            }
                                        }
                                        else {
                                            maxPointX = (1*pointB_xinput.text / conversionX).toFixed(0)
                                            oldX = (1*pointA_xinput.text / conversionX).toFixed(0)
                                            if (1*pointC_xinput.text > 1*pointB_xinput.text) {
                                                maxPointX = (1*pointC_xinput.text / conversionX).toFixed(0)
                                            }
                                            else if (1*pointC_xinput.text < 1*pointA_xinput.text) {
                                                oldX = (1*pointC_xinput.text / conversionX).toFixed(0)
                                            }
                                        }

                                        fill_shape(oldX, (oldY + 1), maxPointX, maxPointY)
                                    }
                                }
                                else if (segment.segment_shape == true &&
                                         (pointB_xinput.text != "" && pointB_yinput.text != "") &&
                                         (pointA_xinput.text != "" && pointA_yinput.text != "")) {
                                    draw_segment()

                                    click = true
                                }

                                ctx.stroke()

                                if (click == true) {
                                    pointA_xinput.text = ""
                                    pointA_yinput.text = ""
                                    pointB_xinput.text = ""
                                    pointB_yinput.text = ""
                                    pointC_xinput.text = ""
                                    pointC_yinput.text = ""
                                    click = false

                                    segment.palette.button = "#e1e1e2"
                                    segment.segment_shape = false
                                    triangle.palette.button = "#e1e1e2"
                                    triangle.triangle_shape = false
                                }
                            }

                            // disegno del cerchio
                            if (circle.circle_shape == true && area.pressedButtons) {
                                pen.pen_button = false
                                rubber.rubber_button = false
                                ctx.strokeStyle = "black"
                                ctx.lineWidth = 2

                                if (radius_input.text == "") {
                                    radius_input.text = 0
                                }

                                ctx.beginPath()
                                ctx.arc(lastX, lastY, (1*radius_input.text / conversionX).toFixed(2), 0, 2 * Math.PI)

                                // codice G del cerchio
                                if (start == true) {
                                    gcodeString += "M00; "
                                }
                                gcodeString += "G90; G00 X" + ((lastX * conversionX) - 1*radius_input.text).toFixed(2) + " Y" + (lastY * conversionY).toFixed(2) + ";"
                                start = true

                                // codice G della circonferenza
                                gcodeString += "G02 I" + radius_input.text + "; "

                                if (bucket.fill_button == true) {   // riempimento cerchio e relativo codice G
                                    ctx.fill()
                                    bucket.fill_button = false

                                    oldX = lastX - (1*radius_input.text / conversionX).toFixed(2)
                                    oldY = lastY - (1*radius_input.text / conversionY).toFixed(2)
                                    maxPointX = 2*(1*radius_input.text / conversionX).toFixed(2)
                                    maxPointY = 2*(1*radius_input.text / conversionX).toFixed(2)
                                    fill_shape(oldX, oldY, maxPointX, maxPointY)
                                }

                                ctx.stroke()

                                circle.palette.button = "#e1e1e2"
                                circle.circle_shape = false
                            }

                            if (bucket.fill_button == false) {
                                bucket.palette.button = "#e1e1e2"
                            }
                        }

                        MouseArea {
                            id: area
                            anchors.fill: parent
                            hoverEnabled: true

                            property int zoom: 0

                            onPressed: {
                                canvas.lastX = mouseX
                                canvas.lastY = mouseY

                                canvas.requestPaint()
                            }

                            onReleased: {
                                canvas.beginPath = true
                            }

                            onPositionChanged: {
                                if (!pressedButtons) {
                                    canvas.lastX = mouseX
                                    canvas.lastY = mouseY
                                }

                                if (pen.pen_button == true || rubber.rubber_button == true) {
                                    canvas.requestPaint()
                                }
                                if (mouseX >= 0 && mouseX <= canvas.width && mouseY >= 0 && mouseY <= canvas.height) {
                                    mouse_xcoordinate.text = " x: " + (mouseX * canvas.conversionX).toFixed(2)
                                    mouse_xcoordinate.visible = true
                                    mouse_ycoordinate.text = " y: " + (mouseY * canvas.conversionY).toFixed(2)
                                    mouse_ycoordinate.visible = true
                                }
                            }

                            onWheel: {
                                if (wheel.angleDelta.y > 0) {
                                    if (zoom != 6) {
                                        page.scale +=0.2
                                        zoom ++
                                    }
                                }
                                else if (wheel.angleDelta.y < 0) {
                                    if (zoom != -3) {
                                        page.scale -=0.2
                                        zoom --
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        width: 20
        height: 20
        anchors.top: tools.bottom
        anchors.topMargin: 15
        anchors.left: table.right
        Text {
            id: mouse_xcoordinate
        }
    }

    Rectangle {
        width: 20
        height: 20
        anchors.top: tools.bottom
        anchors.topMargin: 35
        anchors.left: table.right
        Text {
            id: mouse_ycoordinate
        }
    }

    DashPath {
        id: gcode
    }

    FileDialog {
        id: fileDialog
        title: "Save gcode string"
        folder: shortcuts.home
        selectExisting: false
        onAccepted: {
            if (stamp.stamp_button == false) {
                canvas.gcodeString += "M30;"
            }

            gcode.gcode = canvas.gcodeString
            gcode.save(fileUrl)
        }
    }
}
