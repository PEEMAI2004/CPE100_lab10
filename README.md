# C code that take .csv to calculate avergrades
## Functions
Calculate average grades of each term
Calculate average grades of each subject group
Calculate avgrage grades from all terms

## Instruction
### Rounding
change flag that use in the function

``` C
writeOutputToCSV("flag");
```

flag = 0: round down,
flag = 1: round up,
flag = 2: standard round

### Running
With some IDE, it may show error from gcc

``` bash
/usr/bin/ld: /tmp/ccyOfwUV.o: in function `divideAndRound':
main.c:(.text+0x3ea): undefined reference to `floor'
/usr/bin/ld: main.c:(.text+0x43d): undefined reference to `ceil'
collect2: error: ld returned 1 exit status
```

The error messages indicate that the linker is unable to find the definitions for the `floor` and `ceil` functions. These functions are part of the math library, which needs to be linked into your program.

When compiling your program, you need to add `-lm` at the end of the gcc command to link the math library. Here's an example:

```bash
gcc main.c -o main -lm
```

The `-lm` option tells the linker to link the math library into your program. This should resolve the undefined reference errors for `floor` and `ceil`.