
export const createNewboard = () =>{
    let newBoard = []
    for (let column = 0; column < 32; column++) {
        var row_array = [];
        for (let row = 0; row < 16; row++) {
            row_array.push({
                'x': column,
                'y': row,
                'active': false,
                'color': `rgb(${255}, ${0}, ${0})`
            })
        }
        newBoard.push(row_array);
    }
    console.log(newBoard);
    return newBoard;
}


export const setBoardWithSpectrum = (board, spectrumValues , color) => {
    const maxBarHeight = 16;
    for (let col = 0; col < spectrumValues.length && col < 32; col++) {
        let logValue = Math.log10(spectrumValues[col]) > 1 ? Math.log10(spectrumValues[col]) : 1;
        const barHeight = Math.floor(spectrumValues[col] / 10 * (maxBarHeight / logValue));
        let currentIndex = 0;
        for (let row = maxBarHeight - 1; row >= 0; row--) {
            if (currentIndex < barHeight) {
                board[col][row].active = true;
                board[col][row].color = screenColor[color];
            } else {
                board[col][row].active = false;
            }
            currentIndex++;
        }
    }
    return board;
}
const screenColor = {
    0: 'black',
    1: 'red',
    2: 'green',
    3: 'yellow',
    4: 'blue',
    5: '#FF00FF',
    6: 'teal',
    7: 'white'
}
export const setBoardWithScreen = (board, screen) =>{
    console.log(screen)
    for (let col = 0; col < 32; col++) {
        for (let row = 0; row < 16; row++) {
            if(screen[col][row] != 0){
                board[col][row].active = true;
                board[col][row].color = screenColor[screen[col][row]];
            }
            else {
                board[col][row].active = false;
            }
        }
    }
    return board;
}

export const setBoardPosActive = (board, row, col, active) => {
    board[row][col].active = active
    return board;
}


export const setBoardPosColor = (board, row, col, color) => {
    board[row][col].color = color
    return board;
}


export default {createNewboard, setBoardWithSpectrum , setBoardPosActive, setBoardPosColor};

