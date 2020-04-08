import {from, defer, fromEvent, Observable} from "rxjs";
import {map, takeWhile, scan, delay, filter, flatMap, retry, retryWhen} from "rxjs/operators";

//let numbers = [1,5,10];
//let source = from(numbers);
// let source = Observable.create(observer => {
//     let index = 0;

//     let produceValue = () => {
//         observer.next(numbers[index++]);

//         if(index < numbers.length) {
//             setTimeout(produceValue, 2000);
//         } else {
//             observer.complete();
//         }
//     }

//     produceValue();
// }).pipe(map((n: number) => n * 2), filter(n => n > 4));

// class MyObserver {
//     next(value: any) {
//         console.log(`value: ${value}`);
//     }

//     error(e: any) {
//         console.log(`error: ${e}`);
//     }

//     complete() {
//         console.log('complete');
//     }
// }

// //let circle = document.getElementById('circle');
// let source = fromEvent(document, "mousemove")
//     .pipe(map((e: MouseEvent) => {
//         return {
//             x: e.clientX,
//             y: e.clientY
//         }
//     }));

// function onNext(value) {
//     circle.style.left = value.x;
//     circle.style.top = value.y;

// }
// source.subscribe(
//     onNext, //value => console.log(value),
//     e => console.log(`e: ${e}`),
//     () => console.log('complete')
// );

function load(url: string) {
    return Observable.create(observer => {
        let xhr = new XMLHttpRequest();

        xhr.addEventListener('load', () => {
            if (xhr.status === 200) {
                let data = JSON.parse(xhr.responseText);
                observer.next(data);
                observer.complete();    
            } else {
                observer.error(xhr.statusText);
            }
        });
    
        xhr.open("GET", url);
        xhr.send();    
    }).pipe(retryWhen(retryStrategy({attemtps: 3, interval: 1500})));
}

function loadWithFetch(url: string) {
    return defer(() => {
        return from(fetch(url).then(r => {
            if (r.status === 200) {
                return r.json()
            } else {
                return Promise.reject(r);
            }
        }));
    }).pipe(retryWhen(retryStrategy({attemtps: 3, interval: 1500})));;
}

function retryStrategy({attemtps = 4, interval = 1000}) {
    return (errors) => {
        return errors.pipe(
            scan((acc, value: string) => {
                acc += 1;
                if (acc < attemtps) {
                    return acc;
                } else {
                    throw new Error(value);
                }
            }, 0),
            takeWhile(acc => acc < attemtps),
            delay(interval)
        );
    }
}

function renderMovies(movies) {
    movies.forEach(m => {
        let div = document.createElement('div');
        div.innerText = m.title;
        output.appendChild(div);
    })
}


let output = document.getElementById('output');
let button = document.getElementById('button');

let click = fromEvent(button, 'click');

// click.subscribe(
//     e => load("movies.json"),
//     e => console.log(`error: ${e}`),
//     () => console.log('complete')
// );

//click.pipe(flatMap(e => load('movies.json'))).subscribe(
click.pipe(flatMap(e => loadWithFetch('moviess.json'))).subscribe(
    renderMovies,
    e => console.log(`error: ${e}`),
    () => console.log('complete')    
);
