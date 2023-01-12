#!/bin/bash
#=
exec julia --color=yes --startup-file=no "${BASH_SOURCE[0]}" "$@"
=#

filename = ARGS[1];
dim = parse(UInt32, ARGS[2]);
side = parse(UInt32, ARGS[3]);

@assert isdir(dirname(filename)) ("directory $(dirname(filename)) does not exist.")
@assert dim >= 1 ("dim (number of dimensions) must be a positive integer.")
@assert side >= 2 ("side (number of points in each side) must be at least 2.")

N = side ^ dim;

rows = Iterators.product((0:(side - 1) for _ in 1:dim)...);

open(filename, "w") do file
    write(file, "$(N) $(dim)\n");

    for row in rows
        line = strip(foldl((l, r) -> l * "$r ", row; init=""), [' ']) * '\n';
        write(file, line);
    end
end
