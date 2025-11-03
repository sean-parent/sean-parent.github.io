// Returns the successor of `x`.
// Precondition: `x < INT_MAX`

fn f(x: i32) -> i32 {
    x + 1
}

fn main() {
    println!("{}", f(42));
}
