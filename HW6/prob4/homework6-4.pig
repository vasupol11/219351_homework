lines = LOAD 'web-Google.txt' USING PigStorage('\t') AS (word1:chararray, word2:chararray);
grouped = GROUP lines BY word2;
wordcount = FOREACH grouped GENERATE group, COUNT(maps);
store wordcount into 'google';
