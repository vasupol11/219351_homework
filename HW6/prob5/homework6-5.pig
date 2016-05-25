nodes1 = LOAD 'web-GoogleTest.txt' USING PigStorage(' ') AS (from1:chararray, to1:chararray);
nodes2 = LOAD 'web-GoogleTest.txt' USING PigStorage(' ') AS (from2:chararray, to2:chararray);
joining = JOIN nodes1 BY to1 LEFT OUTER, nodes2 By from2;
Dump joining;
