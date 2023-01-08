dim = parse(UInt32, ARGS[2]);
side = parse(UInt32, ARGS[3]);

@assert dim >= 1 ("dim (number of dimensions) must be a positive integer")
@assert side >= 2 ("side (number of points in each side) must be at least 2")

rows = Iterators.product((0:(side - 1) for _ in 1:dim)...);

open(ARGS[1], "w") do file
    for row in rows
        line = strip(foldl((l, r) -> l * "$r ", row; init=""), [' ']) * '\n';
        write(file, line)
    end
end
