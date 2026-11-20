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
EXAMPLES=$(${FIND} ./tests/data/* -type f)




# $1 - file path
filesize_in_bytes() {
  test -f $1 && ${LS} -l $1 | ${AWK} '{print $5}' | ${TR} -d ' ' || ${ECHO} "0"
}

# $1 - file path
generate_compressed_filename() {
  ${ECHO} "$(${BASENAME} $1).comp"
}

# $1 - file path
generate_decompressed_filename() {
  ${ECHO} "unhuffman-$(${BASENAME} $1)"
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
  local compressed_size=''
  local compression_filename=''
  local fail=0
  local success=0
  local total=$(${ECHO} "$1" | ${WC} -l)
  local inc_success_count=0
  local compression_cmd=''
  local decompression_cmd=''

  build_project

  for example in $1; do
    originalsize=$(filesize_in_bytes "${example}")

    compression_filename=$(generate_compressed_filename "${example}")
    compression_cmd="${HUFFMAN} -i ${example} -o ${compression_filename}"
    ${compression_cmd} >/dev/null 2>&1
    compressed_size=$(filesize_in_bytes "${compression_filename}")
    expected_compressed_size=$(expected_compressed_size_in_bytes "$compression_filename")

    decompression_filename=$(generate_decompressed_filename "${example}")
    decompression_cmd="${UNHUFFMAN} -i ${compression_filename} -o ${decompression_filename}"
    ${decompression_cmd} >/dev/null 2>&1
    decompressed_size=$(filesize_in_bytes "${decompression_filename}")
    expected_decompressed_size=$(expected_decompressed_size_in_bytes "$decompression_filename")

    inc_success_count=1
    if test "${compressed_size}" != "${expected_compressed_size}" ; then
	    echo "Compress failed for example ${example}"
	    echo "  Compress Cmd: ${compression_cmd}"
	    echo "  Expected ${expected_compressed_size} bytes, got ${compressed_size}"
	    inc_success_count=0
    fi

    if test "${decompressed_size}" != "${expected_decompressed_size}" ; then
	    echo "Decompress failed for example ${example}"
	    echo "  Compress Cmd: ${compression_cmd}"
	    echo "  Decompress Cmd: ${decompression_cmd}"
	    echo "  Expected ${expected_decompressed_size} bytes, got ${decompressed_size}"
	    inc_success_count=0
    fi

    if test -e "${compression_filename}"; then
	    ${RM} "${compression_filename}"
    fi

    if test -e "${decompression_filename}"; then
	    ${RM} "${decompression_filename}"
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
