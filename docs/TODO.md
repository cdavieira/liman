# Todo

## Features
* [ ] generalize the HuffmanTree to work with sets other than the ASCII set?
* [ ] support generating/printing the HuffmanTree for strings entered in the commandline
* [ ] support printing the default output filename for some input

## Known problems
* [ ] both '-c' and '-t' options for the 'inspect' command dont work together
> outputfile gets overriden by the last option used

## Code improvements
* [ ] improve filenaming.c implementation
* [ ] make FilenameParts private
* [ ] the 'CompWriterStatistics' struct should be renamed to 'CompWriterEstimate' or something better
* [ ] improve error handling
* [ ] improve implementation of ArgParser.c
* [ ] improve implementation of String.c
* [ ] CompReader/CompWriter API

## Documentation
* [ ] add doxygen for documentation (probably use ai for this)
* [ ] translate portuguese comments in the code to english
* [ ] create diagrams explaining how things work

## Tests
* [ ] test how liman handles big files (>1GiB)
* [ ] add the geekforgeeks test
	- [how the algorithm should work](https://www.geeksforgeeks.org/dsa/huffman-coding-greedy-algo-3/)
	- [online tool to check the huffman method](https://www.w3schools.com/dsa/dsa_ref_huffman_coding.php)
* [ ] save compressed/decompressed sizes in a json file and read that file in test.sh instead of hardcoding values

## Nice to have
* [ ] add const to everything
* [ ] change unsigned long to uint64_t or size_t where suitable
* [ ] proper deallocation when the program fails
* [ ] add linting rule to makefile

## Recently done
* [X] add rule to regenerate compile_commands.json
* [X] run dot automatically
* [X] improve 'opts' handling in liman.c and cli.c
* [X] support detecting if the input file for 'decompress' is a valid comp file or not
