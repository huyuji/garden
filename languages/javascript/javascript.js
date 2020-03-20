if(NaN !== NaN)
{
    console.log("NaN !== NaN");
}

if(0 == -0)
{
    console.log("0 == -0");
}

if(Infinity !== -Infinity)
{
    console.log("Infinity !== -Infinity");
}

if((.3 - .2) !== (.2 - .1))
{
    console.log("(.3 - .2) !== (.2 - .1)");
}

if(!(undefined || null || 0 || -0 || NaN || ""))
{
    console.log('undefined, null, 0, -0, NaN and "" evaluates to false');
}

if(undefined == null)
{
    console.log("undefined == null");
}

// convertibility of one value to another does not imply equality of those two values.
if(undefined !== false)
{
    console.log("undefined !== false");
}

// backslash is ignored if it doesn't form any escape sequence with subsequent characters.
console.log("\hello");

// The temporary objects created when you access a property of a string, number, or boolean are known as wrapper objects.
// string, number, and boolean values differ from objects in that their properties are read-only and that you can’t define new properties on them.
var string = "test";
string.len = 4;
if(string.len === undefined)
{
    console.log("string.len is undefined");
}

// The == equality operator treats a value and its wrapper object as equal, but you can distinguish them with the === strict equality operator.
if("test" == String("test"))
{
    console.log("equal");
}
    
// Object-to-boolean conversions are trivial: all objects (including arrays and functions) convert to true. This is so even for wrapper objects: new Boolean(false)
if(Boolean("false"))
{
    console.log('Boolean("false") evaluates to true');
}

// It is legal and harmless to declare a variable more than once with the var statement. If the repeated declaration has an initializer, it acts as if it were simply an assignment statement.
var variable = 10;
var variable = 11;
console.log(variable);

// JavaScript uses function scope: variables are visible within the function in which they are defined and within any functions that are nested within that function.
// Within its scope, a variable is even visible before it is declared. 
var scope = "global";
function f() {
    console.log(scope); // Prints "undefined", not "global"
    var scope = "local"; // Variable initialized here, but defined everywhere inside the function
    console.log(scope); // Prints "local"
}
f();

// Undefined elements can be included in an array literal.
var sparseArray = [1,,,,5];
console.log(sparseArray);

// A single trailing comma is allowed after the last expression in an array initializer and does not create an undefined element.
var sparseArray = [1,,,,5,];
console.log(sparseArray);

// If a function returns no value, then the value of the invocation expression is undefined.
var noreturn = function() {}
console.log(noreturn());

// The sign of the result of modulo is the same as the sign of the first operand. 
console.log(-5%2); // -1
console.log(-5%-2); // -1

// Modulo also works for floating-point values. 
console.log(6.1%1.2);

// when the + operator is used with strings and numbers, it may not be associative. That is, the result may depend on the order in which operations are performed. 
console.log(1 + 2 + " blind mice"); // => "3 blind mice"
console.log(1 + (2 + " blind mice")); // => "12 blind mice"

// the unary operator + converts its operand to number or NaN. 
// if its operand is already a number, it does nothing.
console.log(+-3); // -3

// the unary operator - also converts its operand to number of NaN.
// however, it negates the result before return.
// --3 doesn't compile.
console.log(-(-3)); // 3

// Surprisingly, NaN, Infinity, and -Infinity all convert to 0 when used as operands of these bitwise operators. 
console.log(NaN >> 0);