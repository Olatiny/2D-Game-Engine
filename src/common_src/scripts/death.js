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

    if (event.type == 1) {
        // print("got here");

        let data = {
            "string": `     You have died.
                       \n     FINAL SCORE: ${event.data.score}
                       \nPress 'esc' to quit and 
                       \n restart to play again.`
        }

        raise(4, playerId, id, JSON.stringify(data));

        // print(data.string);
    }
}