#!/usr/bin/env bash

# commands found in path
LS='ls'
AWK='awk'
FIND='find'
WC='wc'
ECHO='echo'
RM='rm'
MAKE='make'
BASENAME='basename'
TR='tr'

# project specific
HUFFMAN='./liman'
UNHUFFMAN='./liman decompress'
EXAMPLES=$(${FIND} ./tests/* -type f)




# $1 - file path
filesize_in_bytes() {
  test -f $1 && ${LS} -l $1 | ${AWK} '{print $5}' | ${TR} -d ' ' || echo "0"
}

# $1 - file path
generate_compressed_filename() {
  ${ECHO} "$(basename $1).comp"
}

# $1 - file path
generate_decompressed_filename() {
  ${ECHO} "unhuffman-$(basename $1)"
}

# $1 - file path
expected_compressed_size_in_bytes() {
  case "$1" in
  	'a.txt.comp') echo 6
  	;;
  	'teste.txt.comp') echo 79207
  	;;
  	'bible.txt.comp') echo 2577867
  	;;
  	'jpg.jpg.comp') echo 36032
  	;;
  	'gatinhu.png.comp') echo 160362
  	;;
  	'pikachu.gif.comp') echo 4449596
  	;;
  	*) echo 0
  	;;
  esac
  
}

# $1 - file path
expected_decompressed_size_in_bytes() {
  case "$1" in
  	'unhuffman-a.txt') echo 15
  	;;
  	'unhuffman-teste.txt') echo 97042
  	;;
  	'unhuffman-bible.txt') echo 4451368
  	;;
  	'unhuffman-jpg.jpg') echo 35989
  	;;
  	'unhuffman-gatinhu.png') echo 160041
  	;;
  	'unhuffman-pikachu.gif') echo 4459259
  	;;
  	*) echo 0
  	;;
  esac
  
}

build_project() {
  ${MAKE} all 2>&1 >/dev/null
}

clean_project() {
  ${MAKE} clean 2>&1 >/dev/null
}




# $1 - whitespace-separated list of test files
main() {
  local originalsize=''
  local compressedsize=''
  local output=''
  local fail=0
  local success=0
  local total=$(${ECHO} "$1" | ${WC} -l)
  local inc_success_count=0
  local compression_cmd=''
  local decompression_cmd=''

  build_project

  for example in $1; do
    originalsize=$(filesize_in_bytes "${example}")

    output=$(generate_compressed_filename "${example}")
    compression_cmd="${HUFFMAN} -i ${example} -o ${output}"
    ${compression_cmd} >/dev/null 2>&1
    compressedsize=$(filesize_in_bytes "${output}")
    expectedsize=$(expected_compressed_size_in_bytes "$output")

    output2=$(generate_decompressed_filename "${example}")
    decompression_cmd="${UNHUFFMAN} -i ${output} -o ${output2}"
    ${decompression_cmd} >/dev/null 2>&1
    decompressedsize=$(filesize_in_bytes "${output2}")
    expectedsize2=$(expected_decompressed_size_in_bytes "$output2")

    inc_success_count=1
    if test "${compressedsize}" != "${expectedsize}" ; then
	    echo "Compress failed for example ${example}"
	    echo "  Compress Cmd: ${compression_cmd}"
	    echo "  Expected ${expectedsize} bytes, got ${compressedsize}"
	    inc_success_count=0
    fi

    if test "${decompressedsize}" != "${expectedsize2}" ; then
	    echo "Decompress failed for example ${example}"
	    echo "  Compress Cmd: ${compression_cmd}"
	    echo "  Decompress Cmd: ${decompression_cmd}"
	    echo "  Expected ${expectedsize2} bytes, got ${decompressedsize}"
	    inc_success_count=0
    fi

    if test -e "${output}"; then
	    ${RM} "${output}"
    fi

    if test -e "${output2}"; then
	    ${RM} "${output2}"
    fi

    if test "${inc_success_count}" == "1" ; then
      echo "OK: ${example}"
      success=$((success + 1))
    else
      echo "FAIL: ${example}"
      fail=$((fail + 1))
    fi
  done

  clean_project

  echo "Success: ${success}/${total}"
}



main "${EXAMPLES}"
