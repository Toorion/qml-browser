import QtQuick
import QtQml.Models 2.3

DelegateModel {

    function update() {
        if (items.count > 0) {
            items.setGroups(0, items.count, "items");
        }

        appendChilds(0, 0);
    }

    function appendChilds(parentId, depth) {
        let item;
        let n = 0;
        for (let i = 0; i < items.count; ++i) {
            item = items.get(i);

            if (item.model.parentId != parentId) {
                continue;
            }
            item.inVisible = true;
            //visibleItems.move(item.visibleIndex, n, 1);
            n++;
            if(item.model.expanded) {
                appendChilds(item.model.id, depth+1);
            }
        }
    }

    items.onChanged: update()

    groups: DelegateModelGroup {
        id: visibleItems

        name: "visible"
        includeByDefault: false
    }

    filterOnGroup: "visible"
} 
