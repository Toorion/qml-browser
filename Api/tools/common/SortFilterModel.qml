import QtQuick
import QtQml.Models 2.3

DelegateModel {
    id: delegateModel

    property var lessThan: function(left, right) { return true; }
    property var filterAcceptsItem: function(item) { return true; }

    function update() {
        if (items.count > 0) {
            items.setGroups(0, items.count, "items");
        }

        // Filter items
        let visible = [];
        let item;
        for (let i = 0; i < items.count; ++i) {
            item = items.get(i);
            if (filterAcceptsItem(item.model)) {
                visible.push(item);
            }
        }

        // Sort the list of visible items
        visible.sort(function(a, b) {
            return lessThan(a.model, b.model) ? -1 : 1;
        });

        // Add all items to the visible group:
        for (let i = 0; i < visible.length; ++i) {
            item = visible[i];
            item.inVisible = true;
            if (item.visibleIndex !== i) {
                visibleItems.move(item.visibleIndex, i, 1);
            }
        }
    }

    function modelByIndex(idx) {
        return model.get(idx);
    }

    items.onChanged: update()
    onLessThanChanged: update()
    onFilterAcceptsItemChanged: update()

    groups: DelegateModelGroup {
        id: visibleItems

        name: "visible"
        includeByDefault: false
    }

    filterOnGroup: "visible"
} 
