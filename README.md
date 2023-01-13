# Computer vision based light detect program
## Creative
·First, I did literature reading and reviewed papers from scholars around the world on image extraction, pattern recognition and target resolution, among other topics, to understand various ways of visual recognition. 

·Then, I compared the use cases and advantages and disadvantages of various algorithms with the current requirements and selected the algorithms and implementation methods that were suitable for the project. Then, I wrote the code, created the templates needed for template matching. After that, I tested and optimized the code in different modules.

The innovative point was that, compared to the original single pattern recognition scheme, we introduced different environmental conditions, such as image processing and matching screening conditions under different lighting conditions, which greatly improved the accuracy of pattern recognition. After the code optimization, and by adjusting the parameters of the camera, we reduced the impact of the decrease in recognition efficiency corresponding to the increased accuracy to an acceptable level in the application scenario.

## Efforts
While extracting **the areas of interest** from an image, the parameters and techniques used for the constraints affected the **image recognition accuracy**. 

·To solve this problem, my teammates and I have proposed several solutions. The first was to **add the judgment of different environmental conditions** directly. Yet too much if-elses, or other judgemental statement impacting the recognition efficiency. 

·Second, the **camera was adjusted in its control parameters** so that it automatically adapted to changes in environmental lighting. However, this method involved hardware programming and should take time to contact the supplier of the camera. 

After a series of code optimization and experiments, we **combined these two solutions** to solve the problem. 

In order to find the best solution for image extraction, **I was responsible for the part of using several attempts to find the best way of image processing**. 

## Files
"task*.cpp" files are different attemps I chose to find the best parameter and technique.

Video example 2-2.avi is one of the processed videoes where the camera was adjusted. I use these videoes to test my part of the program.