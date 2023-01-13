let gameObject;
let id;

let playerId;

let dead = false;

function start(guid, playId) {
    id = guid;
    gameObject = gethandle(guid);
    playerId = playId;
}

function update(dt) {
    // do nothing
    // print("running");
}

function setRandomPos() {
    let x_rand = Math.random() * 14;
    let y_rand = Math.random() * 14;

    gameObject.x = Math.ceil((x_rand * 25)/50)*50;
    gameObject.y = Math.ceil((y_rand * 25)/50)*50;
}

function onEvent(ev, dt) {
    let event = JSON.parse(ev);
    
    if (event.type == 1) {
        dead = true;
    }

    if (event.type == 0 && !dead && (event.raiseObjID < 1000)) {
        // print("collided");
        setRandomPos();

        raise(2, id, playerId);
    }
}