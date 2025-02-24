# CS170KFoldCross
Create a program that can compare large datasets using K Fold Cross Validation with a Nearest Neighbor model. 

1. After several minutes of googling, it seems like every reddit stackoverflow and Cplusplus answer suggests that I should just make my own ascii txt puller with sstream and ffile. So I used this as a template, subbing the string for a 2x2 array
https://www.geeksforgeeks.org/how-to-read-whole-ascii-file-into-std-string-in-cpp/

2. Thanks to the wonderful videos by Professor Keogh I was able to put together the search function very quickly. Very easy to understand and made a daunting task feel very manageable!

3. After trying to use only the Project 2 Briefing video and psuedo code provided, I hit a wall for most of the day. Output kept saying the object was its own nearest neghbor, the afterwards returning a nearest neighbor of '0' for every other object. Lots of debugging cout later I figured it out. Main problem was I just straight up not using the "current set of features" and using every feature every time. Second problem was I was calculating the distance of only a single objects features to itself, i.e. the distance equation was in the inner-most for loop and I needed another loop to actually loop through the features added. AND I had to figure out a more elegant way of stopping an object from comparing to itself. I used this example (though it mostly talks about optimizing euclidean distance) as a reference for my own equation. 
https://blog.cloudflare.com/computing-euclidean-distance-on-144-dimensions/