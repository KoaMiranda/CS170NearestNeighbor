# CS170NearestNeighbor
Create a program that can compare large datasets using a Nearest Neighbor model. 

1. After several minutes of googling, it seems like every reddit stackoverflow and Cplusplus answer suggests that I should just make my own ascii txt puller with sstream and ffile. So I used this as a template, subbing the string for a 2x2 array
https://www.geeksforgeeks.org/how-to-read-whole-ascii-file-into-std-string-in-cpp/

2. Thanks to the wonderful videos by Professor Keogh I was able to put together the search function very quickly. Very easy to understand and made a daunting task feel very manageable!

3. After trying to use only the Project 2 Briefing video and psuedo code provided, I hit a wall for most of the day. Output kept saying the object was its own nearest neghbor, the afterwards returning a nearest neighbor of '0' for every other object. Lots of debugging cout later I figured it out. Main problem was I just straight up not using the "current set of features" and using every feature every time. Second problem was I was calculating the distance of only a single objects features to itself, i.e. the distance equation was in the inner-most for loop and I needed another loop to actually loop through the features added. AND I had to figure out a more elegant way of stopping an object from comparing to itself. I used this example (though it mostly talks about optimizing euclidean distance) as a reference for my own equation. 
https://blog.cloudflare.com/computing-euclidean-distance-on-144-dimensions/

4. Hooray it's working! First off I realized I was using the wrong small data file (23 instead of 24). Corrected that and started to get suspicious when I was getting 100% accuracy within the 2nd level of the search tree. Watched Prof. Keoghs video again and combed through the code again. That's when I decided to make the output match the example, which made it far less crowded. After I was satisfied with that, I went and tested the Large data (correct one) and it only took like 5-7 minutes which I took as a good sign. There I ended with a much more reasonable accuracy which also alleviated some of my doubts. But I think I'm doing it!

5. Thought Backwards elimination was gonna be as simple as "changing a few inequalities" >.< But turned out to be waaaay more complicated. I think I made it way harder on myself by initially trying to mirror the forward selection as closely as possible. Until a lot of trial and error iterative changes and tears later, I got it functioning. Biggest problems were the fact that features werent being erased like they should, Some features were magically being added back, and the in general the difference between ">" and ">=" in different combinations until I got it right. Still pretty suspicious that small data is still so quickly converging on 100% (like in two tree levels). And large data is pretty close like 98%. But I honestly think its working fine. Once it was looking good, I changed the main to be more user friendly/closer to the example. Hooray!

6. After comparing my (mostly) finished code to the Project briefing video one last time, I realized I should point out the key difference in my approach to this algorithm. Rather than just "zeroing" out the columns of the features I am not using in a given level of the search, I instead utilized a set to hold the indices of the features that I am testing. Then when the accuracy function is called, I ran through the set holding the indices of the features of interest and calculated the euclidean distance using only those. this ensured that only the features we want are being tested. and also made the backwards elimination implementation that much harder lol.

7. oh boy, upon further inspection of my backwards elim code, I realized there 1 large issue still. I forgot how sets work so iterating through them with a regular for loop was erasing and catalouging the wrong indices of the features. So i decided to simplify it to an auto for loop which plays nicer with the set that holds the currentSetfeatures. 
THEN when looking into this I realized ALL my for loops were 0 indexed and that was what was giving me such high accuracies.

8. Aaaaaaaaand I was using small data 24 and big data 109 instead of small 109 and big 24... Remaking my graphs and rewriting the report now.

9. Output formatting with backwards elim. Was showing all 6 features in first level of search and nothing in the last level. This was because of the way I made my cout statements for forwards. Fixed now.

10. Finished up after consulting with Dr. Keogh through up. Reassured me that I am on the right track. Added timer for report measurements. Think I'm finally done!