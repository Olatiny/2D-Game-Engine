// let gameObject;
let id;

let snake = [];
let directions = [];

class Vector2 {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }
}

let speed = 25;
let inputs = [];
let lastInput = 'none';
let dead = false;
let score = 0;

function move(idx, x, y) {
    snake[idx].x += x;
    snake[idx].y += y;

    // snake[0].velocity_x = .0000001 * x;
    // snake[0].velocity_y = .0000001 * y;
}

function start(guid) {
    snake.push(gethandle(guid));
    // print("guid: " + guid);
    // print(typeof guid);
    id = guid;
}

let date = Date.now();

function checkInterval(milliseconds) {
    let currentDate = Date.now();

    if (currentDate - date < milliseconds) {
        return false;
    } else {
        date = Date.now();
        return true;
    }
}

function update(dt) {
    if (!checkInterval(150) || dead) return;

    for (let i = snake.length - 1; i > 0; i--) {
        directions[i] = directions[i - 1];
    }

    // print(inputs);

    directions[0] = lastInput;

    // directions[0] = inputs.length ? inputs[0] : lastInput;
    // if (inputs.length) inputs.splice(0, 1);
    // lastInput = directions[0];

    for (let i = 0; i < snake.length; i++) {
        switch (directions[i]) {
            case 'left':
                move(i, -speed, 0);
                // rotate(snake[i].guid, 180);
                break;
            case 'right':
                move(i, speed, 0);
                // rotate(snake[i].guid, 0);
                break;
            case 'up':
                move(i, 0, -speed);
                // rotate(snake[i].guid, -90);
                break;
            case 'down':
                move(i, 0, speed);
                // rotate(snake[i].guid, 90);
                break;
            default:
                // print('default case in update switch (player)');
                break;
        }
    }
}

function onEvent(ev, dt) {
    let event = JSON.parse(ev);

    // print(ev);
    // print(event.data.keys.A);

    if (event.type == 0 && (event.withObjID != '1004') && !dead) {
        let finalScore = {
            "score" : score
        };

        // print(finalScore.score);
        
        raise(1, id, event.withObjID, JSON.stringify(finalScore));
        dead = true;
        // print("you died. esc to quit, then restart to play again.");
    } else if (event.type == 0 && event.withObjID == '1004' && !dead) {
        score++;
        let currScore = {
            "score" : score
        }

        raise(5, id, "1006", JSON.stringify(currScore));
    }

    if (event.type == 3 && event.raiseObjID == id) {
        let eventKeyData = event.data.keys;

        // print(directions[0]);

        // if (checkInterval(2)) {
            if ((eventKeyData.A || eventKeyData.Left) && directions[0] != 'right') {
                // if (!inputs.includes('left')) inputs.push('left');
                // print('adding left');
                lastInput = 'left';
            } else if ((eventKeyData.W || eventKeyData.Up) && directions[0] != 'down') {
                // if (!inputs.includes('up')) inputs.push('up');
                lastInput = 'up';
            } else if ((eventKeyData.S || eventKeyData.Down) && directions[0] != 'up') {
                // if (!inputs.includes('down')) inputs.push('down');
                lastInput = 'down';
            } else if ((eventKeyData.D || eventKeyData.Right) && directions[0] != 'left') {
                // if (!inputs.includes('right')) inputs.push('right');
                lastInput = 'right';
            }
        // }
    }

    if (event.type == 2 && event.withObjID == id && event.raiseObjID == '1004') {
        // print("hi");
        // print("new id: " + Number(snake[snake.length - 1].guid) + 1);
        // print(snake[snake.length - 1].x, snake[snake.length - 1].y);

        switch (directions[directions.length - 1]) {
            case 'left':
                // move(-speed, 0);
                snake.push(gameobjectfactory(Number(snake[snake.length - 1].guid) + 1, snake[snake.length - 1].x + 25, snake[snake.length - 1].y));
                directions.push('left');
                break;
            case 'right':
                // move(speed);
                snake.push(gameobjectfactory(Number(snake[snake.length - 1].guid) + 1, snake[snake.length - 1].x - 25, snake[snake.length - 1].y));
                directions.push('right');
                break;
            case 'up':
                // move(0, -speed);
                snake.push(gameobjectfactory(Number(snake[snake.length - 1].guid) + 1, snake[snake.length - 1].x, snake[snake.length - 1].y + 25));
                directions.push('up');
                break;
            case 'down':
                // move(0, speed);
                snake.push(gameobjectfactory(Number(snake[snake.length - 1].guid) + 1, snake[snake.length - 1].x, snake[snake.length - 1].y  - 25));
                directions.push('down');
                break;
            default:
                // print('default case in event switch (player)');
                break;
        }

        // print(snake[snake.length - 2].x + ", " + snake[snake.length - 2].y);
        // print(snake[snake.length - 1].x + ", " + snake[snake.length - 1].y);
    }
}