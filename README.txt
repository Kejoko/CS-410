Keegan Kochis
831455801
kkochis@rams.colostate.edu
(913) 952-4882

----- COMPILATION INSTRUCTIONS -----
1. Open a terminal and `cd` into the directory containing the contents of the tar
   file (the directory with the contents of "KeeganKochis04.tar")
2. Run the command 'mkdir build; cd build`
	* We will compile in the "build" directory in order to prevent cmake and make
	  from polluting the top level directory
	* Note that this step is not necessary. If you choose to not do this step there
	  are additional details under the next step on how to move forward
3. Run the command `cmake ..`
	* The `..` denotes that "CMakeLists.txt" is in the directory one level above
	  your current directory (one level above the "build" directory)
	* If you chose to not use the "build" directory and are still operating in
	  the top level directory, run the command `cmake .` instead
4. Run the command `make`
5. You should see the "raytracer" executable successfully built in your current directory

----- EXECUTION INSTRUCTIONS -----
1. Once the "raytracer" executable has been built you should cp or mv it to the directory
   containing the .obj and .mtl files. The executable can be run via the command line by
   passing to it the desired driver file and the desired output file via command line
   arguments. Example executions are shown below. It should be noted that the executable
   will error out if the proper number of arguments (2) are not passed to it. It is
   expected that the "raytracer" executable lives in the same directory as the .obj and
   .mtl files that are requested in the driver file. If the executable is not in the
   same directory as the .obj and .mtl files then it will not be able to read from them.
	EX: ./raytracer driver00.txt driver00.ppm
	EX: ./raytracer ~/Documents/drivers/driver12.txt ~/Desktop/output.ppm
	EX: ./build/raytracer ../driver1000.txt ../solutions/driver1000.ppm

----- SCENE INFORMATION -----
scene1.txt
	* Rendering in 1024x1024 took 5 hours and 14 minutes on my laptop
	* The cube is rendered with flat shading and can be seen refracting through a
	  sphere. In the bottom left a smooth shaded icosphere can be seen in blue.
scene2.txt
	* Rendering in 1024x1024 took 9 hours and 32 minutes on my laptop
	* I attempted to recreate the refraction example image showed in
	  lecture (the glass ball on the beach), in the reference.png file.
	  I would have liked to added more objects on the horizon line as shown
	  in the reference image but I chose to go with spheres due to the already
	  high rendering time. I'm not proud of the hand object I created for this
	  scene. My 3d modeling skills aren't the greatest.