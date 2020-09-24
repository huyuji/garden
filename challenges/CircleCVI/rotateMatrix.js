function rotateMatrix(m) {
    const dimension = m.length;
    const iLimit = Math.floor((m.length + 1) / 2);
    const jLimit = Math.floor(m.length / 2);
    for (let i = 0; i < iLimit; i++){
        for (let j = 0; j < jLimit; j++) {
            const temp = m[j][dimension - 1 -i];
            m[j][dimension - 1 -i] = m[dimension - 1 - i][dimension - 1 -j];
            m[dimension - 1 - i][dimension - 1 -j] = m[dimension - 1 - j][i];
            m[dimension - 1 - j][i] = m[i][j];
            m[i][j] = temp;
        }
    }
}

function printMatrix(m) {
    m.forEach(row => {
        let s = ''
        row.forEach(value => s += value + ',');
        console.log(s);
    });
    console.log('');
}

[
    [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ],
    [
        [1, 2, 3, 4],
        [5, 6, 7, 8],
        [9, 10, 11, 12],
        [13, 14, 15, 16]
    ],
    [
        [1, 2, 3, 4, 5],
        [6, 7, 8, 9, 10],
        [11, 12, 13, 14, 15],
        [16, 17, 18, 19, 20],
        [21, 22, 23, 24, 25]
    ]
].forEach(m => {
    printMatrix(m);
    rotateMatrix(m);
    printMatrix(m);
});
