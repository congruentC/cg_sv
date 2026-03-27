# cg_sv

A string view for C (cgsv). A cgsv points into memory it does not own. You can slice, trim, and split freely because every operation returns a new view into the same underlying bytes rather than copying anything. The one rule is that the pointed-to memory must outlive the view.

Part of [CongruentC](https://github.com/CongruentC).

---

## Usage

Drop `cg_sv.h` into your project. In exactly one translation unit, define the implementation macro before including:

```c
#define CGSV_IMPLEMENTATION
#include "cg_sv.h"
```

Every other file includes it bare:

```c
#include "cg_sv.h"
```

No build system changes are necessary, nor is linking against a separate library.

---

## Example

```c
#define CGSV_IMPLEMENTATION
#include "cg_sv.h"
#include <stdio.h>

int main(void) {
    cgsv line = cgsv_from_cstr("  hello, world  ");
    line = cgsv_trim(line);

    cgsv left, right;
    right = cgsv_split(line, ',', &left);
    right = cgsv_trim(right);

    printf("left:  %.*s\n", CGSV_FMT(left));
    printf("right: %.*s\n", CGSV_FMT(right));
    return 0;
}
```

Output:
```
left:  hello
right: world
```

---

## License

MIT
