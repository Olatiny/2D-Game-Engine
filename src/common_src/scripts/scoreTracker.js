let gameObject;
let id;
let playerId;

function start(guid, playId) {
    id = guid;
    gameObject = gethandle(id);
    playerId = playId;
}

function update(dt) {
    // do nothing
}

function onEvent(ev, dt) {
    let event = JSON.parse(ev);

    if (event.type == 5) {
        // print("got here");

        let data = {
            "string": `score: ${event.data.score}`
        }

        raise(4, playerId, id, JSON.stringify(data));

        // print(data.string);
    }
}