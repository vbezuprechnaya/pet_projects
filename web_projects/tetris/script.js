let username = "DefaultName";
function setCookie(field, name)
{
    const d = new Date();
    d.setTime(d.getTime() + 1000*60*20);
    let expires = "expires=" + d.toUTCString();
    document.cookie += field + name + ";" + expires + ";path=/";
}
function getCookie(cname)
{
    let name = cname + "=";
    let decodedCookie = decodeURIComponent(document.cookie);
    let ca = decodedCookie.split(';');
    for(let i = 0; i <ca.length; i++) {
        let c = ca[i];
        while (c.charAt(0) === ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) === 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

function checkCookie()
{
    let user = getCookie("username");
    if (user !== "")
    {
        username = user;
        clearNameBlock();
    } else {
        document.querySelector('input').addEventListener('keydown', updateName);
    }
}

class TetrisLogic{
    constructor(imgX, imgY, template) {
        this.imgX = imgX;
        this.imgY = imgY;
        this.template = template;
        this.x = squareCountX / 2;
        this.y = 0;
    }

    checkBottom(){
        for(let i = 0; i < this.template.length; i++){
            for (let j = 0; j < this.template.length; j++){
                if (this.template[i][j] === 0)
                    continue;
                let realX = i + this.TruncPos().x;
                let realY = j + this.TruncPos().y;
                if (realY + 1 >= squareCountY) {
                    return false;
                }
                if (gameMap[realY + 1][realX].imgX !== -1) { //?
                    return false;
                }
            }
        }
        return true;
    }

    TruncPos() {
        return {
            x: Math.trunc(this.x),
            y: Math.trunc(this.y)
        };
    }

    checkLeft(){
        for (let i = 0; i < this.template.length; i++) {
            for (let j = 0; j < this.template.length; j++) {
                if (this.template[i][j] === 0)
                    continue;
                let realX = i + this.TruncPos().x;
                let realY = j + this.TruncPos().y;
                if (realX - 1 < 0) {
                    return false;
                }

                if (gameMap[realY][realX - 1].imgX !== -1)
                    return false;
            }
        }
        return true;
    }

    checkRight(){
        for (let i = 0; i < this.template.length; i++) {
            for (let j = 0; j < this.template.length; j++) {
                if (this.template[i][j] === 0)
                    continue;
                let realX = i + this.TruncPos().x;
                let realY = j + this.TruncPos().y;
                if (realX + 1 >= squareCountX) {//можно съесть текстуру при желании
                    return false;
                }

                if (gameMap[realY][realX+1].imgX !== -1)
                    return false;
            }
        }
        return true;
    }

    moveRight() {
        if (this.checkRight())
            this.x += 1;
    }
    moveLeft() {
        if (this.checkLeft())
            this.x -= 1;
    }
    moveDown(){
        if  (this.checkBottom()) {
            this.y += 1;
        }
    }

    changeRotation(){
        let tempTemplate = [];
        for (let i = 0; i < this.template.length; i++)
            tempTemplate[i] = this.template[i].slice();
        let n = this.template.length;
        for (let layer = 0; layer < n / 2; layer++) {
            let first = layer;
            let last = n - 1 - layer;
            for (let i = first; i < last; i++) {
                let offset = i - first;
                let top = this.template[first][i];
                this.template[first][i] = this.template[i][last]; // top = right
                this.template[i][last] = this.template[last][last - offset]; //right = bottom
                this.template[last][last - offset] =
                    this.template[last - offset][first];
                this.template[last - offset][first] = top; // left = top
            }
        }

        for (let i = 0; i < this.template.length; i++) {
            for (let j = 0; j < this.template.length; j++) {
                if (this.template[i][j] === 0) continue;
                let realX = i + this.TruncPos().x;
                let realY = j + this.TruncPos().y;
                if (
                    realX < 0 ||
                    realX >= squareCountX ||
                    realY < 0 ||
                    realY >= squareCountY
                ) {
                    this.template = tempTemplate;
                    return false;
                }
            }
        }
    }
}

const NameBox = document.getElementById('NameBox');
const image = document.getElementById('TetrImage');
const cover = document.getElementById('cover');
const canvas = document.getElementById('canvas');
const NextBlockScr = document.getElementById('NextBlockScr');
const score = document.getElementById('score');
const level = document.getElementById('lvl');
const pname = document.getElementById('playername');
const ctx = canvas.getContext("2d");
const nctx = NextBlockScr.getContext("2d");

const imageSquareSize = 84;
const size = 40;
const framePerSecond = 24;
let gameSpeed;
const squareCountX = canvas.width / size;
const squareCountY = canvas.height / size;
let gamePause = false;
let cflag;

document.getElementById('startBtn').onclick = startGame;
document.getElementById('pauseBtn').onclick = pauseGame;

const tetrominos = [
    new TetrisLogic(0, 84, [
        [0, 1, 0],
        [0, 1, 0],
        [1, 1, 0],
    ]),
    new TetrisLogic(0, 333, [
        [0, 0, 0],
        [1, 1, 1],
        [0, 1, 0],
    ]),
    new TetrisLogic(0, 501, [
        [0, 1, 0],
        [0, 1, 0],
        [0, 1, 1],
    ]),
    new TetrisLogic(0, 168, [
        [0, 0, 0],
        [0, 1, 1],
        [1, 1, 0],
    ]),
    new TetrisLogic(0, 249, [
        [0, 0, 1, 0],
        [0, 0, 1, 0],
        [0, 0, 1, 0],
        [0, 0, 1, 0],
    ]),
    new TetrisLogic(0, 0, [
        [1, 1],
        [1, 1],
    ]),

    new TetrisLogic(0, 417 , [
        [0, 0, 0],
        [1, 1, 0],
        [0, 1, 1],
    ]),
];

let gameMap;
let gameOver;
let currentShape;
let nextShape;
let currentScore;
let initialTwoDArr;


let gameLoop = () => {
    setInterval(update, 1000 / gameSpeed);
    setInterval(draw, 1000 / framePerSecond);
    // checkCookie();
    // console.log(username);
};

function deleteCompleteRows() {
    let flag = false;
    for (let i = 0; i < gameMap.length; i++) {
        let t = gameMap[i];
        console.log(currentScore);
        let isComplete = true;
        for (let j = 0; j < t.length; j++) {
            if (t[j].imgX === -1){
                isComplete = false;
                // console.log("uncompleted row");
            }
        }
        if (isComplete) {
            flag = true;
            // console.log("completed row");
            // currentScore += 100;
            for (let k = i; k > 0; k--) {
                gameMap[k] = gameMap[k - 1];
            }
            let temp = [];
            for (let j = 0; j < squareCountX; j++) {
                temp.push({ imgX: -1, imgY: -1 });
            }
            gameMap[0] = temp;
        }
    }
    // console.log(currentScore);
    return (flag);
}

let update = () => {
    if (gameOver){
        let prevScore = localStorage.getItem(getNameFromLocalStorage() + "score");
        let prevName = localStorage.getItem(getNameFromLocalStorage() + "player");
        if (prevScore !== undefined){
            if (currentScore > prevScore){
                if (prevName === pname) {
                    localStorage.setItem(getNameFromLocalStorage() + "score", currentScore)
                }
                else{
                    localStorage.setItem(getNameFromLocalStorage()+ "player", username)
                    localStorage.setItem(getNameFromLocalStorage() + "score", currentScore)
                }
            }
        }
        else{
            localStorage.setItem(getNameFromLocalStorage()+"score", currentScore)
            localStorage.setItem(getNameFromLocalStorage()+"player", username)
        }
        createTable();
        return;
    }
    if (!gamePause){
        if (currentShape.checkBottom()) {
            currentShape.y += 1;
        }
        else {
            for (let k = 0; k < currentShape.template.length; k++) {
                for (let l = 0; l < currentShape.template.length; l++) {
                    if (currentShape.template[k][l] === 0)
                        continue;
                    gameMap[currentShape.TruncPos().y + l][
                    currentShape.TruncPos().x + k
                        ] = { imgX: currentShape.imgX, imgY: currentShape.imgY };
                }
            }
            if(deleteCompleteRows()){
                deleteCompleteRows();
                currentScore+=100;
            }
            currentShape = nextShape;
            nextShape = getRandomShape();
            if (!currentShape.checkBottom()) {
                gameOver = true;
            }
            currentScore += 10;
        }

    }
    score.innerHTML = "SCORE: " + currentScore;
    level.innerHTML = "LEVEL: " + lvlup();
    let curlvl = level.innerHTML;
    if ((curlvl % 2 ===0)&&(currentScore>100))
        gameSpeed+=4;
};
let drawCurrentTetris = () => {
    for (let i = 0; i < currentShape.template.length; i++) {
        for (let j = 0; j < currentShape.template.length; j++) {
            if (currentShape.template[i][j] === 0)
                continue;
            ctx.drawImage(
                image,
                currentShape.imgX,
                currentShape.imgY,
                imageSquareSize,
                imageSquareSize,
                Math.trunc(currentShape.x) * size + size * i,
                Math.trunc(currentShape.y) * size + size * j,
                size,
                size
            );
        }
    }
};

let drawSquares = () => {
    for (let i = 0; i < gameMap.length; i++) {
        let t = gameMap[i];
        for (let j = 0; j < t.length; j++) {
            if (t[j].imgX === -1) continue;
            ctx.drawImage(
                image,
                t[j].imgX,
                t[j].imgY,
                imageSquareSize,
                imageSquareSize,
                j * size,
                i * size,
                size,
                size
            );
        }
    }
};

let drawNextShape = () => {
    nctx.fillStyle = "#B1CCD3";
    nctx.fillRect(0, 0, NextBlockScr.width, NextBlockScr.height);
    for (let i = 0; i < nextShape.template.length; i++) {
        for (let j = 0; j < nextShape.template.length; j++) {
            if (nextShape.template[i][j] === 0)
                continue;
            nctx.drawImage(
                image,
                nextShape.imgX,
                nextShape.imgY,
                imageSquareSize,
                imageSquareSize,
                size * i,
                size * j + size,
                size,
                size
            );
        }
    }
};

let drawGameOver = () => {
    ctx.fillStyle = 'black';
    ctx.globalAlpha = 0.75;
    ctx.fillRect(0, canvas.height / 2 - 30, canvas.width, 90);
    ctx.globalAlpha = 1;
    ctx.fillStyle = 'white';
    ctx.font = '36px JetBrains Mono';
    ctx.textAlign = 'center';
    ctx.textBaseLine = 'middle';
    ctx.fillText('GAME OVER!', canvas.width /2, canvas.height / 2 + 25);
    };

let draw = () => {
    if (!gamePause) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        drawSquares();
        drawCurrentTetris();
        drawNextShape();
    }
    if (gameOver) {
        drawGameOver();
    }
};

let getRandomShape = () => {
    return Object.create(tetrominos[Math.floor(Math.random() * tetrominos.length)]);
};


let resetVars = () => {
    initialTwoDArr = [];
    for (let i = 0; i < squareCountY; i++) {
        let temp = [];
        for (let j = 0; j < squareCountX; j++) {
            temp.push({ imageX: -1, imageY: -1 });
        }
        initialTwoDArr.push(temp);
    }
    // currentScore = 0;
    gameOver = false;
    currentShape = getRandomShape();
    nextShape = getRandomShape();
    gameMap = initialTwoDArr;
    };

function lvlup (){
    let l= Math.trunc(currentScore/1000);
    return(l);
}

document.querySelectorAll("button").forEach( function(item) {
    item.addEventListener('focus', function() {
        this.blur();
    })
});

function updateName(e)
{
    console.log(e.key);
    const englishAlphabet = /[A-Za-z]/;
    if ((e.key === 'Enter')&&(e.target.value!==""))
    {
        username = e.target.value;
        setCookie('username=', username);
        pname.innerHTML = username;
        console.log(pname);
        localStorage.setItem(pname + "player", username);
        clearNameBlock();
    }
    else if (!englishAlphabet.test(e.key))
    {
        e.preventDefault();
    }

}
function clearNameBlock()
{
    pname.innerHTML = "Player: "+ username;
    NameBox.style.opacity = "0%";
    cover.style.opacity = "0%";
    NameBox.addEventListener('transitionend', function () {
        document.getElementById('PopUpBlock').style.display = "none";
    });
}
function eventL(event) {
    if (event.key === "ArrowLeft") currentShape.moveLeft();
    else if (event.key === "ArrowUp") currentShape.changeRotation();
    else if (event.key === "ArrowRight") currentShape.moveRight();
    else if (event.key === "ArrowDown") currentShape.moveDown();
}
window.addEventListener("keydown", event=>(eventL(event)));

function startGame()
{
    cflag = true;
    currentScore = -110;
    gameSpeed = 3;
    console.log("let's go!");
    gameLoop();
    resetVars();
}
function pauseGame(){
    gamePause =! gamePause
    if (gamePause)
    {
        ctx.fillStyle = 'black';
        ctx.globalAlpha = 0.75;
        ctx.fillRect(0, canvas.height / 2 - 30, canvas.width, 90);
        ctx.globalAlpha = 1;
        ctx.fillStyle = 'white';
        ctx.font = '36px JetBrains Mono';
        ctx.textAlign = 'center';
        ctx.textBaseLine = 'middle';
        ctx.fillText('PAUSE', canvas.width /2, canvas.height / 2 + 25);
    }
    else if (!gamePause)
    {
        gameLoop();
    }
}


function getNameFromLocalStorage() {
    return (pname)
}

checkCookie();

function createTable(){
    if(cflag) {
        cflag = false;
        console.log("create Table")
        document.removeEventListener('keydown', eventL)
        let lastScore = localStorage.getItem(getNameFromLocalStorage() + "score")
        if (lastScore < currentScore.value) {
            localStorage.setItem(getNameFromLocalStorage() + "player", pname)
            localStorage.setItem(getNameFromLocalStorage() + "score", currentScore.value)
            console.log(getNameFromLocalStorage());
        }
        let table = document.getElementById("TopTable")
        let keys = Object.keys(localStorage);
        let allPlayer = []
        let allScore = []
        let newArray = []
        for (let key of keys) {
            if (key.includes("player")) {
                console.log(key)
                allPlayer.push(localStorage.getItem(key))
            }
            if (key.includes("score")) {
                console.log(key)
                allScore.push(localStorage.getItem(key))
            }
        }
        for (let i = 0; i < allScore.length; i++) {
            let obj = {
                name: allPlayer[i],
                curscore: allScore[i]
            }
            newArray.push(obj)
        }
        newArray.sort(function(a, b){
            return b.curscore-a.curscore
        })
        console.log(allScore.length)
        for (let i = 0; i < newArray.length; i++) {
            console.log("in table")
            console.log(allPlayer[i], allScore[i])
            let newRow = document.createElement("tr")
            newRow.innerHTML = '<td>' + newArray[i].name + '</td><td>' + newArray[i].curscore + '</td>'
            table.appendChild(newRow)
        }
    }
}
