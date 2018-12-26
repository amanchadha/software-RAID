#! /bin/csh -f

set resultDir = results
set scriptDir = scripts

set weights = ( 25 25 25 25 ) #6 6 6 6 6 12.5 12.5 12.5 12.5 10 )
if ($#argv == 1) then
    set list = $argv
else
    set list = "0 1 2 3"
else
    echo "usage: run-tests.csh [whichTest]"
    exit 1
endif

make 

if ($status != 0) then
    echo "Build Failed"
    exit 1
endif

@ TOTAL = 0
@ PASS = 0
@ FAIL = 0
@ TAKEN = 0
@ INDEX = 0
foreach test ($list)
    @ INDEX = $INDEX + 1
    set tmpFile = `mktemp`
    # echo $tmpFile

    # this is how you run the particular test
    (${scriptDir}/test${test}.csh > ${tmpFile}.out) >& ${tmpFile}.err
    set testStatus = $status
    
    # check for differences in output
    diff -w ${tmpFile}.out ${resultDir}/${test}.out >& ${tmpFile}.out.diff
    diff -w ${tmpFile}.err ${resultDir}/${test}.err >& ${tmpFile}.err.diff

    @ fail = 0

    # check if standard output, standard error are correct
    foreach type ("out" "err")
	set check = `stat -c %s ${tmpFile}.${type}.diff`
	if ($check != "0") then
	    echo "TEST ${test}: bad std${type} output"
	    echo "** Expected:"
	    cat ${resultDir}/${test}.${type}  | head -10
	    echo "** Got:"
	    cat ${tmpFile}.${type} | head -10
	    echo "** Diff file (which should be empty) instead contains:"
	    cat ${tmpFile}.${type}.diff | head -10
	    echo ""
	    @ fail = $fail + 1
	endif
    end

    # what return code should the program be giving us?
    set expectedStatus = `cat ${resultDir}/${test}.status`
    if ($testStatus != $expectedStatus) then
	echo "TEST ${test}: bad status"
	echo "** Expected return code: $expectedStatus"
	echo "** Got return code     : $testStatus"
	@ fail = $fail + 1
    endif

    if ($fail != 0) then
	echo "TEST ${test}: FAILED"
	@ FAIL = $FAIL + 1
    else
	echo "TEST ${test}: PASSED"
	@ PASS = $PASS + 1
	set TOTAL = `echo "$TOTAL + $weights[$INDEX]" | bc -l`
    endif

    # clean up...
    if( -e out.txt ) then 
       rm out.txt
    endif 
    if( -e in.txt ) then 
       rm in.txt
    endif 
    
    echo ""
end

#if (!(( -e pic.jpg) || ( -e PIC.jpg) || ( -e Pic.jpg))) then
#    echo "pic.jpg not found! Taking 10 points off."
#else
#    set TOTAL = `echo "$TOTAL + 10" | bc -l`
#endif

#set TOTAL = `echo "$TOTAL + 10" | bc -l`

if ($#argv != 1) then
    echo "Summary:"
    echo "$PASS test(s) passed."
    echo "Total (out of 100)"
    echo "$TOTAL"
endif



