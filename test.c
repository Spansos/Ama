// C, not expandable
typedef struct P {
    union {
        int value;
        float value;
    };
    enum { INT, FLOAT } Type;
} P;

int main() {
    P test;
    test.Type = INT;
    test.value = 2;
}

// No more typedef, not expandable
struct P {
    union {
        int value;
        float value;
    };
    enum { INT, FLOAT } Type;
};

int main() {
    P test;
    test.Type = INT;
    test.value = 2;
}

// rust-style enum, not expandable
struct {
    enum {
        int int_;
        float float_;
    } value;
} P;

int main() {
    P test;
    // both should work
    test.int_ = 2;
    test.value = 2;
}

// outside declaration, now expandable
struct P;
enum P::value {
    int int_;
}
enum P::value {
    float float_;
}

int main() {
    P test;
    // both should work
    test.int_ = 2;
    test.value = 2;
}

// templates
struct P[type t, int i] {
    t test;
    vector<int, i> list;
};

// type qualifiers
* i32       /* pointer to i32 */
var i32     /* variable i32 */
var * i32   /* non-static pointer to i32 */
* var i32   /* pointer to non-static i32 */
&*var*var i32 /* reference to pointer to non-static pointer to non-static i32 */

// [bytec] can be used to allocate memory statically / on the stack
* i32 p = [4];
// arrays can be made by using [] in its type
var [8] i32 ar;
// this is equal to
var * i32 ar = [8*sizeof(i32)];

// identifiers - [a-Z_][a-Z_0-9]*
// keywords - struct enum alias break continue while for if else switch case
// types - i8 i16 i32 i64 ui8 ui16 ui32 ui64 f32 f64
// type qualifiers - * & var
// operators - << >> - + * / // & | ^ % ! ?
// assignment - <<= >>= -= += *= /= //= &= |= ^= %= != ?=
// brackets - ( ) { } [ ]
// comments - /*  */
