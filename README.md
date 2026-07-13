<div align = center>
EHS

EHS is a scheduling CLI that works.
</div>

# Table of contents
[Build and run](#build-and-run)

[Contributing](#contributing)

[Special thanks](#special-thanks)

# Build and run
The only way to run ehs is by building from source.

## Dependencies
- A working C compiler: `gcc >= 16.1.1` (probably not that high but that's the version I own lol) 
- (more as we add libraries)

For data analysis: This directory offers a dead simple bash script 
that generates the environment so the analysis scripts know where 
everything lives

To do so, simply copy:
```
chmod +x generate_env.sh
./generate_env.sh
```

## Build instructions
1. Open your terminal application of choice
2. In your desired directory, clone the repo and navigate to the `ehs` directory:
```
git clone https://github.com/Gumperto/ehs.git
cd ehs
```
3. Build the program using the following commands:
```
make
```

If you are thinking of contributing and use the `clangd` LSP, instead run:
```
bear -- make 
```

4. Run the program and do stuff 
```
./build/ehs
```

If you're interested in contributing, check out [contributing](#contributing).

# Contributing
For contributors, check out the [README.md in src/](./src/README.md), as that
document goes into more detail on how certain things work.

If you make changes to the codebase, simply recompile to test it out. Once
satisfied, feel free to open a pull request to the repository and it will
be checked out as soon as we have the bandwidth.

# Special thanks
Thank you for using East Hokusai Services. 

We have providing high quality data analysis services since 2026.

Thanks to [Muhammad Noor Fawi](https://github.com/MNoorFawi) for
inspiring the design of the simulated annealing algorithm.

We extend our thanks to our family, friends, and our close business
partner Dr. Zomboss (formerly West Hokusai Group).
