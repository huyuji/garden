let last;

function test(y, timeout) {
    last = y.i;
    setTimeout(() => { console.log(y.i); }, timeout);
}

test({i: 1}, 2000);
test({i: 2}, 1000);

console.log('last is ' + last);
