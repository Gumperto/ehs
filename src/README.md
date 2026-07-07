# To contributors
Thank you for contributing to East Hokusai Services! As a contributor
to the project, we expect you to read and follow a few guidelines as 
outlined below to ensure project consistency and ease-of-editing. There 
are also some tips if you have never worked on a project like this before.

# Table of contents
[Project organization](#project-organization)

[Header files](#header-files)

# Project organization

For the sake of not losing our sanity, we split our project into
multiple different functional parts, which we'll showcase through
this snazzy tree:

```
ehs/
├── Makefile     # tells the program how to compile
├── README.md    # gives people an idea of what we made
├── .gitignore   # allows git to ignore things we don't want it to know about
├── slides/      # we make our slides here
│   └── ...
├── src/         # all code editing goes here
│   └── ...
└── build/       # compilation goes here; ignored by .gitignore
    └── ...
```

We omitted some details here because they're not insanely relevant
to understanding all this. The main takeaway is that there is an
important distinction between source files and build files, and
it's convenient for us to not mix them up, because compilation
can get very messy, and we want to shield ourselves from this vomit
if it isn't necessary to know what's going on under the hood.

> [!TIP]
> You might ask: "Hey! How do I compile stuff now?" That's the neat
part; you don't. Instead, you have this thing called a `Makefile`,
which tells `gcc` what, where, and how to compile. All you have to
do is, **at the top of the project directory (right under `ehs`)**,
type `make`, and it will do the job. Typing `make clean` will clean
the `build/` directory as well.

> You might ask again: "Hey! My IDE/text editor/LSP keeps yelling at
me about errors if they're not in the same file!" This can be a bit
annoying because it depends on what LSP you're using, but if you're
using `clangd` your LSP, and you have `bear`, you can run `bear -- 
make` to generate a file named `compile_commands.json` at top level 
that shuts up the LSP.

# Header files

Normally, we're used to writing code in one big monolithic file.
This is really crappy, because for really big projects (or even 
medium sized ones such as this), it very quickly becomes a miserable 
experience to slog through all the functions and whatnot.

To solve this problem, we usually learn that we can split big
code files up into little code files that (usually, preferably)
mean something to us. These are called modules, put enough of
them together and you've got a program.

As we're working with C for this, we're gonna need to use something
called "header files". You've seen header files before, they're the
`#include <...>` you always see on top of the code files usually.
It happens that we can make our own! For example, if we write some
function body code in a file `add.c`...

```c
int add(int a, int b) {
    return a + b;
}
```

...and want to reference that file in `main.c`, we create a file 
called `add.h` under the same directory as `add.c`, and in `add.h`, 
we write the following function declaration...

```c
#ifndef ADD // These things are include guards so
#define ADD // you don't accidentally include the
            // same header twice, leading to shenanigans

int add(int a, int b);

#endif      // They're important to learn, but you
            // can dismiss them for a project this scale
```

...this allows us to carry that over to any other file that we like!
For example, we can now write in `main.c`:

```c
#include <stdio.h>
#include "add.h" // Keep in mind that this is double quotes " "!

int main() {
    int a = 1, b = 1;
    printf("Add: %d + %d = %d\n", a, b, add(a, b));
    return 0;
}
```

> [!IMPORTANT]
> When creating your header file, be sure to use include guards (the
`#ifndef` stuff). Additionally, if you write more than one function
inside of a code file, you must source them all in the header file.

For each code file you create, a good heuristic is to create a header file
for it too. For example, if you have the following project directory:

```
ehs/
├── src/
│   ├── add.c
│   ├── subtract.c
│   └── main.c
└── ...
```

Using header files, it will look something like this instead:

```
ehs/
├── src/
│   ├── add.c
│   ├── add.h
│   ├── subtract.c
│   ├── subtract.h
│   └── main.c //main does not need a header file
└── ...
```

> [!WARNING]
> If, for example, you have code files `add.c`, `subtract.c`, etc, and 
their corresponding header files, and you need to include them in 
a code file called `arithmetic.c`, when you include the header `arithmetic.h`
in e.g `main.c`, nothing in the headers of `add.h`, `subtract.h`, etc,
will come with `arithmetic.h`, so beware! (any function in `arithmetic.h`
will still work normally, but you can't use any function in `add.h`)
