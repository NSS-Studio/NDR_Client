import QtQuick 2.0
import QtQuick.Controls 2.5



ComboBox {
    id: comb
    width: 160
    height: 30
    textRole: "text"
    editable: false
    currentIndex: 0

    function addItem (item) {
        mod.append({"text": item.toString()})
    }

    model: ListModel{
        id: mod
    }
}
