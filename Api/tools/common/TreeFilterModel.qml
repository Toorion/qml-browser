import QtQuick
import QtQml.Models 2.3

DelegateModel {

    property var idx: []

    function update() {
        if (items.count > 0) {
            items.setGroups(0, items.count, "items");
        }

        idx = [];
        appendChilds(0, 0);

        let visible = [];
        for (let i = 0; i < idx.length; i++) {
            let item = items.get(idx[i]);
            visible.push(item);
        }

        for (let i = 0; i < visible.length; i++) {
            let item = visible[i];
            item.inVisible = true;
            if (item.visibleIndex !== i) {
                visibleItems.move(item.visibleIndex, i, 1);
            }
        }
    }

    function appendChilds(parentId, depth, countOnly = false) {
        let item;
        let childs = [];
        for (let i = 0; i < items.count; i++) {
            item = items.get(i);

            if (item.model.parentId != parentId) {
                continue;
            }
            childs[childs.length] = {i: i, idx: item.model.idx, id: item.model.id, expanded: item.model.expanded, index: item.model.index}
            if (countOnly) {
                continue;
            }
        }

        if (!countOnly) {
            childs.sort(function(a, b) {
                return a.idx < b.idx ? -1 : 0;
            });

            for (let i = 0; i < childs.length; i++) {
                let child = childs[i];
                idx[idx.length] = child.i;
                let cnt = appendChilds(child.id, depth+1, !child.expanded);
                model.setProperty(child.index, "childsCount", cnt);
                model.setProperty(child.index, "depth", depth);
            }
        }
        return childs.length;
    }


    function move(from, to) {
        if (from == to) {
            return;
        }

        let from1 = idx.indexOf(from);
        let to1 = idx.indexOf(to);

        let m = items.get(from).model;
        m = findModelById(m.id);

        // Find the parent
        let toModel = null;
        let parentId = 0;
        if (from < to) { // move down
            toModel = items.get(to).model;
            parentId = toModel.id;
        } else { // move up
            let to2 = to1 - 1;
            for (let i = 0; i < items.count; i++) {
                if (idx[i] == to2) {
                    toModel = items.get(i).model;
                    parentId = toModel.id;
                    break;
                }
            }
        }

        while(parentId > 0) {
            let parentModel = findModelById(parentId);
            if (parentModel.expanded) {
                break;
            }
            if (parentModel.parentId == 0) {
                parentId = 0;
                break;
            }
            parentId = parentModel.parentId;
        }
        if (parentId != m.id) {
            m.parentId = parentId;
        }

        // Reorder items in folder
        let childs = findChilds(parentId, m.id);
        let toI = 0;
        if (toModel != null) {
            for (let i = 0; i < childs.length; i++) {
                let child = childs[i];
                if (toModel.id == child.id) {
                    toI = i+1;
                    break;
                }
            }
        }
        childs.splice(toI, 0, m);

        for (let i = 0; i < childs.length; i++) {
            let child = childs[i];
            child.idx = i;
            model.save(child);
        }

        visibleItems.move(from1, to1, 1);
    }

    function findModelById(id) {
        for (let i = 0; i < model.rowCount(); i++) {
            let m = model.get(i);
            if (m.id == id) {
                return m;
            }
        }
        return null;
    }

    function findModelIndexById(id) {
        for (let i = 0; i < model.rowCount(); i++) {
            let m = model.get(i);
            if (m.id == id) {
                return i;
            }
        }
        return null;
    }


    function modelByIndex(i) {
        return model.get(idx[i]);
    }

    function setProperty(i, name, value) {
        model.setProperty(idx[i], name, value);
    }


    function findChilds(parentId, skipId = null) {
        let rows = [];
        for (let i = 0; i < model.rowCount(); i++) {
            let m = model.get(i);
            if (m.id == skipId) {
                continue;
            }
            if (m.parentId == parentId) {
                rows[rows.length] = m;
            }
        }
        return rows;
    }

    items.onChanged: update()

    groups: DelegateModelGroup {
        id: visibleItems
        name: "visible"
        includeByDefault: false
    }

    filterOnGroup: "visible"
} 
