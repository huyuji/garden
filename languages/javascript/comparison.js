// empty object does not equal true
if ({} == true) {
  console.log("empty object equals true");
} else {
  console.log("empty object does not equal true");
}

// empty object does not equal false
if ({} == false) {
  console.log("empty object equals false");
} else {
  console.log("empty object does not equal false");
}

// empty object evalutes to true
if ({}) {
  console.log("empty object evalutes to true");
} else {
  console.log("empty object evalutes to false");
}

// empty array does not equal true
if ([] == true) {
  console.log("empty array equals true");
} else {
  console.log("empty array does not equal true");
}

// empty array equals false
if ([] == false) {
  console.log("empty array equals false");
} else {
  console.log("empty array does not equal false");
}

// empty array evalutes to true
if ([]) {
  console.log("empty array evalutes to true");
} else {
  console.log("empty array evalutes to false");
}
