set terminal png size 1080,512
set output 'data.png'
plot "data.txt" using 1 title 'S' with lines, \
     "data.txt" using 2 title 'E' with lines, \
     "data.txt" using 3 title 'I' with lines, \
     "data.txt" using 4 title 'R' with lines
