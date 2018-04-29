generate_count=10

function generate_statement_repeat_hpp () {
    file_name="statement_repeat.hpp"
    echo "Generating $file_name"
    echo '#pragma once' > $file_name
    echo >> $file_name
    echo "#define BBZY_STATEMENT_REPEAT_0(STATEMENT_MACRO)" >> $file_name
    for i in `seq ${generate_count}`
    do
        j=$(( i - 1 ))
        printf '#define BBZY_STATEMENT_REPEAT_%d(STATEMENT_MACRO)\\\n' $i >> $file_name
        printf '\tBBZY_STATEMENT_REPEAT_%d(STATEMENT_MACRO);\\\n' $j >> $file_name
        printf '\tSTATEMENT_MACRO(%d);\n' $j >> $file_name
    done
}

function generate_param_repeat_hpp () {
    file_name="param_repeat.hpp"
    echo "Generating $file_name"
    echo '#pragma once' > $file_name
    echo >> $file_name
    echo "#define BBZY_PARAM_REPEAT_0(PARAM_MACRO)" >> $file_name
    echo "#define BBZY_PARAM_REPEAT_1(PARAM_MACRO) PARAM_MACRO(0)" >> $file_name
    for i in `seq 2 ${generate_count}`
    do
        j=$(( i - 1 ))
        printf '#define BBZY_PARAM_REPEAT_%d(PARAM_MACRO) ' $i >> $file_name
        printf 'BBZY_PARAM_REPEAT_%d(PARAM_MACRO), ' $j >> $file_name
        printf 'PARAM_MACRO(%d)\n' $j >> $file_name
    done

    echo >> $file_name
    echo "#define BBZY_APPEND_PARAM_REPEAT_0(PARAM_MACRO)" >> $file_name
    for i in `seq ${generate_count}`
    do
        j=$(( i - 1 ))
        printf '#define BBZY_APPEND_PARAM_REPEAT_%d(PARAM_MACRO) ' $i >> $file_name
        printf 'BBZY_APPEND_PARAM_REPEAT_%d(PARAM_MACRO), ' $j >> $file_name
        printf 'PARAM_MACRO(%d)\n' $j >> $file_name
    done
}

generate_statement_repeat_hpp
generate_param_repeat_hpp
echo Finished!
