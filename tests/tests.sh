GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m' # No Color

testf() {
	local FT_LS_OUTPUT
	local LS_OUTPUT
	local TEST_NAME=$1
	shift
	FT_LS_OUTPUT=`./ft_ls $@ &>/dev/null`
	FT_LS_EXIT=`echo $?`
	LS_OUTPUT=`ls $@ &>/dev/null`
	LS_EXIT=`echo $?`

	if test $FT_LS_EXIT != $LS_EXIT; then
		echo -e "${RED}[ERROR]${RESET}: test failed \"$TEST_NAME\""
	else
		echo -e "${GREEN}[OK]${RESET}: test passed \"$TEST_NAME\"${RESET}"
	fi
}

############# TESTS START HERE #################
echo ft_ls tester
echo executing tests...
testf "exit code" src
testf "exit code" include
testf "exit code" src/temp
testf "exit code" .
testf "exit code" src . include
testf "exit code" Makefile
testf "exit code" README.md
testf "non existing file" doesntexist.md
