use anyhow::{Ok, Result};

// Returns the successor of `x`.
// Precondition: `x < INT_MAX`

/* fn f(x: i32) -> i32 {
    x + 1
} */
#[derive(Debug)]
struct Point {
    x: i32,
    y: i32,
}

// Return references to two different fields
fn split_coords(p: &mut Point) -> (&mut i32, &mut i32) {
    (&mut p.x, &mut p.y)
}

// Can also return just one field
fn get_x(p: &mut Point) -> &mut i32 {
    &mut p.x
}

// An iterator that holds a mutable slice and returns non-overlapping mutable references
// This is SAFE because we progressively consume the slice using split_first_mut()
struct OddMutIterator<'a> {
    // We hold a mutable slice (not Vec) that we progressively consume
    slice: &'a mut [i32],
    at_even_index: bool,
}

impl<'a> OddMutIterator<'a> {
    fn new(slice: &'a mut [i32]) -> Self {
        OddMutIterator {
            slice,
            at_even_index: true, // Start at index 0 (even)
        }
    }
}

// Implement Iterator - this yields mutable references to odd-indexed elements
impl<'a> Iterator for OddMutIterator<'a> {
    type Item = &'a mut i32;

    fn next(&mut self) -> Option<Self::Item> {
        loop {
            // split_first_mut() is SAFE - it splits the slice into non-overlapping parts:
            // - first: &mut T (the first element)
            // - rest: &mut [T] (the remaining elements)
            // The borrow checker knows these don't overlap!

            // We need to use mem::replace to swap out the slice while holding a borrow
            let slice = std::mem::replace(&mut self.slice, &mut []);
            let (first, rest) = slice.split_first_mut()?;
            self.slice = rest; // Consume the first element

            if self.at_even_index {
                // We're at an even index, skip this element
                self.at_even_index = false;
            } else {
                // We're at an odd index, return this element
                self.at_even_index = true;
                return Some(first);
            }
        }
    }
}

fn demonstrate_mut_iterator() {
    println!("=== Safe iterator using slice splitting ===");
    let mut numbers = vec![0, 1, 2, 3, 4, 5, 6, 7];

    // Create an iterator that visits odd indices (1, 3, 5, 7)
    // We pass a mutable slice, not a Vec
    let iter = OddMutIterator::new(&mut numbers[..]);

    // Modify each odd-indexed element by multiplying by 10
    for elem in iter {
        *elem *= 10;
    }

    // numbers is now [0, 10, 2, 30, 4, 50, 6, 70]
    println!("After modifying odd indices: {:?}", numbers);
    assert_eq!(numbers, vec![0, 10, 2, 30, 4, 50, 6, 70]);
}

// ========== Bonus: Alternative using split_at_mut for chunks ==========

// An iterator that returns mutable references to pairs of elements
struct PairsMutIterator<'a> {
    slice: &'a mut [i32],
}

impl<'a> PairsMutIterator<'a> {
    fn new(slice: &'a mut [i32]) -> Self {
        PairsMutIterator { slice }
    }
}

impl<'a> Iterator for PairsMutIterator<'a> {
    type Item = (&'a mut i32, &'a mut i32);

    fn next(&mut self) -> Option<Self::Item> {
        if self.slice.len() < 2 {
            return None;
        }

        // split_at_mut is also SAFE - splits at an index into two non-overlapping slices
        let slice = std::mem::replace(&mut self.slice, &mut []);
        let (pair, rest) = slice.split_at_mut(2);
        self.slice = rest;

        // split_at_mut again to get two non-overlapping mutable references
        let (first, second) = pair.split_at_mut(1);
        Some((&mut first[0], &mut second[0]))
    }
}

fn demonstrate_pairs_iterator() {
    println!("\n=== Bonus: Iterator returning pairs ===");
    let mut numbers = vec![0, 1, 2, 3, 4, 5];

    let iter = PairsMutIterator::new(&mut numbers[..]);

    // Swap each pair
    for (a, b) in iter {
        std::mem::swap(a, b);
    }

    println!("After swapping pairs: {:?}", numbers);
    assert_eq!(numbers, vec![1, 0, 3, 2, 5, 4]);
}

// fn f(a: &mut Box<i32>) -> () {}

struct EveryNth<'a, T> {
    slice: &'a [T],
    n: usize,
    i: usize,
}

impl<'a, T> Iterator for EveryNth<'a, T> {
    type Item = &'a T;
    fn next(&mut self) -> Option<Self::Item> {
        (self.i < self.slice.len()).then(|| {
            let item = &self.slice[self.i];
            self.i += self.n;
            item
        })
    }
}

type Type = i32;

fn f() -> Result<Type> {
    Ok(1)
}
fn g() -> Result<Type> {
    Ok(2)
}
fn h(a: &mut Type) -> Result<()> {
    Ok(())
}

fn a(a: &mut Type) -> Result<()> {
    let mut x = f()?.g()?;
    h(&mut x)?;
    h(a)?;
    Ok(())
}

fn op(s: String) -> String {
    s + "!"
}

struct S {
    data: String,
}

fn main() {
    let mut s = String::from("Hello, world!");
    s = op(s);

    let mut s2 = S{data: String::from("Hello, world!")};
    s2.data = op(s2.data);

    let mut a = vec![3, 2, 1, 0];
    let (left, right) = a.split_at_mut(2); // Ensure non-overlapping
    right.copy_from_slice(&left);

    println!("{:?}", a);

    let data = [10, 20, 30, 40, 50, 60];
    let iter = EveryNth {
        slice: &data,
        n: 3,
        i: 0,
    };

    for val in iter {
        println!("{}", val); // prints 10, 30, 50
    }

    /*     let mut a = vec![3, 2, 1, 0];
    let src = &a[0..2];
    let dst = &mut a[2..]; // ERROR
    dst.copy_from_slice(src); */

    let mut a = vec![0, 1, 2, 3];
    let e = a.last().unwrap();
    a.clear(); // e is invalid

    let mut a = vec![0, 1, 1, 0];

    let first = a[0];
    a.retain(|x| *x != first);

    println!("{:?}", a);

    /*     struct Person {
        name: String,
        id: i32,
    }

    let b = Person {
        name: "John".to_string(),
        id: 0,
    };
    println!("{:?}", b); */

    //let mut a = vec![0];
    //let p = &a[0]; // p is a projection
    //a.push(1); // error: cannot borrow `a` as mutable because it is also borrowed as immutable
    //println!("p = {:?}", p);
    // Demonstrate safe iterators with mutable references
    demonstrate_mut_iterator();
    demonstrate_pairs_iterator();

    // slide 39 - Return by Reference
    println!("\n=== Other examples ===");
    let mut a = vec![0, 1, 2, 3];
    *a.last_mut().unwrap() = 42;

    println!("{:?}", a);
    // ---

    println!("{}", f(42));

    // slide 40 - Return by Reference
    let mut p = Point { x: 1, y: 2 };
    let (x, y) = split_coords(&mut p);
    *x = 3;
    *y = 4;
    println!("p = {:?}", p);
}
