.pragma library

function init(con, doc) {
    console.log("INIT");
    log.debug("DDDDD");
    log.error("EEEEE");
    null.f()

}

function editGroup(qi, addPopup, itemIdx, main)
{
    log.debug(itemIdx)
    let item = qi.bookmarkModel().get(itemIdx);
    log.debug(item.name);

    for (const i in main.children)
            log.debug(i)

    // nameFld = Qt.application.findChild(addPopup, "itemName");
    // log.debug(nameFld);
    // addPopup.open();
}
